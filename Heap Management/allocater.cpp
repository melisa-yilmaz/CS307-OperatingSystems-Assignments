// Melisa Yilmaz 26777
// Date: 25.12.2021
// Programming Assignment 4
// Heap Management .cpp file

#include "allocater.hpp"
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

// constructor
HeapManager::HeapManager()
{
    heap = new node;
}

// destuctor
HeapManager::~HeapManager()
{
        // n points to the heap
        node *n = heap;

        //deleting each node until there is none
        while (n != nullptr)
        {
            node *deleteNode = n;
            n = n->next;
            delete deleteNode;
            deleteNode = nullptr;
        }
}

//initialize the heap with the requested size
int HeapManager::initHeap(int size)
{
    
    heap->id = -1; heap->size = size; heap->index = 0;
    heap->next = nullptr;
    print();
    return 1;
}

// try to allocate heap space of requested size
int HeapManager::myMalloc(int ID, int size)
{
    pthread_mutex_lock(&mutex1);
    pthread_mutex_lock(&mutex2);
    int check = -1;
    node *ptr = new node;
    ptr = heap;
    bool finish = false;
    while(ptr != NULL && finish == false)
    {
        if(ptr->id == -1 && ptr->size > size)
        {
            cout << "Allocated for thread " << ID << endl;
            ptr->id = ID;
            
            node *newPtr = new node;
            newPtr->next = ptr->next;
            ptr->next = newPtr;
            newPtr->id = -1;
            newPtr->size = ptr->size - size;
            ptr->size = size;
            updateIndex();
            finish = true;
            check = ptr->index;
        }
        
        else if( ptr->id == -1 && ptr->size == size)
        {
            cout << "Allocated for thread " << ID << endl;
            ptr->id = ID;
            finish = true;
            check = ptr->index;
        }
        
        ptr = ptr->next;
    }
    if(finish == false)
    {
        cout << "Can not allocate, requested size " << size <<" for thread " << ID << " is bigger than remaining size " << endl;
    }
    pthread_mutex_unlock(&mutex2);
    print();
    pthread_mutex_unlock(&mutex1);
    return check;
    
}

int HeapManager::myFree(int ID, int index)
{
    
    pthread_mutex_lock(&mutex1);
    pthread_mutex_lock(&mutex2);
    int check = -1;
    bool find = false;
    node *ptr = heap;
    if(ptr->next != NULL)
    {
        node *nextPtr = ptr->next;
        if(ptr->id == ID && ptr->index == index)
        {
            cout << "Freed for thread "<< ID << endl;
            ptr->id = -1;
            if(nextPtr->id == -1)
            {
                node *deleteNode = nextPtr;
                ptr->size = nextPtr->size + ptr->size;
                ptr->next = nextPtr->next;
                delete deleteNode;
                check = 1;
                
            }
            find = true;
        }
        else
        {
            bool end = false;
            while (nextPtr != NULL && end == false)
            {
                if(nextPtr->id == ID && nextPtr->index == index)
                {
                    cout << "Freed for thread "<< ID << endl;
                    nextPtr->id = -1;
                    if(nextPtr->next != NULL && nextPtr->next->id == -1)
                    {
                        node *deleteNode = nextPtr->next;
                        nextPtr->size = nextPtr->size + nextPtr->next->size;
                        nextPtr->next = deleteNode->next;
                        delete deleteNode;
                        end = true;
                       
                    }
                    if(ptr->id == -1)
                    {
                        node *deleteNode = nextPtr;
                        ptr->size = ptr->size + nextPtr->size;
                        ptr->next = deleteNode->next;
                        delete deleteNode;
                        end = true;
                       
                        
                    }
                    find = true;
                    check = 1;
                }
                ptr = nextPtr;
                nextPtr = nextPtr->next;
            }
        }
      
    }
    else
    {
        if(ptr->id == ID && ptr->index == index)
        {
            cout << "Freed for thread "<< ID << endl;
            ptr->id = -1;
            check = 1;
            find = true;
        }
    }
    if(find == false)
    {
        cout << "Can not free, requested index " << index <<" for thread " << ID  << endl;
    }
    else
        updateIndex();
    
    
    pthread_mutex_unlock(&mutex2);
    print();
    pthread_mutex_unlock(&mutex1);
    return check;
}


//print all the variables in the struct which are id, size, index
void HeapManager::print()
{
    pthread_mutex_lock(&mutex2);
    node *n = heap;
    while (n != NULL)
    {
        if(n->next != NULL)
            cout << "[" << n->id << "][" << n->size << "][" << n->index << "]----";
        else
            cout << "[" << n->id << "][" << n->size << "][" << n->index << "] ";
        n = n->next;
    }
    cout << endl;
    pthread_mutex_unlock(&mutex2);
    
}

void HeapManager::updateIndex()
{
    node *n = heap;
    int sum = 0;
    while(n->next != NULL)
    {
        sum += n->size; //sum size of the old nodes and give that value to the next node's index
        n->next->index = sum;
        n = n->next;
    }
}
