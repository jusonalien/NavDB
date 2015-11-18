//
// Created by jusonalien on 2015/11/13.
//


/*
 *Author: jusonalien
 *Email:  jusonalien@qq.com or jusonalien@gmail.com
 *
 *Description: A NOT efficient && naive implementation of B-Plus-Tree data structure
 *             which is described in the <<Database System Concept>> 6th Edition
               Chapter 11.3 B+-Tree Index Files
 *Reference:
 *      https://github.com/ghostrong/algorithm/blob/master/bptree_v2.c
 *      https://github.com/begeekmyfriend/bplustree/blob/master/lib/bplustree.c
 *
 */




#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "bplustree.h"




bptree_node*
search_leaf(bptree_node *root, int key)
{
    bptree_node *result;
    int i;
    if (root == NULL)
        return root;
    result = root;
    while (result->isLeaf != BPLUS_TREE_LEAF) {
        for (i = 0; i < result->num_keys && key >= result->key[i]; ++i) // be careful of here!
            ;
        result = result->pointers[i];
    }
    return (bptree_node *) result;
}

file_record *
search(bptree_node *root, int key)
{
    bptree_node *leaf;
    int i;

    leaf = search_leaf(root, key);

    if (leaf == NULL)
        return NULL;
    /*we can use binary search for more efficiency*/
    for (i = 0; i < leaf->num_keys && key != leaf->key[i]; ++i)
        ;
    if (i == leaf->num_keys)
        return NULL;

    /*
     * pointer Pi points to a file record with search-key value Ki
     */

    return (file_record *) leaf->pointers[i];

}


file_record *
new_file_record(int value)
{
    file_record *rec;
    rec = (file_record *) malloc(sizeof(file_record));

    assert(rec);
    rec->value = value;
    return rec;
}

void print_leafnode_keys(bptree_node* leaf_node)
{
    file_record* file;
    int i;
    printf("key numbers: %d\n",leaf_node->num_keys);
    printf("value is: ");
    for (i = 0;i < leaf_node->num_keys;++i) {
        file = leaf_node->pointers[i];
        printf("%d ",file->value);
    }
    printf("\n");
    printf("key is:   ");
    for (i = 0;i < leaf_node->num_keys;++i) {
        printf("%d ",leaf_node->key[i]);
    }
    printf("\n");
}

bptree_node *
new_bptree_node()
{
    bptree_node *new_node;

    new_node = (bptree_node *) malloc(sizeof(bptree_node));
    new_node->pointers = malloc((MAX_SIZE + 4) * sizeof(void *));
    memset(new_node->pointers,0,sizeof(new_node->pointers));
    new_node->key = malloc((MAX_SIZE + 4) * sizeof(int));
    memset(new_node->key,0,sizeof(new_node->key));
    new_node->parent = NULL;
    /*new_node->next     = NULL;*/
    new_node->isLeaf = BPLUS_TREE_NOT_LEAF;
    new_node->num_keys = 0;


    return new_node;
}


bptree_node *
new_leaf()
{
    bptree_node *Leaf;
    Leaf = new_bptree_node();
    Leaf->isLeaf = BPLUS_TREE_LEAF;
    return Leaf;
}

bptree_node *
new_tree(int key, file_record *rec)
{
    bptree_node *root;
    root = new_leaf();
    root->pointers[0] = rec;
    root->key[0] = key;
    root->pointers[MAX_SIZE - 1] = NULL;
    root->num_keys++;
    return root;
}

/*
 *Seems too much bugs here??
 */
void
destory_tree(bptree_node *root)
{
    int i;
    if (!root->isLeaf) {
        for (i = 0; i < root->num_keys; ++i) {
            free(root->key[i]);
            destory_tree(root->pointers[i]);
        }
        destory_tree(root->pointers[i]); //destory the last pointer
    }
    else {
        for (i = 0; i < root->num_keys; ++i) {
            free(root->key[i]);
            free(root->pointers[i]);
        }
    }
    destory_tree(root);
}

bptree_node *
new_root(bptree_node *left, bptree_node *right, int key)
{
    bptree_node *root;
    root = new_bptree_node();
    root->key[0] = key;
    root->pointers[0] = left;
    root->pointers[1] = right;
    root->num_keys++;
    left->parent = root;
    right->parent = root;
    return root;
}


void
insert_in_leaf(bptree_node *leaf, int key, file_record *rec)
{
    int index;
    for (index = 0; index < leaf->num_keys && leaf->key[index] <= key; index++);
    int tags = index;
    for (index = leaf->num_keys; index > tags; index--) { //copy the nodes from the insert node
        leaf->key[index] = leaf->key[index - 1];
        leaf->pointers[index] = leaf->key[index - 1];
    }
    int B = (tags <= (MAX_SIZE - 1));
    assert(B);
    leaf->key[tags] = key;
    leaf->pointers[tags] = rec;
    leaf->num_keys++;
    //leaf->num_ptrs++;
    return;
}

bptree_node*
insert_in_node(bptree_node *p, bptree_node *c, int key, int index)
{
    int i;
    for (i = p->num_keys; i > index; --i) {
        p->key[i] = p->key[i-1];
        p->pointers[i+1] = p->pointers[i];
    }
    p->key[index] = key;
    p->pointers[index+1] = c;
    p->num_keys++;
    c->parent = p;
    return p;
}

