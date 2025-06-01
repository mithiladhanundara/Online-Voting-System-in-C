// src/main.c
#include <stdio.h>
#include "../include/types.h"

void adminPanel();
void voterPanel();

int main() {
    int choice;
    do {
        printf("\n=== Online Voting System ===\n");
        printf("1. Admin Login\n");
        printf("2. Voter Login\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                adminPanel();
                break;
            case 2:
                voterPanel();
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while(choice != 0);

    return 0;
}
