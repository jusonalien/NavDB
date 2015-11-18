//
// Created by jusonalien on 2015/11/13.
//

/*
 *Author: jusonalien
 *Email:  jusonalien@qq.com or jusonalien@gmail.com
 *
 *Description: A not efficient & naive implementation of B-Plus-Tree
 *              which is described in the <<Database System Concept>> 6th Edition
 *Reference:
 *      https://github.com/ghostrong/algorithm/blob/master/bptree_v2.c
 *      https://github.com/begeekmyfriend/bplustree/blob/master/lib/bplustree.c
 *
 */

#ifndef BPLUSTREE_BPLUSTREE_H
#define BPLUSTREE_BPLUSTREE_H

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>




#define MAX_SIZE            1024

#define BPLUS_TREE_LEAF     1
#define BPLUS_TREE_NOT_LEAF 0
#define BPLUS_TREE_ROOT     2

// typedef struct bptree_node bptree_node;

typedef struct bptree_node {

    int* key;
    void** pointers;
    int num_keys;
    //int num_ptrs;
    struct bptree_node* parent;
    int isLeaf;

}bptree_node;


typedef struct file_record {
    int value;
} file_record;

#define NON_LEAF_NODE_IS_NOT_FULL(node)  ( (node)->num_keys < (MAX_SIZE - 1) )
#define LEAF_IS_NODE(node) ((node)->parent == NULL)
/*
bptree_node* search_leaf(bptree_node *root, int key);

file_record *search(bptree_node* root, int key);
file_record*
        new_file_record(int value);
bptree_node*
        new_bptree_node();
bptree_node*
        new_leaf();
bptree_node*
        new_tree(int key, file_record *rec);
void
        destory_tree(bptree_node* root);
bptree_node*
        new_root(bptree_node* left, bptree_node* right, int key);
void
        insert_in_leaf(bptree_node* leaf, int key, file_record* rec);
void
        insert_in_node(bptree_node* p,bptree_node* c, int key, int index);
bptree_node*
        insert_in_parent(bptree_node* root, bptree_node* n, int keyp, bptree_node* np);
bptree_node*
        insert(bptree_node* root, int key, int value)

*/

#endif //BPLUSTREE_BPLUSTREE_H
