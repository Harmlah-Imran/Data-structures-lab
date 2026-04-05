/*
 * Lab 4 - Task 3
 *
 * UNDERSTANDING THE TASK:
 * Implement rotateLeft, rotateRight, get (at position, removing it), sort (ascending),
 * isPalindrome (two versions), reusing all prior Queue functions.
 *
 * HOW I WILL SOLVE IT:
 * - rotateRight(q, pos): elements from pos to end rotate right by 1 (last wraps to pos).
 *   Before: ( 10 -> 9 -> 2 -> 5 -> -7 -> ), pos=2
 *   After:  ( 10 -> 9 -> -7 -> 2 -> 5 -> )
 *   Method: extract subqueue from pos to end, move last to front of sub, re-insert.
 *
 * - rotateLeft(q, pos): elements from pos to end rotate left by 1 (first wraps to end).
 *   Before: ( 10 -> 9 -> -7 -> 2 -> 5 -> ), pos=2
 *   After:  ( 10 -> 9 -> 2 -> 5 -> -7 -> )
 *   Method: extract subqueue, move first to end of sub, re-insert.
 *
 * - get(q, pos): dequeue element at pos (0-based), return it, queue shrinks by 1.
 *
 * - sort(q): bubble sort using positional access (O(n^2) dequeue/enqueue).
 *
 * - isPalindrome(Queue*): compare element by element from front and back.
 *   STACK USED: to get reverse traversal of queue. Justification: Queue has no
 *   reverse iterator; a Stack is the natural reversal tool.
 *
 * - isPalindrome(Queue*, bool&): single-digit version without extra ADT.
 *   Works by reconstructing the number and checking numerically.
 *   Actually: since we CAN'T use another queue or stack, we compare by rebuilding
 *   the queue and checking symmetry through repeated front/back comparisons using
 *   only dequeue/enqueue (rotate-based front-back comparison).
 *
 * BUGS IN LAB main() FIXED:
 * 1. "display(B, cout)" in get result line - B is undefined; changed to display(A, cout).
 * 2. isPalindrome(D, isIt) check uses "if(B)" instead of "if(isIt)". Fixed.
 * 3. "sort(B)" - B not declared in Task 3 main. Changed to sort(A).
 * 4. D queue: lab code pushes into C not D. Fixed to push into D.
 *
 * LEARNING: Palindrome detection in a queue requires reversing which naturally needs a Stack.
 *
 * SKILLS DEVELOPED: Rotation, sorting, and palindrome detection on FIFO structures.
 */

#include <iostream>
using namespace std;
#include "Stack.h"
#include "MyStack.h"
#include "Queue.h"
#include "MyQueue.h"

const int MAX_SIZE = 10;

// ---- Reused Functions (from Tasks 1 & 2) ----

int getSize(Queue * q)
{
    int count = 0; MyQueue temp(MAX_SIZE*2);
    while(!q->isEmpty()){ temp.enqueue(q->dequeue()); count++; }
    while(!temp.isEmpty()) q->enqueue(temp.dequeue());
    return count;
}

void display(Queue * q, ostream & out)
{
    MyQueue temp(MAX_SIZE*2);
    out << "[ ";
    while(!q->isEmpty()){ int v=q->dequeue(); out<<v<<" "; temp.enqueue(v); }
    out << "]\n";
    while(!temp.isEmpty()) q->enqueue(temp.dequeue());
}

void append(Queue * src, Queue * dst)
{
    MyQueue temp(MAX_SIZE*2);
    while(!src->isEmpty()){ int v=src->dequeue(); temp.enqueue(v); }
    while(!temp.isEmpty()){ int v=temp.dequeue(); src->enqueue(v); if(!dst->isFull()) dst->enqueue(v); }
}

