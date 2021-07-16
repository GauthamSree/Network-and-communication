/**
    Code For :- Find the IP subnet address (first and last) of given IPv4 address and subnet mask.
    @file ipaddress_subnet.cpp
    @author Gautham Sreekumar (GauthamSree)
*/

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <math.h>
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

char findClass(const string& block) {
    int num = 0;
    for (int i = 0; i < block.size(); ++i) {
        char c = block[i];
        num *= 10;
        num += c - '0';
    }
    if (num >= 0 && num <= 127)
        return 'A';
    else if (num >= 128 && num <= 191)
        return 'B';
    else if (num >= 192 && num <= 223)
        return 'C';
    else if (num >= 224 && num <= 239)
        return 'D';
    else
        return 'E';
}

vector<bitset<8>> binaryIP(vector<string> lis) {
    vector<bitset<8>> binIP; 
    for(auto block: lis) {
        int num = 0;
        for (int i = 0; i < block.size(); ++i) {
        char c = block[i];
        num *= 10;
        num += c - '0';
        }
        bitset<8> x = num;
        binIP.push_back(x);
    }
    return binIP;
}

bitset<8> findbitset(int val, int rem, int k, vector<bitset<8>> ip) {
    bitset<8> x;
    for(int i=0; i<8; i++) {
        if(i >= rem) {
            x[i] = ip[k][i];
            continue;
        }
        x[i] = val;
    }
    return x;
}

vector<bitset<8>> find_address(int hostno, vector<bitset<8>> ip, bool first) {
    vector<bitset<8>> address = ip;
    if(hostno <= 8) {
        if(first) {    
            bitset<8> x = findbitset(0, hostno, 3, ip);
            address[3] = x;
        } else {
            bitset<8> x = findbitset(1, hostno, 3, ip);
            address[3] = x;
        }
    }
    else if(hostno>8 && hostno<=16) {
        int rem = hostno - 8;
        if(first) {
            bitset<8> x = findbitset(0, rem, 2, ip);
            bitset<8> allzero = bitset<8>().reset(); 
            address[2] = x;
            address[3] = allzero;
        } else {
            bitset<8> x = findbitset(1, rem, 2, ip);
            bitset<8> allone = bitset<8>().set(); 
            address[2] = x;
            address[3] = allone;
        }
    }
    else if (hostno>16 && hostno<=24) {
        int rem = hostno - 16;
        if (first) {
            bitset<8> x = findbitset(0, rem, 1, ip);
            bitset<8> allzero = bitset<8>().reset();  // 0
            address[1] = x;
            address[2] = allzero;
            address[3] = allzero;       
        } else {
            bitset<8> x = findbitset(1, rem, 1, ip);
            bitset<8> allone = bitset<8>().set(); 
            address[1] = x;
            address[2] = allone;
            address[3] = allone;
        }
    }
    else {
        int rem = hostno - 24;
        if (first) {
        bitset<8> x = findbitset(0, rem, 0, ip);
        bitset<8> allzero = bitset<8>().reset();  // 0
        address[0] = x;
        address[1] = allzero;
        address[2] = allzero;
        address[3] = allzero;
        } else {   
            bitset<8> x = findbitset(1, rem, 0, ip);
            bitset<8> allone = bitset<8>().set(); 
            address[0] = x;
            address[1] = allone;
            address[2] = allone;
            address[3] = allone;
        }
    }
    return address;
} 

vector<int> todecimal(vector<bitset<8>> binadd) {
    vector<int> address;
    for (auto bin: binadd) {
        address.push_back(bin.to_ulong());
    }
    return address;
}

void displayAddress(vector<int> add, vector<bitset<8>> addbin){
    for(int i=0; i<add.size(); i++) {
        if(i == add.size() - 1) {
            cout << add[i];
        } else {
            cout << add[i] << ".";
        }
    }
    cout << " (";
    for(int i=0; i<addbin.size(); i++) {
        if(i == addbin.size() - 1) {
            cout << addbin[i];
        } else {
            cout << addbin[i] << ".";
        }
    }
    cout << ")"<< endl;
}

int main() {
    string ip;
    int subnetNo;
    cout << "\nFind the IP subnet address (first and last) of given IPv4 address and subnet mask\n\n";
    cout<<"Enter the IP Address: ";
    cin>>ip;
    vector<string> slist = split(ip);
    bool IPValid;
    if (slist.size() != 4) {
        IPValid = false;
    } else {
        IPValid = validIPAddress(slist);
    }
    if (!IPValid) {
        cout << "\nIt is not a valid IPv4 address\n"; 
        return 0;
    }
    vector<bitset<8>> binIP = binaryIP(slist); 
    cout << "Enter the subnet: /";
    cin >> subnetNo;
    char ipClass = findClass(slist[0]);
    int hostNo = 32 - subnetNo;
    int noaddress = pow(2, hostNo);
    vector<bitset<8>> firstaddbin = find_address(hostNo, binIP, true);
    vector<bitset<8>> lastaddbin = find_address(hostNo, binIP, false);
    vector<int> firstadd = todecimal(firstaddbin);
    vector<int> lastadd = todecimal(lastaddbin);
    cout << "\nIP class is " << ipClass << endl;
    cout << "No of Address: " << noaddress << endl;
    cout << "\nFirst Address :- \n";
    displayAddress(firstadd, firstaddbin);
    cout << "\nLast Address :- \n";
    displayAddress(lastadd, lastaddbin);
    return 0;
}