/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeIndex.h"

using namespace std;

// external functions and variables for load file and sql command parsing 
extern FILE* sqlin;
int sqlparse(void);


RC SqlEngine::run(FILE* commandline)
{
  fprintf(stdout, "Bruinbase> ");

  // set the command line input and start parsing user input
  sqlin = commandline;
  sqlparse();  // sqlparse() is defined in SqlParser.tab.c generated from
               // SqlParser.y by bison (bison is GNU equivalent of yacc)

  return 0;
}

RC SqlEngine::select(int attr, const string& table, const vector<SelCond>& cond)
{
    RecordFile rf;       // RecordFile containing the table
    RecordId   rid;      // record cursor for table scanning
    BTreeIndex bti;      // BTree Index for iterating through the index
    IndexCursor cursor;  // cursor for scanning index contents
    
    RC     rc;
    int    key;
    string value;
    int    count;
    int    diff;
    int    index;
    
    // open the table file
    if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
        fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
        return rc;
    }
    
    count = 0;
    if (bti.open(table + ".idx", 'r')) {
        // scan the table file from the beginning
        rid.pid = rid.sid = 0;
        while (rid < rf.endRid()) {
            // read the tuple
            if ((rc = rf.read(rid, key, value)) < 0) {
                fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
                goto exit_select;
            }
            
            // check the conditions on the tuple
            for (unsigned i = 0; i < cond.size(); i++) {
                // compute the difference between the tuple value and the condition value
                switch (cond[i].attr) {
                    case 1:
                        diff = key - atoi(cond[i].value);
                        break;
                    case 2:
                        diff = strcmp(value.c_str(), cond[i].value);
                        break;
                }
                
                // skip the tuple if any condition is not met
                switch (cond[i].comp) {
                    case SelCond::EQ:
                        if (diff != 0) goto next_tuple;
                        break;
                    case SelCond::NE:
                        if (diff == 0) goto next_tuple;
                        break;
                    case SelCond::GT:
                        if (diff <= 0) goto next_tuple;
                        break;
                    case SelCond::LT:
                        if (diff >= 0) goto next_tuple;
                        break;
                    case SelCond::GE:
                        if (diff < 0) goto next_tuple;
                        break;
                    case SelCond::LE:
                        if (diff  > 0) goto next_tuple;
                        break;
                }
            }
            
            // the condition is met for the tuple.
            // increase matching tuple counter
            count++;
            
            // print the tuple
            switch (attr) {
                case 1:  // SELECT key
                    fprintf(stdout, "%d\n", key);
                    break;
                case 2:  // SELECT value
                    fprintf(stdout, "%s\n", value.c_str());
                    break;
                case 3:  // SELECT *
                    fprintf(stdout, "%d '%s'\n", key, value.c_str());
                    break;
            }
            
            // move to the next tuple
        next_tuple:
            ++rid;
        }
    } else {
        index = -1;
        for (unsigned i = 0; i < cond.size(); i++) {
            // Skip value columns
            if (cond[i].attr != 1)
                continue;
            
            if (cond[i].comp == SelCond::EQ) {
                index = i;
                break;
            }
            
            if ((cond[i].comp == SelCond::GT || cond[i].comp == SelCond::GE) &&
                (index == -1 || atoi(cond[i].value) > atoi(cond[index].value)))
                index = i;
        }
        
        vector<PageId> empty_parents;
        cursor.pid = bti.getRootPid();
        if (index > -1)
            bti.locate(atoi(cond[index].value), cursor, ROOT_DEPTH, empty_parents);
        else
            bti.locate(0, cursor, ROOT_DEPTH, empty_parents);
        
        while ((bti.readForward(cursor, key, rid))  == 0) {
            
            // read the tuple
            if ((rc = rf.read(rid, key, value)) < 0) {
                fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
                goto exit_select;
            }
            
            // check the conditions on the tuple
            for (unsigned i = 0; i < cond.size(); i++) {
                // compute the difference between the tuple value and the condition value
                switch (cond[i].attr) {
                    case 1:
                        diff = key - atoi(cond[i].value);
                        break;
                    case 2:
                        diff = strcmp(value.c_str(), cond[i].value);
                        break;
                }
                
                // check the condition
                switch (cond[i].comp) {
                    case SelCond::EQ:
                        if (diff != 0) {
                            if (cond[i].attr == 1) goto print_and_exit;
                            else continue;
                        }
                        break;
                    case SelCond::NE:
                        if (diff == 0) continue;
                        break;
                    case SelCond::GT:
                        if (diff <= 0) continue;
                        break;
                    case SelCond::LT:
                        if (diff >= 0) {
                            if (cond[i].attr == 1) goto print_and_exit;
                            else continue;
                        }
                        break;
                    case SelCond::GE:
                        if (diff < 0) continue;
                        break;
                    case SelCond::LE:
                        if (diff > 0) {
                            if (cond[i].attr == 1) goto print_and_exit;
                            else continue;
                        }
                        break;
                }
            }
            
            // the condition is met for the tuple.
            // increase matching tuple counter
            count++;
            
            // print the tuple
            switch (attr) {
                case 1:  // SELECT key
                    fprintf(stdout, "%d\n", key);
                    break;
                case 2:  // SELECT value
                    fprintf(stdout, "%s\n", value.c_str());
                    break;
                case 3:  // SELECT *
                    fprintf(stdout, "%d '%s'\n", key, value.c_str());
                    break;
            }
        }
    }
    