void shiftRight(Queue * q, int pos)
{
    MyQueue temp(MAX_SIZE*2);
    for(int i=0;i<pos&&!q->isEmpty();i++) temp.enqueue(q->dequeue());
    temp.enqueue(0);
    while(!q->isEmpty()) temp.enqueue(q->dequeue());
    while(!temp.isEmpty()){ int v=temp.dequeue(); if(!q->isFull()) q->enqueue(v); }
}

void shiftLeft(Queue * q, int pos)
{
    MyQueue temp(MAX_SIZE*2);
    for(int i=0;i<pos&&!q->isEmpty();i++) temp.enqueue(q->dequeue());
    if(!q->isEmpty()) q->dequeue(); // discard
    while(!q->isEmpty()) temp.enqueue(q->dequeue());
    while(!temp.isEmpty()) q->enqueue(temp.dequeue());
}

void insert(Queue * q, int v, int pos)
{
    shiftRight(q, pos);
    MyQueue temp(MAX_SIZE*2);
    for(int i=0;i<pos&&!q->isEmpty();i++) temp.enqueue(q->dequeue());
    if(!q->isEmpty()) q->dequeue(); // remove 0
    temp.enqueue(v);
    while(!q->isEmpty()) temp.enqueue(q->dequeue());
    while(!temp.isEmpty()) q->enqueue(temp.dequeue());
}

void insert(Queue * src, Queue * dst, int pos)
{
    int srcSize = getSize(src);
    MyQueue srcCopy(MAX_SIZE*2);
    append(src, &srcCopy);
    for(int k=0;k<srcSize;k++) shiftRight(dst, pos+k);
    int dstArr[MAX_SIZE*4], srcArr[MAX_SIZE*2];
    int dc=0, sc=0;
    while(!dst->isEmpty()) dstArr[dc++]=dst->dequeue();
    while(!srcCopy.isEmpty()) srcArr[sc++]=srcCopy.dequeue();
    for(int k=0;k<sc&&(pos+k)<dc;k++) dstArr[pos+k]=srcArr[k];
    for(int k=0;k<dc&&!dst->isFull();k++) dst->enqueue(dstArr[k]);
}

bool isEqual(Queue * a, Queue * b)
{
    int sA=getSize(a), sB=getSize(b);
    if(sA!=sB) return false;
    MyQueue tA(MAX_SIZE*2), tB(MAX_SIZE*2);
    bool eq=true;
    for(int i=0;i<sA;i++){ int va=a->dequeue(), vb=b->dequeue(); tA.enqueue(va); tB.enqueue(vb); if(va!=vb) eq=false; }
    while(!tA.isEmpty()) a->enqueue(tA.dequeue());
    while(!tB.isEmpty()) b->enqueue(tB.dequeue());
    return eq;
}

// ---- New Task 3 Functions ----

// get(q, pos): remove and return element at pos (0-based from front)
int get(Queue * q, int pos)
{
    MyQueue temp(MAX_SIZE*2);
    for(int i=0; i<pos && !q->isEmpty(); i++)
        temp.enqueue(q->dequeue());
    int val = q->isEmpty() ? -1 : q->dequeue(); // element at pos (removed)
    while(!q->isEmpty()) temp.enqueue(q->dequeue());
    while(!temp.isEmpty()) q->enqueue(temp.dequeue());
    return val;
}

