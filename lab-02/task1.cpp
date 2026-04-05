/*
 * Task 1 - AbstractArray Functions
 *
 * BUGS FIXED IN main():
 * 1. "Void" -> "void" in display prototype (capital V is not valid C++)
 * 2. AbstractArray is abstract (pure virtual), so "new AbstractArray(...)" is
 *    illegal. Changed to "new MyArray(...)" throughout main.
 * 3. "C.add(...)" -> "C->add(...)" because C is a pointer.
 * 4. read() prototype had 2 params but call had 3 (passing n). Fixed read to
 *    accept a count parameter.
 * 5. get() and insert() in MyArray used "index > 0" which skips index 0.
 *    Fixed in MyArray.h to "index >= 0".
 */

#include <iostream>
using namespace std;
#include "AbstractArray.h"
#include "MyArray.h"

const int MAX_SIZE = 10;

// reads n integers from the user (1 <= n <= MAX_SIZE)
void read(AbstractArray * aa, istream & in, int n)
{
    for(int i = 0; i < n && !aa->isFull(); i++)
    {
        int v;
        cout << "Enter value " << (i+1) << ": ";
        in >> v;
        aa->add(v);
    }
}

// displays all values in the AbstractArray from index 0 to currentIndex
// Strategy: use get() with indices 0..currentIndex
// We don't know currentIndex directly, so we pop all into a temp stack,
// print, then restore.
// Actually since get() is available, we probe indices until get() fails.
void display(AbstractArray * aa, ostream & out)
{
    // We access via get(); iterate index 0 upward until get() fails
    out << "[ ";
    int v;
    int i = 0;
    while(aa->get(i, v))
    {
        out << v << " ";
        i++;
    }
    out << "]\n";
}

// copy src to dst (dst must have enough capacity)
// Method: use get() on src to read each element, add() to dst
void copy(AbstractArray * src, AbstractArray * dst)
{
    int v;
    int i = 0;
    while(src->get(i, v))
    {
        dst->add(v);
        i++;
    }
}

// shift left from pos: elements from pos+1 onward move one step left (pos is overwritten)
// pos is 0-based
void shiftLeft(AbstractArray * aa, int pos)
{
    int curr, next;
    int i = pos;
    while(true)
    {
        // check if i+1 exists
        if(!aa->get(i+1, next)) break;
        aa->insert(i, next);
        i++;
    }
    // remove the last element (it was duplicated)
    aa->remove(curr);
}

// shift right from pos: elements from pos onward move one step right
// The last element is lost if array is full; a 0 is placed at pos
void shiftRight(AbstractArray * aa, int pos)
{
    // We need to shift elements right starting from pos.
    // First find the size by probing
    int size = 0;
    int tmp;
    while(aa->get(size, tmp)) size++;

    // Add a dummy element to make room (we'll place shifted value)
    // Actually we just add a 0 at the end if not full, then shift
    if(!aa->isFull())
    {
        aa->add(0); // extend by one
        size++;
    }

    // Shift from right: move elements right one step from pos
    for(int i = size - 1; i > pos; i--)
    {
        int val;
        aa->get(i-1, val);
        aa->insert(i, val);
    }
    // Place 0 at pos
    aa->insert(pos, 0);
}

// insert src into dst at position pos (0-based)
// Elements of dst at pos onward shift right, then src elements are inserted
void insert(AbstractArray * src, AbstractArray * dst, int pos)
{
    // Count src size
    int srcSize = 0;
    int tmp;
    while(src->get(srcSize, tmp)) srcSize++;

    // Shift dst right srcSize times from pos
    for(int k = 0; k < srcSize; k++)
        shiftRight(dst, pos + k);

    // Insert src values at pos, pos+1, ...
    for(int k = 0; k < srcSize; k++)
    {
        src->get(k, tmp);
        dst->insert(pos + k, tmp);
    }
}

// calculate maximum and average value of AA
// Uses get() to traverse; restores array untouched
void stats(AbstractArray * aa, int & max, float & average)
{
    int v;
    int i = 0;
    int sum = 0;
    int count = 0;
    bool first = true;

    while(aa->get(i, v))
    {
        if(first || v > max)
        {
            max = v;
            first = false;
        }
        sum += v;
        count++;
        i++;
    }
    average = (count > 0) ? (float)sum / count : 0.0f;
}

int main()
{
    AbstractArray * A = new MyArray(MAX_SIZE);
    int n = 0;
    cout << "Please select a number between 1 and " << MAX_SIZE << ": ";
    cin >> n;
    if(n < 1) n = 1;
    if(n > MAX_SIZE) n = MAX_SIZE;
    read(A, cin, n);
    cout << "Array A Values: ";
    display(A, cout);

    AbstractArray * B = new MyArray(MAX_SIZE);
    copy(A, B);
    cout << "Array B Values: ";
    display(B, cout);

    AbstractArray * C = new MyArray(MAX_SIZE);
    C->add(9);
    C->add(7);
    C->add(5);
    insert(A, C, 2);
    cout << "Array C Values: ";
    display(C, cout);

    shiftLeft(C, 2);
    cout << "Array C after shiftLeft from pos 2: ";
    display(C, cout);

    shiftRight(C, 2);
    cout << "Array C after shiftRight from pos 2: ";
    display(C, cout);

    int mx = 0;
    float ag = 0.0;
    stats(C, mx, ag);
    cout << "Array C Values: ";
    display(C, cout);
    cout << "Average is " << ag << " maximum is " << mx << endl;

    delete A;
    delete B;
    delete C;
    return 0;
}
