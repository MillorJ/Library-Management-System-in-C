#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 1000
#define MAX_USERS 1000
#define MAX_NAME_LENGTH 100
#define MAX_BORROW_LIMIT 5
#define MAX_DAYS_TO_RETURN 15
#define FINE_PER_DAY 2

// Structures for Users and Books
typedef struct {
    int id;
    char title[MAX_NAME_LENGTH];
    char author[MAX_NAME_LENGTH];
    int is_borrowed;  // 1 for borrowed, 0 for available
    time_t borrow_date; // Store the time of borrowing
} Book;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char role[10]; // librarian or member
    int borrowed_books[MAX_BORROW_LIMIT]; // Track the books borrowed by the user
    int total_borrowed; // Count of borrowed books
} User;

// Arrays to store books and users
Book books[MAX_BOOKS];
User users[MAX_USERS];
int total_books = 0;
int total_users = 0;

// Function prototypes
void addUser();
void removeUser();
void listUsers();
void addBook();
void removeBook();
void listBooks();
void searchBook();
void borrowBook();
void returnBook();
void reportBorrowedBooks();
void reportOverdueBooks();
void reportUserFines();
void showMenu();
void librarianMenu();
void memberMenu();
void searchBookByTitle();
void searchBookByAuthor();
void calculateFine(User *user, int book_id);

// Main Function
int main() {
    showMenu();
    return 0;
}

// Functions for User Management
void addUser() {
    if (total_users >= MAX_USERS) {
        printf("Maximum user limit reached!\n");
        return;
    }

    printf("Enter user name: ");
    scanf(" %[^\n]", users[total_users].name);
    printf("Enter role (librarian/member): ");
    scanf("%s", users[total_users].role);
    users[total_users].id = total_users + 1;
    users[total_users].total_borrowed = 0;  // Initialize borrowed books

    printf("User added with ID: %d\n", users[total_users].id);
    total_users++;
}

void removeUser() {
    int id;
    printf("Enter user ID to remove: ");
    scanf("%d", &id);

    for (int i = 0; i < total_users; i++) {
        if (users[i].id == id) {
            for (int j = i; j < total_users - 1; j++) {
                users[j] = users[j + 1];
            }
            total_users--;
            printf("User removed successfully.\n");
            return;
        }
    }
    printf("User not found!\n");
}

void listUsers() {
    printf("Listing all users:\n");
    for (int i = 0; i < total_users; i++) {
        printf("ID: %d, Name: %s, Role: %s, Borrowed Books: %d\n", users[i].id, users[i].name, users[i].role, users[i].total_borrowed);
    }
}

// Functions for Book Management
void addBook() {
    if (total_books >= MAX_BOOKS) {
        printf("Maximum book limit reached!\n");
        return;
    }

    printf("Enter book title: ");
    scanf(" %[^\n]", books[total_books].title);
    printf("Enter author: ");
    scanf(" %[^\n]", books[total_books].author);
    books[total_books].id = total_books + 1;
    books[total_books].is_borrowed = 0;

    printf("Book added with ID: %d\n", books[total_books].id);
    total_books++;
}

void removeBook() {
    int id;
    printf("Enter book ID to remove: ");
    scanf("%d", &id);

    for (int i = 0; i < total_books; i++) {
        if (books[i].id == id) {
            for (int j = i; j < total_books - 1; j++) {
                books[j] = books[j + 1];
            }
            total_books--;
            printf("Book removed successfully.\n");
            return;
        }
    }
    printf("Book not found!\n");
}

void listBooks() {
    printf("Listing all books:\n");
    for (int i = 0; i < total_books; i++) {
        printf("ID: %d, Title: %s, Author: %s, Status: %s\n", books[i].id, books[i].title, books[i].author,
               books[i].is_borrowed ? "Borrowed" : "Available");
    }
}

// Search books by title or author
void searchBook() {
    int choice;
    printf("Search Book:\n");
    printf("1. By Title\n2. By Author\nEnter choice: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            searchBookByTitle();
            break;
        case 2:
            searchBookByAuthor();
            break;
        default:
            printf("Invalid choice!\n");
    }
}

void searchBookByTitle() {
    char title[MAX_NAME_LENGTH];
    printf("Enter book title: ");
    scanf(" %[^\n]", title);

    for (int i = 0; i < total_books; i++) {
        if (strcmp(books[i].title, title) == 0) {
            printf("ID: %d, Title: %s, Author: %s, Status: %s\n", books[i].id, books[i].title, books[i].author,
                   books[i].is_borrowed ? "Borrowed" : "Available");
            return;
        }
    }
    printf("Book not found!\n");
}

void searchBookByAuthor() {
    char author[MAX_NAME_LENGTH];
    printf("Enter book author: ");
    scanf(" %[^\n]", author);

    for (int i = 0; i < total_books; i++) {
        if (strcmp(books[i].author, author) == 0) {
            printf("ID: %d, Title: %s, Author: %s, Status: %s\n", books[i].id, books[i].title, books[i].author,
                   books[i].is_borrowed ? "Borrowed" : "Available");
            return;
        }
    }
    printf("Book not found!\n");
}

