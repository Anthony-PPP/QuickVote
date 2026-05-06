#include <stdio.h>
#include <string.h>

//max number ng students saka candidates
#define MAX 100
#define MAX_CANDIDATES 100
#define POSITIONS 8

//STRUCTURES

// student info (email + password)
struct Student {
    char email[50];
    char password[50];
};

// candidate info (name, position, votes)
struct Candidate {
    char name[50];
    int position;
    int votes;
};


// list ng students
struct Student students[MAX];

// list ng candidates
struct Candidate candidates[MAX_CANDIDATES];

// tiga bilang
int studentCount = 0;
int candidateCount = 0;

// track votes per student per position
int studentVotes[MAX][POSITIONS];

// track if student already voted candidate
int votedCandidate[MAX][MAX_CANDIDATES];

// track if student already voted
int hasVoted[MAX];

// abstain counter
int abstainCount[POSITIONS];

// list of positions
char positions[POSITIONS][40] = {
    "President","Vice President","Secretary","Treasurer",
    "Auditor","Public Information Officer","Business Manager","Senators"
};

// voting limit per position
int voteLimit[POSITIONS] = {1,1,1,1,1,1,2,5};

// admin account
char adminEmail[] = "admin@cbsua.edu.ph";
char adminPass[] = "admin123";

//input validator
int getInt() {
    int x;
    char ch;

// try to read number
    if (scanf("%d", &x) != 1) {
        printf("Invalid input!\n");

// clear wrong input
        while ((ch = getchar()) != '\n' && ch != EOF);
        return -1;
    }

    return x;
}

//login checck
int loginStudent(char e[], char p[]) {
    int i;

// check all students
    for(i = 0; i < studentCount; i++) {
        if(strcmp(students[i].email, e) == 0 &&
           strcmp(students[i].password, p) == 0)
            return i;
    }

    return -1;
}

//show candidates
void viewCandidates() {
    int i, j, num;

    num = 1;

    printf("\n=== CANDIDATES ===\n");

// if no candidates
    if(candidateCount == 0) {
        printf("No candidates yet.\n");
        return;
    }

// show grouped by position
    for(i = 0; i < POSITIONS; i++) {
        for(j = 0; j < candidateCount; j++) {
            if(candidates[j].position == i) {
                printf("%d. %s - %s\n",
                       num,
                       candidates[j].name,
                       positions[i]);
                num++;
            }
        }
    }
}

//register student
void registerStudent() {
    int i, j;

    printf("\n=== REGISTER ===\n");

    printf("Email: ");
    scanf("%s", students[studentCount].email);

    printf("Password: ");
    scanf("%s", students[studentCount].password);

// reset voting status
    hasVoted[studentCount] = 0;

    for(i = 0; i < POSITIONS; i++)
        studentVotes[studentCount][i] = 0;

    for(j = 0; j < MAX_CANDIDATES; j++)
        votedCandidate[studentCount][j] = 0;

    studentCount++;

    printf("Registered!\n");
}

// show positions
void showPositions() {
    int i;

    printf("\nPOSITIONS:\n");

    for(i = 0; i < POSITIONS; i++)
        printf("%d. %s\n", i + 1, positions[i]);
}

//add candidate
void addCandidate() {
    int pos;

    printf("\n=== ADD CANDIDATE ===\n");

    showPositions();

    printf("\nSelect position: ");
    pos = getInt();
    if(pos == -1) return;

    if(pos < 1 || pos > POSITIONS) return;

    printf("Name: ");
    scanf(" %[^\n]", candidates[candidateCount].name);

    candidates[candidateCount].position = pos - 1;

    candidates[candidateCount].votes = 0;

    candidateCount++;

    printf("Added!\n");
}


