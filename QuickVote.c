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
    "President","Vice President","Secretary","Treasurer",
    "Auditor","Public Information Officer","Business Manager","Senators"
};

int voteLimit[POSITIONS] = {1,1,1,1,1,1,2,5};

char adminEmail[] = "admin@cbsua.edu.ph";
char adminPass[] = "admin123";

int loginStudent(char e[], char p[]) {
    int i;
    for(i=0;i<studentCount;i++) {
        if(strcmp(students[i].email,e)==0 && strcmp(students[i].password,p)==0)
            return i;
    }
    return -1;
}

void viewCandidates() {
    int i;

    printf("\n==============================\n");
    printf("        CANDIDATES\n");
    printf("==============================\n");

    if(candidateCount==0){
        printf("No candidates yet.\n");
        return;
    }

    for(i=0;i<candidateCount;i++)
        printf("%d. %s - %s\n", i+1, candidates[i].name, positions[candidates[i].position]);
}

void registerStudent() {
    int i;

    printf("\n==============================\n");
    printf("          REGISTER\n");
    printf("==============================\n");

    printf("Email: ");
    scanf("%s", students[studentCount].email);

    printf("Password: ");
    scanf("%s", students[studentCount].password);

    hasVoted[studentCount] = 0;

    for(i=0;i<POSITIONS;i++)
        studentVotes[studentCount][i] = 0;

    studentCount++;

    printf("Registered!\n");
}

void showPositions() {
    int i;
    printf("\nPOSITIONS:\n");
    for(i=0;i<POSITIONS;i++)
        printf("%d. %s\n", i+1, positions[i]);
}

void addCandidate() {
    int pos;

    printf("\n==============================\n");
    printf("       ADD CANDIDATE\n");
    printf("==============================\n");

    showPositions();

    printf("\nSelect position: ");
    scanf("%d",&pos);

    if(pos<1||pos>POSITIONS) return;

    printf("Name: ");
    scanf(" %[^\n]", candidates[candidateCount].name);

    candidates[candidateCount].position = pos-1;
    candidates[candidateCount].votes = 0;

    candidateCount++;

    printf("Added!\n");
}

void vote(int idx) {
    int pos,i,choice,count=0;
    int valid[MAX_CANDIDATES];

    printf("\n==============================\n");
    printf("          VOTING\n");
    printf("==============================\n");

    showPositions();

    printf("\nSelect position: ");
    scanf("%d",&pos);

    pos--;

    if(pos<0||pos>=POSITIONS) return;

    if(studentVotes[idx][pos] >= voteLimit[pos]) {
        printf("No remaining votes!\n");
        return;
    }

    printf("\n--- %s ---\n", positions[pos]);

    for(i=0;i<candidateCount;i++) {
        if(candidates[i].position==pos) {
            printf("%d. %s\n", count+1, candidates[i].name);
            valid[count++] = i;
        }
    }

    if(count==0) {
        printf("No candidates (ABSTAIN)\n");
        studentVotes[idx][pos]++;
        hasVoted[idx]=1;
        return;
    }

    printf("\nChoose (0 = abstain): ");
    scanf("%d",&choice);

    if(choice==0) {
        studentVotes[idx][pos]++;
        hasVoted[idx]=1;
        printf("Abstained\n");
        return;
    }

    if(choice>=1 && choice<=count) {
        candidates[valid[choice-1]].votes++;
        studentVotes[idx][pos]++;
        hasVoted[idx]=1;
        printf("Vote recorded!\n");
    }
}

void showResults() {
    int i;

    printf("\n==============================\n");
    printf("          RESULTS\n");
    printf("==============================\n");

    for(i=0;i<candidateCount;i++)
        printf("%s - %s : %d votes\n",
               candidates[i].name,
               positions[candidates[i].position],
               candidates[i].votes);
}

void viewRegisteredStudents() {
    int i;

    printf("\n==============================\n");
    printf("    REGISTERED STUDENTS\n");
    printf("==============================\n");

    if(studentCount==0){
        printf("No students registered.\n");
        return;
    }

    for(i=0;i<studentCount;i++)
        printf("%d. %s - %s\n",
               i+1,
               students[i].email,
               hasVoted[i] ? "VOTED" : "NOT VOTED");
}

void studentMenu(int idx) {
    int c;

    do {
        printf("\n==============================\n");
        printf("        STUDENT MENU\n");
        printf("==============================\n");

        printf("1. View Candidates\n2. Vote\n3. Logout\n Choice:");
        scanf("%d",&c);

        if(c==1) viewCandidates();
        else if(c==2) vote(idx);

    } while(c!=3);
}

void adminMenu() {
    int c;

    do {
        printf("\n==============================\n");
        printf("       ADMIN DASHBOARD\n");
        printf("==============================\n");

        printf("1. Add Candidate\n2. View Results\n3. View Students\n4. Logout\n Choice:");
        scanf("%d",&c);

        if(c==1) addCandidate();
        else if(c==2) showResults();
        else if(c==3) viewRegisteredStudents();

    } while(c!=4);
}

void login() {
    char e[50], p[50];
    int idx;

    printf("\n==============================\n");
    printf("           LOGIN\n");
    printf("==============================\n");

    printf("Email: "); scanf("%s", e);
    printf("Password: "); scanf("%s", p);

    if(strcmp(e,adminEmail)==0 && strcmp(p,adminPass)==0) {
        adminMenu();
        return;
    }

    idx = loginStudent(e,p);

    if(idx!=-1) studentMenu(idx);
    else printf("Invalid login!\n");
}

int main() {
    int c;

    do {
        printf("\n==============================\n");
        printf("     QUICKVOTE SYSTEM\n");
        printf("==============================\n");

        printf("1. Login\n2. Register\n3. Exit\n");
        scanf("%d",&c);

        if(c==1) login();
        else if(c==2) registerStudent();

    } while(c!=3);

    return 0;
}
