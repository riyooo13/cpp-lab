/*
Define a class Library with data memberes ISBN, bookTitle, author, and
numberOfCopies. Implement methods to add new books, issue books, return books,
update book details, and delete books. The library's catalob should be maintained in a
file. Specifically, implement the following operations:

1.  Add New Book: Insert a new book's details into the catalog file. Ensure that each book
    has unique ISBN and that duplicate entries are not allowed.
2.  Issue Book: Mark a book as issued by decreasing the number of available copies in the
    catalog file. Ensure that a book cannot be issued if the number of available copies is
    zero.
3.  Return Book: Mark a book as returned by increasing the number of available copies in
    the catalog file.
4.  Update Book Details: Edi the details of a bookk, such as ISBN, bookTitle, author,
    or numberOfCopies, in the catalog file. Ensure that cahnging ISBN does not result in
    duplicates.
5.  Delete Book: Allow the user to delete a book's record from the catalog file only if the
    following conditions are met:
        -   The number of available copies is zero.
        -   The book has not been issued to any user, meaning allcopies have been returned and
            there are no outstanding issues for the book.

Ensure that the number of copies is appropriately updated for each operation and that file
operations maintain data integrity. Provide methods to read from and write to the file for
maintaining the catalog data.
*/

#include<iostream>
#include<fstream>
#include<conio.h>
#include<cstring>

using namespace std;

class Library{
    string ISBN;
    int numberOfCopies;
    char bookTitle[50], author[30];

    public:
        void getData(){
            cout << "Enter ISBN: ";
            cin >> ISBN;

            cout << "Enter title: ";
            cin.ignore();
            cin.getline(bookTitle, 50);

            cout << "Enter author name: ";
            cin.getline(author, 30);

            cout << "Enter the number of copies: ";
            cin >> numberOfCopies;
            cin.ignore();
        }

        void showData(){
            cout << "Book Information" << endl;
            cout << "ISBN: " << ISBN << endl;
            cout << "Title: " << bookTitle << endl;
            cout << "Author: " << author << endl;
            cout << "Number of copies: " << numberOfCopies << endl;
        }
        
        void addBook() {
            system("cls");
            cout << "Add New Book" << endl;
            getData();

            Library temp;
            bool match = false;
            ifstream inf("books.dat", ios::binary);

            if (!inf) {
                ofstream outf("books.dat", ios::binary);
                if (!outf) {
                    cerr << "Error creating file!" << endl;
                    return;
                }
                outf.write(reinterpret_cast<const char *>(this), sizeof(*this));
                outf.close();
                cout << "Book added successfully." << endl;
                return;
            }

            while (!inf.eof()) {
                inf.read(reinterpret_cast<char *>(&temp), sizeof(temp));
                if (temp.ISBN == ISBN) {
                    match = true;
                    cout << "ISBN is repeated." << endl;
                    break;
                }
            }
            inf.close();

            if (!match) {
                ofstream outf("books.dat", ios::app | ios::binary);
                if (!outf) {
                    cerr << "Error to open file!" << endl;
                    return;
                }
                outf.write(reinterpret_cast<const char *>(this), sizeof(*this));
                outf.close();
                cout << "Book added successfully!" << endl;
            }
        
        }

        void issueBook(){
            system("cls");
            string isbn;
            cout << "Enter the ISBN to issue: ";
            cin >> isbn;

            fstream file("books.dat", ios::binary | ios::in | ios::out);
            if (!file) {
                cerr << "Error opening file!" << endl;
                return;
            }

            bool found = false;
            while (file.read(reinterpret_cast<char*>(this), sizeof(*this))) {
                if (ISBN == isbn) {
                    found = true;
                    if (numberOfCopies > 0) {
                        numberOfCopies--;
                        file.seekp(-static_cast<int>(sizeof(*this)), ios::cur);
                        file.write(reinterpret_cast<char*>(this), sizeof(*this));
                        cout << bookTitle << " issued." << endl;
                    } else {
                        cout << "Sorry, this book is unavailable." << endl;
                    }
                    break;
                }
            }

            if (!found) {
                cout << "ISBN " << isbn << " not found!" << endl;
            }

            file.close();
        }

