#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "btree_internal.h"

/*
 * Helper function to retrieve the BtLeafNode from a BtNode.
 */
inline extern BtInternalNode* get_bt_internal_node(BtNode* bt_node) {
    assert(bt_node->type == BT_INTERNAL);
    return bt_node->contents.internal;
}

/*
 * Helper function to retrieve the BtLeafNode from a BtNode.
 */
inline extern BtLeafNode* get_bt_leaf_node(BtNode* bt_node) {
    assert(bt_node->type == BT_LEAF);
    return bt_node->contents.leaf;
}

/*
 * Helper function to check if a BtNode is full.
 */
inline extern bool is_bt_node_full(BtNode* bt_node) {
    switch (bt_node->type) {
        case BT_LEAF:
            return bt_node->contents.leaf->num_records >= BT_NODE_SIZE;
        case BT_INTERNAL:
            return bt_node->contents.internal->num_children >= BT_NODE_SIZE;
    }
}

/*
 * Helper function to allocate a BtLeafNode and initialize it.
 */
BtNode* bt_allocate_leaf_node() {
    BtNode* bt_node = malloc(sizeof(BtNode));
    if (bt_node == NULL) {
        return NULL;
    }

    bt_node->type = BT_LEAF;
    bt_node->contents.leaf = malloc(sizeof(BtLeafNode));
    if (bt_node->contents.leaf == NULL) {
        free(bt_node);
        return NULL;
    }

    bt_node->contents.leaf->num_records = 0;
    return bt_node;
}

/*
 * Helper function to allocate a BtInternalNode and initialize it.
 */
BtNode* bt_allocate_internal_node() {
    BtNode* bt_node = malloc(sizeof(BtNode));
    if (bt_node == NULL) {
        return NULL;
    }

    bt_node->type = BT_INTERNAL;
    bt_node->contents.internal = malloc(sizeof(BtInternalNode));
    if (bt_node->contents.internal == NULL) {
        free(bt_node);
        return NULL;
    }

    bt_node->contents.internal->num_children = 0;
    return bt_node;
}

/*
 * Given a parent and a child BtInternalNode, split the child into two nodes,
 * and update the parent to keep track of this information.
 *
 * @parent_node: Parent node. Must be a BtInternalNode.
 * @child_node: Child node to be split. Must be a BtInternalNode.
 * @child_index: Index of the child to be split.
 *
 * Returns 0 if splitting is successful, and -1 otherwise.
 */
int bt_split_internal_child(BtNode* parent_node, BtNode* child_node,
                            size_t child_index) {
    BtNode* new_child_node = bt_allocate_internal_node();
    if (new_child_node == NULL) {
        return -1;
    }

    // Fetch internal nodes for elements which will participate in splitting.
    BtInternalNode* child = get_bt_internal_node(child_node);
    BtInternalNode* parent = get_bt_internal_node(parent_node);
    BtInternalNode* new_child = get_bt_internal_node(new_child_node);

    // Transfer the last half of nodes from child to new_child.
    for (size_t i = 0; i <= BT_HALF_NODE_SIZE; ++i) {
        new_child->children[i] = child->children[i + BT_HALF_NODE_SIZE];
        new_child->keys[i] = child->keys[i + BT_HALF_NODE_SIZE];
    }

    // Set the bookkeeping straight.
    new_child->num_children = BT_HALF_NODE_SIZE + 1;
    child->num_children = BT_HALF_NODE_SIZE;

    // Shift children in the parents, to insert the new_child in the correct
    // position.
    parent->num_children += 1;
    for (size_t i = parent->num_children - 1; i > child_index; --i) {
        parent->children[i] = parent->children[i - 1];
    }
    for (size_t i = parent->num_children - 2; i > child_index; --i) {
        parent->keys[i] = parent->keys[i - 1];
    }

    // Insert the new child in the correct position and bubble up its first
    // key to the parent.
    parent->children[child_index + 1] = new_child_node;
    parent->keys[child_index] = new_child->keys[0];

    return 0;
}

