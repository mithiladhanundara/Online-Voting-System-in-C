// src/voter.c

#include <stdio.h>
#include <string.h>
#include "../include/types.h"
#include "../include/voter.h"

#define VOTER_FILE "data/voters.dat"
#define CANDIDATE_FILE "data/candidates.dat"

// Register a new voter
void registerVoter() {
    Voter v;
    FILE *fp = fopen(VOTER_FILE, "ab");
    if (!fp) {
        printf("Error opening voter file!\n");
        return;
    }
    printf("Enter new voter ID: ");
    scanf("%s", v.id);
    printf("Enter password: ");
    scanf("%s", v.password);
    v.hasVoted = 0;
    fwrite(&v, sizeof(Voter), 1, fp);
    fclose(fp);
    printf("Voter registered successfully!\n");
}

// Voter login: returns 1 if successful, 0 otherwise. Copies ID to voterId.
int voterLogin(char* voterId) {
    char id[ID_LEN], password[PASS_LEN];
    Voter v;
    int found = 0;

    printf("Enter Voter ID: ");
    scanf("%s", id);
    printf("Enter Password: ");
    scanf("%s", password);

    FILE *fp = fopen(VOTER_FILE, "rb");
    if (!fp) {
        printf("No voters registered yet.\n");
        return 0;
    }

    while(fread(&v, sizeof(Voter), 1, fp)) {
        if(strcmp(v.id, id) == 0 && strcmp(v.password, password) == 0) {
            found = 1;
            strcpy(voterId, v.id);
            break;
        }
    }
    fclose(fp);

    if (found) {
        printf("Login successful!\n");
        return 1;
    } else {
        printf("Invalid ID or password.\n");
        return 0;
    }
}

// Cast a vote (only if the voter hasn't voted yet)
void castVote(const char* voterId) {
    Voter v;
    int found = 0, idx = 0, i = 0;

    // First, check if voter has already voted
    FILE *fp = fopen(VOTER_FILE, "rb+");
    if (!fp) {
        printf("Error opening voter file!\n");
        return;
    }

    // Find voter record and check hasVoted
    while(fread(&v, sizeof(Voter), 1, fp)) {
        if(strcmp(v.id, voterId) == 0) {
            found = 1;
            break;
        }
        idx++;
    }

    if (!found) {
        printf("Voter not found!\n");
        fclose(fp);
        return;
    }

    if (v.hasVoted) {
        printf("You have already voted. Cannot vote again.\n");
        fclose(fp);
        return;
    }

    // List candidates
    Candidate cands[100];
    int count = 0;
    FILE *cfp = fopen(CANDIDATE_FILE, "rb+");
    if (!cfp) {
        printf("No candidates available.\n");
        fclose(fp);
        return;
    }
    printf("\n--- Candidates ---\n");
    printf("ID\tName\n");
    while(fread(&cands[count], sizeof(Candidate), 1, cfp)) {
        printf("%d\t%s\n", cands[count].id, cands[count].name);
        count++;
    }

    // Ask for candidate ID
    int cid, voted = 0;
    printf("Enter candidate ID to vote for: ");
    scanf("%d", &cid);

    // Find candidate and increment votes
    rewind(cfp);
    for(i = 0; i < count; i++) {
        if(cands[i].id == cid) {
            cands[i].votes++;
            voted = 1;
            break;
        }
    }

    if (!voted) {
        printf("Invalid candidate ID.\n");
        fclose(fp);
        fclose(cfp);
        return;
    }

    // Write updated candidate data
    rewind(cfp);
    fwrite(cands, sizeof(Candidate), count, cfp);
    fclose(cfp);

    // Mark voter as voted
    v.hasVoted = 1;
    fseek(fp, idx * sizeof(Voter), SEEK_SET);
    fwrite(&v, sizeof(Voter), 1, fp);
    fclose(fp);

    printf("Vote cast successfully!\n");
}

// Voter panel menu
void voterPanel() {
    int choice;
    char voterId[ID_LEN];

    do {
        printf("\n--- Voter Panel ---\n");
        printf("1. Register as Voter\n");
        printf("2. Login and Vote\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                registerVoter();
                break;
            case 2:
                if(voterLogin(voterId)) {
                    castVote(voterId);
                }
                break;
            case 0:
                printf("Returning to main menu...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while(choice != 0);
}
