
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "BTree.h"

#define MAXIMUM_INPUT_DATA_SIZE 5000
#define MINIMUM_INPUT_DATA_SIZE 5000
#define INPUT_DATA_SIZE_INCREMENT_FACTOR 1

#define MAXIMUM_NODE_SIZE 101
#define MINIMUM_NODE_SIZE 3
#define NODE_SIZE_INCREMENT_FACTOR 1

#define PROCESS_REPETITIONS_PER_NODE_SIZE 3

#define INPUT_DATA_RECONSTRUCTION 10



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



int main(void)
{


    BTree Btree_Index;
    DoubleLinkedBList Selected_Data_List;

    BListNode* CurrentNode = NULL;
    BTreeData Retrieved_Data = NULL;


    int* Input_DataSet = (int*)(malloc(MAXIMUM_INPUT_DATA_SIZE * sizeof(int)));
    int* Copy_Input_DataSet = (int*)(malloc(MAXIMUM_INPUT_DATA_SIZE * sizeof(int)));
    int swap_position;
    int copy_data;
    int low_index_limit = 0;
    int upp_index_limit = MAXIMUM_INPUT_DATA_SIZE - 1;
    int i = 0, j = 0;


    int input_data_size = 0;
    int node_size = 0;
    int process_repetitions = 0;
    int index = 0;
    int btree_node_capacity = 0;


    int Internal_nodes_number = 0;
    int Leaf_nodes_number = 0;
    int Internal_nodes_elements = 0;
    int Leaf_nodes_elements = 0;
    int Btree_Index_height = 0;
    int unbalanced_nodes = 0;
    int improperly_structured_nodes = 0;
    bool error_flag = 0;
    bool empty_flag = 0;



    printf("\n\n\nIntegration test - B-tree index structure\n\n");
    printf("Input data size range            | min size: (%d records) - max size: (%d records)\n", MINIMUM_INPUT_DATA_SIZE, MAXIMUM_INPUT_DATA_SIZE);
    printf("Input data size increment factor | (%d)\n", INPUT_DATA_SIZE_INCREMENT_FACTOR);
    printf("Node size - capacity range       | min size: (%d records) - max size: (%d records)\n", MINIMUM_NODE_SIZE, MAXIMUM_NODE_SIZE);
    printf("Node size increment factor       | (%d)\n", NODE_SIZE_INCREMENT_FACTOR);
    printf("Test processes per node size     | (%d)\n\n\n\n", PROCESS_REPETITIONS_PER_NODE_SIZE);



    for (input_data_size = MINIMUM_INPUT_DATA_SIZE; input_data_size <= MAXIMUM_INPUT_DATA_SIZE; input_data_size += INPUT_DATA_SIZE_INCREMENT_FACTOR)
    {

        low_index_limit = 0;
        upp_index_limit = input_data_size - 1;


        for (i = 0; i < input_data_size; i++)
        {

            Input_DataSet[i] = i;
            Copy_Input_DataSet[i] = i;

        }


        printf("\nInput data size : (%d records)\n", input_data_size);
        printf("------------------------------------------------------------------------------------------------------------\n\n");


        for (node_size = MINIMUM_NODE_SIZE; node_size <= MAXIMUM_NODE_SIZE; node_size += NODE_SIZE_INCREMENT_FACTOR)
        {

            btree_node_capacity = node_size;


            if (btree_node_capacity < MINIMUM_NODE_SIZE)
            {

                printf("Test failed\n\n");
                printf("Exception - Invalid B-tree node capacity\n\n");

                return 1;

            }


            InitializeBTree(&Btree_Index, btree_node_capacity);
            Initialize_BList(&Selected_Data_List);


            printf("Node size - capacity: (%d records)\n", btree_node_capacity);


            for (process_repetitions = 0; process_repetitions < PROCESS_REPETITIONS_PER_NODE_SIZE; process_repetitions++)
            {

                srand(time(0));


                for (j = 0; j < INPUT_DATA_RECONSTRUCTION; j++)
                {

                    for (i = 0; i < input_data_size; i++)
                    {

                        swap_position = (rand() % (upp_index_limit - low_index_limit + 1)) + low_index_limit;

                        copy_data = Input_DataSet[swap_position];
                        Input_DataSet[swap_position] = Input_DataSet[i];
                        Input_DataSet[i] = copy_data;

                    }

                }




                ////////////////////////////////////////////////// Insertion ///////////////////////////////////////////////////




                for (i = 0; i < input_data_size; i++)
                {

                    Retrieved_Data = InitializeBTreeData_TableA(Input_DataSet[i], 0);


                    if (Retrieved_Data->table_id != 1)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (1)\n\n");

                        return 1;

                    }


                    if (BTreeInsertData(&Btree_Index, Retrieved_Data, &Get_PrimaryKey_TableA, &Deallocate_BTreeRecordData_TableA))
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (2)\n\n");

                        return 1;

                    }


                    if (Retrieved_Data->table_id != 1)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (3)\n\n");

                        return 1;

                    }


                    if (!BTreeInsertData(&Btree_Index, Retrieved_Data, &Get_PrimaryKey_TableA, &Deallocate_BTreeRecordData_TableA))
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (4)\n\n");

                        return 1;

                    }


                    Retrieved_Data = NULL;
                    BTreeComputeHeight(Btree_Index, &Btree_Index_height);


                    if (Btree_Index.height != Btree_Index_height)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (5)\n\n");

                        return 1;

                    }


                    if (Btree_Index.size != i + 1)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (6)\n\n");

                        return 1;

                    }


                    BTreeCheckNodeCapacity(Btree_Index, &error_flag);


                    if (error_flag)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (7)\n\n");

                        return 1;

                    }


                    BTreeCheckLeafLinkNodes(Btree_Index, &improperly_structured_nodes);


                    if (improperly_structured_nodes > 0)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (8)\n\n");

                        return 1;

                    }


                    BTreeCheckBalance(Btree_Index, Btree_Index.height, &unbalanced_nodes);


                    if (unbalanced_nodes > 0)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (9)\n\n");

                        return 1;

                    }




                    ////////////////////////////////////////////////// Selection By Primary Key ///////////////////////////////////////////////////




                    for (j = 0; j < i + 1; j++)
                    {

                        Retrieved_Data = NULL;
                        Retrieved_Data = BTreeFastSearchData_ByPrimaryKey(Btree_Index, Input_DataSet[j], &Get_PrimaryKey_TableA);


                        if (Retrieved_Data == NULL || Get_PrimaryKey_TableA(Retrieved_Data) != Input_DataSet[j])
                        {

                            printf("Test failed\n\n");
                            printf("Insertion process failed\n");
                            printf("Exception (10)\n\n");

                            return 1;

                        }

                    }


                    BTreeComputeNodes(Btree_Index, &Internal_nodes_number, &Internal_nodes_elements, &Leaf_nodes_number, &Leaf_nodes_elements);


                    if (Internal_nodes_elements + Leaf_nodes_elements != i + 1)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (11)\n\n");

                        return 1;

                    }


                    if (Internal_nodes_number >= Leaf_nodes_number)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (12)\n\n");

                        return 1;

                    }


                    if (Internal_nodes_elements >= Leaf_nodes_elements)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (13)\n\n");

                        return 1;

                    }


                    if (Internal_nodes_elements + 1 != Leaf_nodes_number)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (14)\n\n");

                        return 1;

                    }

                }




                ////////////////////////////////////////////////// Full Selection ///////////////////////////////////////////////////




                if (!BTreeSelectRecordData_ASC(Btree_Index, &Selected_Data_List, 0, &Get_FieldData_TableA))
                {

                    if (Selected_Data_List.List_Size != input_data_size)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (15)\n\n");

                        return 1;

                    }


                    CurrentNode = Selected_Data_List.List_HeadNode;
                    index = 0;


                    while (CurrentNode != NULL)
                    {

                        if (Get_PrimaryKey_TableA(CurrentNode->RecordData) != Copy_Input_DataSet[index])
                        {

                            printf("Test failed\n\n");
                            printf("Insertion process failed\n");
                            printf("Exception (16)\n\n");

                            return 1;

                        }


                        CurrentNode = CurrentNode->List_NextNode;
                        index++;

                    }


                }
                else {

                    printf("Test failed\n\n");
                    printf("Insertion process failed\n");
                    printf("Exception (17)\n\n");

                    return 1;

                }


                DeleteBList(&Selected_Data_List);




                ////////////////////////////////////////////////// Deletion ////////////////////////////////////////////////////




                srand(time(0));


                for (j = 0; j < INPUT_DATA_RECONSTRUCTION; j++)
                {

                    for (i = 0; i < input_data_size; i++)
                    {

                        swap_position = (rand() % (upp_index_limit - low_index_limit + 1)) + low_index_limit;

                        copy_data = Input_DataSet[swap_position];
                        Input_DataSet[swap_position] = Input_DataSet[i];
                        Input_DataSet[i] = copy_data;

                    }

                }


                for (i = 0; i < input_data_size - 1; i++)
                {

                    BTreeDeleteData(&Btree_Index, Input_DataSet[i], &Retrieved_Data, &Get_PrimaryKey_TableA);
                    Deallocate_BTreeRecordData_TableA(Retrieved_Data);


                    if (!BTreeDeleteData(&Btree_Index, Input_DataSet[i], &Retrieved_Data, &Get_PrimaryKey_TableA))
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (1)\n\n");

                        return 1;

                    }


                    if (Retrieved_Data != NULL)
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (2)\n\n");

                        return 1;

                    }


                    BTreeComputeHeight(Btree_Index, &Btree_Index_height);


                    if (Btree_Index.height != Btree_Index_height)
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (3)\n\n");

                        return 1;

                    }


                    if (Btree_Index.size != input_data_size - (i + 1))
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (4)\n\n");

                        return 1;

                    }


                    BTreeCheckLeafLinkNodes(Btree_Index, &improperly_structured_nodes);


                    if (improperly_structured_nodes > 0)
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (5)\n\n");

                        return 1;

                    }


                    BTreeCheckBalance(Btree_Index, Btree_Index.height, &unbalanced_nodes);


                    if (unbalanced_nodes > 0)
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (6)\n\n");

                        return 1;

                    }


                    BTreeCheckNodeCapacity(Btree_Index, &error_flag);


                    if (error_flag)
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (7)\n\n");

                        return 1;

                    }


                    BTreeComputeNodes(Btree_Index, &Internal_nodes_number, &Internal_nodes_elements, &Leaf_nodes_number, &Leaf_nodes_elements);


                    if (Internal_nodes_elements + Leaf_nodes_elements != input_data_size - (i + 1))
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (8)\n\n");

                        return 1;

                    }


                    if (Internal_nodes_number >= Leaf_nodes_number)
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (9)\n\n");

                        return 1;

                    }


                    if (Internal_nodes_elements >= Leaf_nodes_elements)
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (10)\n\n");

                        return 1;

                    }


                    if (Internal_nodes_elements + 1 != Leaf_nodes_number)
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (11)\n\n");

                        return 1;

                    }



                    for (j = i + 1; j < input_data_size; j++)
                    {

                        Retrieved_Data = NULL;
                        Retrieved_Data = BTreeFastSearchData_ByPrimaryKey(Btree_Index, Input_DataSet[j], &Get_PrimaryKey_TableA);


                        if (Retrieved_Data == NULL || Get_PrimaryKey_TableA(Retrieved_Data) != Input_DataSet[j])
                        {

                            printf("Test failed\n\n");
                            printf("Deletion process failed\n");
                            printf("Exception (12)\n\n");

                            return 1;

                        }

                    }


                    empty_flag = BTreeSelectRecordData_ASC(Btree_Index, &Selected_Data_List, 0, &Get_FieldData_TableA);


                    if (Selected_Data_List.List_Size != input_data_size - (i + 1))
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (13)\n\n");

                        return 1;

                    }


                    Retrieved_Data = NULL;
                    Retrieved_Data = BTreeFastSearchData_ByPrimaryKey( Btree_Index,
                                                                       Get_PrimaryKey_TableA(Selected_Data_List.List_HeadNode->RecordData),
                                                                       &Get_PrimaryKey_TableA );


                    if ( Retrieved_Data == NULL
                         || Get_PrimaryKey_TableA(Retrieved_Data) != Get_PrimaryKey_TableA(Selected_Data_List.List_HeadNode->RecordData) )
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (14)\n\n");

                        return 1;

                    }


                    if (!empty_flag)
                    {

                        CurrentNode = Selected_Data_List.List_HeadNode->List_NextNode;


                        while (CurrentNode != NULL)
                        {

                            Retrieved_Data = NULL;
                            Retrieved_Data = BTreeFastSearchData_ByPrimaryKey(Btree_Index, Get_PrimaryKey_TableA(CurrentNode->RecordData), &Get_PrimaryKey_TableA);


                            if (Retrieved_Data == NULL || Get_PrimaryKey_TableA(Retrieved_Data) != Get_PrimaryKey_TableA(CurrentNode->RecordData))
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (15)\n\n");

                                return 1;

                            }


                            if (Get_PrimaryKey_TableA(CurrentNode->RecordData) <= Get_PrimaryKey_TableA(CurrentNode->List_PrevNode->RecordData))
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (16)\n\n");

                                return 1;

                            }


                            CurrentNode = CurrentNode->List_NextNode;

                        }


                    }
                    else {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (17)\n\n");

                        return 1;

                    }


                    DeleteBList(&Selected_Data_List);

                }


                BTreeDeleteData(&Btree_Index, Input_DataSet[input_data_size - 1], &Retrieved_Data, &Get_PrimaryKey_TableA);
                Deallocate_BTreeRecordData_TableA(Retrieved_Data);


                if (!BTreeDeleteData(&Btree_Index, Input_DataSet[input_data_size - 1], &Retrieved_Data, &Get_PrimaryKey_TableA))
                {

                    printf("Test failed\n\n");
                    printf("Deletion process failed\n");
                    printf("Exception (18)\n\n");

                    return 1;

                }


                if (Retrieved_Data != NULL)
                {

                    printf("Test failed\n\n");
                    printf("Deletion process failed\n");
                    printf("Exception (19)\n\n");

                    return 1;

                }


                if (Btree_Index.height != 0)
                {

                    printf("Test failed\n\n");
                    printf("Deletion process failed\n");
                    printf("Exception (20)\n\n");

                    return 1;

                }


                if (Btree_Index.size != 0)
                {

                    printf("Test failed\n\n");
                    printf("Deletion process failed\n");
                    printf("Exception (21)\n\n");

                    return 1;

                }


                if (Btree_Index.Root != NULL)
                {

                    printf("Test failed\n\n");
                    printf("Deletion process failed\n");
                    printf("Deletion Exception (22)\n\n");

                    return 1;

                }

            }


            printf("(Test passed)\n\n");

        }

    }


    free(Input_DataSet);
    free(Copy_Input_DataSet);


    printf("\n");
    printf("(Integration test passed)\n\n");


    return 0;


}

