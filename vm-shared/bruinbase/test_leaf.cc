#include "BTreeNode.h"
#include "string.h"
using namespace std;

int main() {
    RC status;

    // make a new node

    BTLeafNode leaf;

    // get a handle for the recordfile for small.tbl

    RecordFile rf;
    status = rf.open("small.tbl", 'w');

    // insert an entry into the node using the first record in the table

    int key;
    string value;

    RecordId rid;
    rid.pid = (PageId) 0;
    rid.sid = 0;

    status = rf.read(rid, key, value);

    

    // somehow test that it was inserted? maybe by calling locate with the inserted key
    // and with a key that we have not inserted.

    // we can do more complete testing when we have implemented readEntry
}
