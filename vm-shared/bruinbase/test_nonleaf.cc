#include "stdio.h"
#include "BTreeNode.h"

int test_insert() {
    RC status;

    BTNonLeafNode node;

    int key = 0;
    PageId pid = 0;

    //node.locateChildPtr(searchKey, pid);
    //printf("pid is %i\n", pid);

    //status = node.insert(searchKey, pid);
    //printf("insert() returned %i\n", status);

    for (; key < 10; ++key, ++pid) {
        node.insert(key, pid);
    }

    node.showEntries();
}

int main() {
    printf("\n\n");
    test_insert();
    printf("\n\n");
}
