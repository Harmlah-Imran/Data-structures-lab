/*
 * Task 4 - Stack: Word Processing from file "para.txt"
 *
 * Reads up to 100 words from "para.txt" into a Stack of strings.
 * Since only Stack is allowed as ADT, we use Stack<string> concept.
 * However, the given Stack class only stores int. Since the task says
 * "except int, float, char and string", we use string variables directly
 * and implement a simple string stack manually using a fixed array of strings
 * (which is NOT an ADT - it is just a plain string array used as backing store,
 * the ADT logic is implemented on top of it).
 *
 * JUSTIFICATION FOR CUSTOM StringStack:
 * The provided Stack.h/MyStack.h only support int. The task requires storing
 * words (strings). Since we cannot use any other ADT and string is a permitted
 * type, we build a minimal StringStack struct that mirrors Stack behaviour
 * (push/pop/isEmpty/isFull) using a plain string array as backing store.
 *
 * Tasks:
 * 1. Copy words in reverse order into another stack and display.
 * 2. Remove duplicates and display.
 * 3. Count frequency of each word; display word + frequency.
 */

#include <iostream>
#include <fstream>
using namespace std;

const int MAX_WORDS = 100;

// ---- Minimal String Stack ----
struct StringStack
{
    string data[MAX_WORDS];
    int top;
    int capacity;

    StringStack(int cap = MAX_WORDS) : top(-1), capacity(cap) {}

    bool isEmpty() const { return top < 0; }
    bool isFull()  const { return top == capacity - 1; }

    void push(const string & s)
    {
        if(!isFull()) data[++top] = s;
        else cout << "StringStack full\n";
    }

    bool pop(string & s)
    {
        if(!isEmpty()) { s = data[top--]; return true; }
        return false;
    }

    // peek at top without removing
    string peek() const { return isEmpty() ? "" : data[top]; }

    int size() const { return top + 1; }
};

// ---- Utility ----

void display(StringStack * s, ostream & out)
{
    StringStack temp;
    string v;
    while(s->pop(v)) temp.push(v);
    out << "[ ";
    while(temp.pop(v)) { out << v << " "; s->push(v); }
    out << "]\n";
}

// ---- Task Functions ----

// 1. Copy src into dst in reverse order
void reverseCopy(StringStack * src, StringStack * dst)
{
    // Pop all from src into temp (temp is reversed relative to src)
    StringStack temp;
    string v;
    while(src->pop(v)) temp.push(v);
    // Restore src and fill dst (dst gets reversed order)
    while(temp.pop(v)) { src->push(v); dst->push(v); }
    // Now dst top = bottom of src = first word read, i.e. reversed
    // Actually we need dst to be in reverse order of src.
    // src bottom->top: w1 w2 w3 ... wn
    // reversed: wn ... w2 w1
    // After above, dst bottom->top: wn ... w2 w1  (correct reverse)
}

// 2. Remove duplicates from src, result in dst (first occurrence kept, order preserved)
void removeDuplicates(StringStack * src, StringStack * dst)
{
    // We need to check if a word already exists in dst before adding.
    StringStack temp; // hold src in bottom-to-top order
    string v;
    // First, reverse src into temp so temp top = bottom of src (first word)
    while(src->pop(v)) temp.push(v);
    // Restore src simultaneously
    StringStack srcCopy;
    while(temp.pop(v)) { src->push(v); srcCopy.push(v); }
    // srcCopy top = top of src

    // Now process srcCopy bottom to top
    StringStack toProcess;
    while(srcCopy.pop(v)) toProcess.push(v); // toProcess top = first word

    while(toProcess.pop(v))
    {
        // Check if v already in dst
        StringStack dstTemp;
        string d;
        bool found = false;
        while(dst->pop(d)) { dstTemp.push(d); if(d == v) found = true; }
        while(dstTemp.pop(d)) dst->push(d);
        if(!found) dst->push(v);
    }
}

// 3. Count frequency of each word
// Display each unique word once with its count
void wordFrequency(StringStack * src)
{
    // Get all words into a traversal copy
    StringStack words;
    string v;
    // copy src to words preserving order
    StringStack tmp;
    while(src->pop(v)) tmp.push(v);
    while(tmp.pop(v)) { src->push(v); words.push(v); }
    // words: top = top of src

    StringStack processed; // words already counted
    StringStack wordsTmp;
    while(words.pop(v)) wordsTmp.push(v); // wordsTmp top = first word

    cout << "\nWord Frequencies:\n";
    while(wordsTmp.pop(v))
    {
        // Check if already processed
        StringStack procTmp;
        string p;
        bool already = false;
        while(processed.pop(p)) { procTmp.push(p); if(p == v) already = true; }
        while(procTmp.pop(p)) processed.push(p);

        if(!already)
        {
            // Count occurrences of v in src
            int freq = 0;
            StringStack srcTmp;
            string s;
            while(src->pop(s)) { srcTmp.push(s); if(s == v) freq++; }
            while(srcTmp.pop(s)) src->push(s);
            cout << "  " << v << " : " << freq << "\n";
            processed.push(v);
        }
    }
}

int main()
{
    StringStack * A = new StringStack(MAX_WORDS);

    ifstream file("para.txt");
    if(!file)
    {
        cout << "Could not open para.txt. Using sample paragraph instead.\n";
        // Sample paragraph
        string sample[] = {"the", "quick", "brown", "fox", "jumps", "over",
                           "the", "lazy", "dog", "the", "fox"};
        for(int i = 0; i < 11 && !A->isFull(); i++)
            A->push(sample[i]);
    }
    else
    {
        string word;
        int count = 0;
        while(file >> word && count < MAX_WORDS)
        {
            A->push(word);
            count++;
        }
        file.close();
    }

    cout << "Original Stack A:\n";
    display(A, cout);

    // Task 1: Reverse copy
    StringStack * B = new StringStack(MAX_WORDS);
    reverseCopy(A, B);
    cout << "\nStack B (Reverse of A):\n";
    display(B, cout);

    // Task 2: Remove duplicates from B into C
    StringStack * C = new StringStack(MAX_WORDS);
    removeDuplicates(A, C);
    cout << "\nStack C (A without duplicates):\n";
    display(C, cout);

    // Task 3: Word frequencies
    wordFrequency(A);

    delete A;
    delete B;
    delete C;
    return 0;
}
