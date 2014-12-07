/*
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "BTreeIndex.h"
#include "BTreeNode.h"

using namespace std;

/*
 * BTreeIndex constructor
 */
BTreeIndex::BTreeIndex() {
    rootPid = -1;
    treeHeight = 0;
}

/* 
 * A function used only when testing to set the height of the B+ tree.
 * @return the new height of the tree
 * */
int BTreeIndex::setTreeHeight(int height) {
    return treeHeight = height;
}

/*
 * Open the index file in read or write mode.
 * Under 'w' mode, the index file should be created if it does not exist.
 * @param indexname[IN] the name of the index file
 * @param mode[IN] 'r' for read, 'w' for write
 * @return error code. 0 if no error
 */
RC BTreeIndex::open(const string& indexname, char mode) {
    return pf.open(indexname, mode);
}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close() {
    return pf.close();
}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid) {

    // TODO: error codes
    // TODO: error if the page is full and cannot hold any more nodes

    //TODO: if the index is empty, create a new root and point it to this first
    //key in the B+ tree

    if (rootPid == NULL_PID) {
        // create the first leaf node
        BTLeafNode leaf;
        leaf.insert(key, rid);
        PageId leafPid = pf.endPid();
        leaf.write(leafPid, pf);

        // create the root node, point it to leaf 
        BTNonLeafNode root;
        root.initializeRoot(NULL_PID, key, leafPid);
        rootPid = pf.endPid(); // also sets index::rootPid
        root.write(rootPid, pf);

        treeHeight = 2; // 1 for root + 1 for leaf
        
        return 0;
    }
    else {
        // the tree is not empty

        // TODO: find the leaf that we'd like to insert our key into
        
    }

    return 0;
}

/*
 * Find the leaf-node index entry whose key value is larger than or 
 * equal to searchKey, and output the location of the entry in IndexCursor.
 * IndexCursor is a "pointer" to a B+tree leaf-node entry consisting of
 * the PageId of the node and the SlotID of the index entry.
 * Note that, for range queries, we need to scan the B+tree leaf nodes.
 * For example, if the query is "key > 1000", we should scan the leaf
 * nodes starting with the key value 1000. For this reason,
 * it is better to return the location of the leaf node entry 
 * for a given searchKey, instead of returning the RecordId
 * associated with the searchKey directly.
 * Once the location of the index entry is identified and returned 
 * from this function, you should call readForward() to retrieve the
 * actual (key, rid) pair from the index.
 * @param key[IN] the key to find.
 * @param cursor[OUT] the cursor pointing to the first index entry
 *                    with the key value.
 * @return error code. 0 if no error.
 */
RC BTreeIndex::locate(int searchKey, IndexCursor& cursor, int depth) {
    RC status;

    if (depth == treeHeight) {
        // we are at a leaf node

        // read in the node
        BTLeafNode leaf;
        leaf.read(cursor.pid, pf);

        printf("reading leaf with pid %i\n", cursor.pid);

        // either:
        // keep looking,
        // or return immediately, you found the key
        // or return immediately, the key does not exist in this tree

        RecordId rid;
        int key;

        status = leaf.readEntry(cursor.eid, key, rid);
        if (status != 0) {
            printf("readEntry() returned %i\n", status);
            return -1;
        }
    
        if (key >= searchKey) 
            return 0; // found what we're looking for
        else if (++cursor.eid == leaf.getKeyCount())
            return RC_NO_SUCH_RECORD;
        else
            return locate(searchKey, cursor, depth);
    }
    else {
        // we are a nonleaf
        
        // read in the node
        
        BTNonLeafNode nonleaf;
        nonleaf.read(cursor.pid, pf);

        printf("reading nonleaf with pid %i\n", cursor.pid);

        // continue the search by following the correct pid
        
        nonleaf.locateChildPtr(searchKey, cursor.pid);

        return locate(searchKey, cursor, ++depth);
    }

}

/*
 * Read the (key, rid) pair at the location specified by the index cursor,
 * and move foward the cursor to the next entry.
 * @param cursor[IN/OUT] the cursor pointing to an leaf-node index entry in the b+tree
 * @param key[OUT] the key stored at the index cursor location.
 * @param rid[OUT] the RecordId stored at the index cursor location.
 * @return error code. 0 if no error
 */
RC BTreeIndex::readForward(IndexCursor& cursor, int& key, RecordId& rid) {
    //cursor = {PageId pid, int eid}

    //read in the leaf node

    BTLeafNode leaf;
    leaf.read(cursor.pid, pf);

    //read the (key, rid) pair into key, rid
    RC status = leaf.readEntry(cursor.eid, key, rid);
    if (status != 0) 
        return status;

    //printf("readForward(): key: %i, rid: {pid: %i, sid: %i} \n", key, rid.pid, rid.sid);

    // move the cursor forward to the next entry
    ++cursor.eid;

    return 0;
}

