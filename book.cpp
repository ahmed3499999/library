#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "include/helper.h"
#include "include/book.h"

string Book::separator = "<#";
vector<Book*> Book::registeredBooks = {};

Book::Book(){}
   
Book* Book::registerNewBook(string _isbn, string _title, string _author, string _year, string _genre){
    if (Book::findByISBN(_isbn) != nullptr) return nullptr;

    Book* book = new Book();
    book->ISBN = _isbn;
    book->title = _title;
    book->author = _author;
    book->publicationYear = _year;
    book->genre = _genre;

    registeredBooks.push_back(book);
    saveData();

    return book;
}

string Book::getTitle(){
    return title;
}

string Book::getInfo(){
    return title
    + "\nAuthor: " + author
    + "\nPublished in " + publicationYear
    + "\nGenre: " + genre 
    + "\nISBN: " + ISBN;
}

vector<Book*> Book::getRegisteredBooks(){
    return registeredBooks;
}

bool Book::updateISBN(string _isbn){
    if (Book::findByISBN != nullptr) return false;

    ISBN = _isbn;
    saveData();
    return true;
}

void Book::updateTitle(string _title){
    title = _title;
    saveData();
}

void Book::updateYear(string _year){
    publicationYear = _year;
    saveData();
}

void Book::updateAuthor(string _author){
    author = _author;
    saveData();
}

void Book::updateGenre(string _genre){
    genre = _genre;
    saveData();
}

Book* Book::findByISBN(string isbn){
    for (int i = 0; i < registeredBooks.size(); i++){
        if (registeredBooks[i]->ISBN == isbn){
            return registeredBooks[i];
        }
    }
    
    return nullptr;
}

vector<Book*> Book::findByGenre(string genre){
    vector<Book*> result;
    for (int i = 0; i < registeredBooks.size(); i++){
        if (stringToLower(registeredBooks[i]->genre).find(stringToLower(genre)) != string::npos){
            result.push_back(registeredBooks[i]);
        }
    }

    return result;
}

vector<Book*> Book::findByTitle(string title){
    vector<Book*> result;
    for (int i = 0; i < registeredBooks.size(); i++){
        if (stringToLower(registeredBooks[i]->title).find(stringToLower(title)) != string::npos){
            result.push_back(registeredBooks[i]);
        }
    }

    return result;
}

vector<Book*> Book::findByAuthor(string author){
    vector<Book*> result;
    for (int i = 0; i < registeredBooks.size(); i++){
        if (stringToLower(registeredBooks[i]->author).find(stringToLower(author)) != string::npos){
            result.push_back(registeredBooks[i]);
        }
    }

    return result;
}

void Book::removeBook(){
    for (int i = 0; i < registeredBooks.size(); i++){
        if (ISBN == registeredBooks[i]->ISBN){
            swap(registeredBooks[i], registeredBooks[registeredBooks.size() - 1]);
            registeredBooks.pop_back();
        }
    }
    saveData();
}

void Book::saveData(){
    ofstream output("bookbase.txt");
    for (int i = 0; i < registeredBooks.size(); i++){
        Book* data = registeredBooks[i];

        output << data->ISBN << separator
        << data->title << separator
        << data->author << separator
        << data->publicationYear << separator
        << data->genre << "\n";

    }

    output.close();
}

void Book::loadData(){
    ifstream input("bookbase.txt");
    string line;
    string linecount;

    registeredBooks = {};

    while (true){
        getline(input, line);
        if (line == "\n" || line == "") break;

        auto bookInputs = split(separator, line);
        Book* book = new Book();

        book->ISBN = bookInputs[0];
        book->title = bookInputs[1];
        book->author = bookInputs[2];
        book->publicationYear = bookInputs[3];
        book->genre = bookInputs[4];

        registeredBooks.push_back(book);
    }
    
    input.close();
}