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
        node.insert(key*10, pid);
    }

    node.insert(95, 11);
    node.showEntriesWithFirstPageId();

    PageFile pf;
    status = pf.open("nonleaf.txt",'w');
    printf("pf.open returned %i\n", status);
    status = node.write(0,pf);
    printf("write returned: %i\n", status);

    BTNonLeafNode newNode;
    status = newNode.read(0,pf);
    newNode.showEntriesWithFirstPageId();

}

int test_root(){
    BTNonLeafNode root;
    
    PageId pid1 = 5;
    PageId pid2 = 1;
    int key = 300;
    root.initializeRoot(pid1, key, pid2);
    root.showEntriesWithFirstPageId();
}

int test_insertAndSplit(){
    RC status;
    BTNonLeafNode left;
    BTNonLeafNode right;
        
    left.initializeRoot(1,10,2);
    left.insert(20, 3);
    left.showEntriesWithFirstPageId();
    right.showEntriesWithFirstPageId();

    int midkey = -1;
    PageId pid = 11;
    left.insertAndSplit(25, pid, right, midkey);

    printf("\nAFTER SPLIT\n");
    left.showEntriesWithFirstPageId();
    right.showEntriesWithFirstPageId(); 
}
int main() {
    printf("\n\n");
    //TODO: key is 0 tests
    test_insert();
    //test_root();
   //  test_insertAndSplit();
    printf("\n\n");
}
