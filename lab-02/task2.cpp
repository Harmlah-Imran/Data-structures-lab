/*
 * Task 2 - Stack Functions
 *
 * BUGS FIXED IN main():
 * 1. "Void" -> "void" in display prototype.
 * 2. Stack is abstract, so "new Stack(...)" is illegal. Changed to "new MyStack(...)".
 * 3. "C.push(...)" -> "C->push(...)" because C is a pointer.
 * 4. shiftLeft/shiftRight called in main but the task uses "shiftUp"/"shiftDown".
 *    The main uses shiftLeft/shiftRight - kept those names for consistency with main.
 * 5. read() given only 2 params in prototype but we need a count; added count param.
 *
 * STACK CONSTRAINT NOTE:
 * Stack only has push() and pop(). To "traverse" without losing data we must
 * pop everything out, work on it, then push back. A second temporary stack
 * is used as auxiliary storage wherever needed.
 */

#include <iostream>
using namespace std;
#include "Stack.h"
#include "MyStack.h"

const int MAX_SIZE = 10;

// reads n integers from user into stack (bottom-to-top order)
void read(Stack * s, istream & in, int n)
{
    for(int i = 0; i < n && !s->isFull(); i++)
    {
        int v;
        cout << "Enter value " << (i+1) << ": ";
        in >> v;
        s->push(v);
    }
}

// display: pop all to temp, print while restoring to src
// Order displayed: bottom to top (insertion order)
void display(Stack * s, ostream & out)
{
    MyStack temp(MAX_SIZE);
    int v;
    // pop all to temp (temp is reversed)
    while(s->pop(v))
        temp.push(v);
    out << "[ ";
    // pop from temp = bottom to top of original
    while(temp.pop(v))
    {
        out << v << " ";
        s->push(v); // restore to s in original order
    }
    out << "]\n";
}

// returns size of stack without destroying it
int getSize(Stack * s)
{
    MyStack temp(MAX_SIZE);
    int v, count = 0;
    while(s->pop(v)) { temp.push(v); count++; }
    while(temp.pop(v)) s->push(v);
    return count;
}

// copy src to dst (dst gets same bottom-to-top order as src)
void copy(Stack * src, Stack * dst)
{
    MyStack temp(MAX_SIZE);
    int v;
    while(src->pop(v)) temp.push(v);
    while(temp.pop(v)) { src->push(v); dst->push(v); }
}

// shiftUp (like shiftLeft): remove element at pos (0-based from bottom),
// shift elements above it down by one
void shiftUp(Stack * s, int pos)
{
    int size = getSize(s);
    MyStack temp(MAX_SIZE);
    int v;

    // pop (size - pos) elements into temp
    for(int i = 0; i < size - pos; i++) { s->pop(v); temp.push(v); }

    // discard the element at pos (top of s right now)
    s->pop(v); // discarded

    // push temp back
    while(temp.pop(v)) s->push(v);
}

// shiftDown (like shiftRight): insert 0 at pos (0-based from bottom),
// push everything above one slot up; top element is lost if full
void shiftDown(Stack * s, int pos)
{
    int size = getSize(s);
    MyStack temp(MAX_SIZE);
    int v;

    // pop elements above pos into temp
    for(int i = 0; i < size - pos; i++) { s->pop(v); temp.push(v); }

    // push 0 at pos
    s->push(0);

    // restore temp (if not full)
    while(temp.pop(v) && !s->isFull()) s->push(v);
}

// insert src stack into dst at position pos (0-based from bottom)
void insert(Stack * src, Stack * dst, int pos)
{
    int srcSize = getSize(src);
    // shift dst right srcSize times from pos
    for(int k = 0; k < srcSize; k++)
        shiftDown(dst, pos + k);

    // Now insert src values at pos, pos+1, ...
    // We need src bottom-to-top values in order
    MyStack temp(MAX_SIZE);
    int v;
    // reverse src into temp
    while(src->pop(v)) temp.push(v);
    // restore src; collect in-order into an array-like temp2
    MyStack temp2(MAX_SIZE);
    while(temp.pop(v)) { src->push(v); temp2.push(v); }
    // temp2 now has src in reversed order (top = bottom of src)

    // We need to write values at positions pos, pos+1, ...
    // Use shiftUp at each position to remove the 0 we placed, then insert correct value
    // Simpler: use the get/insert pattern via full stack rebuild
    int srcArr[MAX_SIZE];
    int cnt = 0;
    while(temp2.pop(v)) srcArr[cnt++] = v; // srcArr[0] = bottom of src

    // Rebuild dst with src values inserted
    int dstSize = getSize(dst);
    int dstArr[MAX_SIZE * 2];
    int dstCnt = 0;
    // pop all dst
    MyStack dtmp(MAX_SIZE * 2);
    while(dst->pop(v)) dtmp.push(v);
    while(dtmp.pop(v)) dstArr[dstCnt++] = v; // bottom to top

    // Replace zeros at pos..pos+srcSize-1 with src values
    for(int k = 0; k < srcSize && (pos+k) < dstCnt; k++)
        dstArr[pos + k] = srcArr[k];

    // Push back into dst
    for(int k = 0; k < dstCnt; k++) dst->push(dstArr[k]);
}

// stats: find max and average
void stats(Stack * s, int & max, float & average)
{
    MyStack temp(MAX_SIZE);
    int v;
    int sum = 0, count = 0;
    bool first = true;
    while(s->pop(v))
    {
        temp.push(v);
        if(first || v > max) { max = v; first = false; }
        sum += v;
        count++;
    }
    while(temp.pop(v)) s->push(v);
    average = (count > 0) ? (float)sum / count : 0.0f;
}

int main()
{
    Stack * A = new MyStack(MAX_SIZE);
    int n = 0;
    cout << "Please select a number between 1 and " << MAX_SIZE << ": ";
    cin >> n;
    if(n < 1) n = 1;
    if(n > MAX_SIZE) n = MAX_SIZE;
    read(A, cin, n);
    cout << "Stack A Values: ";
    display(A, cout);

    Stack * B = new MyStack(MAX_SIZE);
    copy(A, B);
    cout << "Stack B Values: ";
    display(B, cout);

    Stack * C = new MyStack(MAX_SIZE);
    C->push(9);
    C->push(7);
    C->push(5);
    insert(A, C, 2);
    cout << "Stack C Values after insert of A at pos 2: ";
    display(C, cout);

    shiftUp(C, 2);
    cout << "Stack C after shiftUp from pos 2: ";
    display(C, cout);

    shiftDown(C, 2);
    cout << "Stack C after shiftDown from pos 2: ";
    display(C, cout);

    int mx = 0;
    float ag = 0.0;
    stats(C, mx, ag);
    cout << "Stack C Values: ";
    display(C, cout);
    cout << "Average is " << ag << " maximum is " << mx << endl;

    delete A;
    delete B;
    delete C;
    return 0;
}