print_and_exit:
    // print matching tuple count if "select count(*)"
    if (attr == 4) {
        fprintf(stdout, "%d\n", count);
    }
    rc = 0;
    
    // close the table file and return
exit_select:
    rf.close();
    return rc;
}
/*
//RC SqlEngine::select(int attr, const string& table, const vector<SelCond>& cond)
//{
//    fprintf(stdout, "In sql select()\n");
//    printf("My name\n");
//    RecordFile rf;   // RecordFile containing the table
//    RecordId   rid;  // record cursor for table scanning
//    BTreeIndex bt_index;
//    IndexCursor i_cursor;
//
//    RC     rc;
//    int    key;     
//    string value;
//    int    count;
//    int    diff;
//
//    // open the table file
//    if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
//    fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
//    return rc;
//    }
//
//    count = 0;
//    rc = bt_index.open(table + ".idx", 'w');
//    if(rc != 0)
//    {
//        // scan the table file from the beginning
//        rid.pid = rid.sid = 0;
//        while (rid < rf.endRid()) {
//        // read the tuple
//        if ((rc = rf.read(rid, key, value)) < 0) {
//          fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
//          goto exit_select;
//        }
//
//        // check the conditions on the tuple
//        for (unsigned i = 0; i < cond.size(); i++) {
//          // compute the difference between the tuple value and the condition value
//          switch (cond[i].attr) {
//          case 1:
//        diff = key - atoi(cond[i].value);
//        break;
//          case 2:
//        diff = strcmp(value.c_str(), cond[i].value);
//        break;
//          }
//
//          // skip the tuple if any condition is not met
//          switch (cond[i].comp) {
//          case SelCond::EQ:
//        if (diff != 0) goto next_tuple;
//        break;
//          case SelCond::NE:
//        if (diff == 0) goto next_tuple;
//        break;
//          case SelCond::GT:
//        if (diff <= 0) goto next_tuple;
//        break;
//          case SelCond::LT:
//        if (diff >= 0) goto next_tuple;
//        break;
//          case SelCond::GE:
//        if (diff < 0) goto next_tuple;
//        break;
//          case SelCond::LE:
//        if (diff > 0) goto next_tuple;
//        break;
//          }
//        }
//
//        // the condition is met for the tuple. 
//        // increase matching tuple counter
//        count++;
//
//        // print the tuple 
//        switch (attr) {
//        case 1:  // SELECT key
//          fprintf(stdout, "%d\n", key);
//          break;
//        case 2:  // SELECT value
//          fprintf(stdout, "%s\n", value.c_str());
//          break;
//        case 3:  // SELECT *
//          fprintf(stdout, "%d '%s'\n", key, value.c_str());
//          break;
//        }
//
//        // move to the next tuple
//        next_tuple:
//        ++rid;
//        }
//    }
//    else{
//        
//        int i = -1;
//        int cond_size = cond.size();
//        
//        for(i=0; i<cond_size; i++){
//            if(cond[i].attr == 1){ //only evaluate key conditions
//                
//                switch(cond[i].comp){
//                    case SelCond::EQ :
//                    {
//                        break;
//                    }
//                    case SelCond::GT :
//                    case SelCond::GE :
//                    {
//                        int flag = 0;
//                        for(int j = i; j<cond_size; j++){
//                            if(i == -1 || atoi(cond[j].value) > atoi(cond[i].value)){
//                                flag = 1;
//                                break;
//                            }
//                        }
//                        if(flag)
//                            break;
//                        
//                    }
//                    default:
//                        continue;
//                }
//            }
//        }
//        
//        //Locate node
//        vector<PageId> empty_parents;
//        i > -1 ? bt_index.locate(atoi(cond[i].value), i_cursor, ROOT_DEPTH, empty_parents) : bt_index.locate(0, i_cursor, ROOT_DEPTH, empty_parents);
//        
//        while(!(bt_index.readForward(i_cursor, key, rid))){
//            
//            rc = rf.read(rid, key, value);
//            if(rc != 0){
//                fprintf(stderr, "rc: %i\n", rc);
//                fprintf(stderr, "Error reading tuple");
//                goto exit_select;
//            }
//            
//            for(int j=0; j<cond_size; j++){
//                if(cond[i].attr == 1){
//                    diff = key - atoi(cond[i].value);
//                }
//                else if(cond[i].attr == 2){
//                    diff = strcmp(value.c_str(), cond[i].value);
//                }
//                else{
//                    fprintf(stderr, "Invalid value for cond attr %i\n", cond[i].attr);
//                    goto exit_select;
//                }
//            }
//            
//            switch(cond[i].comp){
//                case SelCond::EQ:{
//                    if(diff != 0){
//                        if(cond[i].attr == 1)
//                            goto count_star_exit;
//                        continue;
//                    }
//                    break;
//                }
//                case SelCond::NE:{
//                    if(diff == 0)
//                        continue;
//                    break;
//                }
//                case SelCond::GT:{
//                    if(diff <= 0) continue;
//                    break;
//                }
//                case SelCond::LT:{
//                    if(diff >= 0){
//                        if(cond[i].attr == 1)
//                            goto count_star_exit;
//                        continue;
//                    }
//                    break;
//                }
//                case SelCond::GE:{
//                    if(diff < 0)
//                        continue;
//                    break;
//                }
//                case SelCond::LE:{
//                    if(diff > 0){
//                        if(cond[i].attr == 1)
//                            goto count_star_exit;
//                        continue;
//                    }
//                    break;
//                }
//                default:{
//                    fprintf(stderr, "Invalid comp value");
//                    break;
//                }
//            }
//            
//            count++;
//            
//            if(attr == 1){
//                fprintf(stdout, "%d\n", key);
//            }
//            else if(attr == 2){
//                fprintf(stdout, "%s\n", value.c_str());
//            }
//            else if(attr == 3){
//                fprintf(stdout, "%d '%s'\n", key, value.c_str());
//            }
//            else{
//                //Not printing anything because count(*)
//            }
//        }
//        
//        //iterate through conditions
//    }
//
//  // print matching tuple count if "select count(*)"
//count_star_exit:
//  if (attr == 4) {
//    fprintf(stdout, "%d\n", count);
//  }
//  rc = 0;
//
//  // close the table file and return
//  exit_select:
//    rf.close();
//    return rc;
//}
//
 */

