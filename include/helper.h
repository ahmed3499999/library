#include <iostream>
#include <ctime>

using namespace std;

string stringToLower(string s);
tm getTimeNow();
string tmToString(tm time);
tm stringToTm(string s);
tm getTimeAfter(int num);
bool isOverdue(tm time);
vector<string> split(string sep, string input);