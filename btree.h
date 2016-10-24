#ifndef BTREE_GUARD_
#define BTREE_GUARD_

typedef struct BtNode BtNode;
typedef long BtDataType;

/*
 * Defines a pair of data: its id and the value associated with it.
 */
typedef struct {
    size_t id;
    BtDataType val;
} BtPair;

BtNode* bt_create();
int bt_insert(BtNode** root, BtDataType val);
int bt_delete(BtNode** root, BtPair* pair);
void bt_print(BtNode* node);

#endif  // BTREE_GUARD_
