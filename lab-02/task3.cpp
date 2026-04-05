/*
 * Task 3 - Stack: Prime, Perfect Square, Palindrome checks
 *
 * Reads 10 integers into a Stack. For each value checks:
 *   1. Is it Prime?
 *   2. Is it a Perfect Square?
 *   3. Is it a Palindrome (digit-wise)?
 * Then checks if the entire sequence of values forms a Palindrome.
 *
 * Only Stack is used as data structure (no arrays, vectors, etc.)
 * Reuses display(), getSize(), copy() from Task 2 logic.
 */

#include <iostream>
using namespace std;
#include "Stack.h"
#include "MyStack.h"

const int MAX_SIZE = 10;

// ---- Utility functions ----

int getSize(Stack * s)
{
    MyStack temp(MAX_SIZE);
    int v, count = 0;
    while(s->pop(v)) { temp.push(v); count++; }
    while(temp.pop(v)) s->push(v);
    return count;
}

void display(Stack * s, ostream & out)
{
    MyStack temp(MAX_SIZE);
    int v;
    while(s->pop(v)) temp.push(v);
    out << "[ ";
    while(temp.pop(v)) { out << v << " "; s->push(v); }
    out << "]\n";
}

void copy(Stack * src, Stack * dst)
{
    MyStack temp(MAX_SIZE);
    int v;
    while(src->pop(v)) temp.push(v);
    while(temp.pop(v)) { src->push(v); dst->push(v); }
}

// get value at position pos (0-based from bottom) without destroying stack
int getAt(Stack * s, int pos)
{
    int size = getSize(s);
    MyStack temp(MAX_SIZE);
    int v = 0;
    // pop (size - pos - 1) elements to reach pos from top
    int popCount = size - pos - 1;
    for(int i = 0; i < popCount; i++) { s->pop(v); temp.push(v); }
    s->pop(v); // this is the value at pos
    int result = v;
    s->push(v); // put it back
    while(temp.pop(v)) s->push(v);
    return result;
}

// ---- Check functions ----

// isPrime: checks if n is prime (n must be > 1)
bool isPrime(int n)
{
    if(n < 2) return false;
    if(n == 2) return true;
    if(n % 2 == 0) return false;
    for(int i = 3; i * i <= n; i += 2)
        if(n % i == 0) return false;
    return true;
}

// isPerfectSquare: checks if n is a perfect square (n >= 0)
bool isPerfectSquare(int n)
{
    if(n < 0) return false;
    int i = 0;
    while(i * i < n) i++;
    return (i * i == n);
}

// isPalindromeNumber: checks if digits of n form a palindrome
// e.g. 121 -> palindrome, 123 -> not
bool isPalindromeNumber(int n)
{
    if(n < 0) n = -n; // treat negative as positive for digit check
    // Store digits in a stack
    MyStack digits(20);
    int temp = n;
    if(temp == 0) { digits.push(0); }
    while(temp > 0)
    {
        digits.push(temp % 10);
        temp /= 10;
    }
    int size = getSize(&digits);
    // Compare first half with second half using two stacks
    // Copy digits to another stack to get reversed order
    MyStack rev(20);
    copy(&digits, &rev);

    // Compare: digits has top=last digit, rev has top=last digit too (same)
    // We need first digit vs last digit etc.
    // digits stack: top = first digit (most significant)
    // rev stack: top = first digit too (copy preserves order)
    // To get reversed: pop all rev into another
    MyStack revRev(20);
    int v;
    while(rev.pop(v)) revRev.push(v); // revRev top = last digit

    // Now compare digits (top=first) with revRev (top=last)
    for(int i = 0; i < size / 2; i++)
    {
        int d1, d2;
        digits.pop(d1);
        revRev.pop(d2);
        if(d1 != d2) return false;
    }
    return true;
}

// Check if the sequence stored in a Stack is a Palindrome
// (same values reading bottom-to-top as top-to-bottom)
bool isSequencePalindrome(Stack * s)
{
    int size = getSize(s);
    for(int i = 0; i < size / 2; i++)
    {
        int left  = getAt(s, i);
        int right = getAt(s, size - 1 - i);
        if(left != right) return false;
    }
    return true;
}

// Print check for single value
void checkValue(int v)
{
    cout << "Value: " << v << "\n";
    cout << "  Prime:          " << (isPrime(v) ? "Yes" : "No") << "\n";
    cout << "  Perfect Square: " << (isPerfectSquare(v) ? "Yes" : "No") << "\n";
    cout << "  Palindrome:     " << (isPalindromeNumber(v) ? "Yes" : "No") << "\n";
}

int main()
{
    Stack * A = new MyStack(MAX_SIZE);

    cout << "Enter 10 integers:\n";
    for(int i = 0; i < MAX_SIZE; i++)
    {
        int v;
        cout << "Value " << (i+1) << ": ";
        cin >> v;
        A->push(v);
    }

    cout << "\nStack A Values: ";
    display(A, cout);

    cout << "\n--- Individual Value Checks ---\n";
    // Traverse bottom to top using getAt
    int size = getSize(A);
    for(int i = 0; i < size; i++)
    {
        int v = getAt(A, i);
        checkValue(v);
    }

    cout << "\n--- Sequence Palindrome Check ---\n";
    if(isSequencePalindrome(A))
        cout << "The stack sequence IS a Palindrome.\n";
    else
        cout << "The stack sequence is NOT a Palindrome.\n";

    // Demo with known palindrome sequence
    cout << "\n--- Demo: Known Palindrome Sequence [123 456 539 456 123] ---\n";
    Stack * demo = new MyStack(MAX_SIZE);
    demo->push(123);
    demo->push(456);
    demo->push(539);
    demo->push(456);
    demo->push(123);
    display(demo, cout);
    cout << (isSequencePalindrome(demo) ? "IS a Palindrome\n" : "NOT a Palindrome\n");

    delete A;
    delete demo;
    return 0;
}
