#include <stdio.h>
#include <string.h>

#define MAX 100
#define MAX_CANDIDATES 100
#define POSITIONS 8

struct Student {
    char email[50];
    char password[50];
};

struct Candidate {
    char name[50];
    int position;
    int votes;
};

struct Student students[MAX];
struct Candidate candidates[MAX_CANDIDATES];

int studentCount = 0;
int candidateCount = 0;

int studentVotes[MAX][POSITIONS];
int hasVoted[MAX];

char positions[POSITIONS][40] = {
    "President",
    "Vice President",
    "Secretary",
    "Treasurer",
    "Auditor",
    "Public Information Officer",
    "Business Manager",
    "Senators"
};

int voteLimit[POSITIONS] = {1,1,1,1,1,1,2,5};

char adminEmail[] = "admin@cbsua.edu.ph";
char adminPass[] = "admin123";

void header(char title[]) {
    printf("\n============================================\n");
    printf(" %s\n", title);
    printf("============================================\n");
}

int loginStudent(char email[], char pass[]) {
    int i;
    for(i=0;i<studentCount;i++) {
        if(strcmp(students[i].email, email)==0 &&
           strcmp(students[i].password, pass)==0) {
            return i;
        }
    }
    return -1;
}

void viewCandidates() {
    int i;
    header("CANDIDATES");
    if(candidateCount == 0) {
        printf("No candidates yet.\n");
        return;
    }
    for(i=0;i<candidateCount;i++) {
        printf("%d. %s - %s\n",
               i+1,
               candidates[i].name,
               positions[candidates[i].position]);
    }
}

void registerStudent() {
    header("REGISTER");
    printf("Email: ");
    scanf("%s", students[studentCount].email);
    printf("Password: ");
    scanf("%s", students[studentCount].password);
    hasVoted[studentCount] = 0;
    studentCount++;
    printf("Registered!\n");
}

void showPositions() {
    int i;
    printf("\nPOSITIONS:\n");
    for(i=0;i<POSITIONS;i++) {
        printf("%d. %s\n", i+1, positions[i]);
    }
}

void addCandidate() {
    int pos;
    header("ADD CANDIDATE");
    showPositions();
    printf("\nSelect position: ");
    scanf("%d",&pos);
    if(pos < 1 || pos > POSITIONS) return;
    printf("Name: ");
    scanf(" %[^\n]", candidates[candidateCount].name);
    candidates[candidateCount].position = pos - 1;
    candidates[candidateCount].votes = 0;
    candidateCount++;
    printf("Added!\n");
}

void vote(int index) {
    int pos, i, choice;
    int valid[MAX_CANDIDATES];
    int count;

    header("VOTING");
    showPositions();

    printf("\nSelect position: ");
    scanf("%d",&pos);
    pos = pos - 1;

    if(pos < 0 || pos >= POSITIONS) return;

    if(studentVotes[index][pos] >= voteLimit[pos]) {
        printf("No remaining votes!\n");
        return;
    }

    printf("\n--- %s ---\n", positions[pos]);

    count = 0;

    for(i=0;i<candidateCount;i++) {
        if(candidates[i].position == pos) {
            printf("%d. %s\n", count+1, candidates[i].name);
            valid[count] = i;
            count++;
        }
    }

    if(count == 0) {
        printf("No candidates (ABSTAIN)\n");
        studentVotes[index][pos]++;
        hasVoted[index] = 1;
        return;
    }

    printf("\nChoose (0 = abstain): ");
    scanf("%d",&choice);

    if(choice == 0) {
        studentVotes[index][pos]++;
        hasVoted[index] = 1;
        printf("Abstained\n");
        return;
    }

    if(choice >= 1 && choice <= count) {
        candidates[valid[choice-1]].votes++;
        studentVotes[index][pos]++;
        hasVoted[index] = 1;
        printf("Vote recorded!\n");
    }
}

void showResults() {
    int i;
    header("RESULTS");
    for(i=0;i<candidateCount;i++) {
        printf("%s - %s : %d votes\n",
               candidates[i].name,
               positions[candidates[i].position],
               candidates[i].votes);
    }
}

void viewRegisteredStudents() {
    int i;
    header("REGISTERED STUDENTS");
    if(studentCount == 0) {
        printf("No students registered.\n");
        return;
    }
    for(i=0;i<studentCount;i++) {
        printf("%d. %s - %s\n",
               i+1,
               students[i].email,
               hasVoted[i] ? "VOTED" : "NOT VOTED");
    }
}

void studentMenu(int index) {
    int choice;
    do {
        header("STUDENT MENU");
        printf("1. View Candidates\n");
        printf("2. Vote\n");
        printf("3. Logout\n");
        scanf("%d",&choice);
        if(choice == 1) viewCandidates();
        else if(choice == 2) vote(index);
    } while(choice != 3);
}

void adminMenu() {
    int choice;
    do {
        header("ADMIN DASHBOARD");
        printf("1. Add Candidate\n");
        printf("2. View Results\n");
        printf("3. View Registered Students\n");
        printf("4. Logout\n");
        scanf("%d",&choice);
        if(choice == 1) addCandidate();
        else if(choice == 2) showResults();
        else if(choice == 3) viewRegisteredStudents();
    } while(choice != 4);
}

void login() {
    char email[50], pass[50];
    int index;

    header("LOGIN");

    printf("Email: ");
    scanf("%s", email);

    printf("Password: ");
    scanf("%s", pass);

    if(strcmp(email, adminEmail)==0 &&
       strcmp(pass, adminPass)==0) {
        adminMenu();
        return;
    }

    index = loginStudent(email, pass);

    if(index != -1)
        studentMenu(index);
    else
        printf("Invalid login!\n");
}

int main() {
    int choice;

    do {
        header("QUICKVOTE SYSTEM");
        printf("1. Login\n2. Register\n3. Exit\n");
        scanf("%d",&choice);

        if(choice == 1) login();
        else if(choice == 2) registerStudent();
    } while(choice != 3);

    return 0;
}
