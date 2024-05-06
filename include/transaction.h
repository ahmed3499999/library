#include <iostream>
#include <ctime>

using namespace std;

struct Transaction {
    string bookISBN;
    tm checkoutDate;
    bool isReturned;
    tm returnDate;
};