// rotateRight(q, pos): sub-queue from pos onward rotates right by 1
// Last element of sub moves to position pos.
// ( a -> b -> c -> d -> e -> ) pos=2 => ( a -> b -> e -> c -> d -> )
void rotateRight(Queue * q, int pos)
{
    int size = getSize(q);
    if(pos >= size) return;

    // Extract sub-queue from pos to end
    MyQueue prefix(MAX_SIZE*2), sub(MAX_SIZE*2);
    for(int i=0; i<pos && !q->isEmpty(); i++) prefix.enqueue(q->dequeue());
    while(!q->isEmpty()) sub.enqueue(q->dequeue());

    // sub: rotate right = last element goes to front
    int subSize = getSize(&sub);
    if(subSize < 2) {
        while(!prefix.isEmpty()) q->enqueue(prefix.dequeue());
        while(!sub.isEmpty()) q->enqueue(sub.dequeue());
        return;
    }
    // Get last element of sub
    int subArr[MAX_SIZE*2];
    int sc=0;
    while(!sub.isEmpty()) subArr[sc++]=sub.dequeue();
    int last = subArr[sc-1];
    // Rebuild sub: last first, then rest
    sub.enqueue(last);
    for(int i=0;i<sc-1;i++) sub.enqueue(subArr[i]);

    // Rebuild q
    while(!prefix.isEmpty()) q->enqueue(prefix.dequeue());
    while(!sub.isEmpty()) q->enqueue(sub.dequeue());
}

// rotateLeft(q, pos): sub-queue from pos onward rotates left by 1
// First element of sub moves to end.
// ( a -> b -> c -> d -> e -> ) pos=2 => ( a -> b -> d -> e -> c -> )
void rotateLeft(Queue * q, int pos)
{
    int size = getSize(q);
    if(pos >= size) return;

    MyQueue prefix(MAX_SIZE*2), sub(MAX_SIZE*2);
    for(int i=0; i<pos && !q->isEmpty(); i++) prefix.enqueue(q->dequeue());
    while(!q->isEmpty()) sub.enqueue(q->dequeue());

    int subSize = getSize(&sub);
    if(subSize < 2) {
        while(!prefix.isEmpty()) q->enqueue(prefix.dequeue());
        while(!sub.isEmpty()) q->enqueue(sub.dequeue());
        return;
    }
    // Rotate left: first element goes to end (standard queue rotate)
    int first = sub.dequeue();
    sub.enqueue(first);

    while(!prefix.isEmpty()) q->enqueue(prefix.dequeue());
    while(!sub.isEmpty()) q->enqueue(sub.dequeue());
}

// sort(q): sort queue values in ascending order (bubble sort)
void sort(Queue * q)
{
    int size = getSize(q);
    // Drain to array
    int arr[MAX_SIZE*2];
    int n=0;
    while(!q->isEmpty()) arr[n++]=q->dequeue();
    // Bubble sort
    for(int i=0;i<n-1;i++)
        for(int j=0;j<n-1-i;j++)
            if(arr[j]>arr[j+1]){ int t=arr[j]; arr[j]=arr[j+1]; arr[j+1]=t; }
    // Refill
    for(int i=0;i<n;i++) q->enqueue(arr[i]);
}

// isPalindrome(Queue*): checks if queue values are palindrome
// STACK USED: to get values in reverse order for comparison.
// Justification: comparing front and back of queue simultaneously requires
// knowing the back, which needs reversal - naturally done with a Stack.
bool isPalindrome(Queue * q)
{
    int size = getSize(q);
    if(size <= 1) return true;

    // Copy queue into a stack (stack top = last element = rear of queue)
    MyStack stk(MAX_SIZE*2);
    MyQueue temp(MAX_SIZE*2);
    while(!q->isEmpty()){ int v=q->dequeue(); stk.push(v); temp.enqueue(v); }
    while(!temp.isEmpty()) q->enqueue(temp.dequeue());

    // Compare front of queue with top of stack for size/2 times
    MyQueue temp2(MAX_SIZE*2);
    bool palindrome = true;
    for(int i=0;i<size/2;i++)
    {
        int front = q->dequeue();
        int back; stk.pop(back);
        temp2.enqueue(front);
        if(front != back) palindrome = false;
    }
    // Restore queue: put front elements back + skip middle + restore rear via stack
    // Actually just re-display from original; we need to restore q fully
    // Drain remaining q into temp3
    MyQueue temp3(MAX_SIZE*2);
    while(!q->isEmpty()) temp3.enqueue(q->dequeue());
    // Put front elements back first
    while(!temp2.isEmpty()) q->enqueue(temp2.dequeue());
    // Then middle
    while(!temp3.isEmpty()) q->enqueue(temp3.dequeue());
    // Pop remaining from stack (these are the rear elements already in q via original copy)
    // Actually the stack has all elements, and q was restored via temp above.
    // The stack is just for comparison, q is already restored correctly.

    return palindrome;
}

