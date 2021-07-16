/**
    Code For :- Validation of IPv4 address.
    @file ipaddress_validate.cpp
    @author Gautham Sreekumar (GauthamSree)
*/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> split(const string& str){
    int i = 0;
    vector<string> list;
    auto pos = str.find('.');
    while (pos != string::npos){
    list.push_back(str.substr(i, pos - i));
    i = ++pos;
    pos = str.find('.', pos);
    }
    list.push_back(str.substr(i, str.length()));
    return list;
}

bool isValidIPv4Block(const string& block) {
    int num = 0;
    if (block.size() > 0 && block.size() <= 3) {
        for (int i = 0; i < block.size(); ++i) {
            char c = block[i];
            if (!isalnum(c) || (i == 0 && c == '0' && block.size() > 1)) {
                return false;
            } else {
                num *= 10;
                num += c - '0';
            }
        }
        return num <= 255;
    }
    return false;
}

bool validIPAddress(vector<string> lis) {
    for(auto block: lis) {
        if (!isValidIPv4Block(block)) {
            return false;
        }
    }
    return true;
}

int main() {
    string ip;
    cout<<"\nValidation of IPv4 address\n\n";
    cout<<"Enter the IP Address: ";
    cin>>ip;
    vector<string> slist = split(ip);
    bool IPValid;
    if (slist.size() != 4) {
        IPValid = false;
    } else {
        IPValid = validIPAddress(slist);
    }
    if (IPValid) cout << "\nIt is a valid IPv4 address\n";
    else cout << "\nIt is not a valid IPv4 address\n";
    return 0;
}