#include <iostream>
#include <string>
#include "include/member.h"
#include "include/book.h"
#include "include/helper.h"

using namespace std;

void processCheckout();
void processReturn();
void processRenew();
void processOverdue();
void processMembers();
void processMemberModification(Member* member);
void processBooks();
void processBookModification(Book* book);

int main(){
    Member::loadData();
    Book::loadData();

    while (true){
        cout << "---------------------------------------->\n" 
            << "Welcome to the Library Management System!\n"
            << "What would you like to do?\n"
            << "1 - Checkout book to member\n"
            << "2 - Return book from member\n"
            << "3 - Renew membership\n"
            << "4 - Check overdue book returns\n"
            << "5 - Manage Members Database\n"
            << "6 - Manage Books Database\n"
            << "-1 - Exit\n"
            << "Write -1 at anytime to exit from an option\n"
            << "---------------------------------------->\n"; 

        int x;
        cin >> x;
        switch(x){
            case 1:
                processCheckout();
                break;
            case 2:
                processReturn();
                break;
            case 3:
                processRenew();
                break;
            case 4:
                processOverdue();
                break;
            case 5:
                processMembers();
                break;
            case 6:
                processBooks();
                break;
            case -1:
                return 0;
            default:
                cout << "Invalid option, please try again\n";
                continue;
        }
    }
}

void processCheckout(){
    while (true){
        string id, isbn;
        cout << "Please enter the member's ID\n";
        cin >> id;

        if (id == "-1") return;

        Member* member = Member::findByID(id);
        if (member != nullptr){
            cout << "Please enter the book's ISBN\n";
            cin >> isbn;

            if (isbn == "-1") return;

            Book* book = Book::findByISBN(isbn);
            if (book != nullptr){
                member->checkout(isbn);
                cout << "Sucessfully checked out the book " + book->getTitle() + "\n";
                break;
            }
            else {
                cout << "Invalid ISBN\n";
                continue;
            }
        }
        else {
            cout << "Invalid ID\n";
            continue;
        }
    }
}

void processReturn(){
    while(true){
        string id;
        cout << "Enter the member's ID\n";
        cin >> id;

        if (id == "-1") return;

        Member* member = Member::findByID(id);
        if (member != nullptr){
            vector<Transaction> transactions = member->getTransactionHistory();
            if (transactions.size() == 0){
                cout << "This member currently has no borrowed books!\n";
                return;
            }

            int selection;
            cout << "Select the book being returned\n";
            
            vector<Transaction> notReturnedTransactions = {};
            for (int i = 0; i < transactions.size(); i++){
                if (!transactions[i].isReturned){
                    notReturnedTransactions.push_back(transactions[i]);
                }
            }

            for (int i = 0; i < notReturnedTransactions.size(); i++){
                Book* book = Book::findByISBN(notReturnedTransactions[i].bookISBN);
                cout << i + 1 << " - " << book->getTitle() << "\n";
            }

            cin >> selection;
        
            if (selection == -1) return;
        
            if (selection <= notReturnedTransactions.size() && selection > 0){
                if (isOverdue(notReturnedTransactions[selection-1].returnDate)){
                    cout << "Succesfully returned the book but the member has to pay a fine for an overdue return!\n";
                }
                else {
                    cout << "Succesfully returned the book!\n";
                }
                member->returnBook(notReturnedTransactions[selection-1].bookISBN);
                return;
            }
            else {
                cout << "Invalid selection\n";
                continue;
            }
        }
        else {
            cout << "Invalid ID\n";
            continue;
        }
    }
}

void processRenew(){
    while (true){
        string id;
        cout << "Enter the member's ID\n";
        cin >> id;
        
        if (id == "-1") return;

        Member* member = Member::findByID(id);
        if (member != nullptr){
            member->renewMembership();
            cout << "Renewal succesful!\n";
            return;
        }
        else {
            cout << "Invalid ID\n"; 
            continue;
        }
    }
}

