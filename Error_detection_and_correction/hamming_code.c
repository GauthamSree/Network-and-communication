/**
    Code For :- Generation and Correction of Hamming code.
    @file hamming_code.c
    @author Gautham Sreekumar (GauthamSree)
*/

#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#define MAX_SIZE 32

int inp[MAX_SIZE], code[MAX_SIZE];

int EvenParity(int count) {
    if (count % 2 == 0) 
        return 0; 
    else
        return 1; 
}

int hammingCode(int pos, int tbits) {
    int count = 0; 
    int i = pos - 1; 
    while (i < tbits) { 
        for (int j = i; j < i + pos; j++) { 
            if (code[j] == 1) 
                count++; 
        } 
        i += 2 * pos; 
    }
    return count;
}

void makeHammingCode(int r, int tbits) {
    for (int i = 0; i < r; i++) { 
        int pos = (int)pow(2, i); 
        int count = hammingCode(pos, tbits); 
        int val = EvenParity(count);
        code[pos - 1] = val; 
    } 
}

int findRedundant(int m, int correction) {
    int r = 0;
    if(!correction) {
        while((int)pow(2, r) < m + r + 1) {
            r++;
        }
    }
    else {
        while((int)pow(2, r) < m + 1) {
            r++;
        }
    }
    return r;
}

void displayCode(int tbits, int r) {
    for(int i=0; i < tbits; i++) {
        printf("%d ", code[i]);
    }
    printf("\nRedundant bits: %d\n", r);
}

void HammingCorrection(int r, int tbits) {
    int par[r], error=0, errorpos = 0;
    for (int i = 0; i < r; i++) { 
        int pos = (int)pow(2, i); 
        int count = hammingCode(pos, tbits); 
        int val = EvenParity(count);
        par[r-i-1] = val;
        if(val == 1) {
            errorpos += pos; 
            error = 1;
        } 
    } 
    if(error == 0) {
        printf("\nThere is no error.\n");        
    } else {
        printf("\nThere is an Error at postion %d.\n", errorpos);
        if(code[errorpos-1] == 0) {
            code[errorpos-1] = 1;
        } else {
            code[errorpos-1] = 0;
        }
        printf("\nCorrected Code: ");
        displayCode(tbits, r);
    }
}

void DisplayOrginalMessage(int tbits) {
    int rdBit = 0;
    printf("\nOriginal Message: ");
    for (int i = 0; i < tbits; i++) {
        int pos = (int)pow(2, rdBit) - 1;
        if(i == pos) {
            rdBit++;
            continue;
        }
        printf("%d ", code[i]);
    } 
    printf("\n");
}

int main() {
    int ch;
    printf("\nGeneration and Correction of Hamming code\n\n");
    while (1) {
        printf("\n\tMenu");
        printf("\n\t----\n");
        printf("1.Hamming Code\n");
        printf("2.Validate and Correct Hamming Code\n");
        printf("3.Exit\n\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1:
                {
                    int m;
                    printf("Enter no of data bits: ");
                    scanf("%d", &m);
                    printf("Enter the data bits: ");
                    for(int i=0; i<m; i++) {
                        scanf("%d", &inp[i]);
                    }
                    int r = findRedundant(m, 0);
                    int tbits = m + r;
                    int pos = 0, k = 0;
                    for(int i=0; i < tbits; i++) {
                        if( i == ((int)pow(2, k) - 1)){
                            code[i] = -1;
                            k++;
                        } else {
                            code[i] = inp[pos++];
                        }
                    }
                    makeHammingCode(r, tbits);
                    printf("\nThe Hamming Code: ");
                    displayCode(tbits, r);
                    break;
                }
            case 2:
                {
                    int m;
                    printf("Enter no of data bits: ");
                    scanf("%d", &m);
                    int r = findRedundant(m, 1);
                    printf("Enter the data bits: ");
                    for(int i=0; i<m; i++) {
                        scanf("%d", &code[i]);
                    }
                    int pos = 0, k = 0;
                    HammingCorrection(r, m);
                    DisplayOrginalMessage(m);
                    break;
                }
            case 3:
                {
                    exit(0);
                }
            default:
                {
                    printf("Wrong choice... Try Again!");
                    break;
                }
            }
    }
    return 0;
}
