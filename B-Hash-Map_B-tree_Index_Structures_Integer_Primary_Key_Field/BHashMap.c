
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "BHashMap.h"



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void InitializeBHashMap(

        BHashMap* BHashMap,
        int BHashMap_Level_Size,
        int BTreeNodeSize

)
{


    BHashMap->BHashMap_Level_Size = BHashMap_Level_Size;
    BHashMap->BTreeArray = (BTree**)(malloc(BHashMap->BHashMap_Level_Size * sizeof(BTree*)));


    for (int i = 0; i < BHashMap->BHashMap_Level_Size; i++)
    {

        BHashMap->BTreeArray[i] = (BTree*)(malloc(sizeof(BTree)));
        InitializeBTree(BHashMap->BTreeArray[i], BTreeNodeSize);

    }

    BHashMap->size = 0;


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BHashMapInsertData(

        BHashMap* BHashMap,
        BHashMapData RecordData,
        int hash_code,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData),
        bool (*Deallocate_Record_Data)(BTreeData)

)
{


    if (BHashMap->BTreeArray == NULL)
    {

        printf("BHashMap is uninitialized\n\n");

        return 1;

    }


    if (!BTreeInsertData(BHashMap->BTreeArray[hash_code], RecordData, Get_Record_PrimaryKey, Deallocate_Record_Data))
    {

        BHashMap->size += 1;

        return 0;

    }


    return 1;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BHashMapDeleteData(

        BHashMap* BHashMap,
        BTreePrimaryKey PrimaryKey,
        int hash_code,
        BHashMapData* DeletedRecordData,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)

)
{


    if (BHashMap->BTreeArray == NULL)
    {

        //printf("BHashMap is uninitialized\n\n");

        *DeletedRecordData = NULL;

        return 1;

    }


    if (BHashMap->size == 0)
    {

        //printf("BHashMap is empty\n\n");

        *DeletedRecordData = NULL;

        return 1;

    }


    if (BHashMap->BTreeArray[hash_code]->Root == NULL)
    {

        //printf("BHashMap B-tree is empty\n\n");

        *DeletedRecordData = NULL;

        return 1;

    }


    if (!BTreeDeleteData(BHashMap->BTreeArray[hash_code], PrimaryKey, DeletedRecordData, Get_Record_PrimaryKey))
    {

        BHashMap->size -= 1;

        return 0;

    }


    return 1;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BHashMapDisplay(

        BHashMap BHashMap,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)

)
{


    if (BHashMap.BTreeArray == NULL)
    {

        //printf("BHashMap is uninitialized\n\n");

        return;

    }


    if (BHashMap.size == 0)
    {

        //printf("BHashMap is empty\n\n");

        return;

    }


    printf("------------------------- Hash Map -------------------------\n\n");


    for (int i = 0; i < BHashMap.BHashMap_Level_Size; i++)
    {

        BTreeDisplayInOrder(*(BHashMap.BTreeArray[i]), Get_Record_PrimaryKey);

        printf("\n");

    }


    printf("\n------------------------------------------------------------\n\n");


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BHashMapSelectData(

        BHashMap BHashMap,
        struct DoubleLinkedBList* SelectionResultList,
        int Field_Data,
        bool selection_method_flag,

        BTreePrimaryKey (*Get_Record_FieldData)(BTreeData)

)
{


    if (BHashMap.BTreeArray == NULL)
    {

        //printf("BHashMap is uninitialized\n\n");

        return 1;

    }


    if (BHashMap.size == 0)
    {

        //printf("BHashMap is empty\n\n");

        return 1;

    }


    for (int i = 0; i < BHashMap.BHashMap_Level_Size; i++)
    {

        if (BHashMap.BTreeArray[i]->Root != NULL)
        {

            if (!selection_method_flag)
            {

                BTreeSelectRecordData_ASC(*(BHashMap.BTreeArray[i]), SelectionResultList, Field_Data, Get_Record_FieldData);

            }else{

                BTreeSelectRecordData_DESC(*(BHashMap.BTreeArray[i]), SelectionResultList, Field_Data, Get_Record_FieldData);

            }

        }

    }


    if (SelectionResultList->List_HeadNode != NULL)
    {

        return 0;

    }


    return 1;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BHashMapData BHashMapSelectData_ByPrimaryKey(

        BHashMap BHashMap,
        BTreePrimaryKey PrimaryKey,
        int hash_code,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BTreeData)

)
{


    if (BHashMap.BTreeArray == NULL)
    {

        //printf("BHashMap is uninitialized\n\n");

        return NULL;

    }


    if (BHashMap.size == 0)
    {

        //printf("BHashMap is empty\n\n");

        return NULL;

    }


    if (BHashMap.BTreeArray[hash_code]->Root == NULL)
    {

        //printf("BHashMap B-tree is empty - BTreeRecordData not found\n\n");

        return NULL;

    }


    return BTreeFastSearchData_ByPrimaryKey(*(BHashMap.BTreeArray[hash_code]), PrimaryKey, Get_Record_PrimaryKey);


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BHashMapDrop(

        BHashMap* BHashMap,

        bool (*Deallocate_Record_Data)(BTreeData)

)
{


    if (BHashMap->BTreeArray == NULL)
    {

        //printf("BHashMap is uninitialized\n\n");

        return 1;

    }


    for (int i = 0; i < BHashMap->BHashMap_Level_Size; i++)
    {

        if (BHashMap->BTreeArray[i]->Root != NULL)
        {

            BTreeDrop(BHashMap->BTreeArray[i], Deallocate_Record_Data);

        }


        free(BHashMap->BTreeArray[i]);

    }


    free(BHashMap->BTreeArray);


    BHashMap->BTreeArray = NULL;
    BHashMap->size = 0;


    return 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool BHashMapClean(

        BHashMap* BHashMap,

        bool (*Deallocate_Record_Data)(BTreeData)

)
{


    if (BHashMap->BTreeArray == NULL)
    {

        //printf("BHashMap is uninitialized\n\n");

        return 1;

    }


    if (BHashMap->size == 0)
    {

        //printf("BHashMap is empty\n\n");

        return 1;

    }


    for (int i = 0; i < BHashMap->BHashMap_Level_Size; i++)
    {

        if (BHashMap->BTreeArray[i]->Root != NULL)
        {

            BTreeDrop(BHashMap->BTreeArray[i], Deallocate_Record_Data);

        }

    }


    BHashMap->size = 0;


    return 0;


}

