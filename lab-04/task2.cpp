/*
 * Lab 4 - Task 2
 *
 * UNDERSTANDING THE TASK:
 * Implement insert (queue into queue at pos), insert (single value at pos),
 * shiftRight (move elements right from pos, inserting 0), and shiftLeft
 * (remove element at pos, shifting rest left). Reuse Task 1 functions.
 *
 * HOW I WILL SOLVE IT:
 * All operations work by rebuilding the queue:
 * - shiftRight(q, pos): dequeue first pos elements to temp, enqueue 0,
 *   then re-enqueue the rest.
 *   Before: ( 10 -> 9 -> 2 -> 5 -> -7 -> ), pos=2
 *   After:  ( 10 -> 9 -> 0 -> 2 -> 5 -> -7 -> ) [if capacity allows]
 *
 * - shiftLeft(q, pos): dequeue first pos elements to temp, skip (dequeue+discard)
 *   the element at pos, then re-enqueue the rest.
 *   Before: ( 10 -> 9 -> 0 -> 2 -> 5 -> ), pos=2
 *   After:  ( 10 -> 9 -> 2 -> 5 -> )
 *
 * - insert(q, v, pos): shiftRight then place v at pos.
 * - insert(src, dst, pos): shiftRight dst srcSize times, then overwrite with src values.
 *
 * QUEUE NOTATION: ( front -> ... -> rear -> )
 *
 * FUNCTIONS REUSED: display, append, reverseAppend, getSize, stats, isEqual from Task 1.
 *
 * LEARNING: Queue manipulation requires careful drain-and-rebuild since there is
 * no random access. Every positional operation costs O(n).
 *
 * SKILLS DEVELOPED: Positional insertion and shifting in FIFO structures.
 */

#include <iostream>
using namespace std;
#include "Stack.h"
#include "MyStack.h"
#include "Queue.h"
#include "MyQueue.h"

const int MAX_SIZE = 10;

// ---- Reused from Task 1 ----

int getSize(Queue * q)
{
    int count = 0;
    MyQueue temp(MAX_SIZE * 2);
    while(!q->isEmpty()) { temp.enqueue(q->dequeue()); count++; }
    while(!temp.isEmpty()) q->enqueue(temp.dequeue());
    return count;
}

void display(Queue * q, ostream & out)
{
    MyQueue temp(MAX_SIZE * 2);
    out << "[ ";
    while(!q->isEmpty()) { int v = q->dequeue(); out << v << " "; temp.enqueue(v); }
    out << "]\n";
    while(!temp.isEmpty()) q->enqueue(temp.dequeue());
}

void append(Queue * src, Queue * dst)
{
    MyQueue temp(MAX_SIZE * 2);
    while(!src->isEmpty()) { int v = src->dequeue(); temp.enqueue(v); }
    while(!temp.isEmpty()) { int v = temp.dequeue(); src->enqueue(v); if(!dst->isFull()) dst->enqueue(v); }
}

void append(Queue * src, Queue * dst, int no)
{
    MyQueue temp(MAX_SIZE * 2);
    int count = 0;
    while(!src->isEmpty()) { int v = src->dequeue(); temp.enqueue(v); if(count < no && !dst->isFull()) dst->enqueue(v); count++; }
    while(!temp.isEmpty()) src->enqueue(temp.dequeue());
}

void reverseAppend(Queue * src, Queue * dst)
{
    MyStack stk(MAX_SIZE * 2);
    while(!src->isEmpty()) stk.push(src->dequeue());
    MyQueue temp(MAX_SIZE * 2);
    int v;
    while(!stk.isEmpty()) { stk.pop(v); temp.enqueue(v); }
    while(!temp.isEmpty()) stk.push(temp.dequeue());
    while(!stk.isEmpty()) { stk.pop(v); src->enqueue(v); if(!dst->isFull()) dst->enqueue(v); }
}

bool isEqual(Queue * a, Queue * b)
{
    int sA = getSize(a), sB = getSize(b);
    if(sA != sB) return false;
    MyQueue tA(MAX_SIZE*2), tB(MAX_SIZE*2);
    bool eq = true;
    for(int i = 0; i < sA; i++) { int va=a->dequeue(), vb=b->dequeue(); tA.enqueue(va); tB.enqueue(vb); if(va!=vb) eq=false; }
    while(!tA.isEmpty()) a->enqueue(tA.dequeue());
    while(!tB.isEmpty()) b->enqueue(tB.dequeue());
    return eq;
}

// ---- New Task 2 Functions ----

