
#include "DoubleLinkedBList.h"


typedef struct BTree BTree;
typedef struct BTreeNode* BTreeNode;
typedef struct BTreeRecordData* BTreeData;

typedef struct TableB_RecordData* TableB_RecordData;

typedef char* BTreePrimaryKey;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/* General record data - row of a RDBMS table that is linked to the B-tree index structure. */

struct BTreeRecordData
{

    void* RecordData;   // Record data - row of RDBMS a table.
    int table_id;       // Table id identifier. Identifies that the record data corresponds and is stored to a specific RDBMS table.

};


/* Record data - row of the table TableB that is linked to the B-tree index structure. */

struct TableB_RecordData
{

    BTreePrimaryKey Record_PrimaryKey;   // Primary key field of the record.
    int Record_PrimaryKey_Size;          // Allocated memory to store characters - number of characters.

    int Field_Data;                      // A not specific - general field of the record.

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*  B-tree node - basic structural part of the B-tree index. */

struct BTreeNode
{

    BTreeData* RecordDataArray;     // Dynamic array structure that stores the record references of a table.
    BTreeNode* NodeLinkArray;       // Dynamic array structure that stores the next structure level node references of the B-tree index structure.

    bool leaf_type;                 // Flag to specify the node type (internal - leaf node):
                                    //
                                    // Leaf node     - (value: 1)
                                    // Internal node - (value: 0)

    int array_stored_elements;      // Record references that are stored in the node array structure.
    int array_available_capacity;   // Available storage capacity (allocated memory cells) of the node array structure.

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/* B-tree index structure. */

struct BTree
{

    BTreeNode Root;   // Root node of the B-tree index structure.
    int node_size;    // Maximum record references that can be stored in the node array structure.

