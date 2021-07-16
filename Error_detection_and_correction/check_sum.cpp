/**
    Code For :- Generation and Validation of Checksum.
    @file check_sum.cpp
    @author Gautham Sreekumar (GauthamSree)
*/


#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#define MAX_SIZE 20

struct Addition {
    int sum[MAX_SIZE];
    int c;
} Result;
 
int add[MAX_SIZE];

void initAdd(int n) {
    for(int j=0; j<n; j++) {
        if(j == n-1) {
            add[j] = 1;
        }
        else {
            add[j] = 0;
        }
    }
}

void check(struct Addition& r, int n) {
    if (r.c == 1) {
        r.c = 0;
        for(int i = n - 1; i >= 0; i--) {
            int sum = ((add[i] ^ r.sum[i]) ^ r.c);
            r.c = ((add[i] & r.sum[i]) | (add[i] & r.c)) | (r.sum[i] & r.c);
            r.sum[i] = sum; 
        }
        check(r, n);
    } else {
        return;
    }
}
void BinaryAddition(struct Addition& r, vector< vector<int> > a) {
    r.c = 0;
    int n =  a[0].size();
    for(int i = n - 1; i >= 0 ; i--) {
        r.sum[i] = ((a[0][i] ^ a[1][i]) ^ r.c);
        r.c = ((a[0][i] & a[1][i]) | (a[0][i] & r.c)) | (a[1][i] & r.c); 
    }
    initAdd(n);
    check(r, n);
    for(int i = 2; i < a.size() ; i++) {
        for(int j = n - 1; j >= 0 ; j--) {
            int sum = ((a[i][j] ^ r.sum[j]) ^ r.c);
            r.c = ((a[i][j] & r.sum[j]) | (a[i][j] & r.c)) | (r.sum[j] & r.c);
            r.sum[j] = sum; 
        }
        check(r, n);
    }
}

void complement(struct Addition r, int* comp, int len) {
    for(int i=0; i<len; i++) {
        if(r.sum[i] == 0) {
            *(comp + i) = 1;
        } else {
            *(comp + i) = 0;
        }
    }
}

