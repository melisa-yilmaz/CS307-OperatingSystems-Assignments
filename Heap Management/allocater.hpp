// Melisa Yilmaz 26777
// Date: 25.12.2021
// Programming Assignment 4
// Heap Management .hpp file


#ifndef allocater_hpp
#define allocater_hpp

#include <stdio.h>
#include <iostream>
#include <pthread.h>
using namespace std;

struct node {
    int id;
    int size;
    int index;
    node *next;
};

class HeapManager
{
public:
    HeapManager(); // constructor
    ~HeapManager(); // destructor
    
    int initHeap(int size);
    int myMalloc(int ID, int size);
    int myFree(int ID, int index);
    void print();
    
    void updateIndex();
private:
    node *heap;
};
#include "allocater.cpp"
#endif /* allocater_hpp */
