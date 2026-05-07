#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POSITIONS 8

struct StackNode {
    int position;
    int candidateIndex;
    struct StackNode *next;
};

struct Student {
    char email[50];
    char password[50];

    int studentVotes[POSITIONS];
    int hasVoted;

    struct StackNode *voteStack;

    struct Student *next;
};

struct Candidate {
    char name[50];
    int position;
    int votes;

    struct Candidate *next;
};

struct Student *studentHead = NULL;
struct Candidate *candidateHead = NULL;

int abstainCount[POSITIONS];

char positions[POSITIONS][50] = {
    "President","Vice President","Secretary","Treasurer",
    "Auditor","Public Information Officer","Business Manager","Senators"
};

int voteLimit[POSITIONS] = {1,1,1,1,1,1,2,5};

char adminEmail[] = "admin@cbsua.edu.ph";
char adminPass[] = "admin123";

int getInt() {

    int x;
    char ch;

    if(scanf("%d", &x) != 1) {

        printf("Invalid input!\n");

        while((ch = getchar()) != '\n' && ch != EOF);

        return -1;
    }

    return x;
}

int isValidName(char str[]) {

    int i;

    for(i = 0; str[i] != '\0'; i++) {

        if(!((str[i] >= 'A' && str[i] <= 'Z') ||
             (str[i] >= 'a' && str[i] <= 'z') ||
             str[i] == ' ')) {

            return 0;
        }
    }

    return 1;
}

struct Student* loginStudent(char e[], char p[]) {

    struct Student *temp = studentHead;

    while(temp != NULL) {

        if(strcmp(temp->email, e) == 0 &&
           strcmp(temp->password, p) == 0) {

            return temp;
        }

        temp = temp->next;
    }

    return NULL;
}

void showPositions() {

    int i;

    printf("\nPOSITIONS:\n");

    for(i = 0; i < POSITIONS; i++) {
        printf("%d. %s\n", i + 1, positions[i]);
    }
}

void viewCandidates() {

    int i, num;
    struct Candidate *temp;

    num = 1;

    printf("\n=== CANDIDATES ===\n");

    if(candidateHead == NULL) {
        printf("No candidates yet.\n");
        return;
    }

    for(i = 0; i < POSITIONS; i++) {

        temp = candidateHead;

        while(temp != NULL) {

            if(temp->position == i) {

                printf("%d. %s - %s\n",
                       num,
                       temp->name,
                       positions[i]);

                num++;
            }

            temp = temp->next;
        }
    }
}

void registerStudent() {

    struct Student *newStudent;
    struct Student *temp;
    char *domain;
    int i;

    newStudent = (struct Student*)malloc(sizeof(struct Student));

    printf("\n=== REGISTER ===\n");

    printf("Email: ");
    scanf("%s", newStudent->email);

    domain = strstr(newStudent->email, "@cbsua.edu.ph");

    if(domain == NULL || strcmp(domain, "@cbsua.edu.ph") != 0) {

        printf("Invalid email domain!\n");

        free(newStudent);
        return;
    }

    printf("Password: ");
    scanf("%s", newStudent->password);

    newStudent->hasVoted = 0;
    newStudent->voteStack = NULL;

    for(i = 0; i < POSITIONS; i++) {
        newStudent->studentVotes[i] = 0;
    }

    newStudent->next = NULL;

    if(studentHead == NULL) {
        studentHead = newStudent;
    } else {

        temp = studentHead;

        while(temp->next != NULL) {
            temp = temp->next;
        }

        temp->next = newStudent;
    }

    printf("Registered!\n");
}

void addCandidate() {

    struct Candidate *newCandidate;
    struct Candidate *temp;
    int pos;

    newCandidate = (struct Candidate*)malloc(sizeof(struct Candidate));

    printf("\n=== ADD CANDIDATE ===\n");

    showPositions();

    printf("\nSelect position: ");
    pos = getInt();

    if(pos == -1 || pos < 1 || pos > POSITIONS) {
        free(newCandidate);
        return;
    }

    printf("Name: ");
    scanf(" %[^\n]", newCandidate->name);

    if(!isValidName(newCandidate->name)) {
        printf("Invalid name! Letters only.\n");
        free(newCandidate);
        return;
    }

    newCandidate->position = pos - 1;
    newCandidate->votes = 0;
    newCandidate->next = NULL;

    if(candidateHead == NULL) {
        candidateHead = newCandidate;
    } else {

        temp = candidateHead;

        while(temp->next != NULL) {
            temp = temp->next;
        }

        temp->next = newCandidate;
    }

    printf("Added!\n");
}

void undoLastVote(struct Student *student) {

    struct StackNode *temp;

    if(student->voteStack == NULL) {
        printf("Nothing to undo.\n");
        return;
    }

    temp = student->voteStack;

    student->studentVotes[temp->position]--;

    student->voteStack = temp->next;

    free(temp);

    printf("Last vote undone!\n");
}

