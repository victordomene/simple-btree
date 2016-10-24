#include <assert.h>
#include <stdio.h>

#include "btree.h"

int main() {
    BtNode* root = bt_create();
    int err = bt_insert(&root, (BtDataType) 5);
    err = bt_insert(&root, (BtDataType) 10);
    err = bt_insert(&root, (BtDataType) 20);
    err = bt_insert(&root, (BtDataType) 1);
    err = bt_insert(&root, (BtDataType) -5);
    err = bt_insert(&root, (BtDataType) 4);
    err = bt_insert(&root, (BtDataType) 3);
    err = bt_insert(&root, (BtDataType) -50);
    err = bt_insert(&root, (BtDataType) 7);
    err = bt_insert(&root, (BtDataType) 13);
    err = bt_insert(&root, (BtDataType) -51);
    err = bt_insert(&root, (BtDataType) 4);
    err = bt_insert(&root, (BtDataType) 3);
    err = bt_insert(&root, (BtDataType) 1);
    err = bt_insert(&root, (BtDataType) 765);
    err = bt_insert(&root, (BtDataType) 12);
    err = bt_insert(&root, (BtDataType) 98);
    err = bt_insert(&root, (BtDataType) 49);
    err = bt_insert(&root, (BtDataType) 25);
    err = bt_insert(&root, (BtDataType) 68);
    err = bt_insert(&root, (BtDataType) 121);
    err = bt_insert(&root, (BtDataType) 31);
    err = bt_insert(&root, (BtDataType) 1711);
    err = bt_insert(&root, (BtDataType) -111);
    err = bt_insert(&root, (BtDataType) -231);
    err = bt_insert(&root, (BtDataType) 5);
    err = bt_insert(&root, (BtDataType) 20);
    err = bt_insert(&root, (BtDataType) 21);
    err = bt_insert(&root, (BtDataType) 919819);
    err = bt_insert(&root, (BtDataType) -1);
    err = bt_insert(&root, (BtDataType) 53);
    err = bt_insert(&root, (BtDataType) 124);
    err = bt_insert(&root, (BtDataType) -8654);
    err = bt_insert(&root, (BtDataType) -842);
    err = bt_insert(&root, (BtDataType) -962);
    err = bt_insert(&root, (BtDataType) 13813);
    err = bt_insert(&root, (BtDataType) 6423);
    err = bt_insert(&root, (BtDataType) 5244213);
    err = bt_insert(&root, (BtDataType) -5642);
    err = bt_insert(&root, (BtDataType) -112);
    err = bt_insert(&root, (BtDataType) -333);
    err = bt_insert(&root, (BtDataType) 453144);
    err = bt_insert(&root, (BtDataType) 2);
    err = bt_insert(&root, (BtDataType) 134231234);
    err = bt_insert(&root, (BtDataType) 12);
    err = bt_insert(&root, (BtDataType) -1);
    bt_print(root);
    return 0;
}
