
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "BTree.h"



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void Initialize_BList(

        struct DoubleLinkedBList* LinkedList

)
{


    LinkedList->List_HeadNode = NULL;
    LinkedList->List_TailNode = NULL;
    LinkedList->List_Size = 0;

    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct BListNode* Create_BListNode(

        BListData RecordData

)
{


    struct BListNode* NewListNode = (BListNode*)(malloc(sizeof(struct BListNode)));

    NewListNode->RecordData = RecordData;
    NewListNode->List_NextNode = NULL;
    NewListNode->List_PrevNode = NULL;

    return NewListNode;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void InsertBListNode_First(

        struct DoubleLinkedBList* LinkedList,
        BListData RecordData

)
{


    struct BListNode* TempNode = Create_BListNode(RecordData);


    if (LinkedList->List_HeadNode == NULL && LinkedList->List_TailNode == NULL)
    {

        LinkedList->List_HeadNode = TempNode;
        LinkedList->List_TailNode = TempNode;

        LinkedList->List_Size++;

        return;

    }

    TempNode->List_NextNode = LinkedList->List_HeadNode;
    LinkedList->List_HeadNode->List_PrevNode = TempNode;
    LinkedList->List_HeadNode = TempNode;

    LinkedList->List_Size++;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void InsertBListNode_Last(

        struct DoubleLinkedBList* LinkedList,
        BListData RecordData

)
{


    struct BListNode* TempNode = Create_BListNode(RecordData);


    if (LinkedList->List_HeadNode == NULL && LinkedList->List_TailNode == NULL)
    {

        LinkedList->List_HeadNode = TempNode;
        LinkedList->List_TailNode = TempNode;

        LinkedList->List_Size++;

        return;

    }

    TempNode->List_PrevNode = LinkedList->List_TailNode;
    LinkedList->List_TailNode->List_NextNode = TempNode;
    LinkedList->List_TailNode = TempNode;

    LinkedList->List_Size++;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void InsertBListNode_Index(

        struct DoubleLinkedBList* LinkedList,
        BListData RecordData,
        int insertion_position_index

)
{


    int middle_position;


    if ((int)(LinkedList->List_Size % 2) == 0)
    {

        middle_position = (LinkedList->List_Size) / 2 - 1;

    }
    else {

        middle_position = (LinkedList->List_Size) / 2;

    }


    if (insertion_position_index == 0)
    {

        InsertBListNode_First(LinkedList, RecordData);

        return;

    }


    if (insertion_position_index == LinkedList->List_Size)
    {

        InsertBListNode_Last(LinkedList, RecordData);

        return;

    }


    if (insertion_position_index > 0 && insertion_position_index <= middle_position)
    {

        struct BListNode* InputNode = Create_BListNode(RecordData);
        struct BListNode* TempNode = LinkedList->List_HeadNode;
        int i = 0;


        for (i = 0; i < insertion_position_index; i++)
        {

            TempNode = TempNode->List_NextNode;

        }


        TempNode->List_PrevNode->List_NextNode = InputNode;
        InputNode->List_PrevNode = TempNode->List_PrevNode;
        InputNode->List_NextNode = TempNode;
        TempNode->List_PrevNode = InputNode;

        LinkedList->List_Size++;

        return;

    }

    if (insertion_position_index > middle_position && insertion_position_index < LinkedList->List_Size)
    {

        struct BListNode* InputNode = Create_BListNode(RecordData);
        struct BListNode* TempNode = LinkedList->List_TailNode;
        int i = 0, Size = LinkedList->List_Size - 1;


        for (i = 0; i < (Size - insertion_position_index); i++)
        {

            TempNode = TempNode->List_PrevNode;

        }


        TempNode->List_PrevNode->List_NextNode = InputNode;
        InputNode->List_PrevNode = TempNode->List_PrevNode;
        InputNode->List_NextNode = TempNode;
        TempNode->List_PrevNode = InputNode;

        LinkedList->List_Size++;

        return;

    }


    return;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BListData DeleteBListNode_First(

        struct DoubleLinkedBList* LinkedList

)
{


    if (LinkedList->List_HeadNode == NULL && LinkedList->List_TailNode == NULL)
    {

        printf("List is empty\n");

        return NULL;

    }


    BListData CopyData;


    if (LinkedList->List_HeadNode == LinkedList->List_TailNode)
    {

        CopyData = LinkedList->List_HeadNode->RecordData;
        free(LinkedList->List_HeadNode);

        Initialize_BList(LinkedList);

        return CopyData;

    }


    CopyData = LinkedList->List_HeadNode->RecordData;

    LinkedList->List_HeadNode = LinkedList->List_HeadNode->List_NextNode;
    LinkedList->List_HeadNode->List_PrevNode->List_NextNode = NULL;
    free(LinkedList->List_HeadNode->List_PrevNode);
    LinkedList->List_HeadNode->List_PrevNode = NULL;

    LinkedList->List_Size--;

    return CopyData;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BListData DeleteBListNode_Last(

        struct DoubleLinkedBList* LinkedList

)
{


    if (LinkedList->List_HeadNode == NULL && LinkedList->List_TailNode == NULL)
    {

        printf("List is empty\n");

        return NULL;

    }


    BListData CopyData;


    if (LinkedList->List_HeadNode == LinkedList->List_TailNode)
    {

        CopyData = LinkedList->List_TailNode->RecordData;
        free(LinkedList->List_TailNode);

        Initialize_BList(LinkedList);

        return CopyData;

    }


    CopyData = LinkedList->List_TailNode->RecordData;

    LinkedList->List_TailNode = LinkedList->List_TailNode->List_PrevNode;
    LinkedList->List_TailNode->List_NextNode->List_PrevNode = NULL;
    free(LinkedList->List_TailNode->List_NextNode);
    LinkedList->List_TailNode->List_NextNode = NULL;

    LinkedList->List_Size--;

    return CopyData;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BListData DeleteBListNode_Index(

        struct DoubleLinkedBList* LinkedList,
        int deletion_position_index

)
{


    int middle_position;


    if ((int)(LinkedList->List_Size % 2) == 0)
    {

        middle_position = (LinkedList->List_Size) / 2 - 1;

    }
    else {

        middle_position = (LinkedList->List_Size) / 2;

    }


    if (deletion_position_index == 0)
    {

        return DeleteBListNode_First(LinkedList);

    }


    if (deletion_position_index == LinkedList->List_Size - 1)
    {

        return DeleteBListNode_Last(LinkedList);

    }


    if (deletion_position_index > 0 && deletion_position_index <= middle_position)
    {

        struct BListNode* TempNode = LinkedList->List_HeadNode;
        int i = 0;
        BListData CopyData;


        for (i = 0; i < deletion_position_index; i++)
        {

            TempNode = TempNode->List_NextNode;

        }


        CopyData = TempNode->RecordData;

        TempNode->List_PrevNode->List_NextNode = TempNode->List_NextNode;
        TempNode->List_NextNode->List_PrevNode = TempNode->List_PrevNode;
        TempNode->List_NextNode = NULL;
        TempNode->List_PrevNode = NULL;

        free(TempNode);

        LinkedList->List_Size--;

        return CopyData;

    }


    if (deletion_position_index > middle_position && deletion_position_index < LinkedList->List_Size)
    {

        struct BListNode* TempNode = LinkedList->List_TailNode;
        int i = 0, Size = LinkedList->List_Size - 1;
        BListData CopyData;


        for (i = 0; i < (Size - deletion_position_index); i++)
        {

            TempNode = TempNode->List_PrevNode;

        }


        CopyData = TempNode->RecordData;

        TempNode->List_PrevNode->List_NextNode = TempNode->List_NextNode;
        TempNode->List_NextNode->List_PrevNode = TempNode->List_PrevNode;
        TempNode->List_NextNode = NULL;
        TempNode->List_PrevNode = NULL;

        LinkedList->List_Size--;

        return CopyData;

    }


    return NULL;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void DeleteBList(

        struct DoubleLinkedBList* LinkedList

)
{


    if (LinkedList->List_HeadNode == NULL && LinkedList->List_TailNode == NULL)
    {

        printf("List is empty\n");

        return;

    }


    BListNode* TempNode = LinkedList->List_HeadNode;
    BListNode* DeletedNode;


    while (TempNode != NULL)
    {

        DeletedNode = TempNode;
        TempNode = TempNode->List_NextNode;

        free(DeletedNode);

    }


    Initialize_BList(LinkedList);


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void DropBList(

        struct DoubleLinkedBList* LinkedList,

        bool (*Deallocate_Record_Data)(BListData)

)
{


    if (LinkedList->List_HeadNode == NULL && LinkedList->List_TailNode == NULL)
    {

        printf("List is empty\n");

        return;

    }


    BListNode* TempNode = LinkedList->List_HeadNode;
    BListNode* DeletedNode;


    while (TempNode != NULL)
    {

        DeletedNode = TempNode;
        TempNode = TempNode->List_NextNode;

        Deallocate_Record_Data(DeletedNode->RecordData);

        free(DeletedNode);

    }


    Initialize_BList(LinkedList);


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void DisplayBList_Forward(

        struct DoubleLinkedBList LinkedList,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BListData)

)
{


    if (LinkedList.List_HeadNode == NULL && LinkedList.List_TailNode == NULL)
    {

        printf("List is empty\n");

        return;

    }


    printf("List : { ");


    while (LinkedList.List_HeadNode != NULL)
    {

        printf("%s ", Get_Record_PrimaryKey(LinkedList.List_HeadNode->RecordData));
        LinkedList.List_HeadNode = LinkedList.List_HeadNode->List_NextNode;

    }


    printf("}\n");


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void DisplayBList_Backward(

        struct DoubleLinkedBList LinkedList,

        BTreePrimaryKey (*Get_Record_PrimaryKey)(BListData)

)
{


    if (LinkedList.List_HeadNode == NULL && LinkedList.List_TailNode == NULL)
    {

        printf("List is empty\n");

        return;

    }


    printf("List : { ");


    while (LinkedList.List_TailNode != NULL)
    {

        printf("%s ", Get_Record_PrimaryKey(LinkedList.List_TailNode->RecordData));
        LinkedList.List_TailNode = LinkedList.List_TailNode->List_PrevNode;

    }


    printf("}\n");


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void InsertBListPart_First(

        struct DoubleLinkedBList* LinkedList,
        struct DoubleLinkedBList InputLinkedListPart

)
{


    if (InputLinkedListPart.List_HeadNode == NULL && InputLinkedListPart.List_TailNode == NULL)
    {

        printf("Input List is empty\n");

        return;

    }


    if (LinkedList->List_HeadNode == NULL && LinkedList->List_TailNode == NULL)
    {

        LinkedList->List_HeadNode = InputLinkedListPart.List_HeadNode;
        LinkedList->List_TailNode = InputLinkedListPart.List_TailNode;

        LinkedList->List_Size = InputLinkedListPart.List_Size;

        return;

    }


    InputLinkedListPart.List_TailNode->List_NextNode = LinkedList->List_HeadNode;
    LinkedList->List_HeadNode->List_PrevNode = InputLinkedListPart.List_TailNode;
    LinkedList->List_HeadNode = InputLinkedListPart.List_HeadNode;

    LinkedList->List_Size += InputLinkedListPart.List_Size;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void InsertBListPart_Last(

        struct DoubleLinkedBList* LinkedList,
        struct DoubleLinkedBList InputLinkedListPart

)
{


    if (InputLinkedListPart.List_HeadNode == NULL && InputLinkedListPart.List_TailNode == NULL)
    {

        printf("Input List is empty\n");

        return;

    }


    if (LinkedList->List_HeadNode == NULL && LinkedList->List_TailNode == NULL)
    {

        LinkedList->List_HeadNode = InputLinkedListPart.List_HeadNode;
        LinkedList->List_TailNode = InputLinkedListPart.List_TailNode;

        LinkedList->List_Size = InputLinkedListPart.List_Size;

        return;

    }


    InputLinkedListPart.List_HeadNode->List_PrevNode = LinkedList->List_TailNode;
    LinkedList->List_TailNode->List_NextNode = InputLinkedListPart.List_HeadNode;
    LinkedList->List_TailNode = InputLinkedListPart.List_TailNode;

    LinkedList->List_Size += InputLinkedListPart.List_Size;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void InsertBListPart_Index(

        struct DoubleLinkedBList* LinkedList,
        struct DoubleLinkedBList InputLinkedListPart,
        int insertion_position_index

)
{


    int middle_position;


    if ((int)(LinkedList->List_Size % 2) == 0)
    {

        middle_position = (LinkedList->List_Size) / 2 - 1;

    }
    else {

        middle_position = (LinkedList->List_Size) / 2;

    }


    if (insertion_position_index == 0)
    {

        InsertBListPart_First(LinkedList, InputLinkedListPart);

        return;

    }


    if (insertion_position_index == LinkedList->List_Size)
    {

        InsertBListPart_Last(LinkedList, InputLinkedListPart);

        return;

    }


    if (insertion_position_index > 0 && insertion_position_index <= middle_position)
    {

        struct BListNode* TempNode = LinkedList->List_HeadNode;
        int i = 0;


        for (i = 0; i < insertion_position_index; i++)
        {

            TempNode = TempNode->List_NextNode;

        }


        TempNode->List_PrevNode->List_NextNode = InputLinkedListPart.List_HeadNode;
        InputLinkedListPart.List_HeadNode->List_PrevNode = TempNode->List_PrevNode;
        InputLinkedListPart.List_TailNode->List_NextNode = TempNode;
        TempNode->List_PrevNode = InputLinkedListPart.List_TailNode;

        LinkedList->List_Size += InputLinkedListPart.List_Size;

        return;

    }


    if (insertion_position_index > middle_position && insertion_position_index < LinkedList->List_Size)
    {

        struct BListNode* TempNode = LinkedList->List_TailNode;
        int i = 0, Size = LinkedList->List_Size - 1;


        for (i = 0; i < (Size - insertion_position_index); i++)
        {

            TempNode = TempNode->List_PrevNode;

        }


        TempNode->List_PrevNode->List_NextNode = InputLinkedListPart.List_HeadNode;
        InputLinkedListPart.List_HeadNode->List_PrevNode = TempNode->List_PrevNode;
        InputLinkedListPart.List_TailNode->List_NextNode = TempNode;
        TempNode->List_PrevNode = InputLinkedListPart.List_TailNode;

        LinkedList->List_Size += InputLinkedListPart.List_Size;

        return;

    }


    return;


}

