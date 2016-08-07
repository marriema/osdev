#include <generic_tree.h>

/*
 * Create a tree with root = NULL
 * */
gtree_t * tree_create() {
    return (gtree_t*)kcalloc(sizeof(gtree_t), 1);
}

/*
 * Insert a node under subroot
 * */
gtreenode_t * tree_insert(gtree_t * tree, gtreenode_t * subroot, void * value) {
    // Create a treenode
    gtreenode_t * treenode = kcalloc(sizeof(gtreenode_t), 1);
    treenode->children = list_create();
    treenode->value = value;

    // Insert it
    if(!tree->root) {
        tree->root = treenode;
        return treenode;
    }
    list_insert_front(subroot->children, treenode);
    return treenode;
}
