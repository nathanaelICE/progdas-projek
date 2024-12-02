#include "lib.hpp"
#include <fstream>
#include <ctime>

LibrarySystem::LibrarySystem() : loggedInUser("") {
    genres = {
        {"Romance", {
            {"Pride and Prejudice", "A classic romance novel.", 4.7, true},
            {"The Notebook", "A heartfelt love story.", 4.6, true}
        }},
        {"Smutt", {
            {"Fifty Shades of Grey", "A steamy romance novel.", 4.1, true},
            {"After", "A passionate and emotional story.", 4.3, true}
        }}
    };
    loadUsers();
    loadBorrowedBooks();
}

void LibrarySystem::run() {
    mainMenu();
}

void LibrarySystem::mainMenu() {
    int choice;
    cout << "=== Welcome to the Library System ===\n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "3. Logout\n";
    cout << "4. View Genres\n";
    cout << "5. View Borrowed Book List\n";
    cout << "6. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1: registerUser(); break;
        case 2: login(); break;
        case 3: logout(); break;
        case 4: viewGenres(); break;
        case 5: viewBorrowedBooks(); break;
        case 6: saveUsers(); saveBorrowedBooks(); exit(0);
        default:
            cout << "Invalid choice. Try again.\n";
            mainMenu();
    }
}

void LibrarySystem::registerUser() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (users.find(username) != users.end()) {
        cout << "Username already exists.\n";
    } else {
        users[username] = password;
        cout << "Registration successful!\n";
    }
    mainMenu();
}

void LibrarySystem::login() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (users.find(username) != users.end() && users[username] == password) {
        loggedInUser = username;
        cout << "Login successful!\n";
    } else {
        cout << "Invalid username or password.\n";
    }
    mainMenu();
}

void LibrarySystem::logout() {
    if (loggedInUser.empty()) {
        cout << "No user logged in.\n";
    } else {
        loggedInUser = "";
        cout << "Logged out successfully.\n";
    }
    mainMenu();
}

void LibrarySystem::viewGenres() {
    if (loggedInUser.empty()) {
        cout << "Please log in first.\n";
        mainMenu();
        return;
    }

    int genreChoice;
    cout << "Select a genre:\n";
    cout << "1. Romance\n";
    cout << "2. Smutt\n";
    cout << "Enter your choice: ";
    cin >> genreChoice;

    if (genreChoice == 1) {
        viewBooks("Romance");
    } else if (genreChoice == 2) {
        viewBooks("Smutt");
    } else {
        cout << "Invalid choice. Returning to main menu.\n";
        mainMenu();
    }
}

void LibrarySystem::viewBooks(const string& genre) {
    if (genres.find(genre) == genres.end()) {
        cout << "No books available in this genre.\n";
        mainMenu();
        return;
    }

    auto& books = genres[genre];
    for (size_t i = 0; i < books.size(); i++) {
        cout << i + 1 << ". Title: " << books[i].title
             << ", Description: " << books[i].description
             << ", Rating: " << books[i].rating
             << ", Available: " << (books[i].available ? "Yes" : "No") << "\n";
    }

    int bookChoice;
    cout << "Enter the number of the book you want to preview: ";
    cin >> bookChoice;

    if (bookChoice >= 1 && bookChoice <= books.size()) {
        previewBook(books[bookChoice - 1]);
    } else {
        cout << "Invalid choice. Returning to main menu.\n";
        mainMenu();
    }
}

void LibrarySystem::previewBook(Book& book) {
    cout << "Title: " << book.title << "\n";
    cout << "Description: " << book.description << "\n";
    cout << "Rating: " << book.rating << "\n";
    cout << "Available: " << (book.available ? "Yes" : "No") << "\n";

    if (book.available) {
        char borrowChoice;
        cout << "Would you like to borrow this book? (y/n): ";
        cin >> borrowChoice;

        if (borrowChoice == 'y' || borrowChoice == 'Y') {
            book.available = false;
            borrowedBooks.push_back({book.title, getCurrentDate()});
            cout << "You borrowed the book: " << book.title << "\n";
        }
    }
    mainMenu();
}

void LibrarySystem::viewBorrowedBooks() {
    if (borrowedBooks.empty()) {
        cout << "No books borrowed.\n";
    } else {
        cout << "Borrowed Books:\n";
        for (const auto& book : borrowedBooks) {
            cout << "- Title: " << book.title << ", Borrowed on: " << book.borrowDate << "\n";
        }
    }
    mainMenu();
}

string LibrarySystem::getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    return to_string(1900 + ltm->tm_year) + "-" +
           to_string(1 + ltm->tm_mon) + "-" +
           to_string(ltm->tm_mday);
}

void LibrarySystem::loadUsers() {
    ifstream file("users.txt");
    if (file.is_open()) {
        string username, password;
        while (file >> username >> password) {
            users[username] = password;
        }
        file.close();
    }
}

void LibrarySystem::saveUsers() {
    ofstream file("users.txt");
    for (const auto& user : users) {
        file << user.first << " " << user.second << "\n";
    }
}

void LibrarySystem::loadBorrowedBooks() {
    ifstream file("borrowed_books.txt");
    if (file.is_open()) {
        string title, borrowDate;
        while (file >> title >> borrowDate) {
            borrowedBooks.push_back({title, borrowDate});
        }
        file.close();
    }
}

void LibrarySystem::saveBorrowedBooks() {
    ofstream file("borrowed_books.txt");
    for (const auto& book : borrowedBooks) {
        file << book.title << " " << book.borrowDate << "\n";
    }
}

int main() {
    LibrarySystem library;
    library.run();
    return 0;
}