void processOverdue(){
    vector<Member*> members = Member::getRegisteredMembers();
    for (int i = 0; i < members.size(); i++){
        vector<Transaction> transactions = members[i]->getTransactionHistory();
        for (int j = 0; j < transactions.size(); j++){
            if (transactions[j].isReturned) continue;

            if (isOverdue(transactions[j].returnDate)){
                Book* book = Book::findByISBN(transactions[j].bookISBN);
                cout << "Member " + members[i]->getName() + " has not returned \"" + book->getTitle() + "\" on time!\n"; 
            }
        }
    }
}

void processMembers(){
    while (true){
        string input;
        cout << "---------------------------------------->\n"
            << "What do you want to do?\n"
            << "1 - Register new member\n" 
            << "2 - View all members\n"
            << "3 - Search for a specific user\n"
            << "---------------------------------------->\n";
        cin >> input;

        if (input == "-1") return;

        if (input == "1"){
            string id, name, phone;
            cout << "Please enter the member's ID\n";
            cin >> id;
            if (id == "-1") return;

            cout << "Please enter the member's name\n";
            getline(cin, name);
            getline(cin, name);
            if (name == "-1") return;

            cout << "Please enter the member's phone no.\n";
            cin >> phone;
            if (phone == "-1") return;

            if (Member::registerNewMember(id, name, phone)){
                cout << "Succesfully registered the new member!\n";
            }
            else {
                cout << "A member with this data already exists!\n";
            }
        }
        else if (input == "2"){
            while (true){
                int input;
                cout << "Choose the member you would like to modify:\n";
                
                vector<Member*> members = Member::getRegisteredMembers();
                for (int i = 0; i < members.size(); i++){
                    cout << i + 1 << " - " << members[i]->getInfo() << "\n";
                }

                cin >> input;
                if (input == -1) break;

                processMemberModification(members[input-1]);
            }
        }
        else if (input == "3"){
            while (true){
                int input;
                cout << "How do you want to search?\n"
                    << "1 - By ID\n"
                    << "2 - By name\n";
                cin >> input;

                if (input == -1) break;

                if (input == 1){
                    string id;
                    cout << "Please enter the ID\n";
                    cin >> id;

                    Member* member = Member::findByID(id);
                    if (member != nullptr){
                       processMemberModification(member); 
                    }
                    else {
                        cout << "No member matches your search!\n";
                    }
                }
                else if (input == 2){
                    string name;
                    cout << "Please enter the name\n";
                    getline(cin, name);
                    getline(cin, name);

                    vector<Member*> members = Member::findByName(name);
                    if (members.size() == 0){
                        cout << "No member matches your search!\n";
                        continue;
                    }
                    
                    cout << "Choose the member you would like to modify:\n";
                    for (int i = 0; i < members.size(); i++){
                        cout << i + 1 << " - "<< members[i]->getInfo() << "\n";
                    }
                    cin >> input;

                    if (input == -1) break;

                    if (input <= members.size() && input > 0){
                        processMemberModification(members[input-1]);
                    }
                    else {
                        cout << "Invalid selections.\n";
                        continue;
                    }
                }
                else {
                    cout << "Invalid option";
                    break;
                }
            }
        }
        else {
            cout << "Invalid option.\n";
            continue;
        }
    }  
}

void processMemberModification(Member* member){
    while (true){
        int input;
        cout << "---------------------------------------->\n"
            << member->getDetailedInfo() << "\n"
            << "What would you like to do?\n"
            << "1 - Modify ID\n"
            << "2 - Modify name\n"
            << "3 - Modify phone no.\n"
            << "4 - Remove from database\n"
            << "---------------------------------------->\n";
        cin >> input;
        
        if (input == -1) return;
        
        string modification;
        switch(input){
            case 1:
                cout << "Please enter the new ID\n";
                cin >> modification;
                
                if (modification == "-1") return;

                if (member->updateID(modification)){
                    cout << "Succesfully updated the ID!\n";
                    return;
                }
                else {
                    cout << "This ID already exists in the system.\n";
                }
                break;
            case 2:
                cout << "Please enter the new name\n";
                getline(cin, modification);
                getline(cin, modification);

                if (modification == "-1") return;
                
                member->updateName(modification);
                cout << "Succesfully updated the name!\n";
                return;
            case 3:
                cout << "Please enter the new number\n";
                cin >> modification;

                if (modification == "-1") return;
                
                member->updatePhone(modification);
                cout << "Succesfully updated the phone number!\n";
                return;
            case 4:
                cout << "Are you sure you want to delete this member's data?\n"
                    << "1 - Yes\n"
                    << "2 - No\n";
                cin >> modification;

                if (modification != "1") break;
                
                member->removeMember();
                cout << "Member removed succesfully!\n";
                return;
            default:
                cout << "Invalid option\n";
                continue;
        }
    }
}

