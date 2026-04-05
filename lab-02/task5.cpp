/*
 * Task 5 - Binary Search using Templates
 *
 * Implements binarySearch<T>(arr, key) and printSearchResult(index, key)
 * using templates so it works for int, float, and string.
 *
 * The array size is determined by a template parameter N to avoid
 * needing a separate size argument while keeping the interface clean.
 *
 * binarySearch returns the index of the found element or -1 if not found.
 */

#include <iostream>
#include <string>
using namespace std;

// Binary search on a sorted array of type T
// N = number of elements (deduced from array argument)
template <typename T, int N>
int binarySearch(T (&arr)[N], T key)
{
    int low  = 0;
    int high = N - 1;

    while(low <= high)
    {
        int mid = (low + high) / 2;
        if(arr[mid] == key)
            return mid;
        else if(arr[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

// Print search result for any type T
template <typename T>
void printSearchResult(int index, T key)
{
    if(index == -1)
        cout << "Key " << key << " not found in array.\n";
    else
        cout << "Key " << key << " found at index " << index << ".\n";
}

int main()
{
    // Test with integer array (sorted) of size 5
    int intArray[5] = {11, 12, 22, 25, 64};
    int intKey = 22;
    int intIndex = binarySearch(intArray, intKey);
    printSearchResult(intIndex, intKey);

    // Test with float array (sorted) of size 4
    float floatArray[4] = {0.57f, 1.62f, 2.71f, 3.14f};
    float floatKey = 2.71f;
    int floatIndex = binarySearch(floatArray, floatKey);
    printSearchResult(floatIndex, floatKey);

    // Test with string array (sorted) of size 4
    string stringArray[4] = {"apple", "banana", "grape", "orange"};
    string stringKey = "grape";
    int stringIndex = binarySearch(stringArray, stringKey);
    printSearchResult(stringIndex, stringKey);

    // Additional tests: key not found
    int notFound = 99;
    int idx = binarySearch(intArray, notFound);
    printSearchResult(idx, notFound);

    return 0;
}
