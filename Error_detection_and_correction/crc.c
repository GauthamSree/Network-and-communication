/**
    Code For :- Generation and Validation of Cyclic Redundancy Check (CRC) code
    @file crc.c
    @author Gautham Sreekumar (GauthamSree)
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 20

void division(int* data, int divisor[MAX_SIZE], int message, int div) {
    for(int i=0; i<message; i++)
    {
	    if(data[i] == 1)
		    for(int j=0; j<div; j++)
			    data[i+j] ^= divisor[j];
    }
}

int validate(int data[], int n) {
    int valid = 1;
    for(int i=0; i<n; i++) {
	    if(data[i] == 1){
		    valid = 0;
		    break;  
        }
    }
    return valid;
}

void display(int data[], int n) {
    for(int i=0; i<n; i++) {
	    printf("%d ", data[i]); 
    }
    printf("\n");    
}

int main() {
    printf("\nGeneration and Validation of CRC code\n\n");
    while(1) {
        int ch;
        printf("\n\tMenu");
        printf("\n\t----\n");
        printf("1.Generate CRC Code\n");
        printf("2.Validate CRC Code\n");
        printf("3.Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: 
            {
                int lenofmess, lenofdiv;
                printf("Enter length of Message: ");
                scanf("%d", &lenofmess);
                printf("Enter length of Divisor: ");
                scanf("%d", &lenofdiv);
                int n = lenofmess + lenofdiv - 1;
                int data[n], rem[n]; 
                int divisor[lenofdiv]; 
                printf("Enter message bits: ");
                for(int i=0; i<lenofmess; i++) {
                    scanf("%d", &data[i]);
                    rem[i] = data[i];
                }
                printf("Enter Divisor: ");
                for(int i=0; i<lenofdiv; i++) {
                    scanf("%d", &divisor[i]);
                }
                for(int i=lenofmess; i<n; i++) {
                    data[i] = 0;
                    rem[i] = 0;
                }
                division(rem, divisor, lenofmess, lenofdiv);
                for(int i=lenofmess; i<n; i++) {
                    data[i] = rem[i];
                }
                printf("\nCRC Code: ");
                display(data, n);
                break;
            }
            case 2:
            {
                int n, lenofdiv;
                printf("Enter length of Data: ");
                scanf("%d", &n);
                printf("Enter length of Divisor: ");
                scanf("%d", &lenofdiv);
                int lenofmess = n - lenofdiv + 1;
                int data[n], rem[n];
                int divisor[lenofdiv];
                printf("Enter message bits: ");
                for(int i=0; i<n; i++) {
                    scanf("%d", &data[i]);
                    rem[i] = data[i];
                }
                printf("Enter generator: ");
                for(int i=0; i<lenofdiv; i++) {
                    scanf("%d", &divisor[i]);
                }
                division(rem, divisor, lenofmess, lenofdiv);
                int valid = validate(rem, n);
                if(valid==1) {
                    printf("\nData is valid\nOriginal Message: ");
                    display(data, lenofmess);
                }
                else 
                    printf("\nData is invalid. CRC error occurred.\n");
                break;
            }
            case 3:
            {
                exit(0);
            }
            default:
            {
                printf("Wrong Choice. Try Again!\n");
                break;
            }
        }
    }
    return 0;
}