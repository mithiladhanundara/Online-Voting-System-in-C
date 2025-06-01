// include/types.h
#ifndef TYPES_H
#define TYPES_H

#define NAME_LEN 50
#define ID_LEN 20
#define PASS_LEN 20

typedef struct {
    int id;
    char name[NAME_LEN];
    int votes;
} Candidate;

typedef struct {
    char id[ID_LEN];
    char password[PASS_LEN];
    int hasVoted;
} Voter;

#endif
