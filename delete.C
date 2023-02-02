#include "catalog.h"
#include "query.h"


/*
 * Deletes records from a specified relation.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

const Status QU_Delete(const string & relation, 
		       const string & attrName, 
		       const Operator op,
		       const Datatype type, 
		       const char *attrValue)
{

	Status status;
	AttrDesc ad;
	RID rid;
	int tempInt;
	float tempFloat;
	void *filter;


	HeapFileScan *hfs = new HeapFileScan(relation, status);
	if (status != OK) return status;

	switch(type)
	{
		case INTEGER:
            tempInt = atoi(attrValue);
            filter = (char*)&tempInt;
            break;
        case FLOAT:
            tempFloat = atof(attrValue);
            filter = (char*)&tempFloat;
            break;
        case STRING:
            filter = (void*)attrValue;
            break;
	}

	status = attrCat->getInfo(relation, attrName, ad);
	if (status != OK) return status;
	hfs->startScan(ad.attrOffset, ad.attrLen, type, (char*)filter, op);

	if(attrName == "") hfs->startScan(0, 0, type, NULL, op);

    while (status==OK) {
        status = hfs->scanNext(rid);
        if (status == FILEEOF) break;

        hfs->deleteRecord();
    }

	delete hfs;
	return OK;
}
