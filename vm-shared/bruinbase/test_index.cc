#include "BTreeNode.h"
#include "BTreeIndex.h"
#include "string.h"
#include "stdio.h"
#include <vector>

using namespace std;

int test_locate(){
    printf("\n TEST_LOCATE() \n");

    // manually create several nodes which will compose the test index

    RC status ;
    PageFile pf;
    string index_filename = "test.idx";
    int rootPid = 0;
    status = pf.open(index_filename,'w');
    
    BTNonLeafNode root;
    root.initializeRoot(1, 50, 2);
    status = root.write(rootPid,pf);

    BTNonLeafNode leftChild;
    leftChild.initializeRoot(3,25,4);
    status = leftChild.write(1,pf);
    
    BTNonLeafNode rightChild;
    rightChild.initializeRoot(5,75,6);
    status = rightChild.write(2,pf);

    BTLeafNode firstLeaf;
    RecordId rid;
    rid.pid = 99 ;
    rid.sid = 20;

    firstLeaf.insert(5,rid);
    firstLeaf.insert(10,rid);
    firstLeaf.setNextNodePtr(4);
    status = firstLeaf.write(3,pf);

    BTLeafNode secondLeaf;
    secondLeaf.insert(35, rid);
    secondLeaf.insert(45, rid);
    secondLeaf.setNextNodePtr(5);
    status = secondLeaf.write(4,pf);
    
    BTLeafNode thirdLeaf;
    thirdLeaf.insert(60, rid);
    thirdLeaf.insert(70, rid);
    thirdLeaf.setNextNodePtr(6);
    status = thirdLeaf.write(5, pf);
    
    BTLeafNode fourthLeaf;
    fourthLeaf.insert(80, rid);
    fourthLeaf.insert(100, rid);
    fourthLeaf.setNextNodePtr(-1);
    status = fourthLeaf.write(6, pf);

    // TODO: make the index be composed of these nodes.
    BTreeIndex index;
    index.open(index_filename, 'r');
    int height = 3;
    int newHeight = index.setTreeHeight(height);
    printf("the height of the tree is %i\n", newHeight);

    IndexCursor cursor;
    cursor.pid = rootPid;

    // TODO: test locate() on an existing key
    cursor.eid = 0;
    printf("the key 45 is located at {pid: 4, eid: 1}\n");

    vector<int> parents;
    status = index.locate(45, cursor, ROOT_DEPTH, parents);
    print_pids(parents);
    if (status == 0)
        printf("found key 45 at {pid: %i, eid: %i}\n", cursor.pid, cursor.eid);
    else
        printf("locate() failed with error code %i\n", status);

    // TODO: test locate() on a key which does not exist
    

}

int test_insert() {
    // TODO: create a new index
    
    // TODO: insert a single value, test locate and readForward on it
    
    // TODO: insert several values, test locate and readForward on them
}

int main() {
    test_locate();
}