/*
 * Given a parent and a child BtLeafNode, split the child into two nodes, and
 * update the parent to keep track of this information.
 *
 * @parent_node: Parent node. Must be a BtInternalNode.
 * @child_node: Child node to be split. Must be a BtLeafNode.
 * @child_index: Index of the child to be split.
 *
 * Returns 0 if splitting is successful, and -1 otherwise.
 */
int bt_split_leaf_child(BtNode* parent_node, BtNode* child_node,
                        size_t child_index) {
    assert(child_node->type == BT_LEAF);
    assert(parent_node->type == BT_INTERNAL);

    BtNode* new_child_node = bt_allocate_leaf_node();
    if (new_child_node == NULL) {
        return -1;
    }

    // Fetch internal/leaf nodes for elements which will participate in
    // splitting.
    BtLeafNode* child = get_bt_leaf_node(child_node);
    BtInternalNode* parent = get_bt_internal_node(parent_node);
    BtLeafNode* new_child = get_bt_leaf_node(new_child_node);

    // Transfer the last half of records from child to new_child.
    for (size_t i = 0; i <= BT_HALF_NODE_SIZE; ++i) {
        new_child->records[i] = child->records[i + BT_HALF_NODE_SIZE];
    }

    // Set the bookkeeping straight.
    new_child->num_records = BT_HALF_NODE_SIZE + 1;
    child->num_records = BT_HALF_NODE_SIZE;

    // Shift children in the parents, to insert the new_child in the correct
    // position.
    parent->num_children += 1;
    for (size_t i = parent->num_children - 1; i > child_index; --i) {
        parent->children[i] = parent->children[i - 1];
    }
    for (size_t i = parent->num_children - 2; i > child_index; --i) {
        parent->keys[i] = parent->keys[i - 1];
    }

    // Insert the new child in the correct position and bubble up its first
    // val to the parent.
    parent->children[child_index + 1] = new_child_node;
    parent->keys[child_index] = new_child->records[0].val;

    return 0;
}

/*
 * Given a parent and a child BtNode, split the child into two nodes, and
 * update the parent to keep track of this information.
 *
 * @parent: Parent node. Must be a BtInternalNode.
 * @child: Child node to be split.
 * @child_index: Index of the child to be split.
 *
 * Returns 0 if splitting is successful, and -1 otherwise.
 */
inline extern int bt_split_child(BtNode* parent, BtNode* child,
                                 size_t child_index) {
    assert(parent->type == BT_INTERNAL);

    switch (child->type) {
        case BT_LEAF:
            return bt_split_leaf_child(parent, child, child_index);
        case BT_INTERNAL:
            return bt_split_internal_child(parent, child, child_index);
    }
}

/*
 * Routine for inserting a BtPair into a BtNode that is not full, and that is
 * specifically a BtLeafNode.
 *
 * @node: BtNode in which to insert data. Must be a BtLeafNode.
 * @val: Value we want to insert.
 *
 * Returns 0 on a successful insertion, and -1 otherwise.
 */
int bt_insert_leaf_non_full(BtNode* node, BtDataType val) {
    assert(node->type == BT_LEAF);

    BtLeafNode* leaf = get_bt_leaf_node(node);
    size_t i = leaf->num_records;

    // Find the index of the position within the leaf where we must insert,
    // shifting the positions until we get the right one.
    while (i >= 1 && val < leaf->records[i - 1].val) {
        leaf->records[i] = leaf->records[i - 1];
        --i;
    }

    // FIXME: Find about ids here. How do we add these in? Maybe we should
    // be trying to insert a BtPair instead.
    leaf->records[i].val = val;
    leaf->records[i].id = 0;

    // Fix bookkeeping.
    leaf->num_records += 1;
    return 0;
}