void processBooks(){
    while (true){
        string input;
        cout << "---------------------------------------->\n"
            << "What do you want to do?\n"
            << "1 - Register new book\n" 
            << "2 - View all books\n"
            << "3 - Search for a specific book\n"
            << "---------------------------------------->\n";
        cin >> input;

        if (input == "-1") return;

        if (input == "1"){
            string isbn, title, author, year, genre;
            cout << "Please enter the book's ISBN\n";
            cin >> isbn;
            if (isbn == "-1") return;

            cout << "Please enter the book's title\n";
            getline(cin, title);
            getline(cin, title);
            if (title == "-1") return;

            cout << "Please enter the book's author\n";
            getline(cin, author);
            if (author == "-1") return;

            cout << "Please enter the book's publication year\n";
            cin >> year;
            if (year == "-1") return;

            cout << "Please enter the book's genre\n";
            getline(cin, genre);
            getline(cin, genre);
            if (genre == "-1") return;

            if (Book::registerNewBook(isbn, title, author, year, genre)){
                cout << "Succesfully registered the new book!\n";
            }
            else {
                cout << "A book with this data already exists!\n";
            }
        }
        else if (input == "2"){
            while (true){
                int input;
                cout << "Choose the book you would like to modify:\n";

                vector<Book*> books = Book::getRegisteredBooks();                
                for (int i = 0; i < books.size(); i++){
                    cout << i + 1 << " - " << books[i]->getTitle() << "\n";
                }

                cin >> input;
                if (input == -1) break;

                processBookModification(books[input-1]);
            }
        }
        else if (input == "3"){
            while (true){
                int input;
                cout << "---------------------------------------->\n"
                    << "How do you want to search?\n"
                    << "1 - By ISBN\n"
                    << "2 - By title\n"
                    << "3 - By author\n"
                    << "4 - By genre\n"
                    << "---------------------------------------->\n";
                cin >> input;

                if (input == -1) break;

                if (input == 1){
                    string isbn;
                    cout << "Please enter the ISBN\n";
                    cin >> isbn;

                    Book* book = Book::findByISBN(isbn);
                    if (book != nullptr){
                       processBookModification(book); 
                    }
                    else {
                        cout << "No book matches your search!\n";
                    }
                }
                else if (input == 2){
                    string title;
                    cout << "Please enter the title\n";
                    getline(cin, title);
                    getline(cin, title);

                    vector<Book*> books = Book::findByTitle(title);
                    if (books.size() == 0){
                        cout << "No book matches your search!\n";
                        continue;
                    }
                    
                    cout << "Choose the book you would like to modify:\n";
                    for (int i = 0; i < books.size(); i++){
                        cout << i + 1 << " - "<< books[i]->getTitle() << "\n";
                    }
                    cin >> input;

                    if (input == -1) break;

                    processBookModification(books[input-1]);
                }
                else if (input == 3){
                    string author;
                    cout << "Please enter the author's name\n";
                    getline(cin, author);
                    getline(cin, author);

                    vector<Book*> books = Book::findByAuthor(author);
                    if (books.size() == 0){
                        cout << "No book matches your search!\n";
                        continue;
                    }
                    
                    cout << "Choose the book you would like to modify:\n";
                    for (int i = 0; i < books.size(); i++){
                        cout << i + 1 << " - "<< books[i]->getTitle() << "\n";
                    }
                    cin >> input;

                    if (input == -1) break;

                    processBookModification(books[input-1]);
                }
                else if (input == 4){
                    string genre;
                    cout << "Please enter the genre\n";
                    getline(cin, genre);
                    getline(cin, genre);

                    vector<Book*> books = Book::findByGenre(genre);
                    if (books.size() == 0){
                        cout << "No book matches your search!\n";
                        continue;
                    }
                    
                    cout << "Choose the book you would like to modify:\n";
                    for (int i = 0; i < books.size(); i++){
                        cout << i + 1 << " - "<< books[i]->getTitle() << "\n";
                    }
                    cin >> input;

                    if (input == -1) break;

                    processBookModification(books[input-1]);
                }
                else {
                    cout << "Invalid option\n";
                    break;
                }
            }
        }
        else {
            cout << "Invalid option.\n";
            continue;
        }
    }  
}

