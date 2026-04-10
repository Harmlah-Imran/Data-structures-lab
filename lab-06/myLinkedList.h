#pragma once
#include"Node.h"
#include"LinkedList.h"
#include<iostream>
using namespace std;
class myLinkedList :public LinkedList
{
public:
    myLinkedList(const myLinkedList& L);
  	void put(int v, int pos); // put the value v at the pos(ition) given 
    void swap(int xi, int yi);// Swaps the values at position xi and yi 
    void sort(bool asc);
    void insertAtLast(int value);
    void insertAtFirst(int value);
    void display();
    int removeFromFirst();
    int removeFromLast();
    bool isEmpty();
    int getSize(); // returns size of the linked list 
    bool search(int v); // that searches the given value in the list 
    int peek(int pos); // returns the value at pos(ition) 
    int get(int pos); // removes and returns the value at pos(ition) 

};

myLinkedList::myLinkedList(const myLinkedList& L) {
    first = nullptr;
    last = nullptr;

    Node* temp = L.first;          
    while (temp != nullptr) {
        insertAtLast(temp->data);
        temp = temp->next;
    }
}
int myLinkedList::getSize() {               
    if (isEmpty())
        return 0;
    int size = 0;
    Node* temp = first;
    while (temp != nullptr) {      
        size++;
        temp = temp->next;
    }
    return size;                 
}
bool myLinkedList::search(int v) {
    Node* temp = first;
    while (temp != nullptr) {
        if (temp->data = v)
            return true;
        temp = temp->next;
    }
    return false;
}
int myLinkedList::peek(int pos) {
    if (isEmpty() || pos < 0 || pos >= getSize()) {
        cout << "Erooro" << endl;
        return -99999;
    }
    Node* temp = first;
    for (int i = 0; i < pos; i++) {
        temp = temp->next;
    }
    return temp->data;
}
int myLinkedList::get(int pos) {
    if (isEmpty() || pos < 0 || pos >= getSize()) {
        cout << "Erooro" << endl;
        return -99999;
    }
    Node* temp = first;
    for (int i = 0; i < pos; i++) {
        temp = temp->next;
        Node* target = temp->next;   
        int value = target->data;    
        temp->next = target->next;   
        if (target == last)          
            last = temp;
        delete target;
        return value;
    }
    return removeFromFirst();
}

bool myLinkedList::isEmpty()
{
    return first == nullptr && last == nullptr;
}

int myLinkedList::removeFromLast()
{
    if (first == nullptr && last == nullptr) //empty linked list
    {
        cout << "Link list is empty" << endl;
        return -99999;
    }
    else if (first == last) //single node case
    {
        int returingValue = first->data;
        delete last;
        first = nullptr;
        last = nullptr;
        return returingValue;
    }
    else
    {
        Node* t = first;
        while (1)
        {
            if (t->next == last)
            {
                break;
            }
            t = t->next;
        }

        int returingValue = last->data;
        delete last;
        t->next = nullptr;
        last = t;
        return returingValue;
    }
}

int myLinkedList::removeFromFirst()
{
    if (first == nullptr && last == nullptr) 
    {
        cout << "emptyyy link list " << endl;
        return -99999;
    }
    else if (first == last) 
    {
        int returingValue = first->data;
        delete last;
        first = nullptr;
        last = nullptr;
        return returingValue;
    }
    else
    {
        int returingValue = first->data;
        Node* t = first;
        first = first->next;
        delete t;
        t = nullptr;
        return returingValue;
    }
}

void myLinkedList::display()
{
    if (first == nullptr && last == nullptr)
    {
        cout << "->\n";
        return;
    }
    Node* t = first;
    while (1)
    {
        cout << t->data;
        cout << " -> ";
        t = t->next;
        if (t == nullptr)
        {
            break;
        }
    }
    cout << endl;
}

void myLinkedList::insertAtFirst(int value)
{
    Node* nn = new Node;
    nn->data = value;
    nn->next = nullptr;
    if (first == nullptr && last == nullptr) 
    {
        first = nn;
        last = nn;
    }
    else 
    {
        nn->next = first;
        first = nn;
    }
}
void myLinkedList::put(int v, int pos) {
    if (pos<0 || pos>getSize()) {
        cout << "Invalid position" << endl;
        return;
    }
    if (pos == 0) {
        insertAtFirst(v);
        return;
    }
    if (pos == getSize()) {
        insertAtLast(v);
        return;
    }
    Node* nn = new Node;
    nn->data = v;
    Node* temp = first;
    for (int i = 0; i < pos - 1; i++)
        temp = temp->next;
    nn->next = temp->next;
    temp->next = nn;
}
void myLinkedList::swap(int xi, int yi) {
    if (xi < 0 || yi < 0 || xi >= getSize() || yi >= getSize()) {
        cout << "Invalid position" << endl;
        return;
    }
    if (xi == yi)
        return;
    Node* a = first;
    for (int i = 0; i < xi; i++)
        a = a->next;
    Node* b = first;
    for (int i = 0; i < yi; i++)
        b = b->next;
    int temp = a->data;
    a->data = b->data;
    b->data = temp;
}
void myLinkedList::sort(bool asc) {
    int n = getSize();
    for (int i = 0; i < n - 1; i++) {
        Node* temp = first;
        for (int j = 0; j < n - i - 1; j++) {
            if ((asc && temp->data > temp->next->data) || (!asc && temp->data < temp->next->data)) {
                int t = temp->data;
                temp->data = temp->next->data;
                temp->next->data = t;
            }
            temp = temp->next;
        }
    }
}
void myLinkedList::insertAtLast(int value)
{
    Node* nn = new Node;
    nn->data = value;
    nn->next = nullptr;
    if (first == nullptr && last == nullptr) 
    {
        first = nn;
        last = nn;
    }
    else 
    {
        last->next = nn;
        last = nn;
    }
}