/*
 * Routine for inserting a BtPair into a BtNode that is not full, and that is
 * specifically a BtInternalNode.
 *
 * @node: BtNode in which to insert data. Must be a BtInternalNode.
 * @val: Value we want to insert.
 *
 * Returns 0 on a successful insertion, and -1 otherwise.
 */
int bt_insert_internal_non_full(BtNode* node, BtDataType val) {
    assert(node->type == BT_INTERNAL);
    BtInternalNode* internal = get_bt_internal_node(node);

    assert(internal->num_children >= 1);
    size_t i = internal->num_children - 1;

    // Find the index of the child where we want to insert, by comparing
    // them with the keys.
    while (i >= 1 && val < internal->keys[i - 1]) {
        --i;
    }

    BtNode* child = internal->children[i];
    if (is_bt_node_full(child)) {
        // Child is full! We must split it.
        int err = bt_split_child(node, child, i);
        if (err) {
            return -1;
        }

        // Case where splitting makes the current key go to the new splitted
        // child, and not the one we had already.
        if (val > internal->keys[i - 1]) {
            child = internal->children[i + 1];
        }
    }

    // Finish off by inserting into this child, which we now know has
    // space for it.
    return bt_insert_non_full(child, val);
}

/*
 * Inserts a BtPair in a node of the BTree, knowing that this node is
 * not full. This invariant makes insertions possible in a single pass.
 */
inline extern int bt_insert_non_full(BtNode* node, BtDataType val) {
    switch (node->type) {
        case BT_LEAF:
            return bt_insert_leaf_non_full(node, val);
        case BT_INTERNAL:
            return bt_insert_internal_non_full(node, val);
    }
    return 0;
}

/*
 * Inserts a BtPair in the BTree.
 */
int bt_insert(BtNode** root, BtDataType val) {
    assert(root != NULL);

    // Check if the root is full. If it is not, simply insert using
    // bt_insert_non_full, which can handle this case.
    if (!is_bt_node_full(*root)) {
        return bt_insert_non_full(*root, val);
    }

    // The root node is full, so we need a new root.
    BtNode* new_root = bt_allocate_internal_node();
    if (new_root == NULL) {
        return -1;
    }

    // Set the old root as the new node's child.
    BtNode* old_root = *root;
    BtInternalNode* new_root_internal = get_bt_internal_node(new_root);
    new_root_internal->children[0] = old_root;
    new_root_internal->num_children = 1;

    // Set the new root as the actual root.
    *root = new_root;

    // Split the old_root with the new_root.
    int err = bt_split_child(new_root, old_root, 0);
    if (err) {
        return -1;
    }

    // Finish off by inserting the value into the leaf, which we now know
    // can fit it (because we have just split it).
    return bt_insert_non_full(old_root, val);
}

/*
 * Deletes a BtPair from the BTree.
 */
int bt_delete(BtNode** root, BtPair* pair) {
    (void)root;
    (void)pair;
    return 0;
}

/*
 * Creates an empty BTree and returns it.
 */
BtNode* bt_create() { return bt_allocate_leaf_node(); }

/*
 * Helper function to print the BTree. Only used for debugging.
 */
void bt_print(BtNode* node) {
    if (node == NULL) {
        return;
    }
    switch (node->type) {
        case BT_LEAF: {
            BtLeafNode* leaf = get_bt_leaf_node(node);
            printf("LEAF: ");
            for (size_t i = 0; i < leaf->num_records; ++i) {
                printf("%ld, ", leaf->records[i].val);
            }
            printf("\n");
            break;
        }
        case BT_INTERNAL: {
            BtInternalNode* internal = get_bt_internal_node(node);
            printf("INTERNAL: ");
            for (size_t i = 1; i < internal->num_children; ++i) {
                printf("%ld, ", internal->keys[i - 1]);
            }
            printf("\n");
            for (size_t i = 0; i < internal->num_children; ++i) {
                bt_print(internal->children[i]);
            }
            printf("END OF INTERNAL\n");
            break;
        }
    }
}