// Borrowing and Returning Books
void borrowBook() {
    int user_id, book_id;
    printf("Enter user ID: ");
    scanf("%d", &user_id);
    printf("Enter book ID to borrow: ");
    scanf("%d", &book_id);

    // Find user
    for (int i = 0; i < total_users; i++) {
        if (users[i].id == user_id) {
            if (users[i].total_borrowed >= MAX_BORROW_LIMIT) {
                printf("You have already borrowed the maximum number of books!\n");
                return;
            }

            // Find book
            for (int j = 0; j < total_books; j++) {
                if (books[j].id == book_id && !books[j].is_borrowed) {
                    books[j].is_borrowed = 1;
                    books[j].borrow_date = time(NULL); // Store the borrow time
                    users[i].borrowed_books[users[i].total_borrowed] = book_id;
                    users[i].total_borrowed++;

                    printf("You have borrowed the book: %s\n", books[j].title);
                    return;
                }
            }
            printf("Book is already borrowed or does not exist!\n");
            return;
        }
    }
    printf("User not found!\n");
}

void returnBook() {
    int user_id, book_id;
    printf("Enter user ID: ");
    scanf("%d", &user_id);
    printf("Enter book ID to return: ");
    scanf("%d", &book_id);

    // Find user
    for (int i = 0; i < total_users; i++) {
        if (users[i].id == user_id) {
            // Find the borrowed book in the user's list
            for (int j = 0; j < users[i].total_borrowed; j++) {
                if (users[i].borrowed_books[j] == book_id) {
                    // Find book
                    for (int k = 0; k < total_books; k++) {
                        if (books[k].id == book_id && books[k].is_borrowed) {
                            books[k].is_borrowed = 0;
                            calculateFine(&users[i], book_id); // Calculate fine if overdue
                            // Remove book from user's borrowed list
                            users[i].borrowed_books[j] = 0;
                            users[i].total_borrowed--;
                            printf("You have successfully returned the book: %s\n", books[k].title);
                            return;
                        }
                    }
                }
            }
            printf("You have not borrowed this book!\n");
            return;
        }
    }
    printf("User not found!\n");
}

// Calculate fine for overdue books
void calculateFine(User *user, int book_id) {
    time_t current_time = time(NULL);
    for (int i = 0; i < total_books; i++) {
        if (books[i].id == book_id) {
            double days_borrowed = difftime(current_time, books[i].borrow_date) / (60 * 60 * 24);
            if (days_borrowed > MAX_DAYS_TO_RETURN) {
                int overdue_days = (int)(days_borrowed - MAX_DAYS_TO_RETURN);
                int fine = overdue_days * FINE_PER_DAY;
                printf("The book is overdue by %d days. You have a fine of $%d.\n", overdue_days, fine);
            } else {
                printf("The book was returned on time. No fine.\n");
            }
            return;
        }
    }
}

// Report of all borrowed books
void reportBorrowedBooks() {
    printf("Report of Borrowed Books:\n");
    for (int i = 0; i < total_books; i++) {
        if (books[i].is_borrowed) {
            printf("ID: %d, Title: %s, Borrowed by User ID: ", books[i].id, books[i].title);
            for (int j = 0; j < total_users; j++) {
                for (int k = 0; k < users[j].total_borrowed; k++) {
                    if (users[j].borrowed_books[k] == books[i].id) {
                        printf("%d\n", users[j].id);
                        break;
                    }
                }
            }
        }
    }
}

// Report of overdue books
void reportOverdueBooks() {
    time_t current_time = time(NULL);
    printf("Report of Overdue Books:\n");
    for (int i = 0; i < total_books; i++) {
        if (books[i].is_borrowed) {
            double days_borrowed = difftime(current_time, books[i].borrow_date) / (60 * 60 * 24);
            if (days_borrowed > MAX_DAYS_TO_RETURN) {
                printf("ID: %d, Title: %s, Overdue by %d days\n", books[i].id, books[i].title, (int)(days_borrowed - MAX_DAYS_TO_RETURN));
            }
        }
    }
}

// Report of user fines
void reportUserFines() {
    printf("Report of Users with Fines:\n");
    for (int i = 0; i < total_users; i++) {
        for (int j = 0; j < users[i].total_borrowed; j++) {
            calculateFine(&users[i], users[i].borrowed_books[j]);
        }
    }
}

// Main menu function
void showMenu() {
    int choice;
    printf("\nLibrary Management System\n");
    printf("1. Librarian Menu\n2. Member Menu\n3. Exit\nEnter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            librarianMenu();
            break;
        case 2:
            memberMenu();
            break;
        case 3:
            exit(0);
            break;
        default:
            printf("Invalid choice!\n");
            showMenu();
    }
}

// Librarian menu
void librarianMenu() {
    int choice;
    do {
        printf("\nLibrarian Menu\n");
        printf("1. Add User\n2. Remove User\n3. List Users\n4. Add Book\n5. Remove Book\n6. List Books\n7. Report of Borrowed Books\n8. Report of Overdue Books\n9. Report of User Fines\n10. Return to Main Menu\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addUser();
                break;
            case 2:
                removeUser();
                break;
            case 3:
                listUsers();
                break;
            case 4:
                addBook();
                break;
            case 5:
                removeBook();
                break;
            case 6:
                listBooks();
                break;
            case 7:
                reportBorrowedBooks();
                break;
            case 8:
                reportOverdueBooks();
                break;
            case 9:
                reportUserFines();
                break;
            case 10:
                showMenu();
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 10);
}

// Member menu
void memberMenu() {
    int choice;
    do {
        printf("\nMember Menu\n");
        printf("1. Borrow Book\n2. Return Book\n3. Search Book\n4. Return to Main Menu\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                borrowBook();
                break;
            case 2:
                returnBook();
                break;
            case 3:
                searchBook();
                break;
            case 4:
                showMenu();
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 4);
}

