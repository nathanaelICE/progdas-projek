#ifndef LIB_HPP
#define LIB_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

// Book structure
struct Book {
    string title;
    string description;
    double rating;
    bool available;
};

// Borrowed Book structure
struct BorrowedBook {
    string title;
    string borrowDate;
};

// LibrarySystem class
class LibrarySystem {
private:
    map<string, string> users; // username, password
    string loggedInUser;
    map<string, vector<Book>> genres;
    vector<BorrowedBook> borrowedBooks;

    void loadUsers();
    void saveUsers();
    void loadBorrowedBooks();
    void saveBorrowedBooks();
    void mainMenu();
    void registerUser();
    void login();
    void logout();
    void viewGenres();
    void viewBooks(const string& genre);
    void previewBook(Book& book);
    void viewBorrowedBooks();
    string getCurrentDate();

public:
    LibrarySystem();
    void run();
};

#endif