        void returnBook(){
            system("cls");
            string isbn;
            cout << "Enter the ISBN to return: ";
            cin >> isbn;

            fstream file("books.dat", ios::in|ios::out|ios::binary);
            
            bool found = false;
            while (file.read(reinterpret_cast<char*>(this), sizeof(*this))) {
                if (ISBN == isbn) {
                    found = true;
                    file.seekp(-static_cast<int>(sizeof(*this)), ios::cur);
                    file.write(reinterpret_cast<char*>(this), sizeof(*this));
                    cout << bookTitle << " returned." << endl;
                    break;
                }
            }

            if (!found) {
                cout << "Book with ISBN " << isbn << " not found." << endl;
            }

            file.close();
        }

        void updateBook(){
            system("cls");
            string isbn;
            cout << "Enter the ISBN to update: ";
            cin >> isbn;
            cin.ignore();

            fstream file("books.dat", ios::binary | ios::in | ios::out);
            if (!file) {
                cerr << "Error opening file." << endl;
                return;
            }

            bool found = false;
            while (file.read(reinterpret_cast<char *>(this), sizeof(*this))) {
                if (ISBN == isbn) {
                    found = true;
                    cout << "Update details:" << endl;
                    getData();
                    file.seekp(-static_cast<int>(sizeof(*this)), ios::cur);
                    file.write(reinterpret_cast<char *>(this), sizeof(*this));
                    cout << "Book updated." << endl;
                    break;
                }
            }

            if (!found) {
                cout << "Book with " << isbn << " not found." << endl;
            }

            file.close();
        }

        void deleteBook(){
            system("cls");
            string isbn;
            cout << "Enter the ISBN to delete: ";
            cin >> isbn;
            cin.ignore();

            ifstream inf("books.dat", ios::binary);
            if (!inf) {
                cerr << "Error to open file." << endl;
                return;
            }

            ofstream outf("temp.dat", ios::binary);
            if (!outf) {
                cerr << "Error creating temporary file." << endl;
                inf.close();
                return;
            }

            bool found = false;
            while (inf.read(reinterpret_cast<char *>(this), sizeof(*this))) {
                if (ISBN != isbn) {
                    outf.write(reinterpret_cast<const char *>(this), sizeof(*this));
                } else {
                    found = true;
                }
            }

            inf.close();
            outf.close();

            if (found) {
                remove("books.dat");
                rename("temp.dat", "books.dat");
                cout << "Book deleted." << endl;
            } else {
                cout << "ISBN " << isbn << " not found." << endl;
                remove("temp.dat");
            }
        }

        void listBooks() const {
            system("cls");
            cout << "BOOKS" << endl << endl;
            ifstream inf("books.dat", ios::binary | ios::in);
            if (!inf) {
                cerr << "Error opening file!" << endl;
                return;
            }

            Library temp;
            while (inf.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
                temp.showData();
                cout << endl;
            }

            inf.close();
            cout << "End" << endl;
        }

};

int main(){
    char ch;
    Library lib;

    while(true){
        system("cls");
        cout<<"\t\t\t\t\t\t\tBOOKS\n";
        cout<<"\t\t\t\t\t\t1.Add Book\n";
        cout<<"\t\t\t\t\t\t2.Issue Book\n";
        cout<<"\t\t\t\t\t\t3.Return Book\n";
        cout<<"\t\t\t\t\t\t4.Update Book\n";
        cout<<"\t\t\t\t\t\t5.Delete Book\n";
        cout<<"\t\t\t\t\t\t6.Booklist\n";
        cout<<"\t\t\t\t\t\t7.Exit\n";
        cout<<"Enter your choice: ";
        cin >> ch;

        switch(ch){
            case '1':
                lib.addBook();
                break;
            
            case '2':
                lib.issueBook();
                break;
            
            case '3':
                lib.returnBook();
                break;
            
            case '4':
                lib.updateBook();
                break;
            
            case '5':
                lib.deleteBook();
                break;
            
            case '6':
                lib.listBooks();
                break;

            case '7':
                exit(0);
            
            default:
                cout << "Invalid.";
        }

        cout << endl << "Press any key to continue:";
        cin.ignore();
        getch();
    }
}