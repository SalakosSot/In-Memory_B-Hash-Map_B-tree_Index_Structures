## B-Hash Map and B-tree index structures implementation in C

                
----

This software package provides an implementation of the B-Hash Map and B-tree index structures as a structural and functional simulation of a RDBMS in-memory file system indexing sub-system structures. There are 2 versions of these index structures that store record references based on primary key fields of both integer and string type.


### Quality Assurance

                
----

This package includes a set of unit testing function tools, integration tests and memory testing tools in order to be provided a qualitative dynamic, refactorable and maintainable software package.

#### Memory testing tools

- [Valgrind software package](https://valgrind.org/).

- [Valgrind Memcheck](https://valgrind.org/docs/manual/mc-manual.html#mc-manual.errormsgs) tool for problematic inefficient memory management and memory error detection in C.

- [Valgrind Massif](https://valgrind.org/docs/manual/ms-manual.html) & [Massif-Visualizer](https://apps.kde.org/massif-visualizer/) tools for heap profiling in C.

### Documentation

                
----

A complete analysis of the implemented B-Hash Map and B-tree index structures functional and structural parts theory, design, architecture and efficiency in terms of time performance is provided in the theoretical computational study entitled [Analysis and Comparison of Data Structures for Database Indexing](https://www.researchgate.net/profile/Sotirios-Salakos) that was held and conducted as an academic research of the UOWM ECE department.

Furthermore an additional study with theoretical and practical research content on the RDBMS file system on-disk and in-memory sub-systems indexing structures has been conducted. This study entitled [Analysis and Comparison of Binary and Interpolation Search Algorithms in a B-tree](https://dl.acm.org/doi/abs/10.1145/3503823.3503837?casa_token=0xYDsLktaTAAAAAA:amt5I0efVGyXkO4EFeRC7Jg36AfnwEaKEw8UspXtrbE3xckKzCy1-Skw9LdC1kV4S1O6qNdsyfsS) index structure composes a theoretical and computational analysis, comparison and evaluation of the implemented B-tree index structure's functions in term of efficiency and time performance applying the binary and interpolation search algorithms. This study was published and presented in [PCI 2021 - 25th Pan-Hellenic Conference on Informatics](https://dl.acm.org/doi/proceedings/10.1145/3503823) and is a part of the thesis which was held and conducted in the context of computer science academic research in collaboration with the UOWM ECE department.

The complete studies are published and provided via the [ResearchGate](https://www.researchgate.net/profile/Sotirios-Salakos).

&nbsp;

If you use this code, please cite it as:
```
@inproceedings{salakos2021analysis,
  title={Analysis and Comparison of Binary and Interpolation Search Algorithms in a B-tree},
  author={Salakos, Sotirios and Ploskas, Nikolaos},
  booktitle={25th Pan-Hellenic Conference on Informatics},
  pages={74--78},
  year={2021}
}
```


### Dataset

                
----

The initial dataset that was reconstructed and used on the computational processes of the conducted studies is provided on [retail banking demo data](https://data.world/lpetrocelli/retail-banking-demo-data).

&nbsp;

&nbsp;

&nbsp;
### Using Library
----

#### Build Dynamic C Library on Linux
----

&nbsp;
To create the dynamic (shared) C library of B-Hash Map and B-tree index structures using GCC:
```
$ gcc -c BHashMap.c BTree.c DoubleLinkedBList.c -fpic
$ gcc *.o -shared -o Lib-BHashMap_B-tree_indexes.so
```

#### Using Dynamic C Library
----

&nbsp;
To set up the B-tree index structure integration test:
```
$ gcc -c Integration-Test_B-tree.c -o Integration-Test_B-tree.o
$ gcc -o Integration-Test_B-tree Integration-Test_B-tree.o -L. Lib-BHashMap_B-tree_indexes.so
```

To set up the B-Hash Map index structure integration test:
```
$ gcc -c Integration-Test_BHashMap.c -o Integration-Test_BHashMap.o
$ gcc -o Integration-Test_BHashMap Integration-Test_BHashMap.o -L. Lib-BHashMap_B-tree_indexes.so
```

To utilize the shared C library use the following command that allows program loader to locate the library by adding it to the LD_LIBRARY_PATH environment variable or add the Lib-BHashMap_B-tree_indexes.so file to the /usr/local/lib directory that program loader searches automatically for libraries at runtime: 
```
$ export LD_LIBRARY_PATH=:/full/path/to/library/directory
```

#### Utilizing integration tests
----

&nbsp;
Running tests:
```
$ nohup ./Integration-Test_B-tree > TestingReport-Integration-Test_B-tree.out &
$ nohup ./Integration-Test_BHashMap > TestingReport-Integration-Test_BHashMap.out &
```

#### Utilizing integration tests & Valgrind Memcheck
----

&nbsp;
Run integration tests using valgrind memcheck tool:
```
$ nohup valgrind --tool=memcheck --leak-check=yes ./Integration-Test_B-tree > TestingReport-Integration-Test_B-tree.out &
$ nohup valgrind --tool=memcheck --leak-check=yes ./Integration-Test_BHashMap > TestingReport-Integration-Test_BHashMap.out &
```


