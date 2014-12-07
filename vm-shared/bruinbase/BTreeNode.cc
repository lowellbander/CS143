#include "BTreeNode.h"
#include <string.h>

using namespace std;

#define ENTRY_OFFSET sizeof(PageId)

BTLeafNode::BTLeafNode():maxKeyCount(((PageFile::PAGE_SIZE) - sizeof(PageId))/sizeof(Entry))
{
    memset(buffer, 0, PageFile::PAGE_SIZE);
}

void BTLeafNode::showEntries() {
    printf("\nHere are the entries in this node:\n");
    int nKeys = getKeyCount();
    int i = 0;
    for(Entry* current = (Entry*) buffer; i < nKeys; ++current, ++i) {

        int key = -1;
        RecordId rid;

        //TODO: check return value
        readEntry(i, key, rid);

        printf("element #%i: {key: %i, rid: {pid: %i, sid: %i}}\n", i, key, rid.pid, rid.sid);
        
    }

}

int BTLeafNode::getMaxKeyCount() {
    return maxKeyCount;
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)
{
    return pf.read(pid, buffer);
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::write(PageId pid, PageFile& pf)
{ 
    return pf.write(pid, buffer);
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTLeafNode::getKeyCount()
{ 
    int num = 0;
    //printf("counting\n");
    for(Entry* current = (Entry*) buffer; (current->key) != 0 && num < maxKeyCount; current++, num++) {
        //printf("element #%i: {key: %i, rid: {pid: %i, sid: %i}}\n", num, (*current).key, (*current).rid.pid, (*current).rid.sid);
        
    }

    return num;
}

/*
 * Insert a (key, rid) pair to the node.
 * @param key[IN] the key to insert
 * @param rid[IN] the RecordId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTLeafNode::insert(int key, const RecordId& rid)
{ 
    //printf("beginning insert(); keycount is %i\n", getKeyCount());
    //check if we have enough space for new node
    if(getKeyCount() + 1 > maxKeyCount)
        return RC_NODE_FULL;

    //we have space, let's continue
    int eid = -1;

    int status;
    if(status = locate(key,eid)){
        //key not found
        return status;
    }
    else{
       // printf("eid: %i\n", eid);
        //shift stuff to the right
        int nKeys = getKeyCount();
       // printf("nKeys: %i\n", nKeys);

        //TODO: edge case: first insertion
        for (int i = nKeys; i > eid; --i) {
        //    printf("looping ");
            Entry* current = (Entry*) buffer + i; // points to last entry
            *current = *(current - 1);
        }
      //  printf("\n");

        //insert key at new space
        Entry* newEntry = (Entry*) buffer + eid;
        (*newEntry).key = key;
        (*newEntry).rid = rid;

    //    printf("done with insertion\n");
    //    printf("getKeyCount() returns %i after insertion\n", getKeyCount());
    //    printf("newEntry: {key: %i, rid: {pid: %i, sid: %i}}\n", (*newEntry).key, (*newEntry).rid.pid, (*newEntry).rid.sid);

        return 0;
    }    
 }

/*
 * Insert the (key, rid) pair to the node
 * and split the node half and half with sibling.
 * The first key of the sibling node is returned in siblingKey.
 * @param key[IN] the key to insert.
 * @param rid[IN] the RecordId to insert.
 * @param sibling[IN] the sibling node to split with. This node MUST be EMPTY when this function is called.
 * @param siblingKey[OUT] the first key in the sibling node after split.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::insertAndSplit(int key, const RecordId& rid, 
                              BTLeafNode& sibling, int& siblingKey)
{ 
    if(sibling.getKeyCount() > 0)
        return -1; //Should be empty according to spec

    int keyCount = getKeyCount();
    int splitPoint = (keyCount + 1)/2;
    
    int eid = -1;
    locate(key, eid);
    
    if(eid == splitPoint)
        siblingKey = key;
     else
        siblingKey = ((Entry*) buffer + splitPoint)->key;
    
    //move everything from splitPoint to sibling
    for(int i = splitPoint ; i < keyCount ; i++)
    {
        Entry *cur = (Entry *)buffer + i;
        sibling.insert(cur->key, cur->rid);

        cur->key = 0;
        cur->rid.sid = cur->rid.pid = 0;          
    }

    
    //key should be inserted to orig/sibling node based on eid 
    if(eid < splitPoint)
        insert(key, rid);
    else
        sibling.insert(key, rid);

    return 0; 
}

/*
 * Find the entry whose key value is larger than or equal to searchKey
 * and output the eid (entry number) whose key value >= searchKey.
 * Remeber that all keys inside a B+tree node should be kept sorted.
 * @param searchKey[IN] the key to search for
 * @param eid[OUT] the entry number that contains a key larger than or equalty to searchKey
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::locate(int searchKey, int& eid)
{
    Entry *current = (Entry *) buffer;
    int nKeys = getKeyCount();
    //printf("searching for %i\n", searchKey);
    
    for(eid = 0; eid < nKeys; ++eid, ++current) {
      //  printf("current->key: %i\n", current->key);
        if((current->key) >= searchKey) {
            //printf("found! current->key: %i\n", current->key);
            return 0;
            
        }
    } 
    // if all keys are strictly larger than searchKey, 
    // eid shall point to the beginning of the array.
    //printf("NOT FOUND\n");
    
    return 0;
}

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid) {

    // check for leading null-byte
    if (!*buffer){
        printf("Leading null-byte: %i\n", *buffer );
        return -1;
    }

    Entry* e = (Entry*) buffer + eid;
    
    key = e->key;
    rid = e->rid;

    return 0;
}

/*
 * Return the pid of the next slibling node.
 * @return the PageId of the next sibling node 
 */
PageId BTLeafNode::getNextNodePtr()
{ 
    PageId *ptr = (PageId*)(buffer + maxKeyCount*sizeof(Entry));
    return *ptr;
}

/*
 * Set the pid of the next slibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::setNextNodePtr(PageId pid)
{ 
    if(pid < 0 ) return RC_INVALID_PID;
    PageId *ptr = (PageId*)(buffer + maxKeyCount * sizeof(Entry));
    *ptr = pid;
    //TODO: Possible failures?
    RC rc = 0;
    return rc;
}


/*================================================
*                 NON-LEAF
* ================================================
* 
* Buffer structure:
* ------------------------------------------------
*| keyCount | PageId | Entry(Key, PageId) ... ... |
* ------------------------------------------------
*/ 
void BTNonLeafNode::showEntries() {
    printf("\nHere are the (key,pid) pairs:\n");
    int nKeys = getKeyCount();
    int i = 0;
    for (Entry* e = (Entry*)(buffer+ENTRY_OFFSET); i < nKeys + 1; ++e, ++i) {
        printf("entry #%i: {key: %i, pid: %i}\n", i, e->key, e->pid);
    }
}

void BTNonLeafNode::showEntriesWithFirstPageId(){
    PageId* ptr = (PageId *)buffer;
    printf("\nFirst pid: %i",*ptr);
    showEntries();
}
/*
 maxKeyCount = PageFile::PAGE_SIZE - PageId at the end - keyCount - One entry reserved for split
*/
BTNonLeafNode::BTNonLeafNode():maxKeyCount(((PageFile::PAGE_SIZE) - sizeof(PageId)-sizeof(int) - sizeof(Entry))/sizeof(Entry))
{
    memset(buffer, 0, PageFile::PAGE_SIZE);
    keyCount = 0;
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::read(PageId pid, const PageFile& pf){
    
    RC rc =  pf.read(pid, buffer);
    if(rc != 0) return rc;
    
    memcpy(&keyCount, buffer, sizeof(int));
    return 0;
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::write(PageId pid, PageFile& pf){
    memcpy(buffer, &keyCount, sizeof(int));
    RC rc = pf.write(pid, buffer); 
    
    return rc;
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount() {
    int nKeys = 0;
    for (Entry* current = (Entry*) buffer; 
            current->key != 0 && nKeys < maxKeyCount;
            ++current, ++nKeys) {}
    //TODO: 0 keys ?
    return nKeys;
}


/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid){
    if(getKeyCount() + 1 > maxKeyCount)
        return RC_NODE_FULL;

   return insertWithoutSizeCheck(key,pid);
}

RC BTNonLeafNode::insertWithoutSizeCheck(int key, PageId pid) { 
    Entry *entries = (Entry*) (buffer+ENTRY_OFFSET);

    //find the index where the (key, pid) Entry should be inserted
    int nKeys = getKeyCount();    
    int index;
    for (index = 0; index < nKeys; ++index) {
        if (entries[index].key >= key)
            break;
    }

    // shift over the entries to the right of this 
    
    for (int i = nKeys; i > index; --i) {
        printf("shifting ");
        Entry* e = (Entry*)(buffer+ENTRY_OFFSET) + i;
        *e = *(e - 1);
    }
    printf("\n");

    // insert the entry
        
    entries[index].key = key;
    entries[index].pid = pid;

    //Entry* newEntry = (Entry*) buffer + index;
    //(*(newEntry+1)).key = key;
    //(*newEntry).pid = pid;

    return 0;

    //TODO: make sure the last pid is always up-to-date
}

/*
 * Insert the (key, pid) pair to the node
 * and split the node half and half with sibling.
 * The middle key after the split is returned in midKey.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @param sibling[IN] the sibling node to split with. This node MUST be empty when this function is called.
 * @param midKey[OUT] the key in the middle after the split. This key should be inserted to the parent node.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::insertAndSplit(int key, PageId pid, BTNonLeafNode& sibling, int& midKey){ 

    int nkeys = getKeyCount();
    //safe because we set maxKeyCount by reserving one spot for split
    insertWithoutSizeCheck(key, pid);
    
    
    Entry* entries = (Entry *)(buffer + ENTRY_OFFSET);
    int splitPoint = (nkeys+1)/2;
    midKey = (entries+splitPoint)->key;
    printf("Midkey: %i\n", midKey);    
    sibling.initializeRoot((entries+splitPoint)->pid, (entries+splitPoint+1)->key, (entries+1+splitPoint)->pid);
    //delete the midkey
    (entries+splitPoint)->key = (entries+splitPoint)->pid = 0;
    for(int i=splitPoint+1; i<nkeys; i++){
        Entry* cur = (Entry*)(buffer+ENTRY_OFFSET) + i;
        sibling.insert(cur->key, cur->pid);
        
        cur->key = cur->pid = 0;
      }
   
    return 0; 
}

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid) { 

    // TODO: edge case: where the element we're looking for is in the
    // right most pointer

    Entry *current = (Entry*) buffer;
    int nKeys = getKeyCount();
    for (pid = 0; pid < nKeys; ++pid, ++current) {
        if (current->key > searchKey)
            return 0;
    }
    // didn't find it, need to return last pid
    pid = (current + nKeys - 1)->pid;
    return 0;

    //TODO: errors

}

/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{ 
    memset(buffer, 0, PageFile::PAGE_SIZE);
    
    if(buffer == NULL)
        return -1;

    PageId *ptr = (PageId*)buffer;
    *ptr = pid1;
    Entry* firstEntry = (Entry *)(buffer+ENTRY_OFFSET);
    firstEntry->key = key;
    firstEntry->pid = pid2;
    
    return 0;
}

