
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "BTree.h"



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



bool InitializeBTree(

        BTree* BTree,
        int node_size

)
{


    if (node_size < 3)
    {

        printf("Initialization is unsuccessful\n\n");

        return 1;

    }

    BTree->Root = NULL;
    BTree->node_size = node_size;
    BTree->size = 0;
    BTree->height = 0;

    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BTreeInsertData(

        BTree* BTree,
        BTreeData RecordData,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData),
        bool (*Deallocate_Record_Data)(BTreeData)

)
{


    if (BTree->Root == NULL)
    {

        BTree->Root = (BTreeNode)(malloc(sizeof(struct BTreeNode)));
        BTree->Root->leaf_type = 1;
        BTree->Root->array_stored_elements = 1;

        BTree->Root->RecordDataArray = (BTreeData*)(malloc((BTree->node_size / 2) * sizeof(BTreeData)));
        BTree->Root->RecordDataArray[0] = RecordData;

        BTree->Root->NodeLinkArray = NULL;
        BTree->Root->array_available_capacity = BTree->node_size / 2;


        BTree->size += 1;
        BTree->height += 1;

        return 0;

    }


    if (BTreeInsertNode_RootBreakTool(BTree, RecordData, Get_Record_PrimaryKey, Deallocate_Record_Data))
    {

        return 1;

    }

    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BTreeInsertNode_RootBreakTool(

        BTree* BTree,
        BTreeData RecordData,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData),
        bool (*Deallocate_Record_Data)(BTreeData)

)
{


    BTreeNode BTreeLeftNode = NULL;
    BTreeNode BTreeRightNode = NULL;
    BTreeData Node_MiddleRecordData;
    bool flag_duplicate_record = 0;


    BTreeRightNode = BTreeInsertNode_Tool( BTree->Root,
                                           RecordData,
                                           &Node_MiddleRecordData,
                                           BTree->node_size,
                                           &flag_duplicate_record,
                                           Get_Record_PrimaryKey,
                                           Deallocate_Record_Data );


    if (flag_duplicate_record)
    {

        return 1;

    }


    if (BTreeRightNode != NULL)
    {

        BTreeLeftNode = (BTreeNode)(malloc(sizeof(struct BTreeNode)));

        BTreeLeftNode->RecordDataArray = (BTreeData*)(malloc((BTree->node_size / 2) * sizeof(BTreeData)));


        if (!BTree->Root->leaf_type)
        {

            BTreeLeftNode->NodeLinkArray = (BTreeNode*)(malloc((BTree->node_size / 2 + 1) * sizeof(BTreeNode)));

        }
        else {

            BTreeLeftNode->NodeLinkArray = NULL;

        }


        BTreeLeftNode->leaf_type = BTree->Root->leaf_type;
        BTreeLeftNode->array_stored_elements = BTree->Root->array_stored_elements;
        BTreeLeftNode->array_available_capacity = BTree->node_size / 2;

        memmove( BTreeLeftNode->RecordDataArray,
                 BTree->Root->RecordDataArray,
                 sizeof(*(BTree->Root->RecordDataArray)) * BTreeLeftNode->array_stored_elements );


        if (!BTree->Root->leaf_type)
        {

            memmove( BTreeLeftNode->NodeLinkArray,
                     BTree->Root->NodeLinkArray,
                     sizeof(*(BTree->Root->NodeLinkArray)) * (BTreeLeftNode->array_stored_elements + 1) );

        }


        BTree->Root->RecordDataArray = (BTreeData*)(realloc(BTree->Root->RecordDataArray, (BTree->node_size / 2) * sizeof(BTreeData)));


        if (!BTree->Root->leaf_type)
        {

            BTree->Root->NodeLinkArray = (BTreeNode*)(realloc(BTree->Root->NodeLinkArray, (BTree->node_size / 2 + 1) * sizeof(BTreeNode)));

        }
        else {

            BTree->Root->NodeLinkArray = (BTreeNode*)(malloc((BTree->node_size / 2 + 1) * sizeof(BTreeNode)));

        }


        BTree->Root->array_stored_elements = 1;
        BTree->Root->leaf_type = 0;

        BTree->Root->RecordDataArray[0] = Node_MiddleRecordData;
        BTree->Root->NodeLinkArray[0] = BTreeLeftNode;
        BTree->Root->NodeLinkArray[1] = BTreeRightNode;
        BTree->Root->array_available_capacity = BTree->node_size / 2;


        BTree->height += 1;


    }


    BTree->size += 1;

    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BTreeNode BTreeInsertNode_Tool(

        BTreeNode CurrentNode,
        BTreeData RecordData,
        BTreeData* Node_MiddleRecordData,
        int node_size,
        bool* flag_duplicate_record,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData),
        bool (*Deallocate_Record_Data)(BTreeData)

)
{


    BTreeNode BTreeRightNode = NULL;
    int record_position;
    BTreeData middle_record_data;


    record_position = SearchBTreeNode_Record_ByPrimaryKey( CurrentNode->array_stored_elements,
                                                           CurrentNode->RecordDataArray,
                                                           Get_Record_PrimaryKey(RecordData),
                                                           Get_Record_PrimaryKey );


    if ( (record_position < CurrentNode->array_stored_elements)
         && (Get_Record_PrimaryKey(CurrentNode->RecordDataArray[record_position]) == Get_Record_PrimaryKey(RecordData)))
    {

        //printf("Duplicate index: %d\n\n", CurrentNode->RecordDataArray[record_position]->Record_PrimaryKey);

        if (RecordData != CurrentNode->RecordDataArray[record_position])
        {

            Deallocate_Record_Data(RecordData);

        }


        *flag_duplicate_record = 1;


        return NULL;

    }


    if (CurrentNode->leaf_type)
    {

        if (CurrentNode->array_available_capacity == (node_size / 2) && CurrentNode->array_stored_elements == (node_size / 2))
        {

            CurrentNode->RecordDataArray = (BTreeData*)(realloc(CurrentNode->RecordDataArray, node_size * sizeof(BTreeData)));
            CurrentNode->array_available_capacity = node_size;

        }


        memmove( &CurrentNode->RecordDataArray[record_position + 1],
                 &CurrentNode->RecordDataArray[record_position],
                 sizeof(*(CurrentNode->RecordDataArray)) * (CurrentNode->array_stored_elements - record_position) );

        CurrentNode->RecordDataArray[record_position] = RecordData;
        CurrentNode->array_stored_elements++;

    }
    else {

        BTreeRightNode = BTreeInsertNode_Tool( CurrentNode->NodeLinkArray[record_position],
                                               RecordData,
                                               &middle_record_data,
                                               node_size,
                                               flag_duplicate_record,
                                               Get_Record_PrimaryKey,
                                               Deallocate_Record_Data );


        if (BTreeRightNode != NULL)
        {

            if (CurrentNode->array_available_capacity == (node_size / 2) && CurrentNode->array_stored_elements == (node_size / 2))
            {

                CurrentNode->RecordDataArray = (BTreeData*)(realloc(CurrentNode->RecordDataArray, node_size * sizeof(BTreeData)));
                CurrentNode->NodeLinkArray = (BTreeNode*)(realloc(CurrentNode->NodeLinkArray, (node_size + 1) * sizeof(BTreeNode)));
                CurrentNode->array_available_capacity = node_size;

            }


            memmove( &CurrentNode->RecordDataArray[record_position + 1],
                     &CurrentNode->RecordDataArray[record_position],
                     sizeof(*(CurrentNode->RecordDataArray)) * (CurrentNode->array_stored_elements - record_position) );

            memmove( &CurrentNode->NodeLinkArray[record_position + 2],
                     &CurrentNode->NodeLinkArray[record_position + 1],
                     sizeof(*(CurrentNode->NodeLinkArray)) * (CurrentNode->array_stored_elements - record_position) );

            CurrentNode->RecordDataArray[record_position] = middle_record_data;
            CurrentNode->NodeLinkArray[record_position + 1] = BTreeRightNode;
            CurrentNode->array_stored_elements++;

        }
        else {

            return NULL;

        }


    }


    if (CurrentNode->array_stored_elements == node_size)
    {

        int middle_primary_key;


        if ((CurrentNode->array_stored_elements % 2) == 0)
        {

            middle_primary_key = (CurrentNode->array_stored_elements / 2) - 1;

        }
        else {

            middle_primary_key = CurrentNode->array_stored_elements / 2;

        }


        *Node_MiddleRecordData = CurrentNode->RecordDataArray[middle_primary_key];


        BTreeRightNode = (BTreeNode)(malloc(sizeof(struct BTreeNode)));

        BTreeRightNode->RecordDataArray = (BTreeData*)(malloc((node_size / 2) * sizeof(BTreeData)));


        if (!CurrentNode->leaf_type)
        {

            BTreeRightNode->NodeLinkArray = (BTreeNode*)(malloc((node_size / 2 + 1) * sizeof(BTreeNode)));

        }
        else {

            BTreeRightNode->NodeLinkArray = NULL;

        }


        BTreeRightNode->array_stored_elements = CurrentNode->array_stored_elements - middle_primary_key - 1;
        BTreeRightNode->leaf_type = CurrentNode->leaf_type;
        BTreeRightNode->array_available_capacity = node_size / 2;

        memmove( BTreeRightNode->RecordDataArray,
                 &CurrentNode->RecordDataArray[middle_primary_key + 1],
                 sizeof(*(CurrentNode->RecordDataArray)) * BTreeRightNode->array_stored_elements );


        if (!CurrentNode->leaf_type)
        {

            memmove( BTreeRightNode->NodeLinkArray,
                     &CurrentNode->NodeLinkArray[middle_primary_key + 1],
                     sizeof(*(CurrentNode->NodeLinkArray)) * (BTreeRightNode->array_stored_elements + 1) );

        }


        CurrentNode->RecordDataArray = (BTreeData*)(realloc(CurrentNode->RecordDataArray, (node_size / 2) * sizeof(BTreeData)));


        if (!CurrentNode->leaf_type)
        {

            CurrentNode->NodeLinkArray = (BTreeNode*)(realloc(CurrentNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BTreeNode)));

        }


        CurrentNode->array_stored_elements = middle_primary_key;
        CurrentNode->array_available_capacity = node_size / 2;

        return BTreeRightNode;

    }


    return NULL;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int SearchBTreeNode_Record_ByPrimaryKey(

        int array_stored_elements,
        BTreeData* RecordDataArray,
        BTreePrimaryKey PrimaryKey,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)

)
{


    return BTree_Interpolation_Search(array_stored_elements, RecordDataArray, PrimaryKey, Get_Record_PrimaryKey);


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int BTree_Binary_Search(

        int array_stored_elements,
        BTreeData* RecordDataArray,
        BTreePrimaryKey PrimaryKey,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)

)
{


    int first_index = 0;
    int last_index = array_stored_elements - 1;
    int middle_index = (first_index + last_index) / 2;
    int record_position = 0;


    if (PrimaryKey > Get_Record_PrimaryKey(RecordDataArray[array_stored_elements - 1]))
    {

        return array_stored_elements;

    }


    if (PrimaryKey < Get_Record_PrimaryKey(RecordDataArray[0]))
    {

        return 0;

    }


    while (first_index <= last_index)
    {

        if (Get_Record_PrimaryKey(RecordDataArray[middle_index]) < PrimaryKey)
        {

            record_position = middle_index + 1;
            first_index = middle_index + 1;

        }
        else if (Get_Record_PrimaryKey(RecordDataArray[middle_index]) == PrimaryKey)
        {

            record_position = middle_index;

            break;

        }
        else {

            record_position = middle_index;
            last_index = middle_index - 1;

        }


        middle_index = (first_index + last_index) / 2;


    }


    return record_position;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int BTree_Interpolation_Search(

        int array_stored_elements,
        BTreeData* RecordDataArray,
        BTreePrimaryKey PrimaryKey,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)

)
{


    int low_index = 0;
    int high_index = (array_stored_elements - 1);
    BTreePrimaryKey primary_key_low_index = 0;
    BTreePrimaryKey primary_key_high_index = 0;
    int record_position = 0;
    int range_position = 0;


    if (PrimaryKey > Get_Record_PrimaryKey(RecordDataArray[high_index]))
    {

        return high_index + 1;

    }

    if (PrimaryKey < Get_Record_PrimaryKey(RecordDataArray[low_index]))
    {

        return low_index;

    }


    while ( low_index <= high_index
            && PrimaryKey >= Get_Record_PrimaryKey(RecordDataArray[low_index])
            && PrimaryKey <= Get_Record_PrimaryKey(RecordDataArray[high_index]) )
    {

        if (low_index == high_index)
        {

            if (Get_Record_PrimaryKey(RecordDataArray[low_index]) == PrimaryKey)
            {

                return low_index;

            }

            if (PrimaryKey > Get_Record_PrimaryKey(RecordDataArray[low_index]))
            {

                return low_index + 1;

            }

            return low_index;

        }


        primary_key_low_index = Get_Record_PrimaryKey(RecordDataArray[low_index]);
        primary_key_high_index = Get_Record_PrimaryKey(RecordDataArray[high_index]);

        range_position = low_index + (((double)(high_index - low_index)
                         / (primary_key_high_index - primary_key_low_index)) * (PrimaryKey - primary_key_low_index));


        if (Get_Record_PrimaryKey(RecordDataArray[range_position]) == PrimaryKey)
        {

            return range_position;

        }
        else if (Get_Record_PrimaryKey(RecordDataArray[range_position]) < PrimaryKey)
        {

            record_position = range_position + 1;
            low_index = range_position + 1;

        }
        else
        {

            record_position = range_position;
            high_index = range_position - 1;

        }

    }


    return record_position;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BTreeData BTreeSearchData(

        BTree BTree,
        BTreePrimaryKey PrimaryKey,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)

)
{


    if (BTree.Root == NULL)
    {

        printf("B-tree is empty\n\n");

        return NULL;

    }


    return BTreeSearch_Tool(BTree.Root, PrimaryKey, Get_Record_PrimaryKey);


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BTreeData BTreeSearch_Tool(

        BTreeNode CurrentNode,
        BTreePrimaryKey PrimaryKey,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)

)
{


    int record_position = CurrentNode->array_stored_elements;


    for (int i = 0; i < CurrentNode->array_stored_elements; i++)
    {

        printf("Node Search - Primary Key Field: (%d)\n", Get_Record_PrimaryKey(CurrentNode->RecordDataArray[i]));


        if (PrimaryKey <= Get_Record_PrimaryKey(CurrentNode->RecordDataArray[i]))
        {

            if (PrimaryKey == Get_Record_PrimaryKey(CurrentNode->RecordDataArray[i]))
            {

                return CurrentNode->RecordDataArray[i];

            }

            record_position = i;

            break;

        }

    }


    printf("--------------------- Next Level ---------------------\n\n");


    if (!CurrentNode->leaf_type)
    {

        return BTreeSearch_Tool( CurrentNode->NodeLinkArray[record_position],
                                 PrimaryKey,
                                 Get_Record_PrimaryKey );

    }


    return NULL;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BTreeData BTreeFastSearchData_ByPrimaryKey(

        BTree BTree,
        BTreePrimaryKey PrimaryKey,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)

)
{


    if (BTree.Root == NULL)
    {

        //printf("B-tree is empty\n\n");

        return NULL;

    }


    return BTreeFastSearch_Tool(BTree.Root, PrimaryKey, Get_Record_PrimaryKey);


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BTreeData BTreeFastSearch_Tool(

        BTreeNode CurrentNode,
        BTreePrimaryKey PrimaryKey,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)

)
{


    int record_position = CurrentNode->array_stored_elements;

    record_position = SearchBTreeNode_Record_ByPrimaryKey( CurrentNode->array_stored_elements,
                                                           CurrentNode->RecordDataArray,
                                                           PrimaryKey,
                                                           Get_Record_PrimaryKey );


    if( (record_position < CurrentNode->array_stored_elements)
        && (Get_Record_PrimaryKey(CurrentNode->RecordDataArray[record_position]) == PrimaryKey) )
    {

        return CurrentNode->RecordDataArray[record_position];

    }


    if (!CurrentNode->leaf_type)
    {

        return BTreeFastSearch_Tool( CurrentNode->NodeLinkArray[record_position],
                                     PrimaryKey,
                                     Get_Record_PrimaryKey );

    }


    return NULL;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTreeDisplayInOrder(

        BTree BTree,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)

)
{


    if (BTree.Root == NULL)
    {

        printf("B-tree is empty\n\n");

        return;

    }


    BTreeInOrderTraversal(BTree.Root, Get_Record_PrimaryKey);

    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTreeInOrderTraversal(

        BTreeNode CurrentNode,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)

)
{


    if (CurrentNode->leaf_type)
    {


        for (int i = 0; i < CurrentNode->array_stored_elements; i++)
        {

            printf(" (Leaf Node): (%d)", Get_Record_PrimaryKey(CurrentNode->RecordDataArray[i]));

        }

        printf("\n");

        return;

    }


    for (int i = 0; i < CurrentNode->array_stored_elements + 1; i++)
    {

        BTreeInOrderTraversal(CurrentNode->NodeLinkArray[i], Get_Record_PrimaryKey);


        if ((i + 1) < (CurrentNode->array_stored_elements + 1))
        {

            printf(" (Internal Node): (%d)\n", Get_Record_PrimaryKey(CurrentNode->RecordDataArray[i]));

        }

    }


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BTreeDeleteData(

        BTree* BTree,
        BTreePrimaryKey PrimaryKey,
        BTreeData* DeletedRecordData,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)

)
{


    if (BTree->Root == NULL)
    {

        //printf("B-tree is empty\n\n");

        *DeletedRecordData = NULL;

        return 1;

    }


    if (BTree->Root->leaf_type)
    {

        int record_position = SearchBTreeNode_Record_ByPrimaryKey( BTree->Root->array_stored_elements,
                                                                   BTree->Root->RecordDataArray,
                                                                   PrimaryKey,
                                                                   Get_Record_PrimaryKey );


        if ( (record_position < BTree->Root->array_stored_elements)
             && (Get_Record_PrimaryKey(BTree->Root->RecordDataArray[record_position]) == PrimaryKey) )
        {

            *DeletedRecordData = BTree->Root->RecordDataArray[record_position];

            if (BTree->Root->array_stored_elements == 1)
            {

                free(BTree->Root->RecordDataArray);
                free(BTree->Root);

                InitializeBTree(BTree, BTree->node_size);

                return 0;

            }


            BTree_ReplaceRecord(BTree->Root, record_position, BTree->node_size);


            BTree->size -= 1;

            return 0;

        }


        *DeletedRecordData = NULL;

        return 1;

    }


    bool balancing_flag = 0;
    bool internal_record_deletion_flag = 0;
    bool record_existence_flag = 0;


    BTree->Root = BTreeDeleteNode( BTree->Root,
                                   PrimaryKey,
                                   DeletedRecordData,
                                   BTree->node_size,
                                   &balancing_flag,
                                   &internal_record_deletion_flag,
                                   &record_existence_flag,
                                   Get_Record_PrimaryKey );


    if (record_existence_flag == 0)
    {

        BTree->size -= 1;

    }
    else {

        return 1;

    }


    if (balancing_flag == 1)
    {

        BTree->height -= 1;

    }


    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BTreeNode BTreeDeleteNode(

        BTreeNode CurrentNode,
        BTreePrimaryKey PrimaryKey,
        BTreeData* DeletedRecordData,
        int node_size,
        bool* balancing_flag,
        bool* internal_record_deletion_flag,
        bool* record_existence_flag,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)

)
{


    int record_position = SearchBTreeNode_Record_ByPrimaryKey( CurrentNode->array_stored_elements,
                                                               CurrentNode->RecordDataArray,
                                                               PrimaryKey,
                                                               Get_Record_PrimaryKey );


    if ( (record_position < CurrentNode->array_stored_elements)
         && (Get_Record_PrimaryKey(CurrentNode->RecordDataArray[record_position]) == PrimaryKey) )
    {

        BTreeData CopyData = NULL;

        *DeletedRecordData = CurrentNode->RecordDataArray[record_position];

        CopyData = BTree_LeftSubTree_MaxRecord(CurrentNode->NodeLinkArray[record_position]);
        CurrentNode->RecordDataArray[record_position] = CopyData;
        PrimaryKey = Get_Record_PrimaryKey(CopyData);

        *internal_record_deletion_flag = 1;

    }


    if (!CurrentNode->NodeLinkArray[record_position]->leaf_type)
    {

        BTreeDeleteNode( CurrentNode->NodeLinkArray[record_position],
                         PrimaryKey,
                         DeletedRecordData,
                         node_size,
                         balancing_flag,
                         internal_record_deletion_flag,
                         record_existence_flag,
                         Get_Record_PrimaryKey );

        if (*balancing_flag == 1)
        {

            BTreeDelete_NonLeafNode(CurrentNode, record_position, node_size, balancing_flag);

        }
        else {

            *balancing_flag = 0;

        }


        return CurrentNode;

    }


    BTreeDelete_LeafNode( CurrentNode,
                          PrimaryKey,
                          DeletedRecordData,
                          record_position,
                          node_size,
                          balancing_flag,
                          *internal_record_deletion_flag,
                          record_existence_flag,
                          Get_Record_PrimaryKey );


    return CurrentNode;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTreeDelete_NonLeafNode(

        BTreeNode CurrentNode,
        int record_position,
        int node_size,
        bool* balancing_flag

)
{


    if (record_position > 0)
    {

        BTreeNode LeftNode = CurrentNode->NodeLinkArray[record_position - 1];


        if (CurrentNode->array_stored_elements == 1)
        {

            if (LeftNode->array_stored_elements == 1)
            {

                BTree_MergeLeftNodeRecursive(CurrentNode, LeftNode, node_size);

                *balancing_flag = 1;

            }
            else {

                BTree_ReplaceLeftNodeRecursive(CurrentNode, LeftNode, node_size);

                *balancing_flag = 0;

            }

        }
        else {

            if (LeftNode->array_stored_elements > 1)
            {

                BTree_ReplaceMultipleLeftNodeRecursive(CurrentNode, LeftNode, record_position, node_size);

                *balancing_flag = 0;

            }
            else {

                BTree_ReplaceSingleLeftNodeRecursive(CurrentNode, LeftNode, record_position, node_size);

                *balancing_flag = 0;

            }

        }

    }
    else {

        BTreeNode RightNode = CurrentNode->NodeLinkArray[record_position + 1];


        if (CurrentNode->array_stored_elements == 1)
        {

            if (RightNode->array_stored_elements == 1)
            {

                BTree_MergeRightNodeRecursive(CurrentNode, RightNode, node_size);

                *balancing_flag = 1;

            }
            else {

                BTree_ReplaceRightNodeRecursive(CurrentNode, RightNode, node_size);

                *balancing_flag = 0;

            }

        }
        else {

            if (RightNode->array_stored_elements > 1)
            {

                BTree_ReplaceMultipleRightNodeRecursive(CurrentNode, RightNode, record_position, node_size);

                *balancing_flag = 0;

            }
            else {

                BTree_ReplaceSingleRightNodeRecursive(CurrentNode, RightNode, record_position, node_size);

                *balancing_flag = 0;

            }

        }

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTreeDelete_LeafNode(

        BTreeNode CurrentNode,
        BTreePrimaryKey PrimaryKey,
        BTreeData* DeletedRecordData,
        int record_position,
        int node_size,
        bool* balancing_flag,
        bool internal_record_deletion_flag,
        bool* record_existence_flag,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)

)
{


    BTreeNode NextLevelNode = CurrentNode->NodeLinkArray[record_position];

    int deletion_position = SearchBTreeNode_Record_ByPrimaryKey( NextLevelNode->array_stored_elements,
                                                                 NextLevelNode->RecordDataArray,
                                                                 PrimaryKey,
                                                                 Get_Record_PrimaryKey );


    if ( (deletion_position < NextLevelNode->array_stored_elements)
         && (Get_Record_PrimaryKey(NextLevelNode->RecordDataArray[deletion_position]) == PrimaryKey) )
    {

        if (internal_record_deletion_flag == 0)
        {

            *DeletedRecordData = NextLevelNode->RecordDataArray[deletion_position];

        }


        if (NextLevelNode->array_stored_elements > 1)
        {

            BTree_ReplaceRecord(NextLevelNode, deletion_position, node_size);

            *balancing_flag = 0;

            return;

        }


        if (CurrentNode->array_stored_elements == 1)
        {



            if ((CurrentNode->NodeLinkArray[0]->array_stored_elements == 1) && (CurrentNode->NodeLinkArray[1]->array_stored_elements == 1))
            {

                if (record_position == 0)
                {

                    BTree_MergeRightNode(CurrentNode, node_size);

                    *balancing_flag = 1;

                }


                if (record_position == 1)
                {

                    BTree_MergeLeftNode(CurrentNode, node_size);

                    *balancing_flag = 1;

                }


                return;

            }

            if ((CurrentNode->NodeLinkArray[0]->array_stored_elements > 1) || (CurrentNode->NodeLinkArray[1]->array_stored_elements > 1))
            {

                if (CurrentNode->NodeLinkArray[1]->array_stored_elements > 1)
                {

                    BTree_RebalanceRightNode(CurrentNode, NextLevelNode, node_size);

                }
                else {

                    BTree_RebalanceLeftNode(CurrentNode, NextLevelNode, node_size);

                }


                *balancing_flag = 0;

                return;

            }


        }

        if (CurrentNode->array_stored_elements > 1)
        {

            if (record_position == CurrentNode->array_stored_elements)
            {

                if (CurrentNode->NodeLinkArray[record_position - 1]->array_stored_elements > 1)
                {

                    BTree_SwapLeftNode(CurrentNode, NextLevelNode, record_position, node_size);

                }
                else {

                    BTree_MergeSingleNodeRight(CurrentNode, record_position, node_size);

                }

            }
            else {

                if (record_position == 0)
                {

                    if (CurrentNode->NodeLinkArray[record_position + 1]->array_stored_elements > 1)
                    {

                        BTree_SwapRightNode(CurrentNode, NextLevelNode, record_position, node_size);

                    }
                    else {

                        BTree_MergeSingleNodeLeft(CurrentNode, NextLevelNode, record_position, node_size);

                    }

                }
                else {

                    if (CurrentNode->NodeLinkArray[record_position - 1]->array_stored_elements > 1)
                    {

                        BTree_SwapLeftNode(CurrentNode, NextLevelNode, record_position, node_size);

                    }
                    else {

                        if (CurrentNode->NodeLinkArray[record_position + 1]->array_stored_elements > 1)
                        {

                            BTree_SwapRightNode(CurrentNode, NextLevelNode, record_position, node_size);

                        }
                        else {

                            BTree_MergeSingleNodeRight(CurrentNode, record_position, node_size);

                        }

                    }

                }

            }


            *balancing_flag = 0;

            return;

        }


    }


    *DeletedRecordData = NULL;

    *record_existence_flag = 1;


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTree_ReplaceRecord(

        BTreeNode CurrentNode,
        int record_position,
        int node_size

)
{


    for (int i = record_position; i < (CurrentNode->array_stored_elements - 1); i++)
    {

        CurrentNode->RecordDataArray[i] = CurrentNode->RecordDataArray[i + 1];

    }


    CurrentNode->array_stored_elements--;


    if (CurrentNode->array_available_capacity == node_size && (CurrentNode->array_stored_elements + 1) == (node_size / 2))
    {

        CurrentNode->RecordDataArray = (BTreeData*)(realloc(CurrentNode->RecordDataArray, (node_size / 2) * sizeof(BTreeData)));
        CurrentNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTree_MergeLeftNode(

        BTreeNode CurrentNode,
        int node_size

)
{


    CurrentNode->array_stored_elements++;
    CurrentNode->leaf_type = 1;


    if (CurrentNode->array_available_capacity == (node_size / 2) && (CurrentNode->array_stored_elements - 1) == (node_size / 2))
    {

        CurrentNode->RecordDataArray = (BTreeData*)(realloc(CurrentNode->RecordDataArray, node_size * sizeof(BTreeData)));
        CurrentNode->array_available_capacity = node_size;

    }


    CurrentNode->RecordDataArray[1] = CurrentNode->RecordDataArray[0];
    CurrentNode->RecordDataArray[0] = CurrentNode->NodeLinkArray[0]->RecordDataArray[0];


    free(CurrentNode->NodeLinkArray[0]->RecordDataArray);
    free(CurrentNode->NodeLinkArray[0]);

    free(CurrentNode->NodeLinkArray[1]->RecordDataArray);
    free(CurrentNode->NodeLinkArray[1]);

    free(CurrentNode->NodeLinkArray);
    CurrentNode->NodeLinkArray = NULL;

    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTree_MergeRightNode(

        BTreeNode CurrentNode,
        int node_size

)
{


    CurrentNode->array_stored_elements++;
    CurrentNode->leaf_type = 1;


    if (CurrentNode->array_available_capacity == (node_size / 2) && (CurrentNode->array_stored_elements - 1) == (node_size / 2))
    {

        CurrentNode->RecordDataArray = (BTreeData*)(realloc(CurrentNode->RecordDataArray, node_size * sizeof(BTreeData)));
        CurrentNode->array_available_capacity = node_size;

    }


    CurrentNode->RecordDataArray[1] = CurrentNode->NodeLinkArray[1]->RecordDataArray[0];

    free(CurrentNode->NodeLinkArray[0]->RecordDataArray);
    free(CurrentNode->NodeLinkArray[0]);

    free(CurrentNode->NodeLinkArray[1]->RecordDataArray);
    free(CurrentNode->NodeLinkArray[1]);

    free(CurrentNode->NodeLinkArray);
    CurrentNode->NodeLinkArray = NULL;

    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTree_RebalanceLeftNode(

        BTreeNode CurrentNode,
        BTreeNode NextLevelNode,
        int node_size

)
{


    NextLevelNode->RecordDataArray[0] = CurrentNode->RecordDataArray[0];
    CurrentNode->RecordDataArray[0] = CurrentNode->NodeLinkArray[0]->RecordDataArray[CurrentNode->NodeLinkArray[0]->array_stored_elements - 1];

    CurrentNode->NodeLinkArray[0]->array_stored_elements--;


    if (CurrentNode->NodeLinkArray[0]->array_available_capacity == node_size && (CurrentNode->NodeLinkArray[0]->array_stored_elements + 1) == (node_size / 2))
    {

        CurrentNode->NodeLinkArray[0]->RecordDataArray = (BTreeData*)(realloc(CurrentNode->NodeLinkArray[0]->RecordDataArray, (node_size / 2) * sizeof(BTreeData)));
        CurrentNode->NodeLinkArray[0]->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTree_RebalanceRightNode(

        BTreeNode CurrentNode,
        BTreeNode NextLevelNode,
        int node_size

)
{


    NextLevelNode->RecordDataArray[0] = CurrentNode->RecordDataArray[0];
    CurrentNode->RecordDataArray[0] = CurrentNode->NodeLinkArray[1]->RecordDataArray[0];


    for (int i = 0; i < (CurrentNode->NodeLinkArray[1]->array_stored_elements - 1); i++)
    {

        CurrentNode->NodeLinkArray[1]->RecordDataArray[i] = CurrentNode->NodeLinkArray[1]->RecordDataArray[i + 1];

    }


    CurrentNode->NodeLinkArray[1]->array_stored_elements--;


    if (CurrentNode->NodeLinkArray[1]->array_available_capacity == node_size && (CurrentNode->NodeLinkArray[1]->array_stored_elements + 1) == (node_size / 2))
    {

        CurrentNode->NodeLinkArray[1]->RecordDataArray = (BTreeData*)(realloc(CurrentNode->NodeLinkArray[1]->RecordDataArray, (node_size / 2) * sizeof(BTreeData)));
        CurrentNode->NodeLinkArray[1]->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTree_SwapLeftNode(

        BTreeNode CurrentNode,
        BTreeNode NextLevelNode,
        int record_position,
        int node_size

)
{


    BTreeNode PreviousNode = CurrentNode->NodeLinkArray[record_position - 1];

    NextLevelNode->RecordDataArray[0] = CurrentNode->RecordDataArray[record_position - 1];
    CurrentNode->RecordDataArray[record_position - 1] = PreviousNode->RecordDataArray[PreviousNode->array_stored_elements - 1];

    PreviousNode->array_stored_elements--;


    if (PreviousNode->array_available_capacity == node_size && (PreviousNode->array_stored_elements + 1) == (node_size / 2))
    {

        PreviousNode->RecordDataArray = (BTreeData*)(realloc(PreviousNode->RecordDataArray, (node_size / 2) * sizeof(BTreeData)));
        PreviousNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTree_SwapRightNode(

        BTreeNode CurrentNode,
        BTreeNode NextLevelNode,
        int record_position,
        int node_size

)
{


    BTreeNode NextNode = CurrentNode->NodeLinkArray[record_position + 1];

    NextLevelNode->RecordDataArray[0] = CurrentNode->RecordDataArray[record_position];
    CurrentNode->RecordDataArray[record_position] = NextNode->RecordDataArray[0];


    for (int i = 0; i < (NextNode->array_stored_elements - 1); i++)
    {

        NextNode->RecordDataArray[i] = NextNode->RecordDataArray[i + 1];

    }


    NextNode->array_stored_elements--;


    if (NextNode->array_available_capacity == node_size && (NextNode->array_stored_elements + 1) == (node_size / 2))
    {

        NextNode->RecordDataArray = (BTreeData*)(realloc(NextNode->RecordDataArray, (node_size / 2) * sizeof(BTreeData)));
        NextNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTree_MergeSingleNodeRight(

        BTreeNode CurrentNode,
        int record_position,
        int node_size

)
{


    BTreeNode PreviousNode = CurrentNode->NodeLinkArray[record_position - 1];

    PreviousNode->array_stored_elements++;


    if (PreviousNode->array_available_capacity == (node_size / 2) && (PreviousNode->array_stored_elements - 1) == (node_size / 2))
    {

        PreviousNode->RecordDataArray = (BTreeData*)(realloc(PreviousNode->RecordDataArray, node_size * sizeof(BTreeData)));
        PreviousNode->array_available_capacity = node_size;

    }


    PreviousNode->RecordDataArray[PreviousNode->array_stored_elements - 1] = CurrentNode->RecordDataArray[record_position - 1];

    free(CurrentNode->NodeLinkArray[record_position]->RecordDataArray);
    free(CurrentNode->NodeLinkArray[record_position]);


    for (int i = (record_position - 1); i < (CurrentNode->array_stored_elements - 1); i++)
    {

        CurrentNode->RecordDataArray[i] = CurrentNode->RecordDataArray[i + 1];
        CurrentNode->NodeLinkArray[i + 1] = CurrentNode->NodeLinkArray[i + 2];

    }


    CurrentNode->array_stored_elements--;


    if (CurrentNode->array_available_capacity == node_size && (CurrentNode->array_stored_elements + 1) == (node_size / 2))
    {

        CurrentNode->RecordDataArray = (BTreeData*)(realloc(CurrentNode->RecordDataArray, (node_size / 2) * sizeof(BTreeData)));
        CurrentNode->NodeLinkArray = (BTreeNode*)(realloc(CurrentNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BTreeNode)));
        CurrentNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTree_MergeSingleNodeLeft(

        BTreeNode CurrentNode,
        BTreeNode NextLevelNode,
        int record_position,
        int node_size

)
{


    BTreeNode NextNode = CurrentNode->NodeLinkArray[record_position + 1];

    NextLevelNode->array_stored_elements++;


    if (NextLevelNode->array_available_capacity == (node_size / 2) && (NextLevelNode->array_stored_elements - 1) == (node_size / 2))
    {

        NextLevelNode->RecordDataArray = (BTreeData*)(realloc(NextLevelNode->RecordDataArray, node_size * sizeof(BTreeData)));
        NextLevelNode->array_available_capacity = node_size;

    }


    NextLevelNode->RecordDataArray[0] = CurrentNode->RecordDataArray[record_position];
    NextLevelNode->RecordDataArray[1] = NextNode->RecordDataArray[0];

    free(NextNode->RecordDataArray);
    free(NextNode);


    for (int i = record_position; i < (CurrentNode->array_stored_elements - 1); i++)
    {

        CurrentNode->RecordDataArray[i] = CurrentNode->RecordDataArray[i + 1];
        CurrentNode->NodeLinkArray[i + 1] = CurrentNode->NodeLinkArray[i + 2];

    }


    CurrentNode->array_stored_elements--;


    if (CurrentNode->array_available_capacity == node_size && (CurrentNode->array_stored_elements + 1) == (node_size / 2))
    {

        CurrentNode->RecordDataArray = (BTreeData*)(realloc(CurrentNode->RecordDataArray, (node_size / 2) * sizeof(BTreeData)));
        CurrentNode->NodeLinkArray = (BTreeNode*)(realloc(CurrentNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BTreeNode)));
        CurrentNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTree_MergeLeftNodeRecursive(

        BTreeNode CurrentNode,
        BTreeNode LeftNode,
        int node_size

)
{


    CurrentNode->array_stored_elements++;


    if (CurrentNode->array_available_capacity == (node_size / 2) && (CurrentNode->array_stored_elements - 1) == (node_size / 2))
    {

        CurrentNode->RecordDataArray = (BTreeData*)(realloc(CurrentNode->RecordDataArray, node_size * sizeof(BTreeData)));
        CurrentNode->NodeLinkArray = (BTreeNode*)(realloc(CurrentNode->NodeLinkArray, (node_size + 1) * sizeof(BTreeNode)));
        CurrentNode->array_available_capacity = node_size;

    }


    CurrentNode->RecordDataArray[1] = CurrentNode->RecordDataArray[0];
    CurrentNode->RecordDataArray[0] = LeftNode->RecordDataArray[0];

    CurrentNode->NodeLinkArray[2] = CurrentNode->NodeLinkArray[1];
    CurrentNode->NodeLinkArray[1] = LeftNode->NodeLinkArray[1];
    CurrentNode->NodeLinkArray[0] = LeftNode->NodeLinkArray[0];

    free(LeftNode->RecordDataArray);
    free(LeftNode->NodeLinkArray);
    free(LeftNode);

    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTree_ReplaceLeftNodeRecursive(

        BTreeNode CurrentNode,
        BTreeNode LeftNode,
        int node_size

)
{


    BTreeNode NewNode = (BTreeNode)(malloc(sizeof(struct BTreeNode)));

    NewNode->array_stored_elements = 1;
    NewNode->leaf_type = 0;
    NewNode->RecordDataArray = (BTreeData*)(malloc((node_size / 2) * sizeof(BTreeData)));
    NewNode->NodeLinkArray = (BTreeNode*)(malloc((node_size / 2 + 1) * sizeof(BTreeNode)));
    NewNode->array_available_capacity = node_size / 2;

    NewNode->RecordDataArray[0] = CurrentNode->RecordDataArray[0];
    NewNode->NodeLinkArray[0] = LeftNode->NodeLinkArray[LeftNode->array_stored_elements];
    NewNode->NodeLinkArray[1] = CurrentNode->NodeLinkArray[1];
    CurrentNode->NodeLinkArray[1] = NewNode;
    CurrentNode->RecordDataArray[0] = LeftNode->RecordDataArray[LeftNode->array_stored_elements - 1];

    LeftNode->array_stored_elements--;


    if (LeftNode->array_available_capacity == node_size && (LeftNode->array_stored_elements + 1) == (node_size / 2))
    {

        LeftNode->RecordDataArray = (BTreeData*)(realloc(LeftNode->RecordDataArray, (node_size / 2) * sizeof(BTreeData)));
        LeftNode->NodeLinkArray = (BTreeNode*)(realloc(LeftNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BTreeNode)));
        LeftNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTree_MergeRightNodeRecursive(

        BTreeNode CurrentNode,
        BTreeNode RightNode,
        int node_size

)
{


    CurrentNode->array_stored_elements++;


    if (CurrentNode->array_available_capacity == (node_size / 2) && (CurrentNode->array_stored_elements - 1) == (node_size / 2))
    {

        CurrentNode->RecordDataArray = (BTreeData*)(realloc(CurrentNode->RecordDataArray, node_size * sizeof(BTreeData)));
        CurrentNode->NodeLinkArray = (BTreeNode*)(realloc(CurrentNode->NodeLinkArray, (node_size + 1) * sizeof(BTreeNode)));
        CurrentNode->array_available_capacity = node_size;

    }


    CurrentNode->RecordDataArray[1] = RightNode->RecordDataArray[0];

    CurrentNode->NodeLinkArray[1] = RightNode->NodeLinkArray[0];
    CurrentNode->NodeLinkArray[2] = RightNode->NodeLinkArray[1];

    free(RightNode->RecordDataArray);
    free(RightNode->NodeLinkArray);
    free(RightNode);

    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTree_ReplaceRightNodeRecursive(

        BTreeNode CurrentNode,
        BTreeNode RightNode,
        int node_size

)
{


    BTreeNode NewNode = (BTreeNode)(malloc(sizeof(struct BTreeNode)));

    NewNode->array_stored_elements = 1;
    NewNode->leaf_type = 0;
    NewNode->RecordDataArray = (BTreeData*)(malloc((node_size / 2) * sizeof(BTreeData)));
    NewNode->NodeLinkArray = (BTreeNode*)(malloc((node_size / 2 + 1) * sizeof(BTreeNode)));
    NewNode->array_available_capacity = node_size / 2;

    NewNode->RecordDataArray[0] = CurrentNode->RecordDataArray[0];
    NewNode->NodeLinkArray[0] = CurrentNode->NodeLinkArray[0];
    NewNode->NodeLinkArray[1] = RightNode->NodeLinkArray[0];
    CurrentNode->NodeLinkArray[0] = NewNode;
    CurrentNode->RecordDataArray[0] = RightNode->RecordDataArray[0];


    for (int i = 0; i < (RightNode->array_stored_elements - 1); i++)
    {

        RightNode->RecordDataArray[i] = RightNode->RecordDataArray[i + 1];

    }


    for (int i = 0; i < RightNode->array_stored_elements; i++)
    {

        RightNode->NodeLinkArray[i] = RightNode->NodeLinkArray[i + 1];

    }


    RightNode->array_stored_elements--;


    if (RightNode->array_available_capacity == node_size && (RightNode->array_stored_elements + 1) == (node_size / 2))
    {

        RightNode->RecordDataArray = (BTreeData*)(realloc(RightNode->RecordDataArray, (node_size / 2) * sizeof(BTreeData)));
        RightNode->NodeLinkArray = (BTreeNode*)(realloc(RightNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BTreeNode)));
        RightNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTree_ReplaceMultipleLeftNodeRecursive(

        BTreeNode CurrentNode,
        BTreeNode LeftNode,
        int record_position,
        int node_size

)
{


    BTreeNode NewNode = (BTreeNode)(malloc(sizeof(struct BTreeNode)));

    NewNode->array_stored_elements = 1;
    NewNode->leaf_type = 0;
    NewNode->RecordDataArray = (BTreeData*)(malloc((node_size / 2) * sizeof(BTreeData)));
    NewNode->NodeLinkArray = (BTreeNode*)(malloc((node_size / 2 + 1) * sizeof(BTreeNode)));
    NewNode->array_available_capacity = node_size / 2;

    NewNode->RecordDataArray[0] = CurrentNode->RecordDataArray[record_position - 1];
    NewNode->NodeLinkArray[0] = LeftNode->NodeLinkArray[LeftNode->array_stored_elements];
    NewNode->NodeLinkArray[1] = CurrentNode->NodeLinkArray[record_position];
    CurrentNode->NodeLinkArray[record_position] = NewNode;
    CurrentNode->RecordDataArray[record_position - 1] = LeftNode->RecordDataArray[LeftNode->array_stored_elements - 1];

    LeftNode->array_stored_elements--;


    if (LeftNode->array_available_capacity == node_size && (LeftNode->array_stored_elements + 1) == (node_size / 2))
    {

        LeftNode->RecordDataArray = (BTreeData*)(realloc(LeftNode->RecordDataArray, (node_size / 2) * sizeof(BTreeData)));
        LeftNode->NodeLinkArray = (BTreeNode*)(realloc(LeftNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BTreeNode)));
        LeftNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTree_ReplaceSingleLeftNodeRecursive(

        BTreeNode CurrentNode,
        BTreeNode LeftNode,
        int record_position,
        int node_size

)
{


    LeftNode->array_stored_elements++;

    if (LeftNode->array_available_capacity == (node_size / 2) && (LeftNode->array_stored_elements - 1) == (node_size / 2))
    {

        LeftNode->RecordDataArray = (BTreeData*)(realloc(LeftNode->RecordDataArray, node_size * sizeof(BTreeData)));
        LeftNode->NodeLinkArray = (BTreeNode*)(realloc(LeftNode->NodeLinkArray, (node_size + 1) * sizeof(BTreeNode)));
        LeftNode->array_available_capacity = node_size;

    }


    LeftNode->RecordDataArray[LeftNode->array_stored_elements - 1] = CurrentNode->RecordDataArray[record_position - 1];
    LeftNode->NodeLinkArray[LeftNode->array_stored_elements] = CurrentNode->NodeLinkArray[record_position];


    for (int i = record_position - 1; i < (CurrentNode->array_stored_elements - 1); i++)
    {

        CurrentNode->RecordDataArray[i] = CurrentNode->RecordDataArray[i + 1];
        CurrentNode->NodeLinkArray[i + 1] = CurrentNode->NodeLinkArray[i + 2];

    }


    CurrentNode->array_stored_elements--;


    if (CurrentNode->array_available_capacity == node_size && (CurrentNode->array_stored_elements + 1) == (node_size / 2))
    {

        CurrentNode->RecordDataArray = (BTreeData*)(realloc(CurrentNode->RecordDataArray, (node_size / 2) * sizeof(BTreeData)));
        CurrentNode->NodeLinkArray = (BTreeNode*)(realloc(CurrentNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BTreeNode)));
        CurrentNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTree_ReplaceMultipleRightNodeRecursive(

        BTreeNode CurrentNode,
        BTreeNode RightNode,
        int record_position,
        int node_size

)
{


    BTreeNode NewNode = (BTreeNode)(malloc(sizeof(struct BTreeNode)));

    NewNode->array_stored_elements = 1;
    NewNode->leaf_type = 0;
    NewNode->RecordDataArray = (BTreeData*)(malloc((node_size / 2) * sizeof(BTreeData)));
    NewNode->NodeLinkArray = (BTreeNode*)(malloc((node_size / 2 + 1) * sizeof(BTreeNode)));
    NewNode->array_available_capacity = node_size / 2;

    NewNode->RecordDataArray[0] = CurrentNode->RecordDataArray[record_position];
    NewNode->NodeLinkArray[0] = CurrentNode->NodeLinkArray[record_position];
    NewNode->NodeLinkArray[1] = RightNode->NodeLinkArray[0];
    CurrentNode->NodeLinkArray[record_position] = NewNode;
    CurrentNode->RecordDataArray[record_position] = RightNode->RecordDataArray[0];


    for (int i = 0; i < (RightNode->array_stored_elements - 1); i++)
    {

        RightNode->RecordDataArray[i] = RightNode->RecordDataArray[i + 1];

    }


    for (int i = 0; i < RightNode->array_stored_elements; i++)
    {

        RightNode->NodeLinkArray[i] = RightNode->NodeLinkArray[i + 1];

    }


    RightNode->array_stored_elements--;


    if (RightNode->array_available_capacity == node_size && (RightNode->array_stored_elements + 1) == (node_size / 2))
    {

        RightNode->RecordDataArray = (BTreeData*)(realloc(RightNode->RecordDataArray, (node_size / 2) * sizeof(BTreeData)));
        RightNode->NodeLinkArray = (BTreeNode*)(realloc(RightNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BTreeNode)));
        RightNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTree_ReplaceSingleRightNodeRecursive(

        BTreeNode CurrentNode,
        BTreeNode RightNode,
        int record_position,
        int node_size

)
{


    RightNode->array_stored_elements++;


    if (RightNode->array_available_capacity == (node_size / 2) && (RightNode->array_stored_elements - 1) == (node_size / 2))
    {

        RightNode->RecordDataArray = (BTreeData*)(realloc(RightNode->RecordDataArray, node_size * sizeof(BTreeData)));
        RightNode->NodeLinkArray = (BTreeNode*)(realloc(RightNode->NodeLinkArray, (node_size + 1) * sizeof(BTreeNode)));
        RightNode->array_available_capacity = node_size;

    }


    RightNode->RecordDataArray[1] = RightNode->RecordDataArray[0];
    RightNode->RecordDataArray[0] = CurrentNode->RecordDataArray[record_position];


    for (int i = RightNode->array_stored_elements; i > 0; i--)
    {

        RightNode->NodeLinkArray[i] = RightNode->NodeLinkArray[i - 1];

    }


    RightNode->NodeLinkArray[0] = CurrentNode->NodeLinkArray[record_position];


    for (int i = record_position; i < (CurrentNode->array_stored_elements - 1); i++)
    {

        CurrentNode->RecordDataArray[i] = CurrentNode->RecordDataArray[i + 1];

    }


    for (int i = record_position; i < CurrentNode->array_stored_elements; i++)
    {

        CurrentNode->NodeLinkArray[i] = CurrentNode->NodeLinkArray[i + 1];

    }


    CurrentNode->array_stored_elements--;


    if (CurrentNode->array_available_capacity == node_size && (CurrentNode->array_stored_elements + 1) == (node_size / 2))
    {

        CurrentNode->RecordDataArray = (BTreeData*)(realloc(CurrentNode->RecordDataArray, (node_size / 2) * sizeof(BTreeData)));
        CurrentNode->NodeLinkArray = (BTreeNode*)(realloc(CurrentNode->NodeLinkArray, (node_size / 2 + 1) * sizeof(BTreeNode)));
        CurrentNode->array_available_capacity = node_size / 2;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BTreeData BTree_LeftSubTree_MaxRecord(

        BTreeNode CurrentNode

)
{


    while (!CurrentNode->leaf_type)
    {

        CurrentNode = CurrentNode->NodeLinkArray[CurrentNode->array_stored_elements];

    }


    return CurrentNode->RecordDataArray[CurrentNode->array_stored_elements - 1];


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BTreeData InitializeBTreeData_TableA(

        BTreePrimaryKey Record_PrimaryKey,
        int Field_Data

)
{


    BTreeData NewRecordData = (BTreeData)(malloc(sizeof(struct BTreeRecordData)));

    NewRecordData->RecordData = (TableA_RecordData)(malloc(sizeof(struct TableA_RecordData)));
    NewRecordData->table_id = 1;

    ((TableA_RecordData)(NewRecordData->RecordData))->Record_PrimaryKey = Record_PrimaryKey;
    ((TableA_RecordData)(NewRecordData->RecordData))->Field_Data = Field_Data;

    return NewRecordData;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BTreePrimaryKey Get_PrimaryKey_TableA(

        BTreeData Input_RecordData

)
{


    return ((TableA_RecordData)(Input_RecordData->RecordData))->Record_PrimaryKey;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int Get_FieldData_TableA(

        BTreeData Input_RecordData

)
{


    return ((TableA_RecordData)(Input_RecordData->RecordData))->Field_Data;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BTreeSelectRecordData_ASC(

        BTree BTree,
        struct DoubleLinkedBList* SelectionResultList,
        int Field_Data,

        int (*Get_Record_FieldData)(BTreeData)

)
{


    if (BTree.Root == NULL)
    {

        //printf("B-tree is empty\n\n");

        return 1;

    }


    BTreeSelectRecordData_ASC_Tool(BTree.Root, SelectionResultList, Field_Data, Get_Record_FieldData);


    if (SelectionResultList->List_HeadNode == NULL)
    {

        return 1;

    }


    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTreeSelectRecordData_ASC_Tool(

        BTreeNode CurrentNode,
        struct DoubleLinkedBList* SelectionResultList,
        int Field_Data,

        int (*Get_Record_FieldData)(BTreeData)

)
{


    if (CurrentNode->leaf_type)
    {

        for (int i = 0; i < CurrentNode->array_stored_elements; i++)
        {

            if (Get_Record_FieldData(CurrentNode->RecordDataArray[i]) == Field_Data)
            {

                InsertBListNode_Last(SelectionResultList, CurrentNode->RecordDataArray[i]);

            }

        }


        return;

    }


    for (int i = 0; i < CurrentNode->array_stored_elements; i++)
    {

        BTreeSelectRecordData_ASC_Tool( CurrentNode->NodeLinkArray[i],
                                        SelectionResultList,
                                        Field_Data,
                                        Get_Record_FieldData );


        if (Get_Record_FieldData(CurrentNode->RecordDataArray[i]) == Field_Data)
        {

            InsertBListNode_Last(SelectionResultList, CurrentNode->RecordDataArray[i]);

        }

    }


    BTreeSelectRecordData_ASC_Tool( CurrentNode->NodeLinkArray[CurrentNode->array_stored_elements],
                                    SelectionResultList,
                                    Field_Data,
                                    Get_Record_FieldData );


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BTreeSelectRecordData_DESC(

        BTree BTree,
        struct DoubleLinkedBList* SelectionResultList,
        int Field_Data,

        int (*Get_Record_FieldData)(BTreeData)

)
{


    if (BTree.Root == NULL)
    {

        //printf("B-tree is empty\n\n");

        return 1;

    }


    BTreeSelectRecordData_DESC_Tool( BTree.Root,
                                     SelectionResultList,
                                     Field_Data,
                                     Get_Record_FieldData );


    if (SelectionResultList->List_HeadNode == NULL)
    {

        return 1;

    }


    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTreeSelectRecordData_DESC_Tool(

        BTreeNode CurrentNode,
        struct DoubleLinkedBList* SelectionResultList,
        int Field_Data,

        int (*Get_Record_FieldData)(BTreeData)

)
{


    if (CurrentNode->leaf_type)
    {

        for (int i = 0; i < CurrentNode->array_stored_elements; i++)
        {

            if (Get_Record_FieldData(CurrentNode->RecordDataArray[i]) == Field_Data)
            {

                InsertBListNode_First(SelectionResultList, CurrentNode->RecordDataArray[i]);

            }

        }


        return;

    }


    for (int i = 0; i < CurrentNode->array_stored_elements; i++)
    {

        BTreeSelectRecordData_DESC_Tool( CurrentNode->NodeLinkArray[i],
                                         SelectionResultList,
                                         Field_Data,
                                         Get_Record_FieldData );


        if (Get_Record_FieldData(CurrentNode->RecordDataArray[i]) == Field_Data)
        {

            InsertBListNode_First(SelectionResultList, CurrentNode->RecordDataArray[i]);

        }

    }


    BTreeSelectRecordData_DESC_Tool( CurrentNode->NodeLinkArray[CurrentNode->array_stored_elements],
                                     SelectionResultList,
                                     Field_Data,
                                     Get_Record_FieldData );


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BTreeDrop(

        BTree* BTree,

        bool (*Deallocate_Record_Data)(BTreeData)

)
{


    if (BTree->Root == NULL)
    {

        printf("B-tree is empty\n\n");

        return 1;

    }


    BTreeDropNode(BTree->Root, Deallocate_Record_Data);

    InitializeBTree(BTree, BTree->node_size);

    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTreeDropNode(

        BTreeNode CurrentNode,

        bool (*Deallocate_Record_Data)(BTreeData)

)
{


    if (CurrentNode->leaf_type)
    {

        for (int i = 0; i < CurrentNode->array_stored_elements; i++)
        {

            Deallocate_Record_Data(CurrentNode->RecordDataArray[i]);

        }

        free(CurrentNode->RecordDataArray);
        free(CurrentNode);

        return;

    }


    for (int i = 0; i < CurrentNode->array_stored_elements; i++)
    {

        BTreeDropNode(CurrentNode->NodeLinkArray[i], Deallocate_Record_Data);

        Deallocate_Record_Data(CurrentNode->RecordDataArray[i]);

    }


    BTreeDropNode(CurrentNode->NodeLinkArray[CurrentNode->array_stored_elements], Deallocate_Record_Data);


    free(CurrentNode->RecordDataArray);
    free(CurrentNode->NodeLinkArray);
    free(CurrentNode);


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool Deallocate_BTreeRecordData_TableA(

        BTreeData RecordData_ToBe_Deleted

)
{


    if (RecordData_ToBe_Deleted == NULL)
    {

        printf("Record data - not found\n\n");

        return 1;

    }


    free(((TableA_RecordData)(RecordData_ToBe_Deleted->RecordData)));
    free(RecordData_ToBe_Deleted);

    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BTreeComputeNodes(

        BTree BTree,
        int* internal_nodes,
        int* internal_nodes_records,
        int* leaf_nodes,
        int* leaf_nodes_records

)
{


    if (BTree.Root == NULL)
    {

        printf("B-tree is empty\n\n");

        return 1;

    }


    *internal_nodes = 0;
    *internal_nodes_records = 0;
    *leaf_nodes = 0;
    *leaf_nodes_records = 0;

    BTreeComputeNodes_Tool( BTree.Root,
                            internal_nodes,
                            internal_nodes_records,
                            leaf_nodes,
                            leaf_nodes_records) ;

    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTreeComputeNodes_Tool(

        BTreeNode CurrentNode,
        int* internal_nodes,
        int* internal_nodes_records,
        int* leaf_nodes,
        int* leaf_nodes_records

)
{


    if (CurrentNode->leaf_type)
    {

        *leaf_nodes += 1;
        *leaf_nodes_records += CurrentNode->array_stored_elements;

        return;

    }

    *internal_nodes += 1;
    *internal_nodes_records += CurrentNode->array_stored_elements;

    for (int i = 0; i < CurrentNode->array_stored_elements + 1; i++)
    {

        BTreeComputeNodes_Tool( CurrentNode->NodeLinkArray[i],
                                internal_nodes,
                                internal_nodes_records,
                                leaf_nodes,
                                leaf_nodes_records );

    }


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BTreeComputeHeight(

        BTree BTree,
        int* height

)
{


    if (BTree.Root == NULL)
    {

        printf("Unable to compute B-tree height\n\n");

        return 1;

    }


    *height = 0;

    while (!BTree.Root->leaf_type)
    {

        BTree.Root = BTree.Root->NodeLinkArray[0];

        *height += 1;

    }


    *height += 1;


    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BTreeCheckNodeCapacity(

        BTree BTree,
        bool* error_flag

)
{


    if (BTree.Root == NULL)
    {

        printf("B-tree is empty\n\n");

        return 1;

    }


    *error_flag = 0;
    BTreeCheckNodeCapacity_Tool(BTree.Root, BTree.node_size, error_flag);


    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTreeCheckNodeCapacity_Tool(

        BTreeNode CurrentNode,
        int node_size,
        bool* error_flag

)
{


    if (CurrentNode->leaf_type)
    {


        if (CurrentNode->array_stored_elements >= node_size)
        {

            printf("Exception - B-tree leaf node number of records addresses\n");

            *error_flag = 1;

        }


        if (!(CurrentNode->array_available_capacity == node_size || CurrentNode->array_available_capacity == node_size / 2))
        {

            printf("Exception - B-tree leaf node capacity\n");

            *error_flag = 1;

        }


        if (CurrentNode->array_available_capacity == node_size / 2 && CurrentNode->array_stored_elements > CurrentNode->array_available_capacity)
        {

            printf("Exception (1) - B-tree leaf node capacity in compare with records addresses\n");

            *error_flag = 1;

        }


        if (CurrentNode->array_available_capacity == node_size && CurrentNode->array_stored_elements < CurrentNode->array_available_capacity / 2)
        {

            printf("Exception (2) - B-tree leaf node capacity in compare with records addresses\n");

            *error_flag = 1;

        }


        return;

    }


    if (CurrentNode->array_stored_elements >= node_size)
    {

        printf("Exception - B-tree internal node number of records addresses\n");

        *error_flag = 1;

    }


    if (!(CurrentNode->array_available_capacity == node_size || CurrentNode->array_available_capacity == node_size / 2))
    {

        printf("Exception - B-tree internal node capacity\n");

        *error_flag = 1;

    }


    if (CurrentNode->array_available_capacity == node_size / 2 && CurrentNode->array_stored_elements > CurrentNode->array_available_capacity)
    {

        printf("Exception (1) - B-tree internal node capacity in compare with records addresses\n");

        *error_flag = 1;

    }


    if (CurrentNode->array_available_capacity == node_size && CurrentNode->array_stored_elements < CurrentNode->array_available_capacity / 2)
    {

        printf("Exception (2) - B-tree internal node capacity in compare with records addresses\n");

        *error_flag = 1;

    }


    for (int i = 0; i < CurrentNode->array_stored_elements + 1; i++)
    {

        BTreeCheckNodeCapacity_Tool(CurrentNode->NodeLinkArray[i], node_size, error_flag);

    }


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BTreeCheckBalance(

        BTree BTree,
        int BTree_height,
        int* unbalanced_nodes

)
{


    if (BTree.Root == NULL)
    {

        printf("B-tree is empty\n\n");

        return 1;

    }


    int node_height = 1;
    *unbalanced_nodes = 0;
    BTreeCheckBalance_Tool(BTree.Root, node_height, BTree_height, unbalanced_nodes);


    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTreeCheckBalance_Tool(

        BTreeNode CurrentNode,
        int node_height,
        int BTree_height,
        int* unbalanced_nodes

)
{


    if (CurrentNode->leaf_type)
    {

        if (node_height != BTree_height)
        {

            *unbalanced_nodes += 1;

        }

        return;

    }


    for (int i = 0; i < CurrentNode->array_stored_elements + 1; i++)
    {

        BTreeCheckBalance_Tool( CurrentNode->NodeLinkArray[i],
                                node_height + 1,
                                BTree_height,
                                unbalanced_nodes );

    }


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BTreeCheckLeafLinkNodes(

        BTree BTree,
        int* improperly_structured_nodes

)
{


    if (BTree.Root == NULL)
    {

        printf("B-tree is empty\n\n");

        return 1;

    }


    *improperly_structured_nodes = 0;
    BTreeCheckLeafLinkNodes_Tool(BTree.Root, improperly_structured_nodes);


    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BTreeCheckLeafLinkNodes_Tool(

        BTreeNode CurrentNode,
        int* improperly_structured_nodes

)
{


    if (CurrentNode->leaf_type)
    {

        if (CurrentNode->NodeLinkArray != NULL)
        {

            *improperly_structured_nodes += 1;

        }

        return;

    }


    for (int i = 0; i < CurrentNode->array_stored_elements + 1; i++)
    {

        BTreeCheckLeafLinkNodes_Tool(CurrentNode->NodeLinkArray[i], improperly_structured_nodes);

    }


}