void vote(struct Student *student) {

    struct Candidate *temp;
    struct Candidate *valid[100];

    struct StackNode *newNode;

    int pos, choice;
    int count;
    int i;

    count = 0;
    i = 1;

    printf("\n=== VOTING ===\n");

    showPositions();

    printf("\nSelect position: ");
    pos = getInt();

    if(pos == -1) return;

    pos--;

    if(pos < 0 || pos >= POSITIONS) return;

    if(student->studentVotes[pos] >= voteLimit[pos]) {
        printf("No remaining votes!\n");
        return;
    }

    temp = candidateHead;

    while(temp != NULL) {

        if(temp->position == pos) {

            printf("%d. %s\n", i, temp->name);

            valid[count] = temp;

            count++;
            i++;
        }

        temp = temp->next;
    }

    if(count == 0) {

        printf("No candidates\n");

        student->studentVotes[pos]++;
        abstainCount[pos]++;
        student->hasVoted = 1;

        return;
    }

    printf("Choose (0 = abstain): ");
    choice = getInt();

    if(choice == 0) {

        printf("Abstained\n");

        student->studentVotes[pos]++;
        abstainCount[pos]++;
        student->hasVoted = 1;

        return;
    }

    if(choice >= 1 && choice <= count) {

        valid[choice - 1]->votes++;

        student->studentVotes[pos]++;
        student->hasVoted = 1;

        newNode = (struct StackNode*)malloc(sizeof(struct StackNode));

        newNode->position = pos;
        newNode->candidateIndex = choice - 1;

        newNode->next = student->voteStack;
        student->voteStack = newNode;

        printf("Vote recorded!\n");
    }
}

void showResults() {

    struct Candidate *temp;
    struct Candidate *arr[100];
    struct Candidate *swap;

    int i, j, k, count;

    printf("\n=== RESULTS ===\n");

    if(candidateHead == NULL) {
        printf("No candidates yet.\n");
        return;
    }

    for(i = 0; i < POSITIONS; i++) {

        count = 0;
        temp = candidateHead;

        while(temp != NULL) {

            if(temp->position == i) {
                arr[count] = temp;
                count++;
            }

            temp = temp->next;
        }

        for(j = 0; j < count - 1; j++) {

            for(k = 0; k < count - j - 1; k++) {

                if(arr[k]->votes < arr[k + 1]->votes) {

                    swap = arr[k];
                    arr[k] = arr[k + 1];
                    arr[k + 1] = swap;
                }
            }
        }

        printf("\n--- %s ---\n", positions[i]);

        if(count == 0) {
            printf("No candidates.\n");
        } else {

            for(j = 0; j < count; j++) {

                printf("%d. %s - %d votes\n",
                       j + 1,
                       arr[j]->name,
                       arr[j]->votes);
            }
        }

        printf("ABSTAIN: %d\n", abstainCount[i]);
    }
}

void viewRegisteredStudents() {

    struct Student *temp;
    int num;

    num = 1;
    temp = studentHead;

    printf("\n=== STUDENTS ===\n");

    if(temp == NULL) {
        printf("No students registered.\n");
        return;
    }

    while(temp != NULL) {

        printf("%d. %s - %s\n",
               num,
               temp->email,
               temp->hasVoted ? "VOTED" : "NOT VOTED");

        num++;
        temp = temp->next;
    }
}

void studentMenu(struct Student *student) {

    int c;

    do {

        printf("\n--- STUDENT MENU ---\n");
        printf("1. View Candidates\n");
        printf("2. Vote\n");
        printf("3. Undo Last Vote\n");
        printf("4. Logout\n");

        c = getInt();

        if(c == 1) viewCandidates();
        else if(c == 2) vote(student);
        else if(c == 3) undoLastVote(student);

    } while(c != 4);
}

void adminMenu() {

    int c;

    do {

        printf("\n--- ADMIN MENU ---\n");
        printf("1. Add Candidate\n");
        printf("2. View Results\n");
        printf("3. View Students\n");
        printf("4. Logout\n");

        c = getInt();

        if(c == 1) addCandidate();
        else if(c == 2) showResults();
        else if(c == 3) viewRegisteredStudents();

    } while(c != 4);
}

void login() {

    char e[50], p[50];
    struct Student *student;

    printf("\n=== LOGIN ===\n");

    printf("Email: ");
    scanf("%s", e);

    printf("Password: ");
    scanf("%s", p);

    if(strcmp(e, adminEmail) == 0 &&
       strcmp(p, adminPass) == 0) {

        adminMenu();
        return;
    }

    student = loginStudent(e, p);

    if(student != NULL)
        studentMenu(student);
    else
        printf("Invalid login!\n");
}

int main() {

    int c;

    do {

        printf("\n==============================\n");
        printf("     QUICKVOTE SYSTEM\n");
        printf("==============================\n");

        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");

        c = getInt();

        if(c == 1) login();
        else if(c == 2) registerStudent();

    } while(c != 3);

    return 0;
}
