#include "catalog.h"
#include "query.h"


/*
 * Inserts a record into the specified relation.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

const Status QU_Insert(const string & relation, 
	const int attrCnt, 
	const attrInfo attrList[])
{

    Status status;
    int relAttrCnt;
    AttrDesc* relAttrs;

    // open InsertFileScan object to search for provided relation
    InsertFileScan resultRel(relation, status);
    if(status != OK)
        return status;

    // get catalog info
    status = attrCat->getRelInfo(relation, relAttrCnt, relAttrs);
    if(status != OK)
        return status;

    // find the total record size
    int recLen = 0;
    for(int i = 0; i < relAttrCnt; i++)
        recLen += relAttrs[i].attrLen;

    // allocate pointer with size equal to total record size
    char outputData[recLen];
    Record outputRec;
    outputRec.data = (void*) outputData;
    outputRec.length = recLen;

    for(int i = 0; i < attrCnt; i++) {

        // looking for matches
        for(int j = 0; j < relAttrCnt; j++) {

            // check if attribute name is equal
            if(strcmp(attrList[i].attrName, relAttrs[j].attrName) == 0) {

                // don't allow null values
                if(attrList[i].attrValue == NULL)
                    return ATTRTYPEMISMATCH;

                char* actualAttrValue;

                // change to proper attribute type
                int type = attrList[i].attrType;
                if(type == INTEGER) {
                    int tempInt;
                    tempInt = atoi((char*) attrList[i].attrValue);
                    actualAttrValue = (char*) &tempInt;
                }
                else if(type == FLOAT) {
                    float tempFloat;
                    tempFloat = atof((char*) attrList[i].attrValue);
                    actualAttrValue = (char*) &tempFloat;
                }
                else if(type == STRING) {
                    actualAttrValue = (char*) attrList[i].attrValue;
                }

                // match found, hard copy data into output record
                memcpy(outputData + relAttrs[j].attrOffset, actualAttrValue, relAttrs[j].attrLen);
            }

        }

    }

    RID outRID;

    // record created, insert into table
    status = resultRel.insertRecord(outputRec, outRID);
    if(status != OK)
        return status;

    return OK;

}

