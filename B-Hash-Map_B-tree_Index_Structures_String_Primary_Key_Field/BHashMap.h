
#include "BTree.h"


typedef struct BHashMap BHashMap;
typedef struct BTreeRecordData* BHashMapData;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/* BHashMap index structure. */

struct BHashMap
{

    BTree** BTreeArray;        // Array structure of B-tree index structures.
    int BHashMap_Level_Size;   // Set of BHashMap B-tree index structures.
    int size;                  // Stored records in the BHashMap B-tree index structures.

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Initializes the BHashMap index structure.
*
*  Return value:  Nothing specific
*
*/

void InitializeBHashMap(

        BHashMap* BHashMap,        // BHashMap index structure object.
        int BHashMap_Level_Size,   // Set of BHashMap B-tree index structures.
        int node_size              // Maximum record references that can be stored in a B-tree structure node of the BHashMap.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Inserts record data in a B-tree structure of the BHashMap
*  based on a hash code value of a specific primary key field.
*
*  Return value:
*
*  value: 0 - insertion is successful
*  value: 1 - insertion is unsuccessful
*
*/

bool BHashMapInsertData(

        BHashMap* BHashMap,                                   // BHashMap index structure object.
        BHashMapData RecordData,                              // Record data - to be inserted.
        int hash_code,                                        // Hash code value to locate the insertion position - B-tree index structure
                                                              // that the record will be stored.

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData),  // Receives the primary key field of the table that is linked to the BHashMap B-tree.
        bool (*Deallocate_Record_Data)(BTreeData)             // Deallocates the record data of the table that is linked to the BHashMap B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes record data in a B-tree structure of the BHashMap
*  based on a hash code value and a primary key field of a specific primary key field.
*
*  Return value:
*
*  value: 0 - deletion is successful
*  value: 1 - deletion is unsuccessful
*
*/

bool BHashMapDeleteData(

        BHashMap* BHashMap,                                   // BHashMap index structure object.
        BTreePrimaryKey PrimaryKey,                           // Primary key field of the record - to be deleted.
        int hash_code,                                        // Hash code value to locate the deletion position - B-tree index structure
                                                              // from which the record will be removed.
        BHashMapData* DeletedRecordData,                      // Record data - to be deleted.

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)   // Receives the primary key field of the table that is linked to the BHashMap B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Displays the BHashMap record data by a primary key fields.
*
*  Return value:  Nothing specific
*
*/

void BHashMapDisplay(

        BHashMap BHashMap,                                    // BHashMap index structure object.

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)   // Receives the primary key field of the table that is linked to the BHashMap B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Selects record data from a B-tree structure of the BHashMap based on a non-primary field.
*
*  Return value:
*
*  value: 0 - selection is successful
*  value: 1 - selection is unsuccessful
*
*/

bool BHashMapSelectData(

        BHashMap BHashMap,                               // BHashMap index structure object.
        struct DoubleLinkedBList* SelectionResultList,   // Result list that contains the selected record data.
        int Field_Data,                                  // Field that the record data selection is based on.

        bool selection_method_flag,                      // Flag to set the selection method (ASC - ascending and DESC - descending)
                                                         // of the BHashMap B-tree structures parts:
                                                         //
                                                         // ASC selection  - (value: 0)
                                                         // DESC selection - (value: 1)


        int (*Get_Record_FieldData)(BTreeData)           // Receives the generic field data of the table that is linked to the BHashMap B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Selects record data from a B-tree structure of the BHashMap based on a primary key field.
*
*  Return value:  Selected record data
*
*/

BHashMapData BHashMapSelectData_ByPrimaryKey(

        BHashMap BHashMap,                                    // BHashMap index structure object.
        BTreePrimaryKey PrimaryKey,                           // Primary key field of the record - to be selected.
        int hash_code,                                        // Hash code value to locate the position - B-tree index structure
                                                              // in witch the record is stored.

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)   // Receives the primary key field of the table that is linked to the BHashMap B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Drops completely the BHashMap structure and the stored records set.
*
*  Return value:
*
*  value: 0 - deletion is successful
*  value: 1 - deletion is unsuccessful
*
*/

bool BHashMapDrop(

        BHashMap* BHashMap,                         // BHashMap index structure object.

        bool (*Deallocate_Record_Data)(BTreeData)   // Deallocates the record data of the table that is linked to the BHashMap B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Drops - removes all the B-tree nodes and records of the BHashMap.
*
*  Return value:
*
*  value: 0 - deletion is successful
*  value: 1 - deletion is unsuccessful
*
*/

bool BHashMapClean(

        BHashMap* BHashMap,                         // BHashMap index structure object.

        bool (*Deallocate_Record_Data)(BTreeData)   // Deallocates the record data of the table that is linked to the BHashMap B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  DJB2 hash algorithm generates a  hash id from a string primary key fields.
*
*  Return value:  Hash code.
*
*/

int BTree_DJB2_Hash_PrimaryKeyStr(

        char* record_primary_key,   // Primary key field.
        int hashmap_level_size      // Set of BHashMap B-tree index structures.

);