// isPalindrome(Queue*, bool&): for single digit values only, no extra ADT allowed
// Method: use mathematical digit reversal of the whole number formed by the queue,
// but that doesn't make sense for a sequence. Instead we use a rotate-based approach:
// Compare front and back by counting size, then rotating to access each position.
// This uses ONLY the queue itself - no additional Stack or Queue.
void isPalindrome(Queue * q, bool & result)
{
    int size = getSize(q);
    result = true;
    if(size <= 1) return;

    // For each pair (i, size-1-i), compare values using rotation
    // We'll rotate the queue to bring front to known position
    // Simple approach: drain to known positions using dequeue/enqueue pattern
    // Since values are single digits (0-9), we can mathematically check:
    // Rotate queue to get element at position i:
    // Rotate i times to bring element i to front, peek, rotate back

    for(int i = 0; i < size/2; i++)
    {
        // Get element at position i (from front): rotate i times
        for(int r=0;r<i;r++){ int v=q->dequeue(); q->enqueue(v); }
        int front = q->dequeue(); q->enqueue(front);
        // Restore rotation
        for(int r=0;r<size-i-1;r++){ int v=q->dequeue(); q->enqueue(v); }

        // Get element at position size-1-i: rotate (size-1-i) times
        for(int r=0;r<(size-1-i);r++){ int v=q->dequeue(); q->enqueue(v); }
        int back = q->dequeue(); q->enqueue(back);
        // Restore rotation
        for(int r=0;r<i;r++){ int v=q->dequeue(); q->enqueue(v); }

        if(front != back){ result = false; return; }
    }
}

int main()
{
    Queue* A = new MyQueue(MAX_SIZE);
    A->enqueue(10);
    A->enqueue(9);
    A->enqueue(2);
    A->enqueue(5);
    A->enqueue(-7);
    cout << "Queue A Values: ";
    display(A, cout);

    rotateRight(A, 2);
    cout << "Queue A after rotateRight from pos 2: ";
    display(A, cout);

    rotateLeft(A, 2);
    cout << "Queue A after rotateLeft from pos 2: ";
    display(A, cout);

    int v = get(A, 3);
    cout << "Value at pos 3: " << v << "\n";
    cout << "Queue A after get from pos 3: ";
    display(A, cout);

    if(isPalindrome(A))
        cout << "A is a Palindrome\n";
    else
        cout << "A is NOT a Palindrome\n";

    // Known palindrome: 234 56 786 56 234
    Queue* C = new MyQueue(MAX_SIZE);
    C->enqueue(234);
    C->enqueue(56);
    C->enqueue(786);
    C->enqueue(56);
    C->enqueue(234);
    cout << "\nQueue C: ";
    display(C, cout);
    if(isPalindrome(C))
        cout << "C is a Palindrome\n";
    else
        cout << "C is NOT a Palindrome\n";

    // Single-digit palindrome: 3 5 7 5 3
    Queue* D = new MyQueue(MAX_SIZE);
    D->enqueue(3);
    D->enqueue(5);
    D->enqueue(7);
    D->enqueue(5);
    D->enqueue(3);
    cout << "\nQueue D (single digits): ";
    display(D, cout);
    bool isIt = false;
    isPalindrome(D, isIt);
    if(isIt)
        cout << "D is a Palindrome\n";
    else
        cout << "D is NOT a Palindrome\n";

    sort(A);
    cout << "\nQueue A after sort ascending: ";
    display(A, cout);

    delete A; delete C; delete D;
    return 0;
}
