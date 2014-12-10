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

#define ROOT_DEPTH 1

using namespace std;

void print_pids(std::vector<PageId> pids) {
    printf("current list of parent pid's:");
    for (vector<PageId>::const_iterator i = pids.begin(); i != pids.end(); ++i)
        cout << *i << ", ";
    printf("\n");
}

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
int BTreeIndex::setTreeHeight(int height) { return treeHeight = height; }

int BTreeIndex::getRootPid() { return rootPid; }

/*
 * Open the index file in read or write mode.
 * Under 'w' mode, the index file should be created if it does not exist.
 * @param indexname[IN] the name of the index file
 * @param mode[IN] 'r' for read, 'w' for write
 * @return error code. 0 if no error
 */
RC BTreeIndex::open(const string& indexname, char mode) {
    // TODO: set rootPid, treeHeight
    return pf.open(indexname, mode);
}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close() { 
    // TODO: save rootPid, treeHeight to file
    return pf.close(); 
}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid) {
    RC status;

    // TODO: error codes
    // TODO: error if the page is full and cannot hold any more nodes
    // TODO: go through all of this code and be sure we're writing and reading
    // whenever we need to

    //if the index is empty, create a new root and point it to this first
    //key in the B+ tree

    if (rootPid == NULL_PID) {
        printf("empty index . . .\n");
        // create the first leaf node
        BTLeafNode leaf;
        leaf.insert(key, rid);
        PageId leafPid = pf.endPid();
        status = leaf.write(leafPid, pf);
        printf("leaf.write() returned %i. here's our newly created leaf:\n", status);
        leaf.showEntries();
        printf("the new leaf has %i keys\n", leaf.getKeyCount());

        // create the root node, point it to leaf 
        BTNonLeafNode root;
        root.initializeRoot(NULL_PID, key, leafPid);
        rootPid = pf.endPid(); // also sets index::rootPid
        status = root.write(rootPid, pf);

        printf("the leaf's pid is %i\n", leafPid);
        printf("the root's pid is %i\n", rootPid);
        printf("root.write() returned %i. here's our newly created root:\n", status);
        root.showEntriesWithFirstPageId();

        treeHeight = 2; // 1 for root + 1 for leaf
        
        return 0;
    }
    else {
        // the tree is not empty

        // find the leaf that we'd like to insert our key into
        IndexCursor cursor;
        cursor.pid = rootPid;
        cursor.eid = 0;
        vector<PageId> parents;
        status = locate(key, cursor, ROOT_DEPTH, parents);
        // cursor.pid is now the pid of the leaf node that we'd like to insert into
        
        BTLeafNode leaf;
        PageId leafPid = cursor.pid;
        leaf.read(leafPid, pf);
        printf("leaf.getKeyCount(): %i\n", leaf.getKeyCount());

        // if there's room we can insert here
        if (!leaf.isFull()) {
            printf("There's still room in this leaf for insertion()\n");
            //leaf.showEntries();
            status = leaf.insert(key, rid);
            leaf.write(leafPid, pf);
            printf("after insertion:\n");
            leaf.showEntries();
            return status;
        } else {
            printf("LEAF IS FULL! splitting and inserting midkey in parent\n");
            //{
            //    printf("this part of index::insert() not yet implemented\n");
            //    return -1;
            //}
            // TODO: 
            // there isn't room in this leaf, so we need to insert and split,
            // then insert the sibKey into the parent until we can
            // insert into a parent nonleaf node without splitting.

            BTLeafNode sibling;
            int siblingKey = -1;

            leaf.insertAndSplit(key, rid, sibling, siblingKey);
            leaf.write(leafPid, pf);

            // save the sibling to file
            int siblingPid = pf.endPid();
            sibling.write(siblingPid, pf);

            int keyToInsert = siblingKey;
            PageId pidToInsert = siblingPid;

            print_pids(parents);

            for (vector<PageId>::reverse_iterator parent_pid_itr = parents.rbegin();
                    parent_pid_itr != parents.rend(); ++parent_pid_itr) {

                printf("keyToInsert: %i, pidToInsert: %i\n", keyToInsert, pidToInsert);

                BTNonLeafNode parent;
                PageId parentPid = *parent_pid_itr;
                parent.read(parentPid, pf);
                printf("inserting into parent with pid %i\n", parentPid);
                printf("before insert:\n");
                parent.showEntriesWithFirstPageId();

                // check to see if there is room in the parent
                if (!parent.isFull()) {
                    printf("room in parent\n");
                    parent.insert(keyToInsert, pidToInsert);
                    status = parent.write(parentPid, pf);
                    printf("after insert:\n");
                    parent.showEntriesWithFirstPageId();
                    return status;
                }
                else {
                    // no room in parent, 
                    // parent, insert & split, then check for room in parent's
                    // parent until we can simply insert().

                    // NOTE: pibling is a non-gendered term to describe the
                    // sibling of a parent. Credit: Aman Agarwal.
                    BTNonLeafNode pibling;
                    int midKey = -1;
                    parent.insertAndSplit(keyToInsert, pidToInsert, pibling, midKey);
                    parent.write(parentPid, pf);
                    PageId piblingPid = pf.endPid();
                    pibling.write(piblingPid, pf);

                    keyToInsert = midKey;
                    pidToInsert = piblingPid;

                    // check to see whether we need to create a new root
                    if (parentPid == rootPid) {
                        // create and properly initialize new root
                        BTNonLeafNode root;
                        root.initializeRoot(parentPid, keyToInsert, piblingPid);
                        rootPid = pf.endPid(); // updates index.rootPid
                        return root.write(rootPid, pf);
                    }
                }
            }
        }
    }
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
RC BTreeIndex::locate(int searchKey, IndexCursor& cursor, int depth, 
                                                    vector<PageId>& parents) {
    RC status;

    if (depth == treeHeight) {
        // we are at a leaf node

        // read in the node
        BTLeafNode leaf;
        leaf.read(cursor.pid, pf);

        printf("reading leaf with pid %i\n", cursor.pid);
        leaf.showEntries();

        return leaf.locate(searchKey, cursor.eid);
    }
    else {
        // we are a nonleaf, possibly the root.
        
        // read in the node
        
        BTNonLeafNode nonleaf;
        nonleaf.read(cursor.pid, pf);

        if (cursor.pid == rootPid)
            printf("reading root (pid: %i)\n", rootPid);
        else
            printf("reading nonleaf with pid %i\n", cursor.pid);
        nonleaf.showEntriesWithFirstPageId();
        
        // append the pid of this node to the list of parents
        parents.push_back(cursor.pid);

        // continue the search by following the correct pid
        
        nonleaf.locateChildPtr(searchKey, cursor.pid);

        return locate(searchKey, cursor, ++depth, parents);
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

