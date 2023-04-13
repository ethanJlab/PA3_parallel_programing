#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <future>


using namespace std;

std::mutex mtx;

// define a node for a linked list
struct Node
{
    int ID;
    Node* next;
};

const int numPresents = 500000;
int insertCount = 0;
int tyCount = 0;
int presentNum = 1;

bool tyCards[numPresents] = {false};
bool unorderedBag[numPresents] = {true};
bool globalNodeExists;
int globalPresentNum;

Node* createList();
void insertNode(Node* head, int ID);
void writeThankYou(int ID);
void removeNode(Node* head, int ID);
bool nodeExists(Node* head, int ID);
int getPresent();
bool allTYCards();
void nodeExistsAtomic(Node* head, int ID);
void getPresentAtomic();



int main()
{
    Node* head = createList();
    //int present;
    //bool flag;
    cout << "Program started" << endl;
    /*
    for(int i = 1; i < numPresents + 1; i++)
    {
        getPresentAtomic();
        present = globalPresentNum;
        insertNode(head, present);
        writeThankYou(present);

    }
    */
    
   // add multithreading
    //while(!allTYCards())    
    for(int i = 0; i < numPresents; i++)
    {
        // first servent gets the present
        thread t1(getPresentAtomic);
        t1.join();
        
        // once servent one gets the present, servent two checks if the present is in the chain
        //bool flag = nodeExists(head, globalPresentNum);
        bool flag = false;
        // if the present is not in the chain, servent three inserts the present into the chain and a thank you card is written
        if(!flag)
        {
            thread t3(insertNode, head, globalPresentNum);
            thread t4(writeThankYou, globalPresentNum);
            t3.join();
            t4.join();
        }      
        
    }
    

    cout << "Program finished" << endl;
    cout << "" << allTYCards() << endl;
    cout << "Insert count: " << insertCount << endl;
    cout << "Thank you count: " << tyCount << endl;

    return 0;
}

// function that returns the head of a linked list
Node* createList()
{
    // create a linked list with 5 nodes
    Node* head = new Node;
    head->ID = 0;
    return head;
}

void insertNode(Node* head, int ID)
{
    // insert a node in the correct position based on the ID
    
    if (head->next == NULL)
    {
        head->next = new Node;
        head->next->ID = ID;
    }
    else
    {
        if (ID < head->next->ID)
        {
            cout << "Inserting " << ID << endl;
            insertCount++;
            Node* temp = new Node;
            temp->ID = ID;
            temp->next = head->next;
            head->next = temp;
        }
        else
        {
            insertNode(head->next, ID);
        }
    }
}

void writeThankYou(int ID)
{
    // write a thank you note to the person with the given ID
    tyCards[ID] = true;
    cout << "Thank you " << ID << endl;
    tyCount++;
}

void removeNode(Node* head, int ID)
{
    // remove the node with the given ID
    if (head->next != NULL)
    {
        if (head->next->ID == ID)
        {
            Node* temp = head->next;
            head->next = head->next->next;
            delete temp;
        }
        else
        {
            removeNode(head->next, ID);
        }
    }
}

bool nodeExists(Node* head, int ID)
{
    // return true if the node with the given ID exists
    if (head->next != NULL)
    {
        if (head->next->ID == ID)
        {
            return true;
        }
        else
        {
            return nodeExists(head->next, ID);
        }
    }
    else
    {
        return false;
    }
}

void nodeExistsAtomic(Node* head, int ID)
{
    mtx.lock();
    globalNodeExists = nodeExists(head, ID);   
    mtx.unlock();
}

int getPresent()
{
    // return a random index of a true value in the unorderedBag array
    /*int index = rand() % numPresents;
    while (unorderedBag[index])
    {
        index = rand() % numPresents;
    }*/
    int index = presentNum;
    presentNum++;

    cout << "Present got " << index << endl;
    return index;
}

void getPresentAtomic()
{
    mtx.lock();
    globalPresentNum = getPresent();
    mtx.unlock();
}

bool allTYCards()
{
    // return true if all the thank you cards have been written
    for (int i = 0; i < numPresents; i++)
    {
        if (!tyCards[i])
        {
            return false;
        }
    }
    return true;
}