/*
 *leaf node's numbers of key and ptr should be the same  MAX_SIZE
 * while the non-leaf nodes' number of key should be MAX_SIZE-1 and
 * number of ptr should be MAX_SIZE
 */


bptree_node *
insert_in_parent(bptree_node *root, bptree_node *n, int keyp, bptree_node *np)
{
    //if (n->parent == NULL) {
    if (LEAF_IS_NODE(n)) {
        return new_root(n, np, keyp); // may be should be rewritten
    }

    bptree_node *parent = n->parent;
    int index, i;
    for (index = 0; index < parent->num_keys && parent->key[index] < keyp; ++index)
        ;
    //if (parent->num_keys < MAX_SIZE - 1) { //take care~  P has less than MAX_SIZE pointer
    if (NON_LEAF_NODE_IS_NOT_FULL(parent)) {
        insert_in_node(parent, np, keyp, index);
        return root;
    }
    else {
        assert(parent->num_keys == (MAX_SIZE - 1));
        bptree_node* t= new_bptree_node();
        for (i = 0;i < MAX_SIZE - 1;++i) {
            t->key[i] = parent->key[i];
            t->pointers[i] = parent->pointers[i];
            t->num_keys++;
        }
        assert(parent->num_keys == t->num_keys);
        t->pointers[i] = parent->pointers[i];
        insert_in_node(t, np, keyp, index);
        assert((parent->num_keys + 1)== t->num_keys);
        bptree_node* pp = new_bptree_node();
        int cp_pos = (MAX_SIZE - 1) / 2;
        /*erase a non-leaf node*/
        for (i = 0;i < MAX_SIZE - 1;++i) {
            parent->key[i] = 0;
            parent->pointers[i] = NULL;
        }
        parent->pointers[i] = NULL;
        parent->num_keys = 0;

        for (i = 0;i < cp_pos;++i) {
            parent->key[i] = t->key[i];
            parent->pointers[i] = t->pointers[i];

            parent->num_keys++;
        }
        assert(parent->num_keys == cp_pos);
        parent->pointers[i] = t->pointers[i];
        assert(i == cp_pos);
        int keypp = t->key[i];
        int j;
        bptree_node* par_ptr;
        for(i = cp_pos +1,j = 0;i < t->num_keys;++i,++j) {
            pp->pointers[j] = t->pointers[i];
            pp->key[j] = t->key[i];
            par_ptr = t->pointers[i];
            par_ptr->parent = pp;
            pp->num_keys++;
        }
        assert((pp->num_keys + cp_pos + 1) == t->num_keys);
        pp->pointers[j] = t->pointers[i];
        par_ptr = t->pointers[i];
        par_ptr->parent = pp;
        bptree_node* ppp = parent->parent;
        if (ppp) assert(ppp->num_keys <= (MAX_SIZE - 1));
        return insert_in_parent(root, parent, keypp, pp);
    }
}

bptree_node *
insert(bptree_node *root, int key, int value)
{
    file_record *rec = new_file_record(value);
    bptree_node *leaf;
    int index, cond;

    leaf = search_leaf(root, key);


    if (!leaf) { // cannot find the leaf, so the tree is empty
        return new_tree(key, rec);
    }

    rec = new_file_record(value);

    /* leaf node's numbers of key and ptr should be the same  MAX_SIZE - 1 */
    if (NON_LEAF_NODE_IS_NOT_FULL(leaf)) {
        insert_in_leaf(leaf, key, rec);
        return root;
    }
    else { /* leaf has full key values already, split it then insert it */
        bptree_node *lp = new_leaf();
        bptree_node *t = new_leaf();
        assert(leaf->num_keys == MAX_SIZE - 1);
        int i;
        for (i = 0; i < MAX_SIZE - 1; ++i) {
            t->key[i]      = leaf->key[i];
            t->pointers[i] = leaf->pointers[i];
            t->num_keys++;
        }

        insert_in_leaf(t, key, rec);

        lp->pointers[MAX_SIZE - 1]  = leaf->pointers[MAX_SIZE - 1];
        leaf->pointers[MAX_SIZE - 1] = lp;

        for (i = 0; i < MAX_SIZE - 1; ++i) { // Erase L.P1 through L.K[n-1] from L
            leaf->pointers[i] = NULL;
            leaf->key[i] = -1;
        }
        leaf->num_keys = 0;
        int cp_pos = (MAX_SIZE - 1) / 2; // take care!

        for (i = 0; i <= cp_pos; ++i) {
            leaf->pointers[i] = t->pointers[i];
            leaf->key[i] = t->key[i];
            leaf->num_keys++;
        }
        int j;
        for (i = cp_pos + 1, j = 0; i < MAX_SIZE && j < MAX_SIZE; ++i, ++j) {
            lp->pointers[j] = t->pointers[i];
            lp->key[j] = t->key[i];
            lp->num_keys++;
        }
        int keyp = lp->key[0]; // keyp should be the smallest key-value in lp. a little not sure~

        return insert_in_parent(root, leaf, keyp, lp);
    }
}


