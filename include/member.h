#include <iostream>
#include <vector>
#include <ctime>
#include "transaction.h"

using namespace std;

class Member{
    private:
    string id;
    string name;
    string phoneNumber;
    tm joinDate;
    tm membershipRenewalDate;
    vector<Transaction> transactionHistory;

    static string separator;
    static vector<Member*> registeredMembers;
    
    Member();
    
    static void saveData();
    
    public:
    void checkout(string ISBN);
    bool returnBook(string ISBN);
    void renewMembership();
    bool updateID(string id);
    void updateName(string name);
    void updatePhone(string PhoneNumber);
    void removeMember();
    vector<Transaction> getTransactionHistory();
    string getName();
    string getInfo();
    string getDetailedInfo();
    
    static void loadData();
    static bool registerNewMember(string id, string name, string phone);
    static vector<Member*> getRegisteredMembers();
    static Member* findByID(string id);
    static vector<Member*> findByName(string name);
};

/*save structure
#
id
name
phone
status
joindate
membershipend
transaction1.ISBN
transaction1.date
etc
#
*/