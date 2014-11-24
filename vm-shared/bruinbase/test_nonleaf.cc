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

int test_root(){
    BTNonLeafNode root;
    
    PageId pid1 = 0;
    PageId pid2 = 1;
    int key = 300;
    root.initializeRoot(pid1, key, pid2);
    root.showEntries();
}
int main() {
    printf("\n\n");
    //test_insert();
    test_root();
    printf("\n\n");
}
