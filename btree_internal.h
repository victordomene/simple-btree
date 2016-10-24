#ifndef BTREE_INTERNAL_GUARD_
#define BTREE_INTERNAL_GUARD_

#include "btree.h"

#define BT_NODE_SIZE 5
#define BT_HALF_NODE_SIZE 2

/*
 * Defines the type of node in the BTree.
 */
typedef enum { BT_INTERNAL, BT_LEAF } BtNodeType;

/*
 * Defines a leaf node in the BTree, which is simply a collection of records
 * and some bookkeeping information.
 *
 * FIXME: Add a "next" field to perform better for selects.
 */
typedef struct {
    size_t num_records; // Number of records in this leaf.
    BtPair records[BT_NODE_SIZE]; // Records in this leaf.
} BtLeafNode;

/*
 * Defines an internal node in the BTree. This is a collection of keys and
 * children, as well as some bookkeeping.
 *
 * If A is the i-th children, then keys[i] will be larger or equal to any
 * element in A, which is smaller or equal to any element in the j-th children,
 * where j > i. This just means the keys define the ranges of the children.
 */
typedef struct {
    size_t num_children; // Number of children in this internal node.
    long keys[BT_NODE_SIZE - 1]; // Keys dividing ranges of children.
    BtNode* children[BT_NODE_SIZE]; // Children in this internal node.
} BtInternalNode;

/*
 * Union that allows us to index into the tree and differentiate between
 * a leaf and an internal node. Used to mimic abstract types.
 */
typedef union {
    BtLeafNode* leaf;
    BtInternalNode* internal;
} BtNodeContents;

/*
 * Defines a node i the BTree. It contains a type, which can be INTERNAL or
 * LEAF, and contains a union which can be used to index into the appropriate
 * node contents.
 */
struct BtNode {
    BtNodeType type; // Type of this BtNode.
    BtNodeContents contents; // Union that can be used to access contents.
};

/*
 * Declaration of internal functions.
 */

BtNode* bt_allocate_leaf_node();
BtNode* bt_allocate_internal_node();

inline extern int bt_insert_non_full(BtNode* node, BtDataType val);
int bt_insert_leaf_non_full(BtNode* node, BtDataType val);
int bt_insert_internal_non_full(BtNode* node, BtDataType val);

inline extern int bt_split_child(BtNode* parent, BtNode* child,
                                 size_t child_index);
int bt_split_internal_child(BtNode* parent_node, BtNode* child_node,
                            size_t child_index);
int bt_split_leaf_child(BtNode* parent_node, BtNode* child_node,
                        size_t child_index);

#endif  // BTREE_INTERNAL_GUARD_
