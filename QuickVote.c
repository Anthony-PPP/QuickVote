#include <stdio.h>
#include <string.h>

#define MAX 100
#define MAX_CANDIDATES 50
#define MAX_POSITIONS 10

// ---------- STRUCTS ----------
struct Student {
    char email[50];
    char password[50];
    int hasVoted;
};

struct Candidate {
    char name[50];
    char position[50];
    int votes;
};

// ---------- GLOBALS ----------
struct Student students[MAX];
struct Candidate candidates[MAX_CANDIDATES];

int studentCount = 0;
int candidateCount = 0;

int electionOpen = 0;

// Admin credentials (UPDATED DOMAIN)
char adminEmail[] = "admin@cbsua.edu.ph";
char adminPass[] = "admin123";

// ---------- EMAIL VALIDATION ----------
int isValidEmail(char email[]) {
    char domain[] = "@cbsua.edu.ph";
    int lenEmail = strlen(email);
    int lenDomain = strlen(domain);

    if(lenEmail < lenDomain) return 0;

    // check if email ends with domain
    if(strcmp(email + (lenEmail - lenDomain), domain) == 0) {
        return 1;
    }

    return 0;
}

// ---------- FUNCTIONS ----------

void registerStudent() {
    char email[50];

    printf("\n=== REGISTER ===\n");

    printf("Enter email (Institutional email only): ");
    scanf("%s", email);

    if(!isValidEmail(email)) {
        printf("Invalid email domain! Use your IE only.\n");
        return;
    }

    strcpy(students[studentCount].email, email);

    printf("Enter password: ");
    scanf("%s", students[studentCount].password);

    students[studentCount].hasVoted = 0;
    studentCount++;

    printf("Registration successful!\n");
}

int studentLogin(char email[], char pass[]) {
    int i;
    for(i = 0; i < studentCount; i++) {
        if(strcmp(email, students[i].email) == 0 &&
           strcmp(pass, students[i].password) == 0) {
            return i;
        }
    }
    return -1;
}

// ---------- CANDIDATE MANAGEMENT ----------

void addCandidate() {
    if(candidateCount >= MAX_CANDIDATES) {
        printf("Candidate limit reached!\n");
        return;
    }

    printf("\n=== ADD CANDIDATE ===\n");
    printf("Enter name: ");
    scanf(" %[^\n]", candidates[candidateCount].name);

    printf("Enter position: ");
    scanf(" %[^\n]", candidates[candidateCount].position);

    candidates[candidateCount].votes = 0;
    candidateCount++;

    printf("Candidate added!\n");
}

void removeCandidate() {
    int i, index;

    if(candidateCount == 0) {
        printf("No candidates to remove.\n");
        return;
    }

    printf("\n=== REMOVE CANDIDATE ===\n");
    for(i = 0; i < candidateCount; i++) {
        printf("%d. %s (%s)\n", i+1,
               candidates[i].name,
               candidates[i].position);
    }

    printf("Enter number to remove: ");
    scanf("%d", &index);

    if(index < 1 || index > candidateCount) {
        printf("Invalid choice!\n");
        return;
    }

    for(i = index-1; i < candidateCount-1; i++) {
        candidates[i] = candidates[i+1];
    }

    candidateCount--;
    printf("Candidate removed!\n");
}

// ---------- DISPLAY ----------

void showCandidatesByPosition() {
    int i, j;
    char printed[MAX_POSITIONS][50];
    int printedCount = 0;
    int found;

    if(candidateCount == 0) {
        printf("No candidates available.\n");
        return;
    }

    printf("\n=== CANDIDATES ===\n");

    for(i = 0; i < candidateCount; i++) {
        found = 0;

        for(j = 0; j < printedCount; j++) {
            if(strcmp(printed[j], candidates[i].position) == 0) {
                found = 1;
                break;
            }
        }

        if(!found) {
            strcpy(printed[printedCount], candidates[i].position);
            printedCount++;

            printf("\nPosition: %s\n", candidates[i].position);

            for(j = 0; j < candidateCount; j++) {
                if(strcmp(candidates[j].position,
                          candidates[i].position) == 0) {
                    printf(" - %s\n", candidates[j].name);
                }
            }
        }
    }
}

// ---------- VOTING ----------