void vote(int idx) {
    int pos, i, choice, count;
    int valid[MAX_CANDIDATES];

    count = 0;

    printf("\n=== VOTING ===\n");

    showPositions();

    printf("\nSelect position: ");
    pos = getInt();
    if(pos == -1) return;

    pos--;

    if(pos < 0 || pos >= POSITIONS) return;

// check vote limit per position
    if(studentVotes[idx][pos] >= voteLimit[pos]) {
        printf("No remaining votes!\n");
        return;
    }

    printf("\n--- %s ---\n", positions[pos]);

// list candidates in the position
    for(i = 0; i < candidateCount; i++) {
        if(candidates[i].position == pos) {
            printf("%d. %s\n", count + 1, candidates[i].name);
            valid[count] = i;
            count++;
        }
    }

    if(count == 0) {
        printf("No candidates\n");
        studentVotes[idx][pos]++;
        abstainCount[pos]++;
        hasVoted[idx] = 1;
        return;
    }

    printf("\nChoose (0 = abstain): ");
    choice = getInt();
    if(choice == -1) return;

// abstain option
    if(choice == 0) {
        printf("Abstained\n");
        studentVotes[idx][pos]++;
        abstainCount[pos]++;
        hasVoted[idx] = 1;
        return;
    }

// valid vote
    if(choice >= 1 && choice <= count) {

        int selected = valid[choice - 1];

// prevent double vote per candidate
        if(votedCandidate[idx][selected] == 0) {
            candidates[selected].votes++;
            votedCandidate[idx][selected] = 1;
            studentVotes[idx][pos]++;
            hasVoted[idx] = 1;

            printf("Vote recorded!\n");
        }
        else {
            printf("Already voted this candidate!\n");
        }
    }
}

//show results
void showResults() {
    int i, j;

    printf("\n=== RESULTS ===\n");

    if(candidateCount == 0) {
        printf("No candidates yet.\n");
        return;
    }

// show results grouped by position
    for(i = 0; i < POSITIONS; i++) {
        for(j = 0; j < candidateCount; j++) {
            if(candidates[j].position == i) {
                printf("%s - %s : %d votes\n",
                       candidates[j].name,
                       positions[i],
                       candidates[j].votes);
            }
        }

//abstain per position
        printf("ABSTAIN - %s : %d\n", positions[i], abstainCount[i]);
    }
}

// show students
void viewRegisteredStudents() {
    int i;

    printf("\n=== STUDENTS ===\n");

    if(studentCount == 0) {
        printf("No students registered.\n");
        return;
    }

    for(i = 0; i < studentCount; i++) {
        printf("%d. %s - %s\n",
               i + 1,
               students[i].email,
               hasVoted[i] ? "VOTED" : "NOT VOTED");
    }
}


void studentMenu(int idx) {
    int c;

    do {
        printf("\n--- STUDENT MENU ---\n");
        printf("1. View Candidates\n2. Vote\n3. Logout\nChoice:");

        c = getInt();
        if(c == -1) continue;

        if(c == 1) viewCandidates();
        else if(c == 2) vote(idx);

    } while(c != 3);
}

void adminMenu() {
    int c;

    do {
        printf("\n--- ADMIN MENU ---\n");
        printf("1. Add Candidate\n2. View Results\n3. View Students\n4. Logout\nChoice:");

        c = getInt();
        if(c == -1) continue;

        if(c == 1) addCandidate();
        else if(c == 2) showResults();
        else if(c == 3) viewRegisteredStudents();

    } while(c != 4);
}

void login() {
    char e[50], p[50];
    int idx;

    printf("\n=== LOGIN ===\n");

    printf("Email: ");
    scanf("%s", e);

    printf("Password: ");
    scanf("%s", p);

// check admin
    if(strcmp(e, adminEmail) == 0 &&
       strcmp(p, adminPass) == 0) {
        adminMenu();
        return;
    }

// check student
    idx = loginStudent(e, p);

    if(idx != -1) studentMenu(idx);
    else printf("Invalid login!\n");
}

int main() {
    int c;

    do {
        printf("\n==============================\n");
        printf("     QUICKVOTE SYSTEM\n");
        printf("==============================\n");

        printf("1. Login\n2. Register\n3. Exit\nChoice:");

// get user menu choice
        c = getInt();

// if input is invalid, go back to menu
        if(c == -1) continue;

// if user chooses login
        if(c == 1) login();

// if user chooses register
        else if(c == 2) registerStudent();

// repeat menu until user chooses exit (3)
    } while(c != 3);

    return 0;
}
