
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "BHashMap.h"

#define MAXIMUM_INPUT_DATA_SIZE 5000
#define MINIMUM_INPUT_DATA_SIZE 5000
#define INPUT_DATA_SIZE_INCREMENT_FACTOR 1
#define B_HASH_MAP_LEVEL_SIZE 5

#define MAXIMUM_NODE_SIZE 101
#define MINIMUM_NODE_SIZE 3
#define NODE_SIZE_INCREMENT_FACTOR 1

#define PROCESS_REPETITIONS_PER_NODE_SIZE 3

#define INPUT_DATA_RECONSTRUCTION 10



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



int main(void)
{


    BHashMap BHashMap_Index;
    DoubleLinkedBList Selected_Data_List;
    BTreeData Retrieved_Data = NULL;
    BListNode* TempNode = NULL;


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


    int hybrid_index_total_size = 0;
    int btree_height = 0;
    int improperly_structured_nodes;
    int unbalanced_nodes;
    int Internal_nodes_number;
    int Internal_nodes_elements;
    int Leaf_nodes_number;
    int Leaf_nodes_elements;
    bool empty_flag;
    bool error_flag;
    bool execution_status;



    printf("\n\n\nIntegration test - BHashMap index structure\n\n");

    printf("Hash Map B-tree Nodes            | (%d)\n", B_HASH_MAP_LEVEL_SIZE);
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


            printf("Node size - capacity: (%d records)\n", btree_node_capacity);

            InitializeBHashMap(&BHashMap_Index, B_HASH_MAP_LEVEL_SIZE, btree_node_capacity);


            for (process_repetitions = 0; process_repetitions < PROCESS_REPETITIONS_PER_NODE_SIZE; process_repetitions++)
            {

                Initialize_BList(&Selected_Data_List);


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

                    execution_status = BHashMapInsertData( &BHashMap_Index,
                                                           Retrieved_Data,
                                                           Input_DataSet[i] % B_HASH_MAP_LEVEL_SIZE,
                                                           &Get_PrimaryKey_TableA,
                                                           &Deallocate_BTreeRecordData_TableA );


                    if (execution_status)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (1)\n\n");

                        return 1;

                    }


                    execution_status = BHashMapInsertData( &BHashMap_Index,
                                                           Retrieved_Data,
                                                           Input_DataSet[i] % B_HASH_MAP_LEVEL_SIZE,
                                                           &Get_PrimaryKey_TableA,
                                                           &Deallocate_BTreeRecordData_TableA );


                    if (!execution_status)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (2)\n\n");

                        return 1;

                    }


                    Retrieved_Data = NULL;


                    if (BHashMap_Index.size != i + 1)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (3)\n\n");

                        return 1;


                    }


                    hybrid_index_total_size = 0;


                    for (int j = 0; j < B_HASH_MAP_LEVEL_SIZE; j++)
                    {

                        if (BHashMap_Index.BTreeArray[j]->Root != NULL)
                        {

                            hybrid_index_total_size += BHashMap_Index.BTreeArray[j]->size;
                            BTreeComputeHeight(*(BHashMap_Index.BTreeArray[j]), &btree_height);


                            if (BHashMap_Index.BTreeArray[j]->height != btree_height)
                            {

                                printf("Test failed\n\n");
                                printf("Insertion process failed\n");
                                printf("Exception (4)\n\n");

                            }

                        }

                    }


                    if (hybrid_index_total_size != i + 1)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (5)\n\n");

                        return 1;

                    }


                    hybrid_index_total_size = 0;


                    for (int j = 0; j < B_HASH_MAP_LEVEL_SIZE; j++)
                    {

                        if (BHashMap_Index.BTreeArray[j]->Root != NULL)
                        {

                            empty_flag = BTreeSelectRecordData_ASC(*(BHashMap_Index.BTreeArray[j]), &Selected_Data_List, 0, &Get_FieldData_TableA);
                            TempNode = Selected_Data_List.List_HeadNode->List_NextNode;


                            hybrid_index_total_size += Selected_Data_List.List_Size;


                            if (!empty_flag)
                            {

                                while (TempNode != NULL)
                                {

                                    if (Get_PrimaryKey_TableA(TempNode->RecordData) <= Get_PrimaryKey_TableA(TempNode->List_PrevNode->RecordData))
                                    {

                                        printf("Test failed\n\n");
                                        printf("Insertion process failed\n");
                                        printf("Exception (6)\n\n");

                                        return 1;

                                    }


                                    TempNode = TempNode->List_NextNode;

                                }


                            }


                            DeleteBList(&Selected_Data_List);

                        }

                    }


                    if (hybrid_index_total_size != i + 1)
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (7)\n\n");

                        return 1;

                    }

                }


                hybrid_index_total_size = 0;


                for (int i = 0; i < B_HASH_MAP_LEVEL_SIZE; i++)
                {

                    if (BHashMap_Index.BTreeArray[i]->Root != NULL)
                    {

                        BTreeCheckLeafLinkNodes(*(BHashMap_Index.BTreeArray[i]), &improperly_structured_nodes);


                        if (improperly_structured_nodes > 0)
                        {

                            printf("Test failed\n\n");
                            printf("Insertion process failed\n");
                            printf("Exception (8)\n\n");

                            return 1;

                        }


                        BTreeCheckBalance(*(BHashMap_Index.BTreeArray[i]), BHashMap_Index.BTreeArray[i]->height, &unbalanced_nodes);


                        if (unbalanced_nodes > 0)
                        {

                            printf("Test failed\n\n");
                            printf("Insertion process failed\n");
                            printf("Exception (9)\n\n");

                            return 1;

                        }


                        BTreeCheckNodeCapacity(*(BHashMap_Index.BTreeArray[i]), &error_flag);


                        if (error_flag)
                        {

                            printf("Test failed\n\n");
                            printf("Insertion process failed\n");
                            printf("Exception (10)\n\n");

                            return 1;

                        }


                        BTreeComputeNodes( *(BHashMap_Index.BTreeArray[i]),
                                           &Internal_nodes_number,
                                           &Internal_nodes_elements,
                                           &Leaf_nodes_number,
                                           &Leaf_nodes_elements );

                        hybrid_index_total_size += Internal_nodes_elements + Leaf_nodes_elements;


                        if (Internal_nodes_elements + Leaf_nodes_elements != BHashMap_Index.BTreeArray[i]->size)
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


                        if (Internal_nodes_elements + 1 != Leaf_nodes_number)
                        {

                            printf("Test failed\n\n");
                            printf("Insertion process failed\n");
                            printf("Exception (13)\n\n");

                            return 1;

                        }

                    }

                }


                if (hybrid_index_total_size != input_data_size)
                {

                    printf("Test failed\n\n");
                    printf("Insertion process failed\n");
                    printf("Exception (14)\n\n");

                    return 1;

                }




                ////////////////////////////////////////////////// Selection By Primary Key ///////////////////////////////////////////////////




                for (i = 0; i < input_data_size; i++)
                {

                    Retrieved_Data = NULL;
                    Retrieved_Data = BHashMapSelectData_ByPrimaryKey( BHashMap_Index,
                                                                      Input_DataSet[i],
                                                                      Input_DataSet[i] % B_HASH_MAP_LEVEL_SIZE,
                                                                      &Get_PrimaryKey_TableA );


                    if (Retrieved_Data == NULL || Get_PrimaryKey_TableA(Retrieved_Data) != Input_DataSet[i])
                    {

                        printf("Test failed\n\n");
                        printf("Insertion process failed\n");
                        printf("Exception (15)\n\n");

                        return 1;

                    }

                }




                ////////////////////////////////////////////////// Full Selection //////////////////////////////////////////////




                empty_flag = BHashMapSelectData(BHashMap_Index, &Selected_Data_List, 0, 0, &Get_FieldData_TableA);


                if (empty_flag || Selected_Data_List.List_Size != input_data_size)
                {

                    printf("Test failed\n\n");
                    printf("Selection process failed\n");
                    printf("Exception (1)\n\n");

                    return 1;

                }


                TempNode = Selected_Data_List.List_HeadNode;


                if (!empty_flag)
                {

                    while (TempNode != NULL)
                    {

                        Retrieved_Data = NULL;
                        Retrieved_Data = BHashMapSelectData_ByPrimaryKey( BHashMap_Index,
                                                                          Get_PrimaryKey_TableA(TempNode->RecordData),
                                                                          Get_PrimaryKey_TableA(TempNode->RecordData) % B_HASH_MAP_LEVEL_SIZE,
                                                                          &Get_PrimaryKey_TableA );


                        if (Retrieved_Data == NULL || Get_PrimaryKey_TableA(Retrieved_Data) != Get_PrimaryKey_TableA(TempNode->RecordData))
                        {

                            printf("Test failed\n\n");
                            printf("Selection process failed\n");
                            printf("Exception (2)\n\n");

                            return 1;

                        }


                        TempNode = TempNode->List_NextNode;

                    }


                }


                DeleteBList(&Selected_Data_List);




                ////////////////////////////////////////////////// Deletion //////////////////////////////////////////////




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


                for (i = 0; i < (input_data_size - 1); i++)
                {

                    execution_status = BHashMapDeleteData( &BHashMap_Index,
                                                           Input_DataSet[i],
                                                           Input_DataSet[i] % B_HASH_MAP_LEVEL_SIZE,
                                                           &Retrieved_Data,
                                                           &Get_PrimaryKey_TableA );


                    if (execution_status)
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (1)\n\n");

                        return 1;

                    }else {

                        Deallocate_BTreeRecordData_TableA(Retrieved_Data);

                    }


                    execution_status = BHashMapDeleteData( &BHashMap_Index,
                                                           Input_DataSet[i],
                                                           Input_DataSet[i] % B_HASH_MAP_LEVEL_SIZE,
                                                           &Retrieved_Data,
                                                           &Get_PrimaryKey_TableA );


                    if (!execution_status)
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (2)\n\n");

                        return 1;

                    }


                    if (BHashMap_Index.size != input_data_size - (i + 1))
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (3)\n\n");

                        return 1;


                    }


                    hybrid_index_total_size = 0;


                    for (int j = 0; j < B_HASH_MAP_LEVEL_SIZE; j++)
                    {

                        if (BHashMap_Index.BTreeArray[j]->Root != NULL)
                        {

                            hybrid_index_total_size += BHashMap_Index.BTreeArray[j]->size;
                            BTreeComputeHeight(*(BHashMap_Index.BTreeArray[j]), &btree_height);


                            if (BHashMap_Index.BTreeArray[j]->height != btree_height)
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (4)\n\n");

                            }

                        }

                    }


                    if (hybrid_index_total_size != input_data_size - (i + 1))
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (5)\n\n");

                        return 1;

                    }


                    hybrid_index_total_size = 0;


                    for (int j = 0; j < B_HASH_MAP_LEVEL_SIZE; j++)
                    {

                        if (BHashMap_Index.BTreeArray[j]->Root != NULL)
                        {

                            empty_flag = BTreeSelectRecordData_ASC(*(BHashMap_Index.BTreeArray[j]), &Selected_Data_List, 0, &Get_FieldData_TableA);
                            TempNode = Selected_Data_List.List_HeadNode->List_NextNode;


                            hybrid_index_total_size += Selected_Data_List.List_Size;


                            if (!empty_flag)
                            {

                                while (TempNode != NULL)
                                {

                                    if (Get_PrimaryKey_TableA(TempNode->RecordData) <= Get_PrimaryKey_TableA(TempNode->List_PrevNode->RecordData))
                                    {

                                        printf("Test failed\n\n");
                                        printf("Deletion process failed\n");
                                        printf("Exception (6)\n\n");

                                        return 1;

                                    }


                                    TempNode = TempNode->List_NextNode;

                                }


                            }


                            DeleteBList(&Selected_Data_List);

                        }

                    }


                    if (hybrid_index_total_size != input_data_size - (i + 1))
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (7)\n\n");

                        return 1;

                    }


                    hybrid_index_total_size = 0;


                    for (int j = 0; j < B_HASH_MAP_LEVEL_SIZE; j++)
                    {

                        if (BHashMap_Index.BTreeArray[j]->Root != NULL)
                        {

                            BTreeCheckLeafLinkNodes(*(BHashMap_Index.BTreeArray[j]), &improperly_structured_nodes);


                            if (improperly_structured_nodes > 0)
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (8)\n\n");

                                return 1;

                            }


                            BTreeCheckBalance(*(BHashMap_Index.BTreeArray[j]), BHashMap_Index.BTreeArray[j]->height, &unbalanced_nodes);


                            if (unbalanced_nodes > 0)
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (9)\n\n");

                                return 1;

                            }


                            BTreeCheckNodeCapacity(*(BHashMap_Index.BTreeArray[j]), &error_flag);


                            if (error_flag)
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (10)\n\n");

                                return 1;

                            }


                            BTreeComputeNodes( *(BHashMap_Index.BTreeArray[j]),
                                               &Internal_nodes_number,
                                               &Internal_nodes_elements,
                                               &Leaf_nodes_number,
                                               &Leaf_nodes_elements );

                            hybrid_index_total_size += Internal_nodes_elements + Leaf_nodes_elements;


                            if (Internal_nodes_elements + Leaf_nodes_elements != BHashMap_Index.BTreeArray[j]->size)
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (11)\n\n");

                                return 1;

                            }


                            if (Internal_nodes_number >= Leaf_nodes_number)
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (12)\n\n");

                                return 1;

                            }


                            if (Internal_nodes_elements + 1 != Leaf_nodes_number)
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (13)\n\n");

                                return 1;

                            }

                        }

                    }


                    if (hybrid_index_total_size != input_data_size - (i + 1))
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (14)\n\n");

                        return 1;

                    }


                    for (j = i + 1; j < input_data_size; j++)
                    {

                        Retrieved_Data = NULL;
                        Retrieved_Data = BHashMapSelectData_ByPrimaryKey( BHashMap_Index,
                                                                          Input_DataSet[j],
                                                                          Input_DataSet[j] % B_HASH_MAP_LEVEL_SIZE,
                                                                          &Get_PrimaryKey_TableA );


                        if (Retrieved_Data == NULL || Get_PrimaryKey_TableA(Retrieved_Data) != Input_DataSet[j])
                        {

                            printf("Test failed\n\n");
                            printf("Deletion process failed\n");
                            printf("Exception (15)\n\n");

                            return 1;

                        }

                    }


                    empty_flag = BHashMapSelectData(BHashMap_Index, &Selected_Data_List, 0, 0, &Get_FieldData_TableA);


                    if (empty_flag || Selected_Data_List.List_Size != input_data_size - (i + 1))
                    {

                        printf("Test failed\n\n");
                        printf("Deletion process failed\n");
                        printf("Exception (16)\n\n");

                        return 1;

                    }


                    TempNode = Selected_Data_List.List_HeadNode;


                    if (!empty_flag)
                    {

                        while (TempNode != NULL)
                        {

                            Retrieved_Data = NULL;
                            Retrieved_Data = BHashMapSelectData_ByPrimaryKey( BHashMap_Index,
                                                                              Get_PrimaryKey_TableA(TempNode->RecordData),
                                                                              Get_PrimaryKey_TableA(TempNode->RecordData) % B_HASH_MAP_LEVEL_SIZE,
                                                                              &Get_PrimaryKey_TableA );


                            if (Retrieved_Data == NULL || Get_PrimaryKey_TableA(Retrieved_Data) != Get_PrimaryKey_TableA(TempNode->RecordData))
                            {

                                printf("Test failed\n\n");
                                printf("Deletion process failed\n");
                                printf("Exception (17)\n\n");

                                return 1;

                            }


                            TempNode = TempNode->List_NextNode;

                        }


                    }


                    DeleteBList(&Selected_Data_List);

                }


                execution_status = BHashMapDeleteData( &BHashMap_Index,
                                                       Input_DataSet[input_data_size - 1],
                                                       Input_DataSet[input_data_size - 1] % B_HASH_MAP_LEVEL_SIZE,
                                                       &Retrieved_Data,
                                                       &Get_PrimaryKey_TableA );


                if (execution_status)
                {

                    printf("Test failed\n\n");
                    printf("Deletion process failed\n");
                    printf("Exception (18)\n\n");

                    return 1;

                }else {

                    Deallocate_BTreeRecordData_TableA(Retrieved_Data);

                }


                if (BHashMap_Index.size != 0)
                {

                    printf("Test failed\n\n");
                    printf("Deletion process failed\n");
                    printf("Exception (19)\n\n");

                    return 1;

                }


                if (!BHashMapClean(&BHashMap_Index, &Deallocate_BTreeRecordData_TableA))
                {

                    printf("Test failed\n\n");
                    printf("Deletion process failed\n");
                    printf("Exception (20)\n\n");

                    return 1;

                }

            }


            BHashMapDrop(&BHashMap_Index, &Deallocate_BTreeRecordData_TableA);

            printf("(Test passed)\n\n");

        }

    }


    free(Input_DataSet);
    free(Copy_Input_DataSet);


    printf("\n");
    printf("(Integration test passed)\n\n");


    return 0;


}

