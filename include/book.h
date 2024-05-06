#include <iostream>
#include <vector>

using namespace std;

class Book{
    private:
    string ISBN;
    string title;
    string author;
    string publicationYear;
    string genre;
    
    static string separator;
    static vector<Book *> registeredBooks;

    Book();
    
    static void saveData();

    public:
    bool updateISBN(string isbn);
    void updateTitle(string title);
    void updateYear(string year);
    void updateAuthor(string author);
    void updateGenre(string genre);
    void removeBook();
    string getTitle();
    string getInfo();

    static void loadData();
    static vector<Book*> getRegisteredBooks();
    static Book* registerNewBook(string ISBN, string title, string author, string year, string genre);
    static Book* findByISBN(string isbn);
    static vector<Book*> findByTitle(string title);
    static vector<Book*> findByGenre(string genre);
    static vector<Book*> findByAuthor(string author);
};

/*save structure
ISBN
title
author
publication year
genre
*/