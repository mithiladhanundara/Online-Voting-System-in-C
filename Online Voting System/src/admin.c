// src/admin.c

#include <stdio.h>
#include <string.h>
#include "../include/types.h"
#include "../include/admin.h"

#define ADMIN_PASS "admin123"
#define VOTER_FILE "data/voters.dat"
#define CANDIDATE_FILE "data/candidates.dat"

// Admin authentication
int adminLogin() {
    char pass[PASS_LEN];
    printf("Enter admin password: ");
    scanf("%s", pass);
    if(strcmp(pass, ADMIN_PASS) == 0) {
        printf("Login successful!\n");
        return 1;
    } else {
        printf("Incorrect password.\n");
        return 0;
    }
}

// Register a new candidate
void registerCandidate() {
    Candidate c;
    FILE *fp = fopen(CANDIDATE_FILE, "ab");
    if (!fp) {
        printf("Error opening candidates file!\n");
        return;
    }
    printf("--- Register Candidate ---\n");
    printf("Enter candidate name: ");
    scanf("%49s", c.name);
    printf("Enter candidate ID (number): ");
    scanf("%d", &c.id);
    c.votes = 0;
    fwrite(&c, sizeof(Candidate), 1, fp);
    fclose(fp);
    printf("Candidate registered successfully!\n");
}

// List candidates
void listCandidates() {
    Candidate c;
    FILE *fp = fopen(CANDIDATE_FILE, "rb");
    if (!fp) {
        printf("No candidates registered yet.\n");
        return;
    }
    printf("\n--- List of Candidates ---\n");
    printf("ID\tName\n");
    while(fread(&c, sizeof(Candidate), 1, fp)) {
        printf("%d\t%s\n", c.id, c.name);
    }
    fclose(fp);
}

// Show election results
void showResults() {
    Candidate c, winner;
    int found = 0;
    FILE *fp = fopen(CANDIDATE_FILE, "rb");
    if (!fp) {
        printf("No candidates registered yet.\n");
        return;
    }
    printf("\n--- Election Results ---\n");
    printf("ID\tName\tVotes\n");
    while(fread(&c, sizeof(Candidate), 1, fp)) {
        printf("%d\t%s\t%d\n", c.id, c.name, c.votes);
        if (!found || c.votes > winner.votes) {
            winner = c;
            found = 1;
        }
    }
    fclose(fp);
    if (found)
        printf("Winner: %s (ID: %d) with %d votes\n", winner.name, winner.id, winner.votes);
    else
        printf("No votes cast yet.\n");
}

// Delete a voter by ID
void deleteVoter() {
    char targetId[ID_LEN];
    printf("Enter Voter ID to delete: ");
    scanf("%19s", targetId);

    FILE *fp = fopen(VOTER_FILE, "rb");
    FILE *temp = fopen("data/voters_temp.dat", "wb");
    if (!fp || !temp) {
        printf("Error opening files.\n");
        return;
    }

    Voter v;
    int found = 0;
    while (fread(&v, sizeof(Voter), 1, fp)) {
        if (strcmp(v.id, targetId) != 0) {
            fwrite(&v, sizeof(Voter), 1, temp);
        } else {
            found = 1;
        }
    }
    fclose(fp);
    fclose(temp);

    remove(VOTER_FILE);
    rename("data/voters_temp.dat", VOTER_FILE);

    if (found)
        printf("Voter deleted successfully.\n");
    else
        printf("Voter ID not found.\n");
}

// Delete a candidate by ID
void deleteCandidate() {
    int targetId;
    printf("Enter Candidate ID to delete: ");
    scanf("%d", &targetId);

    FILE *fp = fopen(CANDIDATE_FILE, "rb");
    FILE *temp = fopen("data/candidates_temp.dat", "wb");
    if (!fp || !temp) {
        printf("Error opening files.\n");
        return;
    }

    Candidate c;
    int found = 0;
    while (fread(&c, sizeof(Candidate), 1, fp)) {
        if (c.id != targetId) {
            fwrite(&c, sizeof(Candidate), 1, temp);
        } else {
            found = 1;
        }
    }
    fclose(fp);
    fclose(temp);

    remove(CANDIDATE_FILE);
    rename("data/candidates_temp.dat", CANDIDATE_FILE);

    if (found)
        printf("Candidate deleted successfully.\n");
    else
        printf("Candidate ID not found.\n");
}

// Export results to a text file (optional feature)
void exportResults() {
    Candidate c;
    FILE *fp = fopen(CANDIDATE_FILE, "rb");
    FILE *out = fopen("data/results.txt", "w");
    if (!fp || !out) {
        printf("Error exporting results.\n");
        return;
    }
    fprintf(out, "ID\tName\tVotes\n");
    while(fread(&c, sizeof(Candidate), 1, fp)) {
        fprintf(out, "%d\t%s\t%d\n", c.id, c.name, c.votes);
    }
    fclose(fp);
    fclose(out);
    printf("Results exported to data/results.txt\n");
}

// Admin panel menu
void adminPanel() {
    if(!adminLogin()) return;
    int choice;
    do {
        printf("\n--- Admin Panel ---\n");
        printf("1. Register Candidate\n");
        printf("2. List Candidates\n");
        printf("3. Show Results\n");
        printf("4. Delete Voter\n");
        printf("5. Delete Candidate\n");
        printf("6. Export Results\n");
        printf("0. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: registerCandidate(); break;
            case 2: listCandidates(); break;
            case 3: showResults(); break;
            case 4: deleteVoter(); break;
            case 5: deleteCandidate(); break;
            case 6: exportResults(); break;
            case 0: printf("Logging out...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while(choice != 0);
}
