#include <iostream>
#include <chrono>
#include <ctime>
#include <vector>

using namespace std;
using namespace chrono;

string stringToLower(string s){
    for (int i = 0; i < s.size(); i++)
        s[i] = tolower(s[i]);

    return s;
}

tm getTimeNow(){
    auto now = system_clock::now();
    auto timeNow = system_clock::to_time_t(now);

    tm timeStruct = *localtime(&timeNow);

    return timeStruct;
}

tm getTimeAfter(int numberOfHours){
    auto now = system_clock::now();
    auto timeAfter = system_clock::to_time_t(now + hours(numberOfHours));

    tm timeStruct = *localtime(&timeAfter);

    return timeStruct;
}

string tmToString(tm time){
    string result;

    result += to_string(time.tm_year + 1900) + "-";

    if (time.tm_mon < 10) {
        result += "0";
    }
    result += to_string(time.tm_mon + 1) + "-";

    if (time.tm_mday < 10) {
        result += "0";
    }
    result += to_string(time.tm_mday);

    return result;
}

tm stringToTm(string date){
    tm result;

    result.tm_year = stoi(date.substr(0, 4)) - 1900;
    result.tm_mon = stoi(date.substr(5, 2)) - 1;
    result.tm_mday = stoi(date.substr(8, 2));
    result.tm_hour = 0;
    result.tm_min = 0;
    result.tm_sec = 0;

    return result;
}

bool isOverdue(tm time){
    tm timeNow = getTimeNow();
    return difftime(mktime(&timeNow), mktime(&time)) > 0;
}

vector<string> split(string separator, string input){
    vector<string> result = {};
    while (input != ""){
        int index = input.find(separator);
        if (index == string::npos){
            result.push_back(input);
            break;
        }

        result.push_back(input.substr(0, index));
        input.erase(0, index + separator.size());
    }

    return result;
}
