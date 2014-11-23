#include "BTreeNode.h"
#include "string.h"
#include "stdio.h"
using namespace std;

int main() {
    printf("\n");

    RC status;

    // make a new node

    BTLeafNode leaf;
    //leaf.showBuffer();

    // get a handle for the recordfile for small.tbl

    RecordFile rf;
    status = rf.open("xsmall.tbl", 'w');
    printf("rf.open() returned %i\n", status);

    // insert an entry into the node using the first record in the table

    int key;
    string value;

    RecordId rid;
    RecordId last = rf.endRid();
    printf("last rid is {pid: %i, sid: %i}\n", last.pid, last.sid);

    printf("\n--------------------------------------------------\n");

    const int numInserts = 8;

    for (int i = 0; i < numInserts; ++i) {
        printf("\n");

        rid.pid = (PageId) 0;
        rid.sid = i;

        status = rf.read(rid, key, value);
        printf("rf.read() returned %i\n", status);
        printf("prepping insert of {%i, '%s'}\n", key, value.c_str());

        status = leaf.insert(key, rid);
        printf("leaf.insert() returned %i\n", status);
        //leaf.showBuffer();
        printf("\n--------------------------------------------------\n");
    }

    // somehow test that it was inserted? maybe by calling locate with the inserted key
    // and with a key that we have not inserted.

    // we can do more complete testing when we have implemented readEntry
    printf("\n");
}
