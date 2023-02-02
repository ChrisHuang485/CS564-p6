// #include "catalog.h"
// #include "query.h"


// // forward declaration
// const Status ScanSelect(const string & result, 
// 			const int projCnt, 
// 			const AttrDesc projNames[],
// 			const AttrDesc *attrDesc, 
// 			const Operator op, 
// 			const char *filter,
// 			const int reclen);

// /*
//  * Selects records from the specified relation.
//  *
//  * Returns:
//  * 	OK on success
//  * 	an error code otherwise
//  */

// const Status QU_Select(const string & result, 
// 		       const int projCnt, 
// 		       const attrInfo projNames[],
// 		       const attrInfo *attr, 
// 		       const Operator op, 
// 		       const char *attrValue)
// {
//    // Qu_Select sets up things and then calls ScanSelect to do the actual work
//     cout << "Doing QU_Select " << endl;

// }


// const Status ScanSelect(const string & result, 
// #include "stdio.h"
// #include "stdlib.h"
// 			const int projCnt, 
// 			const AttrDesc projNames[],
// 			const AttrDesc *attrDesc, 
// 			const Operator op, 
// 			const char *filter,
// 			const int reclen)
// {
//     cout << "Doing HeapFileScan Selection using ScanSelect()" << endl;


// }






// #include "catalog.h"
// #include "query.h"


// // forward declaration
// const Status ScanSelect(const string & result, 
// 			const int projCnt, 
// 			const AttrDesc projNames[],
// 			const AttrDesc *attrDesc, 
// 			const Operator op, 
// 			const char *filter,
// 			const int reclen);

// /*
//  * Selects records from the specified relation.
//  *
//  * Returns:
//  * 	OK on success
//  * 	an error code otherwise
//  */

// const Status QU_Select(const string & result, 
// 		       const int projCnt, 
// 		       const attrInfo projNames[],
// 		       const attrInfo *attr, 
// 		       const Operator op, 
// 		       const char *attrValue)
// {
//    // Qu_Select sets up things and then calls ScanSelect to do the actual work
//     cout << "Doing QU_Select " << endl;

// 	Status status;
// 	AttrDesc projAttrDesc[projCnt];
// 	AttrDesc attrDesc;
// 	Operator myOp; 
// 	const char *myfilter;
// 	int reclen;



// 	reclen = 0;
	
// 	for (int i = 0; i < projCnt; i++) {
// 		Status status = attrCat->getInfo(projNames[i].relName,
//                                 projNames[i].attrName,
//                                 projAttrDesc[i]);
// 		reclen += projAttrDesc[i].attrLen;
//         if (status != OK){return status;}
// 	}


// 	if (attr != NULL) {
// 		status = attrCat->getInfo(attr->relName, attr->attrName, attrDesc);

// 		int tempInt;
// 		float tempFloat;

// 		if (attr->attrType == INTEGER) {
// 			tempInt = atoi(attrValue);
// 			myfilter = (char*)&tempInt;
// 		} else if (attr->attrType == FLOAT) {
// 			tempFloat = atof(attrValue);
// 			myfilter = (char*)&tempFloat;
// 		} else if (attr->attrType == STRING) {
// 			myfilter = attrValue;
// 		}
// 		myOp = op;
// 	} else {
// 		strcpy(attrDesc.relName, projNames[0].relName);
// 		strcpy(attrDesc.attrName, projNames[0].attrName);
// 		attrDesc.attrOffset = 0;
// 		attrDesc.attrLen = 0;
// 		attrDesc.attrType = STRING;
// 		myfilter = NULL;
// 		myOp = EQ;
// 	}

// 	status = ScanSelect(result, projCnt, projAttrDesc, &attrDesc, myOp, myfilter, reclen);
// 	if (status != OK) {return status;}

// 	return OK;
// }


// const Status ScanSelect(const string & result, 
// #include "stdio.h"
// #include "stdlib.h"
// 			const int projCnt, 
// 			const AttrDesc projNames[],
// 			const AttrDesc *attrDesc, 
// 			const Operator op, 
// 			const char *filter,
// 			const int reclen)
// {
//     cout << "Doing HeapFileScan Selection using ScanSelect()" << endl;

// 	Status status;

	
// 	InsertFileScan resultRel(result, status);
// 	if (status != OK) { return status; }

// 	char outputData[reclen];
//     Record outputRec;
//     outputRec.data = (void *) outputData;
//     outputRec.length = reclen;

// 	HeapFileScan relScan(attrDesc->relName, status);
//     if (status != OK) { return status; }


// 	status = relScan.startScan(attrDesc->attrOffset,
// 							 attrDesc->attrLen,
// 							 (Datatype) attrDesc->attrType,
// 							 filter, 
// 							 op);
//     if (status != OK) { return status; }
    
// 	RID outerRID;
//     Record outerRec;
// 	while (relScan.scanNext(outerRID) == OK)
//     {
// 		status = relScan.getRecord(outerRec);
//         ASSERT(status == OK);

