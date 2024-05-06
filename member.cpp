#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <chrono>
#include <string>
#include "include/helper.h"
#include "include/book.h"
#include "include/member.h"

using namespace std;
using namespace chrono;

string Member::separator = "<#";

vector<Member*> Member::registeredMembers;

Member::Member(){}

bool Member::registerNewMember(string _id, string _name, string _phone){
    if (Member::findByID(_id) != nullptr) return false;

    Member* member = new Member();
    member->id = _id;
    member->name = _name;
    member->phoneNumber = _phone;

    member->joinDate = getTimeNow();
    member->membershipRenewalDate = getTimeAfter(24 * 30);
    
    registeredMembers.push_back(member);
    saveData();

    return true;
}

void Member::checkout(string ISBN){
    //check if book exists
    Transaction transaction;
    transaction.bookISBN = ISBN;
    transaction.isReturned = false;
    transaction.checkoutDate = getTimeNow();
    transaction.returnDate = getTimeAfter(3 * 7 * 24); // 3 weeks * 7 days * 24 hours

    transactionHistory.push_back(transaction);
    saveData();
}

bool Member::returnBook(string ISBN){
    for (int i = 0; i < transactionHistory.size(); i++){
        if (transactionHistory[i].bookISBN == ISBN && !transactionHistory[i].isReturned){
            transactionHistory[i].isReturned = true;
            transactionHistory[i].returnDate = getTimeNow();
            saveData();
            return true;
        }
    }

    return false;
}

void Member::renewMembership(){
    membershipRenewalDate = getTimeAfter(24*30);

    saveData();
}

bool Member::updateID(string _id){
    if (Member::findByID(_id) != nullptr) return false;

    id = _id;
    saveData();

    return true;
}

void Member::updateName(string _name){
    this->name = _name;
    saveData();
}

void Member::updatePhone(string _phone){
    phoneNumber = _phone;
    saveData();
}

Member* Member::findByID(string id){
    for (int i = 0; i < registeredMembers.size(); i++){
        if(id == registeredMembers[i]->id){
            return registeredMembers[i];
        }
    }

    return nullptr;
}

vector<Transaction> Member::getTransactionHistory(){
    return transactionHistory;
}

vector<Member*> Member::getRegisteredMembers(){
    return Member::registeredMembers;
}

vector<Member*> Member::findByName(string name){
    vector<Member*> result;
    for (int i = 0; i < registeredMembers.size(); i++){
        if(stringToLower(registeredMembers[i]->name).find(stringToLower(name)) != string::npos){
            result.push_back(registeredMembers[i]);
        }
    }

    return result;
}

void Member::removeMember(){
    for (int i = 0; i < registeredMembers.size(); i++){
        if (id == registeredMembers[i]->id){
            swap(registeredMembers[i], registeredMembers[registeredMembers.size() - 1]);
            registeredMembers.pop_back();
        }
    }
    saveData();
}
void Member::saveData(){
    ofstream output("memberbase.txt");
    
    for (int i = 0; i < registeredMembers.size(); i++){
        Member* data = registeredMembers[i];

        output << data->id << separator 
        << data->name << separator 
        << data->phoneNumber << separator 
        << tmToString(data->joinDate) << separator
        << tmToString(data->membershipRenewalDate) << "\n";

        for (int j = 0; j < data->transactionHistory. size(); j++){
            output << "{+}" << separator
            << data->transactionHistory[j].bookISBN << separator
            << data->transactionHistory[j].isReturned << separator
            << tmToString(data->transactionHistory[j].checkoutDate) << separator
            << tmToString(data->transactionHistory[j].returnDate) << "\n";
        }
    }

    output.close();
}

void Member::loadData(){
    ifstream input("memberbase.txt");
    string line;

    registeredMembers = {};

    while(true){
        getline(input, line);
        if(line == "\n" || line == "") break;
        while (true){
            if(line == "\n" || line == "") break;

            auto memberInputs = split(separator, line);
            Member* member = new Member();

            member->id = memberInputs[0];
            member->name = memberInputs[1];
            member->phoneNumber = memberInputs[2];
            member->joinDate = stringToTm(memberInputs[3]);
            member->membershipRenewalDate = stringToTm(memberInputs[4]);

            while (true){
                getline(input, line);
                if (line.find("{+}") == 0){
                    Transaction trans;
                    line.erase(0, 5);
                    auto transInputs = split(separator ,line);
                    trans.bookISBN = transInputs[0];
                    trans.isReturned = stoi(transInputs[1]);
                    trans.checkoutDate = stringToTm(transInputs[2]);
                    trans.returnDate = stringToTm(transInputs[3]);

                    member->transactionHistory.push_back(trans);
                    continue;
                }
                else {
                    break;
                } 
            }

            Member::registeredMembers.push_back(member);
        }
    }
    input.close();
}

string Member::getName(){
    return name;
}

string Member::getInfo(){
    string result = "ID: " + id
        + ", Name: " + name 
        + ", Phone: " + phoneNumber 
        + ", Joined on " + tmToString(joinDate);
        
    return result;
}
string Member::getDetailedInfo(){
    string result = "ID: " + id
        + "\nName: " + name 
        + "\nPhone: " + phoneNumber 
        + "\nJoined on " + tmToString(joinDate)
        + "\nMembership renew on " + tmToString(membershipRenewalDate);

    if (transactionHistory.size() > 0 ){
        result += "\nTransaction history: \n";
        for (int i = 0; i < transactionHistory.size(); i++){
            string isbn = transactionHistory[i].bookISBN;
            string title = Book::findByISBN(transactionHistory[i].bookISBN)->getTitle();
            string checkoutDate = tmToString(transactionHistory[i].checkoutDate);
            string returnDate = tmToString(transactionHistory[i].returnDate);
            result += to_string(i+1) + " - ISBN: " +  isbn
            + "\n    Title: " + title
            + "\n    Borrowed on " + checkoutDate;
            if (transactionHistory[i].isReturned){
                result += "\n    Returned on " + returnDate;
            }
            else {
                result += "\n    Will be returned on " + returnDate;
            }

            result += "\n";
        }
    }
    else {
        result += "\nNo transactions have been done by this member.";
    }

    return result;
}

/*save structure
#
id
name
phone
joindate
membershipend
transaction1.ISBN
transaction1.isreturned;
transaction1.checkdate
transaction1.returndate
#
*/

/*
friend ostream&operator<<(ostream output,item m){
    output<<m.price<<m.name<<m.category<<m.quantity;
    return output;
}
*/