// shiftRight: insert 0 at position pos (0-based), elements at pos and beyond move right.
// If queue is full the last element is dropped.
// Before: ( a -> b -> c -> d -> ), pos=1
// After:  ( a -> 0 -> b -> c -> d -> ) [if room]
void shiftRight(Queue * q, int pos)
{
    MyQueue temp(MAX_SIZE * 2);
    // Dequeue first pos elements into temp
    for(int i = 0; i < pos && !q->isEmpty(); i++)
        temp.enqueue(q->dequeue());
    // Enqueue 0 at this position
    temp.enqueue(0);
    // Enqueue remaining elements (drop last if full after rebuild)
    while(!q->isEmpty())
        temp.enqueue(q->dequeue());
    // Rebuild q from temp (up to maxCapacity)
    int count = 0;
    MyQueue restore(MAX_SIZE * 2);
    while(!temp.isEmpty()) { restore.enqueue(temp.dequeue()); count++; }
    int kept = 0;
    while(!restore.isEmpty())
    {
        int v = restore.dequeue();
        if(!q->isFull()) { q->enqueue(v); kept++; }
    }
}

// shiftLeft: remove element at position pos (0-based), elements after pos move left.
// Before: ( a -> b -> c -> d -> ), pos=1 => ( a -> c -> d -> )
void shiftLeft(Queue * q, int pos)
{
    MyQueue temp(MAX_SIZE * 2);
    // Dequeue first pos elements into temp
    for(int i = 0; i < pos && !q->isEmpty(); i++)
        temp.enqueue(q->dequeue());
    // Discard element at pos
    if(!q->isEmpty()) q->dequeue();
    // Keep remaining
    while(!q->isEmpty()) temp.enqueue(q->dequeue());
    // Rebuild
    while(!temp.isEmpty()) q->enqueue(temp.dequeue());
}

// insert(q, v, pos): insert value v at position pos (0-based)
// shiftRight first to make room, then put v at pos
void insert(Queue * q, int v, int pos)
{
    shiftRight(q, pos);
    // Now rebuild: place v at pos
    MyQueue temp(MAX_SIZE * 2);
    for(int i = 0; i < pos && !q->isEmpty(); i++)
        temp.enqueue(q->dequeue());
    // discard the 0 we placed
    if(!q->isEmpty()) q->dequeue();
    // place v
    temp.enqueue(v);
    // keep rest
    while(!q->isEmpty()) temp.enqueue(q->dequeue());
    while(!temp.isEmpty()) q->enqueue(temp.dequeue());
}

// insert(src, dst, pos): insert all of src into dst starting at position pos
// Reuses shiftRight and insert(q,v,pos) logic.
// src = ( x -> y -> ), dst = ( a -> b -> c -> ), pos=1
// => dst = ( a -> x -> y -> b -> c -> )
void insert(Queue * src, Queue * dst, int pos)
{
    int srcSize = getSize(src);
    // Collect src values
    MyQueue srcCopy(MAX_SIZE * 2);
    append(src, &srcCopy);

    // Make room in dst: shift right srcSize times from pos
    for(int k = 0; k < srcSize; k++)
        shiftRight(dst, pos + k);

    // Replace the 0s we inserted with actual src values
    // Rebuild dst element by element
    int dstSize = getSize(dst);
    int dstArr[MAX_SIZE * 4];
    int srcArr[MAX_SIZE * 2];
    int dc = 0, sc = 0;

    // Drain dst
    while(!dst->isEmpty()) dstArr[dc++] = dst->dequeue();
    // Drain srcCopy
    while(!srcCopy.isEmpty()) srcArr[sc++] = srcCopy.dequeue();

    // Replace 0s at pos..pos+srcSize-1 with src values
    for(int k = 0; k < sc && (pos+k) < dc; k++)
        dstArr[pos + k] = srcArr[k];

    // Refill dst
    for(int k = 0; k < dc && !dst->isFull(); k++)
        dst->enqueue(dstArr[k]);
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

    Queue* B = new MyQueue(MAX_SIZE);
    insert(B, -100, 2); // B is empty, pos=2 > size, just enqueue at end
    cout << "Queue B Values after insert -100 at pos 2: ";
    display(B, cout);

    insert(A, B, 2);
    cout << "Queue B Values after inserting A at pos 2: ";
    display(B, cout);

    shiftRight(B, 2);
    cout << "Queue B Values after shiftRight from pos 2: ";
    display(B, cout);

    shiftLeft(B, 2);
    cout << "Queue B Values after shiftLeft from pos 2: ";
    display(B, cout);

    delete A;
    delete B;
    return 0;
}
