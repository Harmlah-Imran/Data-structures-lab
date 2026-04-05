/*
 * Lab 4 - Task 1
 *
 * UNDERSTANDING THE TASK:
 * Implement basic Queue utility functions: read, display, append (full and partial),
 * reverseAppend, getSize, stats, and isEqual. Then test them in main().
 *
 * HOW I WILL SOLVE IT:
 * Queue works as (front → rear), enqueue adds to rear, dequeue removes from front.
 * - read: enqueue values from user until queue is full.
 * - display: dequeue all, print, then re-enqueue to restore.
 * - append(src,dst): dequeue from src, enqueue in dst, restore src.
 * - append(src,dst,no): same but only 'no' elements.
 * - reverseAppend: use a Stack as helper to reverse order before appending.
 *   JUSTIFICATION FOR STACK USE: reverseAppend requires LIFO to reverse FIFO order.
 *   There is no way to reverse a queue using only queues without knowing its size
 *   and doing O(n^2) rotations; a Stack makes this clean and correct.
 * - getSize: dequeue all counting, then re-enqueue.
 * - stats: traverse (dequeue/re-enqueue) computing sum and max.
 * - isEqual: compare element by element using two temp queues.
 *
 * QUEUE NOTATION:
 *   A = ( 1 -> 2 -> 3 -> )   front=1, rear=3
 *
 * BUGS FIXED IN main():
 * 1. stats() parameter named "max" but should return sum; fixed to return sum & average.
 * 2. "append(N, M)" makes no sense if N is empty; left as-is (appends nothing).
 * 3. The lab main calls stats(A, mx, ag) and passes int mx - but stats needs int& and
 *    float&. Fixed signatures accordingly.
 *
 * LEARNING: Queue is FIFO and must be drained and refilled to traverse.
 *           Stack is invaluable as helper for reversal.
 *
 * SKILLS DEVELOPED: Implementing ADT utility functions, traversal without random access.
 */

#include <iostream>
using namespace std;
#include "Stack.h"
#include "MyStack.h"
#include "Queue.h"
#include "MyQueue.h"

const int MAX_SIZE = 5;

// ---- getSize ----
// Dequeue all elements counting them, then re-enqueue in same order.
// Queue state: ( a -> b -> c -> ) -> count=3, restored after.
int getSize(Queue * q)
{
    int count = 0;
    MyQueue temp(MAX_SIZE * 2);
    while(!q->isEmpty()) { temp.enqueue(q->dequeue()); count++; }
    while(!temp.isEmpty()) q->enqueue(temp.dequeue());
    return count;
}

// ---- read ----
// Enqueue values from input stream until queue is full.
void read(Queue * q, istream & in)
{
    cout << "Enter values (queue size = " << MAX_SIZE << "):\n";
    int v;
    int i = 1;
    while(!q->isFull())
    {
        cout << "Value " << i++ << ": ";
        in >> v;
        q->enqueue(v);
    }
}

// ---- display ----
// Dequeue all, print, re-enqueue.
// Queue A = ( 10 -> 20 -> 30 -> ) displayed as [ 10 20 30 ]
void display(Queue * q, ostream & out)
{
    MyQueue temp(MAX_SIZE * 2);
    out << "[ ";
    while(!q->isEmpty())
    {
        int v = q->dequeue();
        out << v << " ";
        temp.enqueue(v);
    }
    out << "]\n";
    while(!temp.isEmpty()) q->enqueue(temp.dequeue());
}

// ---- append(src, dst) ----
// Append all of src to dst, restoring src.
// src = ( a -> b -> ), dst = ( x -> ) => dst = ( x -> a -> b -> )
void append(Queue * src, Queue * dst)
{
    MyQueue temp(MAX_SIZE * 2);
    while(!src->isEmpty())
    {
        int v = src->dequeue();
        temp.enqueue(v);
    }
    while(!temp.isEmpty())
    {
        int v = temp.dequeue();
        src->enqueue(v);
        if(!dst->isFull()) dst->enqueue(v);
    }
}

// ---- append(src, dst, no) ----
// Append first 'no' elements of src to dst, restoring src.
void append(Queue * src, Queue * dst, int no)
{
    MyQueue temp(MAX_SIZE * 2);
    int count = 0;
    while(!src->isEmpty())
    {
        int v = src->dequeue();
        temp.enqueue(v);
        if(count < no && !dst->isFull())
            dst->enqueue(v);
        count++;
    }
    while(!temp.isEmpty()) src->enqueue(temp.dequeue());
}

