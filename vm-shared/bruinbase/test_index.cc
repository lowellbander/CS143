#include "BTreeNode.h"
#include "BTreeIndex.h"
#include "string.h"
#include "stdio.h"

using namespace std;

int test_locate(){
    printf("\n TEST_LOCATE() \n");

    // create several nodes which will compose the test index

    RC status ;
    PageFile pf;
    string index_filename = "textindex.txt";
    status = pf.open(index_filename,'w');
    
    BTNonLeafNode root;
    root.initializeRoot(1, 50, 2);
    status = root.write(0,pf);

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

    // TODO: test locate() on an existing key
    // TODO: test locate() on a key which does not exist
    

}

int main() {
    test_locate();
}