    int size;         // Stored records in the B-tree index structure.
    int height;       // Number of nodes levels of the B-tree index structure.

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Initializes the B-tree index structure.
*
*  Return value:
*
*  value: 0 - initialization is successful
*  value: 1 - initialization is unsuccessful
*
*/

bool InitializeBTree(

        BTree* BTree,   // B-tree index structure object.
        int node_size   // Maximum record references that can be stored in the node array structure.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Inserts a record data in the B-tree index structure that is linked to a table based on a primary key field.
*
*  Return value:
*
*  value: 0 - insertion is successful
*  value: 1 - insertion is unsuccessful
*
*/

bool BTreeInsertData(

        BTree* BTree,                                          // B-tree index structure object.
        BTreeData RecordData,                                  // Record data to be inserted in the B-tree.

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData),   // Receives the primary key field of the table that is linked to the B-tree.
        bool (*Deallocate_Record_Data)(BTreeData)              // Deallocates the record data of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Inserts a record data and rebuilds nodes in the B-tree index structure based on a primary key field.
*  Especially implements the root node split, re-balancing and reconstruction processes.
*
*
*        (Root node is a leaf node and is full)
*
*            [ E1 ] [ E2 ]  <--[ E3 ]    --->      [ E2 ]
*           /      |      \                       /      \
*         NULL    NULL   NULL                 [ E1 ]    [ E3 ]
*
*
*        (Root node is an internal node and is full)
*                                                                                                              [  E8  ]
*                [ E4 ]     [ E8 ]                                                                      /                    \
*             /          |          \                                                             [ E4 ]                      [ E12 ]
*       [ E2 ]         [ E6 ]        [ E10 ] [ E12 ]                             --->            /      \                    /       \
*      /      \       /      \      /       |       \                                      [ E2 ]        [ E6 ]        [ E10 ]        [ E14 ]
*  [ E1 ]  [ E3 ] [ E5 ]   [ E7 ] [ E9 ] [ E11 ]     [ E13 ] [ E14 ] <--[ E15 ]           /      \      /      \      /       \      /       \
*                                                                                      [ E1 ]  [ E3 ] [ E5 ] [ E7 ] [ E9 ] [ E11 ] [ E13 ] [ E15 ]
*
*  Return value:
*
*  value: 0 - insertion is successful
*  value: 1 - insertion is unsuccessful
*
*/

bool BTreeInsertNode_RootBreakTool(

        BTree* BTree,                                          // B-tree index structure object.
        BTreeData RecordData,                                  // Record data to be inserted in the B-tree.

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData),   // Receives the primary key field of the table that is linked to the B-tree.
        bool (*Deallocate_Record_Data)(BTreeData)              // Deallocates the record data of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Inserts a record data and rebuilds nodes in the B-tree index structure based on a primary key field.
*  Especially implements the leaf and internal levels nodes split, re-balancing and reconstruction processes.
*
*
*     A) The record to be inserted can be stored in the leaf node
*
*                   [ E4 ]                                           [ E4 ]
*                 /        \                                       /        \
*           [ E2 ]          [ E6 ]                 --->       [ E2 ]         [ E6 ]
*          /      \        /      \                          /      \       /      \
*       [ E1 ]  [ E3 ]  [ E5 ]  [ E7 ]  <--[ E8 ]         [ E1 ]  [ E3 ] [ E5 ]  [ E7 ][ E8 ]
*
*
*
*     B) The record can not be stored in the leaf node and the upper level linked node has available capacity
*
*                   [ E4 ]                                                [ E4 ]
*                 /        \                                            /        \
*           [ E2 ]          [ E6 ]                      --->      [ E2 ]          [ E6 ] [ E8 ]
*          /      \        /      \                              /      \        /      |      \
*       [ E1 ]  [ E3 ]  [ E5 ]  [ E7 ][ E8 ] <--[ E9 ]        [ E1 ]  [ E3 ] [ E5 ]   [ E7 ]  [ E9 ]
*
*
*     C) The record can not be stored in the leaf node and the upper level linked node has not available capacity
*
*
*                   [ E4 ]                                                               [ E4 ]     [ E8 ]
*                 /        \                                                          /          |          \
*           [ E2 ]          [ E6 ] [ E8 ]                           --->        [ E2 ]         [ E6 ]        [ E10 ]
*          /      \        /      |      \                                    /      \       /       \      /       \
*       [ E1 ]  [ E3 ]  [ E5 ]  [ E7 ]   [ E9 ][ E10 ] <--[ E11 ]          [ E1 ]  [ E3 ] [ E5 ]   [ E7 ] [ E9 ]  [ E11 ]
*
*
*  Return value:  B-tree node object
*
*/

BTreeNode BTreeInsertNode_Tool(

        BTreeNode CurrentNode,                                 // B-tree node.
        BTreeData RecordData,                                  // Record data to be inserted in the B-tree.
        BTreeData* Node_MiddleRecordData,                      // Middle record data of a specific node record reference array structure.
        int node_size,                                         // Maximum record references that can be stored in the node array structure.

        bool* flag_duplicate_record,                           // Flag to specify if the record (to be inserted) already exists in the B-tree index structure:
                                                               //
                                                               // Exists     - (value: 1)
                                                               // Not Exists - (value: 0)

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData),   // Receives the primary key field of the table that is linked to the B-tree.
        bool (*Deallocate_Record_Data)(BTreeData)              // Deallocates the record data of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Locates the record data (based on a primary key field) in a record array structure or
*  the position in a node array structure (next level node) that the record can be located - found.
*
*  Return value:  Position of the record to be located in the record - node array structures of the B-tree node
*
*/

int SearchBTreeNode_Record_ByPrimaryKey(

        int array_stored_elements,                            // Record references that are stored in the node array structure.
        BTreeData* RecordDataArray,                           // Record data array of the B-tree node where the search process is implemented.
        BTreePrimaryKey PrimaryKey,                           // Primary key field - to be located.

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)   // Receives the primary key field of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Binary search of an array structure.
*
*  Return value:  Position of the record to be located in the record - node array structures of the B-tree node
*
*/

int BTree_Binary_Search(

        int array_stored_elements,                            // Record references that are stored in the node array structure.
        BTreeData* RecordDataArray,                           // Record data array of the B-tree node where the search process is implemented.
        BTreePrimaryKey PrimaryKey,                           // Primary key field - to be located.

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)   // Receives the primary key field of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Locates the position in the B-tree index structure node where a specific record is stored based on a primary key field.
*
*  Debugging - testing tool.
*
*  Return value:  Record data - to be located
*
*/

BTreeData BTreeSearchData(

        BTree BTree,                                          // B-tree index structure object.
        BTreePrimaryKey PrimaryKey,                           // Primary key field - to be located.

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)   // Receives the primary key field of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Locates the position in the B-tree index structure node where a specific record is stored based on a primary key field.
*  Implements the basic - main location process.
*
*  Debugging - testing tool.
*
*  Return value:  Record data - to be located
*
*/

BTreeData BTreeSearch_Tool(

        BTreeNode CurrentNode,                                // B-tree node.
        BTreePrimaryKey PrimaryKey,                           // Primary key field - to be located.

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)   // Receives the primary key field of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
*  Locates the position in the B-tree index structure node where a specific record is stored based on a primary key field.
*
*  Return value:  Record data - to be located
*
*/

BTreeData BTreeFastSearchData_ByPrimaryKey(

        BTree BTree,                                          // B-tree data structure object.
        BTreePrimaryKey PrimaryKey,                           // Primary key field - to be located.

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)   // Receives the primary key field of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
*  Locates the position in the B-tree index structure node where a specific record is stored based on a primary key field.
*  Implements the basic - main location process.
*
*  Return value:  Record data - to be located
*
*/

BTreeData BTreeFastSearch_Tool(

        BTreeNode CurrentNode,                                // B-tree node.
        BTreePrimaryKey PrimaryKey,                           // Primary key field - to be located.

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)   // Receives the primary key field of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Displays in order the primary key field of every B-tree node record data.
*
*  Return value:  Nothing specific
*
*/

void BTreeDisplayInOrder(

        BTree BTree,                                          // B-tree index structure object.

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)   // Receives the primary key field of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Displays in order the primary key field of every B-tree node record data.
*  Implements the basic - main display process.
*
*  Return value:  Nothing specific
*
*/

void BTreeInOrderTraversal(

        BTreeNode CurrentNode,                                // B-tree node.

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)   // Receives the primary key field of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data that is stored in the B-tree index structure based on a primary key field.
*
*  Return value:
*
*  value: 0 - deletion is successful
*  value: 1 - deletion is unsuccessful
*
*/

bool BTreeDeleteData(

        BTree* BTree,                                         // B-tree index structure object.
        BTreePrimaryKey PrimaryKey,                           // Primary key field of the record data to be deleted.
        BTreeData* DeletedRecordData,                         // Record data - to be deleted.

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)   // Receives the primary key field of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data that is stored in the B-tree index structure based on a primary key field.
*  Especially implements the record data deletion in a leaf - internal node and the nodes re-balancing - reconstruction.
*
*  Return value:  B-tree node object
*
*/

BTreeNode BTreeDeleteNode(

        BTreeNode CurrentNode,                                // B-tree node.
        BTreePrimaryKey PrimaryKey,                           // Primary key field of the record data to be deleted.
        BTreeData* DeletedRecordData,                         // Record data - to be deleted.
        int node_size,                                        // Maximum record references that can be stored in the node array structure.

        bool* balancing_flag,                                 // Flag to specify if the balancing process has been activated to the non leaf node levels:
                                                              //
                                                              // Balance can not be restored to the leaf node level.
                                                              // Balancing process activation   - (value: 1)
                                                              // Balancing process inactivation - (value: 0)

        bool* internal_record_deletion_flag,                  // Flag to specify if the internal node record data deletion procedure has been activated:
                                                              //
                                                              // The record data to be deleted is in an internal node.
                                                              // Internal node deletion procedure activation   - (value: 1)
                                                              // Internal node deletion procedure inactivation - (value: 0)

        bool* record_existence_flag,                          // Flag to specify if the record data to be deleted exists:
                                                              //
                                                              // Record data not exists - (value: 1)
                                                              // Record data exists     - (value: 0)

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)   // Receives the primary key field of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data that is stored in an internal node based on a primary key field.
*  Especially implements the internal nodes re-balancing - reconstruction of the upper - internal nodes levels.
*
*  Return value:  Nothing specific
*
*/

void BTreeDelete_NonLeafNode(

        BTreeNode CurrentNode,   // B-tree node.
        int record_position,     // Position of the next level linked node in the node array structure
                                 // that the nodes set reconstruction - re-balancing process was implemented.

        int node_size,           // Maximum record references that can be stored in the node array structure.

        bool* balancing_flag     // Flag to specify if the balancing process has been activated to the non leaf node levels.
                                 //
                                 // Balance can not be restored to the leaf node level.
                                 // Balancing process activation   - (value: 1)
                                 // Balancing process inactivation - (value: 0)

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data that is stored in a leaf node based on a primary key field.
*  Especially implements the record data deletion in the leaf nodes bottom level
*  and the re-balancing - reconstruction of the nodes set that the deletion was implemented.
*
*  Return value:  Nothing specific
*
*/

void BTreeDelete_LeafNode(

        BTreeNode CurrentNode,                                // B-tree node.
        BTreePrimaryKey PrimaryKey,                           // Primary key field of the record data to be deleted.
        BTreeData* DeletedRecordData,                         // Record data - to be deleted.
        int record_position,                                  // Position of the record data to be deleted in the next level leaf node record array structure.
        int node_size,                                        // Maximum record references that can be stored in the node array structure.

        bool* balancing_flag,                                 // Flag to specify if the balancing process has been activated to the non leaf node levels:
                                                              //
                                                              // Balance can not be restored to the leaf node level.
                                                              // Balancing process activation   - (value: 1)
                                                              // Balancing process inactivation - (value: 0)

        bool internal_record_deletion_flag,                   // Flag to specify if the internal node record data deletion procedure has been activated:
                                                              //
                                                              // The record data to be deleted is in an internal node.
                                                              // Internal node deletion procedure activation   - (value: 1)
                                                              // Internal node deletion procedure inactivation - (value: 0)

        bool* record_existence_flag,                          // Flag to specify if the record data to be deleted exists:
                                                              //
                                                              // Record data not exists - (value: 1)
                                                              // Record data exists     - (value: 0)

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)   // Receives the primary key field of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data from a leaf node in the case that the leaf node contains multiple records.
*  Implements the deletion nodes set reconstruction - re-balancing.
*
*  Return value:  Nothing specific
*
*/

void BTree_ReplaceRecord(

        BTreeNode CurrentNode,
        int record_position,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data from a leaf node in the case that the leaf node contains a single record,
*  the upper level linked node contains a single record and the left side node contains a single record.
*  Implements the deletion nodes set reconstruction - re-balancing.
*
*
*	[ E1 ]                [ E2 ] [ E1 ]
*      /      \     ---->    /      |      \
*  [ E2 ]    [ X ]         Null    Null   Null
*
*
*  Return value:  Nothing specific
*
*/

void BTree_MergeLeftNode(

        BTreeNode CurrentNode,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data from a leaf node in the case that the leaf node contains a single record,
*  the upper level linked node contains a single record and the right side node contains a single record.
*  Implements the deletion nodes set reconstruction - re-balancing.
*
*
*      [ E1 ]                [ E1 ] [ E2 ]
*     /      \     ---->    /      |      \
*  [ X ]   [ E2 ]         Null    Null   Null
*
*
*  Return value:  Nothing specific
*
*/

void BTree_MergeRightNode(

        BTreeNode CurrentNode,
        int node_size
);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data from a leaf node in the case that the leaf node contains a single record,
*  the upper level linked node contains a single record and the left side node contains multiple records.
*  Implements the deletion nodes set reconstruction - re-balancing.
*
*
*	      [ E3 ]                  [ E2 ]
*            /      \     ---->      /      \
*   [ E1 ][ E2 ]   [ X ]         [ E1 ]   [ E3 ]
*
*
*  Return value:  Nothing specific
*
*/

void BTree_RebalanceLeftNode(

        BTreeNode CurrentNode,
        BTreeNode NextLevelNode,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data from a leaf node in the case that the leaf node contains a single record,
*  the upper level linked node contains a single record and the right side node contains multiple records.
*  Implements the deletion nodes set reconstruction - re-balancing.
*
*
*      [ E1 ]                       [ E2 ]
*     /      \           ---->     /      \
*  [ X ]   [ E2 ][ E3 ]         [ E1 ]   [ E3 ]
*
*
*  Return value:  Nothing specific
*
*/

void BTree_RebalanceRightNode(

        BTreeNode CurrentNode,
        BTreeNode NextLevelNode,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data from a leaf node in the case that the leaf node contains a single record,
*  the upper level linked node contains multiple records and the left side node contains multiple records.
*  Implements the deletion nodes set reconstruction - re-balancing.
*
*
*	         [ E2 ]   [ E5 ]                     [ E2 ] [ E4 ]
*              /        |        \       --->       /      |      \
*        [ E1 ]  [  E3 ] [ E4 ]   [ X ]         [ E1 ]  [ E3 ]  [ E5 ]
*
*
*  Return value:  Nothing specific
*
*/

void BTree_SwapLeftNode(

        BTreeNode CurrentNode,
        BTreeNode NextLevelNode,
        int record_position,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data from a leaf node in the case that the leaf node contains a single record,
*  the upper level linked node contains multiple records and the right side node contains multiple records.
*  Implements the deletion nodes set reconstruction - re-balancing.
*
*
*	         [ E1 ]   [ E4 ]                     [ E2 ] [ E4 ]
*              /        |        \       --->       /      |      \
*         [ X ]  [  E2 ] [ E3 ]  [ E5 ]         [ E1 ]  [ E3 ]  [ E5 ]
*
*
*  Return value:  Nothing specific
*
*/

void BTree_SwapRightNode(

        BTreeNode CurrentNode,
        BTreeNode NextLevelNode,
        int record_position,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data from a leaf node in the case that the leaf node contains a single record,
*  the upper level linked node contains multiple records and the left side node contains a single record.
*  Implements the deletion nodes set reconstruction - re-balancing.
*
*
*	         [ E2 ] [ E4 ]                   [ E2 ]
*               /      |      \     --->       /        \
*            [ E1 ]  [ E3 ]  [ X ]          [ E1 ]      [ E3 ][ E4 ]
*
*
*  Return value:  Nothing specific
*
*/

void BTree_MergeSingleNodeRight(

        BTreeNode CurrentNode,
        int record_position,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deletes a record data from a leaf node in the case that the leaf node contains a single record,
*  the upper level linked node contains multiple records and the right side node contains a single record.
*  Implements the deletion nodes set reconstruction - re-balancing.
*
*
*	         [ E1 ] [ E3 ]                       [ E3 ]
*               /      |      \     --->            /      \
*            [ X ]  [ E2 ]  [ E4 ]         [ E1 ][ E2 ]  [ E4 ]
*
*
*  Return value:  Nothing specific
*
*/

void BTree_MergeSingleNodeLeft(

        BTreeNode CurrentNode,
        BTreeNode NextLevelNode,
        int record_position,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Implements the internal nodes set reconstruction - re-balancing in the case that
*  the next (under) level linked node reconstruction - re-balancing was implemented unsuccessfully.
*
*  The upper level linked node contains a single record and the left side node contains a single record.
*
*
*               [ E4 ]                            [ E2 ] [ E4 ]
*              /      \                          /      |      \
*          [ E2 ]      [ E5 ][ E6 ]  --->    [ E1 ]   [ E3 ]    [ E5 ][ E6 ]
*         /      \
*     [ E1 ]      [ E3 ]
*
*
*  Return value:  Nothing specific
*
*/

void BTree_MergeLeftNodeRecursive(

        BTreeNode CurrentNode,
        BTreeNode LeftNode,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Implements the internal nodes set reconstruction - re-balancing in the case that
*  the next (under) level linked node reconstruction - re-balancing was implemented unsuccessfully.
*
*  The upper level linked node contains a single record and the left side node contains multiple records.
*
*
*                        [ E6 ]                                [ E4 ]
*                       /      \                             /        \
*            [ E2 ] [ E4 ]      [ E7 ][ E8 ]   --->     [ E2 ]          [ E6 ]
*           /      |     \                             /      \        /      \
*     [ E1 ]     [ E3 ]   [ E5 ]                    [ E1 ]  [ E3 ]  [ E5 ]  [ E7 ][ E8 ]
*
*
*  Return value:  Nothing specific
*
*/

void BTree_ReplaceLeftNodeRecursive(

        BTreeNode CurrentNode,
        BTreeNode LeftNode,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Implements the internal nodes set reconstruction - re-balancing in the case that
*  the next (under) level linked node reconstruction - re-balancing was implemented unsuccessfully.
*
*  The upper level linked node contains a single record and the right side node contains a single record.
*
*
*  Return value:  Nothing specific
*
*/

void BTree_MergeRightNodeRecursive(

        BTreeNode CurrentNode,
        BTreeNode RightNode,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Implements the internal nodes set reconstruction - re-balancing in the case that
*  the next (under) level linked node reconstruction - re-balancing was implemented unsuccessfully.
*
*  The upper level linked node contains a single record and the right side node contains multiple records.
*
*
*  Return value:  Nothing specific
*
*/

void BTree_ReplaceRightNodeRecursive(

        BTreeNode CurrentNode,
        BTreeNode RightNode,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Implements the internal nodes set reconstruction - re-balancing in the case that
*  the next (under) level linked node reconstruction - re-balancing was implemented unsuccessfully.
*
*  The upper level linked node contains multiple records and the left side node contains multiple records.
*
*
*                          [ E6 ]   [ E9 ]                                       [ E4 ]     [ E9 ]
*                       /         |        \                                  /          |          \
*          [ E2 ] [ E4 ]    [ E7 ] [ E8 ]   [ E11 ]        --->        [ E2 ]          [ E6 ]        [ E11 ]
*         /      |     \                   /       \                  /      \        /      \      /       \
*     [ E1 ]  [ E3 ]  [ E5 ]           [ E10 ]   [ E12 ]           [ E1 ]  [ E3 ] [ E5 ]  [ E7 ] [ E10 ]  [ E12 ]
*                                                                                         [ E8 ]
*
*  Return value:  Nothing specific
*
*/

void BTree_ReplaceMultipleLeftNodeRecursive(

        BTreeNode CurrentNode,
        BTreeNode LeftNode,
        int record_position,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Implements the internal nodes set reconstruction - re-balancing in the case that
*  the next (under) level linked node reconstruction - re-balancing was implemented unsuccessfully.
*
*  The upper level linked node contains multiple records and the left side node contains a single record.
*
*
*                   [ E4 ]   [ E7 ]                                             [ E7 ]
*                 /        |        \                                      /              \
*          [ E2 ]    [ E5 ] [ E6 ]   [ E9 ]        --->       [ E2 ] [ E4 ]                [ E9 ]
*         /      \                  /      \                 /      |      \              /       \
*      [ E1 ]  [ E3 ]            [ E8 ]  [ E10 ]          [ E1 ] [ E3 ]  [ E5 ][ E6 ]  [ E8 ]  [ E10 ]
*
*
*  Return value:  Nothing specific
*
*/

void BTree_ReplaceSingleLeftNodeRecursive(

        BTreeNode CurrentNode,
        BTreeNode LeftNode,
        int record_position,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Implements the internal nodes set reconstruction - re-balancing in the case that
*  the next (under) level linked node reconstruction - re-balancing was implemented unsuccessfully.
*
*  The upper level linked node contains multiple records and the right side node contains multiple records.
*
*
*  Return value:  Nothing specific
*
*/

void BTree_ReplaceMultipleRightNodeRecursive(

        BTreeNode CurrentNode,
        BTreeNode RightNode,
        int record_position,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Implements the internal nodes set reconstruction - re-balancing in the case that
*  the next (under) level linked node reconstruction - re-balancing was implemented unsuccessfully.
*
*  The upper level linked node contains multiple records and the right side node contains a single record.
*
*
*  Return value:  Nothing specific
*
*/

void BTree_ReplaceSingleRightNodeRecursive(

        BTreeNode CurrentNode,
        BTreeNode RightNode,
        int record_position,
        int node_size

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Locates and receives the maximum record data by a primary key field
*  of the next left linked node path (subtree) from the root to the leaf nodes level.
*
*  Return value:  Record data - to be located
*
*/

BTreeData BTree_LeftSubTree_MaxRecord(

        BTreeNode CurrentNode

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
*  Initializes and creates the record data of the TableB.
*
*  Return value:  Created record data
*
*/

BTreeData InitializeBTreeData_TableB(

        BTreePrimaryKey Record_PrimaryKey,
        int Record_PrimaryKey_Size,

        int Field_Data

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Receives the primary key field of the TableB.
*
*  Return value:  Primary key field
*
*/

BTreePrimaryKey Get_PrimaryKey_TableB(

        BTreeData Input_RecordData

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Receives the generic field data of TableB.
*
*  Return value: Field data
*
*/

int Get_FieldData_TableB(

        BTreeData Input_RecordData

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Selects the record data (ascending - by a non-primary field) in the B-tree
*  and stores that record data in a Double Linked List structure.
*
*  Return value:
*
*  value: 0 - selection is successful
*  value: 1 - selection is unsuccessful
*
*/

bool BTreeSelectRecordData_ASC(

        BTree BTree,                                         // B-tree index structure object.
        struct DoubleLinkedBList* SelectionResultList,       // Result list that contains the selected record data.
        int Field_Data,                                      // Field that the record data selection is based on.

        int (*Get_Record_FieldData)(BTreeData)   // Receives the generic field data of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Selects the record data (ascending - by a non-primary field) in the B-tree
*  and stores that record data in a Double Linked List structure.
*  Especially performs the main part of the selection.
*
*  Return value:  Nothing specific
*
*/

void BTreeSelectRecordData_ASC_Tool(

        BTreeNode CurrentNode,                               // B-tree node.
        struct DoubleLinkedBList* SelectionResultList,       // Result list that contains the selected record data.
        int Field_Data,                                      // Field that the record data selection is based on.

        int (*Get_Record_FieldData)(BTreeData)   // Receives the generic field data of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Selects the record data (descending - by a non-primary field) in the B-tree
*  and stores that record data in a Double Linked List structure.
*
*  Return value:
*
*  value: 0 - selection is successful
*  value: 1 - selection is unsuccessful
*
*/

bool BTreeSelectRecordData_DESC(

        BTree BTree,                                         // B-tree index structure object.
        struct DoubleLinkedBList* SelectionResultList,       // Result list that contains the selected record data.
        int Field_Data,                                      // Field that the record data selection is based on.

        int (*Get_Record_FieldData)(BTreeData)   // Receives the generic field data of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Selects the record data (descending - by a non-primary field) in the B-tree
*  and stores that record data in a Double Linked List structure.
*  Especially performs the main part of the selection.
*
*  Return value:  Nothing specific
*
*/

void BTreeSelectRecordData_DESC_Tool(

        BTreeNode CurrentNode,                               // B-tree node.
        struct DoubleLinkedBList* SelectionResultList,       // Result list that contains the selected record data.
        int Field_Data,                                      // Field that the record data selection is based on.

        int (*Get_Record_FieldData)(BTreeData)   // Receives the generic field data of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Drops - removes all the nodes and record data from the Β-tree.
*
*  Return value:
*
*  value: 0 - B-tree deletion is successful
*  value: 1 - B-tree deletion is unsuccessful
*
*/

bool BTreeDrop(

        BTree* BTree,                               // B-tree index structure object.

        bool (*Deallocate_Record_Data)(BTreeData)   // Deallocates the record data of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Drops - removes all the nodes and record data from the Β-tree.
*  Especially performs the main part of the deletion process.
*
*  Return value:  Nothing specific
*
*/

void BTreeDropNode(

        BTreeNode CurrentNode,                      // B-tree node.

        bool (*Deallocate_Record_Data)(BTreeData)   // Deallocates the record data of the table that is linked to the B-tree.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Deallocation - deletion of the TableB record data.
*
*  Return value:
*
*  value: 0 - deletion is successful
*  value: 1 - deletion is unsuccessful
*
*/

bool Deallocate_BTreeRecordData_TableB(

        BTreeData RecordData_ToBe_Deleted   // Record data - to be deleted.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Computes the number of internal and leaf B-tree nodes and their stored record data.
*
*  Debugging - testing tool.
*
*  Return value:
*
*  value: 0 - computation is successful
*  value: 1 - computation is unsuccessful
*
*/

bool BTreeComputeNodes(

        BTree BTree,                   // B-tree index structure object.
        int* internal_nodes,           // Number of internal B-tree nodes.
        int* internal_nodes_records,   // Number of internal B-tree nodes stored record data.
        int* leaf_nodes,               // Number of leaf B-tree nodes.
        int* leaf_nodes_records        // Number of leaf B-tree nodes stored record data.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Computes the number of internal and leaf B-tree nodes and their stored record data.
*  Especially performs the main part of the computation process.
*
*  Debugging - testing tool.
*
*  Return value:  Nothing specific
*
*/

void BTreeComputeNodes_Tool(

        BTreeNode CurrentNode,         // B-tree node.
        int* internal_nodes,           // Number of internal B-tree nodes.
        int* internal_nodes_records,   // Number of internal B-tree nodes stored record data.
        int* leaf_nodes,               // Number of leaf B-tree nodes.
        int* leaf_nodes_records        // Number of leaf B-tree nodes stored record data.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Computes the B-tree structure height.
*
*  Debugging - testing tool.
*
*  Return value:
*
*  value: 0 - computation is successful
*  value: 1 - computation is unsuccessful
*
*/

bool BTreeComputeHeight(

        BTree BTree,   // B-tree index structure object.
        int* height    // B-tree height.

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Checks for invalid B-tree nodes record - node array structures capacity and stored data.
*
*  Debugging - testing tool.
*
*  Return value:
*
*  value: 0 - procedure is successful
*  value: 1 - procedure is unsuccessful
*
*/

bool BTreeCheckNodeCapacity(

        BTree BTree,       // B-tree index structure object.

        bool* error_flag   // Flag to check if the B-tree nodes capacity is properly defined:
                           //
                           // Incorrect definition - (value: 1)
                           // Correct definition   - (value: 0)

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Checks for invalid B-tree nodes record - node array structures capacity and stored data.
*  Especially performs the main part of the procedure.
*
*  Debugging - testing tool.
*
*  Return value:  Nothing specific
*
*/

void BTreeCheckNodeCapacity_Tool(

        BTreeNode CurrentNode,   // B-tree node.
        int node_size,           // Maximum record references that can be stored in the node array structure.

        bool* error_flag         // Flag to check if the B-tree nodes capacity is correctly defined:
                                 //
                                 // Incorrect definition - (value: 1)
                                 // Correct definition   - (value: 0)

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Checks if the B-tree structure is balanced.
*
*  Debugging - testing tool.
*
*  Return value:
*
*  value: 0 - procedure is successful
*  value: 1 - procedure is unsuccessful
*
*/

bool BTreeCheckBalance(

        BTree BTree,            // B-tree index structure object.
        int BTree_height,       // B-tree height.

        int* unbalanced_nodes   // Number of unbalanced leaf nodes.
                                // Exception - (value > 0)

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Checks if the B-tree structure is balanced.
*  Especially performs the main part of the procedure.
*
*  Debugging - testing tool.
*
*  Return value:  Nothing specific
*
*/

void BTreeCheckBalance_Tool(

        BTreeNode CurrentNode,   // B-tree node.
        int node_height,         // Current node height.
        int BTree_height,        // B-tree height.

        int* unbalanced_nodes    // Number of unbalanced leaf nodes.
                                 // Exception - (value > 0)

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Checks if the nodes of the bottom - leaf level contain node references array structures.
*
*  Debugging - testing tool.
*
*  Return value:
*
*  value: 0 - procedure is successful
*  value: 1 - procedure is unsuccessful
*
*/

bool BTreeCheckLeafLinkNodes(

        BTree BTree,                       // B-tree index structure object.

        int* improperly_structured_nodes   // Number of improperly structured leaf nodes.
                                           // Exception - (value > 0)

);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*  Checks if the nodes of the bottom - leaf level contain node references array structures.
*  Especially performs the main part of the procedure.
*
*  Debugging - testing tool.
*
*  Return value:  Nothing specific
*
*/

void BTreeCheckLeafLinkNodes_Tool(

        BTreeNode CurrentNode,             // B-tree node.

        int* improperly_structured_nodes   // Number of improperly structured leaf nodes.
                                           // Exception - (value > 0)

);