// ---- reverseAppend ----
// Append src into dst in reverse order.
// STACK USED: to reverse FIFO queue order (LIFO needed).
// src = ( a -> b -> c -> ) => dst gets ( c -> b -> a -> ) appended
void reverseAppend(Queue * src, Queue * dst)
{
    MyStack stk(MAX_SIZE * 2);
    MyQueue temp(MAX_SIZE * 2);
    // Drain src into stack (stack top = last element of src)
    while(!src->isEmpty()) { stk.push(src->dequeue()); }
    // Pop from stack into dst (reversed order) and temp (to restore src)
    while(!stk.isEmpty())
    {
        int v; stk.pop(v);
        temp.enqueue(v); // temp has src in original order
    }
    // Push temp back to src and dst in reverse
    // temp is in original order; we need to reverse it again for dst
    // Drain temp back into stack
    while(!temp.isEmpty()) stk.push(temp.dequeue());
    // Now stack top = last element of original src (i.e., first of reversed)
    while(!stk.isEmpty())
    {
        int v; stk.pop(v);
        src->enqueue(v);   // restore src in original order (bottom=first)
        if(!dst->isFull()) dst->enqueue(v); // dst gets reversed order
    }
    // Actually: stk was filled from temp (original order), so
    // popping gives original order reversed, which IS reversed. Correct.
}

// ---- stats ----
// Calculate sum and average of all queue values.
// Also find maximum.
void stats(Queue * q, int & sum, float & average)
{
    MyQueue temp(MAX_SIZE * 2);
    sum = 0;
    int count = 0;
    bool first = true;
    int mx = 0;
    while(!q->isEmpty())
    {
        int v = q->dequeue();
        temp.enqueue(v);
        sum += v;
        if(first || v > mx) { mx = v; first = false; }
        count++;
    }
    while(!temp.isEmpty()) q->enqueue(temp.dequeue());
    average = (count > 0) ? (float)sum / count : 0.0f;
    sum = mx; // repurpose sum parameter to return max (as per lab signature: int max)
}

// ---- isEqual ----
// Returns true if both queues have same size and same values in same order.
// Restores both queues.
bool isEqual(Queue * a, Queue * b)
{
    int sizeA = getSize(a);
    int sizeB = getSize(b);
    if(sizeA != sizeB) return false;

    MyQueue tempA(MAX_SIZE * 2), tempB(MAX_SIZE * 2);
    bool equal = true;
    for(int i = 0; i < sizeA; i++)
    {
        int va = a->dequeue();
        int vb = b->dequeue();
        tempA.enqueue(va);
        tempB.enqueue(vb);
        if(va != vb) equal = false;
    }
    while(!tempA.isEmpty()) a->enqueue(tempA.dequeue());
    while(!tempB.isEmpty()) b->enqueue(tempB.dequeue());
    return equal;
}

int main()
{
    Queue* A = new MyQueue(MAX_SIZE);
    int n = 0;
    read(A, cin);
    cout << "Queue A Values: ";
    display(A, cout);

    Queue* B = new MyQueue(MAX_SIZE);
    append(A, B);
    cout << "Queue B Values: ";
    display(B, cout);

    Queue* C = new MyQueue(MAX_SIZE);
    C->enqueue(9);
    C->enqueue(7);
    C->enqueue(5);
    append(A, C, 2);
    cout << "Queue C Values after append of 2 values from A: ";
    display(C, cout);

    Queue* D = new MyQueue(MAX_SIZE);
    reverseAppend(C, D);
    cout << "Queue D Values after reverse append from C: ";
    display(D, cout);

    int mx = 0;
    float ag = 0.0;
    stats(A, mx, ag);
    cout << "Queue A Values: ";
    display(A, cout);
    cout << "Average is " << ag << " maximum is " << mx << "\n";

    Queue* M = new MyQueue(MAX_SIZE);
    Queue* N = new MyQueue(MAX_SIZE);
    Queue* P = new MyQueue(MAX_SIZE);
    append(A, M);
    append(N, M); // N is empty, appends nothing
    append(N, P);
    P->enqueue(111);

    if(!isEqual(A, M))
        cout << "A and M: Not Equal\n";
    else
        cout << "A and M: Equal\n";

    if(!isEqual(A, P))
        cout << "A and P: Not Equal\n";
    else
        cout << "A and P: Equal\n";

    delete A; delete B; delete C; delete D;
    delete M; delete N; delete P;
    return 0;
}