void display(int arr[], int m) {
    for(int i = 0; i<m; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

bool ChecksumValidation(int comp[], int m) {
    for(int i=0; i<m; i++) {
        if(comp[i] == 1) {
            return false;
        }
    }
    return true;
}

map<char, vector<int> > hextobin { 
    {'0', {0, 0, 0, 0}}, {'1', {0, 0, 0, 1}}, {'2', {0, 0, 1, 0}}, {'3', {0, 0, 1, 1}},
    {'4', {0, 1, 0, 0}}, {'5', {0, 1, 0, 1}}, {'6', {0, 1, 1, 0}}, {'7', {0, 1, 1, 1}},
    {'8', {1, 0, 0, 0}}, {'9', {1, 0, 0, 1}}, {'A', {1, 0, 1, 0}}, {'B', {1, 0, 1, 1}},
    {'C', {1, 1, 0, 0}}, {'D', {1, 1, 0, 1}}, {'E', {1, 1, 1, 0}}, {'F', {1, 1, 1, 1}}
};
map<vector<int>, char > bintohex { 
    {{0, 0, 0, 0}, '0'}, {{0, 0, 0, 1}, '1'}, {{0, 0, 1, 0}, '2'}, {{0, 0, 1, 1}, '3'},
    {{0, 1, 0, 0}, '4'}, {{0, 1, 0, 1}, '5'}, {{0, 1, 1, 0}, '6'}, {{0, 1, 1, 1}, '7'},
    {{1, 0, 0, 0},'8'}, {{1, 0, 0, 1}, '9'}, {{1, 0, 1, 0}, 'A'}, {{1, 0, 1, 1}, 'B'},
    {{1, 1, 0, 0}, 'C'}, {{1, 1, 0, 1}, 'D'}, {{1, 1, 1, 0}, 'E'}, {{1, 1, 1, 1}, 'F'}
};

vector<int> StringToBinary(string s) {
    vector<int> vec;
    for(int i=0; i < s.length(); i++) {
        vector<int> vc = hextobin[toupper(s[i])];
        vec.insert(vec.end(), vc.begin(), vc.end());
    }
    return vec;
}

string BinaryToString(vector<int> vec) {
    int n = vec.size()/4;
    string s = "";
    auto first = vec.cbegin();
    auto last = vec.cbegin() + 4;
    for(int i=0; i < n; i++) {
        vector<int> tmp(first, last);
        s += bintohex[tmp];
        first = last;
        last = last + 4;
    }
    return s;
}

void GenerateMenu() {
    int ch;
    cout << "\n\tGenerate CheckSum";
    cout << "\n\t-----------------\n";
    cout << "1.Binary\n";
    cout << "2.HexaDecimal\n";
    cout << "3.Go Back\n\n";
    cout << "Enter choice: ";
    cin >> ch;
    
    switch (ch) {
        case 1:
        {
            int n, m, comp[MAX_SIZE];
            cout << "Enter number of data: ";
            cin >> n;
            cout << "Enter length of each data: ";
            cin >> m;
            vector< vector<int> > vec(n, vector<int> (m));
            for(int i = 0; i < n; i++)
            {
                cout << "Enter " << (i+1) << "th data: ";
                for(int j = 0; j < m; j++)
                {
                    cin >> vec[i][j];
                }
            }
            BinaryAddition(Result, vec);
            complement(Result, comp, m);
            cout << "\nChecksum: ";
            display(comp, m);
            break;
        }
        case 2:
        {
            int n, comp[MAX_SIZE];
            cout << "Enter number of data: ";
            cin >> n;
            string s[n];
            vector< vector<int> > vec;
            for(int i = 0; i < n; i++) {
                cout << "Enter " << (i+1) << "th Data: ";
                cin >> s[i];
                vector<int> tmp = StringToBinary(s[i]);
                vec.push_back(tmp);
            }
            int m = vec[0].size();
            BinaryAddition(Result, vec);
            complement(Result, comp, m);
            vector<int> tmp(comp, comp + m);
            string finalRes = BinaryToString(tmp);
            cout << "\nChecksum (in Hex): " << finalRes << endl;
            cout << "Checksum (in Binary): ";
            display(comp, m);
            break;
        }
        case 3:
        {
            return;
        }
        default:
        {
            cout << "Wrong choice... Try Again!";
            break;
        }
    }
}

void ValidationMenu() {
    int ch;
    cout << "\n\tValidate CheckSum";
    cout << "\n\t-----------------\n";
    cout << "1.Binary\n";
    cout << "2.HexaDecimal\n";
    cout << "3.Go Back\n\n";
    cout << "Enter choice: ";
    cin >> ch;
    
    switch (ch) {
        case 1:
        {
            int n, m, comp[MAX_SIZE];
            cout << "Enter number of data: ";
            cin >> n;
            cout << "Enter length of each data: ";
            cin >> m;
            vector< vector<int> > vec(n, vector<int> (m));
            for(int i = 0; i < n; i++)
            {
                cout << "Enter " << (i+1) << "th Data: ";
                for(int j = 0; j < m; j++)
                {
                    cin >> vec[i][j];
                }
            }
            BinaryAddition(Result, vec);
            complement(Result, comp, m);
            cout << "\nSum: ";
            display(comp, m);
            bool isValid = ChecksumValidation(comp, m);
            if(isValid) {
                cout << "\nData is correct.\n";
            }
            else {
                cout << "\nData is incorrect.\n";
            }
            break;
        }
        case 2:
        {
            int n, comp[MAX_SIZE];
            cout << "Enter number of data: ";
            cin >> n;
            string s[n];
            vector< vector<int> > vec;
            for(int i = 0; i < n; i++) {
                cout << "Enter " << (i+1) << "th Data: ";
                cin >> s[i];
                vector<int> tmp = StringToBinary(s[i]);
                vec.push_back(tmp);
            }
            int m = vec[0].size();
            BinaryAddition(Result, vec);
            complement(Result, comp, m);
            cout << "\nSum: ";
            display(comp, m);
            bool isValid = ChecksumValidation(comp, m);
            if(isValid) {
                cout << "\nData is correct.\n";
            }
            else {
                cout << "\nData is incorrect.\n";
            }
            break;
        }
        case 3:
        {
            return;
        }
        default:
        {
            cout << "Wrong choice... Try Again!";
            break;
        }
    }
}

int main() {
    int ch;
    printf("\nGeneration and Validation of Checksum\n\n");
    while (1) {  
        cout << "\n\tMenu";
        cout << "\n\t----\n";
        cout << "1.Find CheckSum\n";
        cout << "2.Validate CheckSum\n";
        cout << "3.Exit\n\n";
        cout << "Enter choice: ";
        cin >> ch;

        switch (ch) {
            case 1:
            {
                GenerateMenu();
                break;
            }
            case 2:
            {
                ValidationMenu();
                break; 
            }
            case 3:
            {
                exit(0);
            }
            default:
            {
                cout << "Wrong choice... Try Again!";
                break;
            }
        }
    }
    return 0; 
}

    