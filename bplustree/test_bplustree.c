//
// Created by jusonalien on 2015/11/13.
//

/*
 *Author: jusonalien
 *Email:  jusonalien@qq.com or jusonalien@gmail.com
 *
 *Description: A NOT efficient & naive implementation of B-Plus-Tree
 *              which is described in the <<Database System Concept>> 6th Edition
 *Reference:
 *      https://github.com/ghostrong/algorithm/blob/master/bptree_v2.c
 *      https://github.com/begeekmyfriend/bplustree/blob/master/lib/bplustree.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "bplustree.h"

bptree_node* d_insert(bptree_node* node, int key, int value)
{
    printf("insert key %d insert value %d\n",key,value);
    return insert(node,key,value);
}

void print_bptree(bptree_node* root)
{
    file_record* file;
    int i ;
    if (root->isLeaf == BPLUS_TREE_LEAF) {

        for (i = 0;i <= root->num_keys;++i) {
            file = root->pointers[i];
            printf("%d ",file->value);
        }
        printf("\n");
        return  ;
    }
    else {
        for (i = 0;i <= root->num_keys;++i) {
            print_bptree(root->pointers[i]);
        }
    }
}
void d_search(bptree_node* root, int key)
{
    file_record* file = search(root, key);
    if(file == NULL) {
        printf("NO result for %d\n",key);
    }
    else
        printf("search result: %d\n",file->value);
}

void
print_isleaf(bptree_node* node)
{
    if (node->isLeaf == BPLUS_TREE_LEAF)
        printf("LEAF\n");
    else
        printf("NOT Leaf\n");
}
void
print_leaf(bptree_node* leaf)
{
    int i;
    for(i = 0;i < leaf->num_keys;++i)
        printf("%d ",leaf->key[i]);
    puts("");
}
int main()
{

    bptree_node* root = NULL;
    int i, search_test,insert_test;
    search_test  = 1 << 10;
    insert_test = 1 << 10;
    for (i = 1;i <= insert_test;++i) {
        root = d_insert(root,i,i);
        //d_search(root,i-1);
    }
    for (i = 1;i <= insert_test / 3; ++i) {
        delete_bptree_node(root,i);
    }
    for (i = 1;i <= search_test;++i) {
        d_search(root,i);
    }
    puts("");
    return 0;
}