// 		int outputOffset = 0;
// 		for (int i = 0; i < projCnt; i++) {
// 			memcpy(outputData + outputOffset,
// 				(char*)outerRec.data + projNames[i].attrOffset,
// 				projNames[i].attrLen);
// 			outputOffset += projNames[i].attrLen;
// 		}

// 		RID retRID;
// 		status = resultRel.insertRecord(outputRec, retRID);
// 		if (status != OK) { return status; }
// 	}
// 	return OK;
// }


#include "catalog.h"
#include "query.h"
#include "stdio.h"
#include "stdlib.h"


// forward declaration
const Status ScanSelect(const string & result, 
			const int projCnt, 
			const AttrDesc projNames[],
			const AttrDesc *attrDesc, 
			const Operator op, 
			const char *filter,
			const int reclen);

/*
 * Selects records from the specified relation.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 *
 * LOG:
 * 2012/11/28 JH: First implementation.
 * 2012/12/03 JH: Various debugs.
 */

const Status QU_Select(const string & result, 
		       const int projCnt, 
		       const attrInfo projNames[],
		       const attrInfo *attr, 
		       const Operator op, 
		       const char *attrValue)
{
    // Qu_Select sets up things and then calls ScanSelect to do the actual work
    cout << "Doing QU_Select " << endl;

    Status status;
    AttrDesc *attrDesc;
    AttrDesc projAttrDesc[projCnt];
    int attrCnt,i,j,reclen,searchAttr;
    
    // reject invalid format
    if ( (attr != NULL)
        && (attr->attrType <0 || attr->attrType>2) )
        return BADCATPARM;

    // get info of all attributes in requested relation
    if ( (status=attrCat->getRelInfo(projNames[0].relName, attrCnt, attrDesc)) != OK)
        return status;
    
    reclen=0;
    for (i=0; i<attrCnt; i++) {
        for (j=0; j<projCnt; j++)
            if (strcmp(attrDesc[i].attrName,projNames[j].attrName)==0) {
                // if current attribute is part of projection:
                // 1) calculate length of a record
                // 2) fill in projection attr desc
                reclen += attrDesc[i].attrLen;
                projAttrDesc[j] = attrDesc[i];
            }
        if (attr!=NULL && strcmp(attrDesc[i].attrName,attr->attrName) == 0)
            searchAttr=i;
    }
    
    // pass info along to ScanSelect for the actual work
    
    if (attr==NULL) // unconditional select
        return ScanSelect(result, projCnt, projAttrDesc,
                          &projAttrDesc[0],
                          EQ, NULL, reclen);

    return ScanSelect(result, projCnt, projAttrDesc,
                      &attrDesc[searchAttr],
                      op, attrValue, reclen);
}


const Status ScanSelect(const string & result, 
			const int projCnt,
			const AttrDesc projNames[],
			const AttrDesc *attrDesc, 
			const Operator op, 
			const char *filter,
			const int reclen)
{
    cout << "Doing HeapFileScan Selection using ScanSelect()" << endl;
    
    Status status;
    RID rid;
    Record rec;
    int i;
    Datatype t;
    
    Record resultRec;
    char outputData[reclen];
    resultRec.data = (void*)outputData;
    resultRec.length = reclen;
    int outputOffset;
    
    // open relation file to read, result file to write
    InsertFileScan resultFile(result,status);
    if (status!=OK) return status;
    HeapFileScan relFile(projNames[0].relName,status);
    if (status!=OK) return status;

    void *realFilter;
    
    // set up scan condition
    if (filter!=NULL) {
        // if a specific filter is given, perform possible data conversion
        
        switch (attrDesc->attrType) {
            case 1: {
                t = INTEGER;
                int iVal = atoi(filter);
                realFilter = &iVal;
                break;
            }
            case 2: {
                t = FLOAT;
                float fVal = atof(filter);
                realFilter = &fVal;
                break;
            }
            case 0: {
                t = STRING;
                realFilter = (void*)filter;
                break;
            }
        }
        status=relFile.startScan(attrDesc->attrOffset,
                                 attrDesc->attrLen,
                                 t, (char*)realFilter, op);
    } else {
        // no specific filter is given; set up scan for sequential scan
        status=relFile.startScan(attrDesc->attrOffset,
                                 attrDesc->attrLen,
                                 t, NULL, op);
    }

    if  ( status != OK ) return status;
    
    while (relFile.scanNext(rid) == OK) {
        // current record matches search criteria

        // get record
        if ( (status = relFile.getRecord(rec)) != OK)
            return status;
        
        // 1) create new record from projection
        outputOffset = 0;
        for (i=0; i<projCnt; i++) {
            memcpy(outputData + outputOffset,
                   (char*)rec.data + projNames[i].attrOffset,
                   projNames[i].attrLen);
            outputOffset += projNames[i].attrLen;
        }
        
        // 2) store into result file
        RID outRID;
        status = resultFile.insertRecord(resultRec, outRID);
        if (status!=OK)
            return status;
    }

    return OK;
}