RC SqlEngine::load(const string& table, const string& loadfile, bool index)
{
	fprintf(stderr, "In sql load()\n");

        //buffer to read in from loadfile
  	string buffer;

	//file descriptors
	ifstream inputStream;
	RecordFile rf;
        BTreeIndex bt_index;

	//vars for table
	int key;
	string value;
	RecordId rid;

	//open() needs const char*
	inputStream.open(loadfile.c_str(), std::ifstream::in);
	if(inputStream.good())
	{
		RC status = rf.open(table + ".tbl", 'w');
		if(status != 0)
			return status;	
                if(index){
                    status = bt_index.open(table + ".idx",'w');
                    if(status != 0)
                        return status;
                }
		while(inputStream.good())
		{
			getline(inputStream, buffer);
                        if(buffer == "")
                            break;
        
			status = parseLoadLine(buffer, key, value);
			if(status != 0)
			{
				fprintf(stderr, "Couldn't parse line from %s\n", loadfile.c_str());
				break;	
			}	

			status = rf.append(key,value,rid);
			if(status != 0)
			{
				fprintf(stderr, "Couldn't insert key: %i\n",key );
                                break;
			}

                        if(index){
                            status = bt_index.insert(key, rid);
                            if(status != 0){
                                fprintf(stderr, "Couldn't insert into index");
                                break;
                            }
                        }
		}

                inputStream.close();
                status = rf.close();
                if(status != 0){
                    fprintf(stderr, "Error closing recordFile\n");
                    return status;
                }
                if(index){              
                    status = bt_index.close();
                    if(status != 0){
                        fprintf(stderr, "Error closing BTreeIndex\n");
                        return status
;                   }
                }
		return status;	
	}
	else
	{
		fprintf(stderr, "Couldn't open handle for %s\n", loadfile.c_str() );
	}

  return 0;
}

RC SqlEngine::parseLoadLine(const string& line, int& key, string& value)
{
    const char *s;
    char        c;
    string::size_type loc;
    
    // ignore beginning white spaces
    c = *(s = line.c_str());
    while (c == ' ' || c == '\t') { c = *++s; }

    // get the integer key value
    key = atoi(s);

    // look for comma
    s = strchr(s, ',');
    if (s == NULL) { return RC_INVALID_FILE_FORMAT; }

    // ignore white spaces
    do { c = *++s; } while (c == ' ' || c == '\t');
    
    // if there is nothing left, set the value to empty string
    if (c == 0) { 
        value.erase();
        return 0;
    }

    // is the value field delimited by ' or "?
    if (c == '\'' || c == '"') {
        s++;
    } else {
        c = '\n';
    }

    // get the value string
    value.assign(s);
    loc = value.find(c, 0);
    if (loc != string::npos) { value.erase(loc); }

    return 0;
}