void vote(int index) {
    int i, choice, count;
    char printed[MAX_POSITIONS][50];
    int printedCount = 0;
    int j, found;

    if(!electionOpen) {
        printf("Election is CLOSED.\n");
        return;
    }

    if(students[index].hasVoted) {
        printf("You already voted!\n");
        return;
    }

    if(candidateCount == 0) {
        printf("No candidates available.\n");
        return;
    }

    printf("\n=== VOTING ===\n");

    for(i = 0; i < candidateCount; i++) {
        found = 0;

        for(j = 0; j < printedCount; j++) {
            if(strcmp(printed[j], candidates[i].position) == 0) {
                found = 1;
                break;
            }
        }

        if(!found) {
            strcpy(printed[printedCount], candidates[i].position);
            printedCount++;

            printf("\nPosition: %s\n", candidates[i].position);

            count = 0;
            for(j = 0; j < candidateCount; j++) {
                if(strcmp(candidates[j].position,
                          candidates[i].position) == 0) {
                    count++;
                    printf("%d. %s\n", count, candidates[j].name);
                }
            }

            printf("Choose candidate: ");
            scanf("%d", &choice);

            count = 0;
            for(j = 0; j < candidateCount; j++) {
                if(strcmp(candidates[j].position,
                          candidates[i].position) == 0) {
                    count++;
                    if(count == choice) {
                        candidates[j].votes++;
                    }
                }
            }
        }
    }

    students[index].hasVoted = 1;
    printf("Voting complete!\n");
}

// ---------- RESULTS ----------

void showResults() {
    int i;

    printf("\n=== RESULTS ===\n");
    for(i = 0; i < candidateCount; i++) {
        printf("%s (%s) - %d votes\n",
               candidates[i].name,
               candidates[i].position,
               candidates[i].votes);
    }
}

void studentStatus() {
    int i;

    printf("\n=== STUDENT STATUS ===\n");
    for(i = 0; i < studentCount; i++) {
        printf("%s - %s\n",
               students[i].email,
               students[i].hasVoted ? "HAS VOTED" : "NOT VOTED");
    }
}

void resetVotes() {
    int i;

    for(i = 0; i < candidateCount; i++) {
        candidates[i].votes = 0;
    }

    for(i = 0; i < studentCount; i++) {
        students[i].hasVoted = 0;
    }

    printf("Votes reset!\n");
}

// ---------- MENUS ----------

void studentMenu(int index) {
    int choice;

    do {
        printf("\n===== STUDENT DASHBOARD =====\n");
        printf("1. View Candidates\n");
        printf("2. Vote\n");
        printf("3. Logout\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: showCandidatesByPosition(); break;
            case 2: vote(index); break;
        }

    } while(choice != 3);
}

void adminMenu() {
    int choice;

    do {
        printf("\n===== ADMIN DASHBOARD =====\n");
        printf("1. Add Candidate\n");
        printf("2. Remove Candidate\n");
        printf("3. Open Election\n");
        printf("4. Close Election\n");
        printf("5. View Results\n");
        printf("6. Student Status\n");
        printf("7. Reset Votes\n");
        printf("8. Logout\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addCandidate(); break;
            case 2: removeCandidate(); break;
            case 3: electionOpen = 1; printf("Election OPENED\n"); break;
            case 4: electionOpen = 0; printf("Election CLOSED\n"); break;
            case 5: showResults(); break;
            case 6: studentStatus(); break;
            case 7: resetVotes(); break;
        }

    } while(choice != 8);
}

// ---------- LOGIN ----------

void login() {
    char email[50], pass[50];

    printf("\n=== LOGIN ===\n");
    printf("Email (Institutional email only): ");
    scanf("%s", email);

    if(!isValidEmail(email)) {
        printf("Invalid email domain!\n");
        return;
    }

    printf("Password: ");
    scanf("%s", pass);

    // Admin
    if(strcmp(email, adminEmail) == 0 &&
       strcmp(pass, adminPass) == 0) {
        printf("Admin login successful!\n");
        adminMenu();
        return;
    }

    // Student
    {
        int index = studentLogin(email, pass);
        if(index != -1) {
            printf("Student login successful!\n");
            studentMenu(index);
        } else {
            printf("Invalid credentials!\n");
        }
    }
}

// ---------- MAIN ----------

int main() {
    int choice;

    do {
        printf("\n===== QUICKVOTE SYSTEM =====\n");
        printf("1. Login\n");
        printf("2. Register (Student)\n");
        printf("3. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: login(); break;
            case 2: registerStudent(); break;
        }

    } while(choice != 3);

    return 0;
}
