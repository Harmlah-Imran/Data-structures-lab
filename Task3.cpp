#include <iostream>
using namespace std;
class Book {
    string Bookname;
    string authorName;
    int pages;
public:
    Book() {
        Bookname = "Unknown";
        authorName="Unknown";
       pages= 0;
    }
    Book(string book,string author,int noOfcopies) {
        Bookname = book;
        authorName = author;
        pages = noOfcopies;
    }
    void display() {
        cout << "Book name: " << Bookname << endl;
        cout << "Author: " << authorName << endl;
        cout << "No of copies: " << pages << endl;
    }
    int getpages() {
        return pages;
    }
    string gettitle() { return Bookname; }

};
class Newspaper {
    string publisher;
    string dateofpublishing;
    string edition;
public:
    Newspaper() {
        publisher="Unknown";
        dateofpublishing= "Unknown";
        edition= "Unknown";
    }
    Newspaper(string nameofpublisher,string date,string editionofnewspaper) {
        publisher = nameofpublisher;
        dateofpublishing = date;
        edition = editionofnewspaper;
    }
    void display() {
        cout << "Publisher name: " << publisher << endl;
        cout << "Date published: " << dateofpublishing << endl;
        cout << "Edition: " << edition << endl;
    }
    string getedition() { return edition; }
    string getName() { return publisher; }
};
class Library {
    Book books[100];
    Newspaper newspaper[100];
    int bookcount;
    int newspapercount;

public:
    Library() {
        bookcount=0;
        newspapercount = 0;
    }
    void addBook(Book b) {
        books[bookcount] = b;
        bookcount++;
    }
    void addNewspaper(Newspaper n){
        newspaper[newspapercount] = n;
        newspapercount++;
    }
    void displayCollection() {
        for (int i = 0; i < bookcount; i++) {
            books[i].display();
        }
        for (int i = 0; i < newspapercount; i++) {
            newspaper[i].display();
        }
    }
    void sortBooksByPages() {
        for (int i = 0; i < bookcount; i++) {
            for (int j = i+1; j < bookcount; j++) {
                if (books[i].getpages() > books[i + 1].getpages()) {
                    Book temp = books[i];
                    books[i] = books[j];
                    books[j] = temp;
                }
            }
        }


    }
    void sortNewspapersByEdition() {
        for (int i = 0; i < newspapercount; i++) {
            for (int j = i + 1; j < newspapercount; j++) {
                if (newspaper[i].getedition() > newspaper[i + 1].getedition()) {
                    Newspaper temp = newspaper[i];
                    newspaper[i] = newspaper[j];
                    newspaper[j] = temp;
                }
            }
        }
    }
    Book* searchBookByTitle(string title) {
        for (int i = 0; i < bookcount; i++) {
            if (books[i].gettitle() == title) {
              
                return &books[i];
            }
        }
    }
    Newspaper* searchNewspaperByName(string name) {
        for (int i = 0; i < bookcount; i++) {
            if (newspaper[i].getName() == name) {
              
                return &newspaper[i];
            }
        }
    }
  
};
int main() {
    // Create book objects
    Book book1("The Catcher in the Rye", "J.D. Salinger", 277);
    Book book2("To Kill a Mockingbird", "Harper Lee", 324);

    // Create newspaper objects
    Newspaper newspaper1("Washington Post", "2024-10-13", "Morning Edition");
    Newspaper newspaper2("The Times", "2024-10-12", "Weekend Edition");

    // Create a library object
    Library library;

    // Add books and newspapers to the library
    cout << "................." << endl;
    library.addBook(book1);
    library.addBook(book2);
    library.addNewspaper(newspaper1);
    library.addNewspaper(newspaper2);

    // Display the entire collection
    cout << "Before Sorting:\n";
    library.displayCollection();

    // Sort books by pages and newspapers by edition
    cout << "................." << endl;
    library.sortBooksByPages();
    library.sortNewspapersByEdition();

    cout << "\nAfter Sorting:\n";
    library.displayCollection();

    cout << "................." << endl;
    // Search for a book by title
    Book* foundBook = library.searchBookByTitle("The Catcher in the Rye");
    if (foundBook) {
        cout << "\nFound Book:\n";
        foundBook->display();
    }
    else {
        cout << "\nBook not found.\n";
    }
    cout << "................." << endl;
    // Search for a newspaper by name
    Newspaper* foundNewspaper = library.searchNewspaperByName("The Times");
    if (foundNewspaper) {
        cout << "\nFound Newspaper:\n";
        foundNewspaper->display();
    }
    else {
        cout << "\nNewspaper not found.\n";
    }



    return 0;
}