void processBookModification(Book* book){
    while (true){
        int input;
        cout << "---------------------------------------->\n"
            << book->getInfo() << "\n"
            << "What would you like to do?\n"
            << "1 - Modify ISBN\n"
            << "2 - Modify title\n"
            << "3 - Modify author name\n"
            << "4 - Modify genre\n"
            << "5 - Modify publication year\n"
            << "6 - Remove from database\n";

        cin >> input;
        
        if (input == -1) return;
        
        string modification;
        switch(input){
            case 1:
                cout << "Please enter the new ISBN\n";
                getline(cin, modification);
                getline(cin, modification); 
                
                if (modification == "-1") return;

                if (book->updateISBN(modification)){
                    cout << "Succesfully updated the ISBN!\n";
                    return;
                }
                else {
                    cout << "This ISBN already exists in the system.\n";
                }
                break;
            case 2:
                cout << "Please enter the new title\n";
                getline(cin, modification);
                getline(cin, modification);

                if (modification == "-1") return;
                
                book->updateTitle(modification);
                cout << "Succesfully updated the title!\n";
                return;
            case 3:
                cout << "Please enter the new name\n";
                getline(cin, modification);
                getline(cin, modification);

                if (modification == "-1") return;
                
                book->updateAuthor(modification);
                cout << "Succesfully updated the author's name!\n";
                return;
            case 4:
                cout << "Please enter the new genre\n";
                getline(cin, modification);
                getline(cin, modification);

                if (modification == "-1") return;
                
                book->updateGenre(modification);
                cout << "Succesfully updated the book's genre!\n";
                return;
            case 5:
                cout << "Please enter the new year\n";
                cin >> modification;

                if (modification == "-1") return;
                
                book->updateYear(modification);
                cout << "Succesfully updated the book's genre!\n";
                return;
            case 6:
                cout << "Are you sure you want to delete this book's data?\n"
                    << "1 - Yes\n"
                    << "2 - No\n";
                cin >> modification;

                if (modification != "1") break;
                
                book->removeBook();
                cout << "Book removed succesfully!\n";
                return;
            default:
                cout << "Invalid option\n";
                continue;
        }
    }
}

//Library project
//Inventory: add, remove, modify, search, details(name,title, author, genre, status(borrowed, available))
//Customers: add, remove, modify, details (ID,name, membership status), borrow history, currently borrowed books
//check out/ return books
//keep track of dates and alert for overdue books

/*
Welcome to the Library Management System!
What would you like to do?
1 - Checkout book
2 - Return book
3 - Renew membership
4 - Check overdue book returnals
5 - Manage Members Database
6 - Manage Books Database
-1 - Exit
Write Exit at anytime to exit out from an option
*/

/* Checkout book
Please enter the book's ISBN
Please enter the member's ID
Done!
*/
/* Return book
Please enter the book's ISBN
Please enter the member's ID
Done!/Overdue return! The member will have to pay a fine!
*/
/* Renew membership
Please take the membership fee from the member and enter their ID
Membership renewed succesfully!/Invalid ID please try again or type Exit to exit this option
*/
/*check overdue book
found x members that haven't returned their books yet
select which member you would like to view
1- member one
2- member two

displaying memberonefirstname's overdue fines
ID: memberid
Name: fullname
book1 should have been returned on returndate
book2 etc
*/

/*
*/