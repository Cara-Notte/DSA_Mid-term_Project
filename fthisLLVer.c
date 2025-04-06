#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    bench = 1, crunches, curls, deadlift, legPresses, pullUps, squat,
    benchPR, crunchesPR, curlsPR, deadliftPR, legPressesPR, pullUpsPR, squatPR
} ExerciseType;

typedef struct Workout {
    ExerciseType exercise;
    int sets;
    int reps;
    float weight;
} Workout;

typedef struct PR {
    ExerciseType exercise;
    float maxWeight;
    int maxSets;
    int maxReps;
} PR;

typedef struct WorkoutQueue {
    ExerciseType exercise;
    struct WorkoutQueue *next;
} WorkoutQueue;

typedef struct WorkoutHistory {
    Workout workout;
    struct WorkoutHistory *next;
} WorkoutHistory;

typedef struct FitnessChampion {
    char username[50];
    int totalWorkouts;
} FitnessChampion;

typedef struct StrengthRecord {
    char username[50];
    float bestWeight;
    int bestSets;
    int bestReps;
} StrengthRecord;

typedef struct account{
    char username[50];
    char password[17];
    int day;
    int month;
    int year;
    char phoneNum[15];
    char email[30];
    char city[50];

    WorkoutQueue *plannedFront;
    WorkoutQueue *plannedRear;
    WorkoutHistory *completedTop;
    PR personalRecords[7];

    struct account *next;
}Account;

typedef struct loginHistory {
    char username[50];
    char city[50];
    struct loginHistory *next;
}LoginHistory;

typedef struct queue {
    LoginHistory *front, *rear;
    int count;
}Queue;

void initializeQueue(Queue *q) {
    q->front = q->rear = NULL;
    q->count = 0;
}

void enqueue(Queue *q, char username[], char city[]) {
    LoginHistory *newNode = (LoginHistory*)malloc(sizeof(LoginHistory));
    newNode->next = NULL;
    if (!newNode) return;
    strcpy(newNode->username, username);
    strcpy(newNode->city, city);

    if (q->count == 5) {
        LoginHistory *temp = q->front;
        q->front = q->front->next;
        temp->next = NULL;
        free(temp);
        q->count--;
    }

    if (!q->rear) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }

    q->count++;
}

void deleteLoginHistory(Queue *q) {
    if (!q->front) {
        printf("No login history to delete!\n\n");
        return;
    }

    LoginHistory *temp;
    while (q->front) {
        temp = q->front;
        q->front = q->front->next;
        free(temp);
    }

    q->rear = NULL;
    q->count = 0;

    printf("Login history successfully deleted!\n");
}

void displayQueue(Queue *q) {
    if (q->count == 0) {
        printf("No login history available.\n");
        return;
    }
    printf("===================================\n");
    printf("           Login History          \n");
    printf("===================================\n");

    LoginHistory *current = q->front;
    while (current) {
        printf("Username: %s\n", current->username);
        printf("City    : %s\n", current->city);
        printf("--------------------\n");
        current = current->next;
    }
}

void modifyAccount(Account *head, char user[]) {
    Account *current = head;

    while (current) {
        if (strcmp(current->username, user) == 0) {
            int choice;
            printf("\n============= Modify Account Details ===============\n");
            printf("1. Change Password\n");
            printf("2. Change Birthdate\n");
            printf("3. Change Phone Number\n");
            printf("4. Change Email\n");
            printf("5. Change City\n");
            printf("6. Cancel\n");

            printf("Choose an option: ");
            scanf("%d", &choice);
            getchar();

            switch (choice) {
                case 1:
                    printf("Enter new password: ");
                    scanf("%s", current->password);
                    break;
                case 2:
                    printf("Enter new birthdate (DD MM YYYY): ");
                    scanf("%d %d %d", &current->day, &current->month, &current->year);
                    break;
                case 3:
                    printf("Enter new phone number: ");
                    scanf(" %[^\n]", current->phoneNum);
                    break;
                case 4:
                    printf("Enter new email: ");
                    scanf(" %[^\n]", current->email);
                    break;
                case 5:
                    printf("Enter new city: ");
                    scanf(" %[^\n]", current->city);
                    break;
                case 6:
                    printf("Modification canceled.\n");
                    return;
                default:
                    printf("Invalid choice!\n");
                    return;
            }

            FILE *file = fopen("accLL.txt", "w");
            if (!file) {
                printf("Error: Unable to update account file!\n");
                return;
            }

            Account *temp = head;
            while (temp) {
                fprintf(file, "%s#%s#%d#%d#%d#%s#%s#%s\n", temp->username, temp->password, temp->day, temp->month, temp->year, temp->phoneNum, temp->email, temp->city);
                temp = temp->next;
            }
            fclose(file);

            printf("Account details successfully updated!\n");
            return;
        }
        current = current->next;
    }

    printf("Error: Account not found!\n");
}

void enqueueWorkout(Account *acc, ExerciseType ex) {
    WorkoutQueue *newNode = (WorkoutQueue*)malloc(sizeof(WorkoutQueue));
    newNode->exercise = ex;
    newNode->next = NULL;

    if (!acc->plannedRear) {
        acc->plannedFront = acc->plannedRear = newNode;
    } else {
        acc->plannedRear->next = newNode;
        acc->plannedRear = newNode;
    }
}

ExerciseType dequeueWorkout(Account *acc) {
    if (!acc->plannedFront) {
        return -1;
    }

    WorkoutQueue *temp = acc->plannedFront;
    ExerciseType ex = temp->exercise;

    acc->plannedFront = acc->plannedFront->next;
    if (!acc->plannedFront) {
        acc->plannedRear = NULL;
    }

    free(temp);
    return ex;
}

void addWorkout(Account *acc, Workout w) {
    WorkoutHistory *newNode = (WorkoutHistory*)malloc(sizeof(WorkoutHistory));
    newNode->workout = w;
    newNode->next = NULL;

    if (!acc->completedTop) {
        acc->completedTop = newNode;
    } else {
        WorkoutHistory *current = acc->completedTop;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void updatePR(Account *acc, Workout w) {
    if (w.exercise >= benchPR && w.exercise <= squatPR) {
        int prIndex = w.exercise - benchPR;
        if (prIndex < 0 || prIndex >= 7) {
            return;
        }
        PR *pr = &acc->personalRecords[prIndex];

        if (w.weight > pr->maxWeight ||(w.weight == pr->maxWeight && w.sets > pr->maxSets) ||(w.weight == pr->maxWeight && w.sets == pr->maxSets && w.reps > pr->maxReps)) {
            pr->maxWeight = w.weight;
            pr->maxSets = w.sets;
            pr->maxReps = w.reps;
        }
    }
}

void showLeaderboardHeader(const char *title) {
    printf("\n*--*-*-*--*--*-*-*--**--*-*-*--*--*-*-*--*\n");
    printf("   %s\n", title);
    printf("*--*-*-*--*--*-*-*--**--*-*-*--*--*-*-*--*\n");
}

void showExerciseChoices() {
    printf("\nChoose Your Challenge:\n");
    printf("1. Bench Press Kings\n");
    printf("2. Crunch Masters\n");
    printf("3. Curl Champions\n");
    printf("4. Deadlift Dominators\n");
    printf("5. Leg Press Legends\n");
    printf("6. Pull-Up Pros\n");
    printf("7. Squat Sovereigns\n");
    printf("8. Return to Main Menu\n");
}

void saveAccounts(Account *head) {
    FILE *file = fopen("accLL.txt", "w");
    Account *current = head;

    while (current) {
        fprintf(file, "%s#%s#%d#%d#%d#%s#%s#%s#", current->username, current->password, current->day, current->month, current->year, current->phoneNum, current->email, current->city);
        ExerciseType planned[50] = {0};
        WorkoutQueue *wnode = current->plannedFront;
        int i = 0;
        while (wnode && i < 50) {
            planned[i++] = wnode->exercise;
            wnode = wnode->next;
        }
        int front = (i > 0) ? 0 : -1;
        int rear = (i > 0) ? (i - 1) : -1;
        fprintf(file, "%d#%d#-1#", front, rear);
        for (int j = 0; j < 50; j++) {
            fprintf(file, "%d#", planned[j]);
        }

        WorkoutHistory *hnode = current->completedTop;
        if (!hnode) {
            fprintf(file, "#");
        } else {
            while (hnode) {
                fprintf(file, "%d#%d#%d#%.2f#", hnode->workout.exercise, hnode->workout.sets, hnode->workout.reps, hnode->workout.weight);
                hnode = hnode->next;
            }
            fprintf(file, "#");
        }


        for (int i = 0; i < 7; i++) {
            fprintf(file, "%d#%.2f#%d#%d#", current->personalRecords[i].exercise, current->personalRecords[i].maxWeight, current->personalRecords[i].maxSets, current->personalRecords[i].maxReps);
        }
        fprintf(file, "\n");
        current = current->next;
    }
    fclose(file);
}

Account* loadAccounts() {
    FILE *file = fopen("accLL.txt", "r");
    if (!file) return NULL;

    Account *head = NULL, *tail = NULL;
    char buffer[2048];

    while (fgets(buffer, sizeof(buffer), file)) {
        Account *newAcc = (Account*)malloc(sizeof(Account));
        memset(newAcc, 0, sizeof(Account));
        newAcc->plannedFront = newAcc->plannedRear = NULL;
        newAcc->completedTop = NULL;

        char *token = strtok(buffer, "#\n");
        if(token) strncpy(newAcc->username, token, 49);
        token = strtok(NULL, "#\n");
        if(token) strncpy(newAcc->password, token, 16);
        token = strtok(NULL, "#\n");
        if(token) newAcc->day = atoi(token);
        token = strtok(NULL, "#\n");
        if(token) newAcc->month = atoi(token);
        token = strtok(NULL, "#\n");
        if(token) newAcc->year = atoi(token);
        token = strtok(NULL, "#\n");
        if(token) strncpy(newAcc->phoneNum, token, 14);
        token = strtok(NULL, "#\n");
        if(token) strncpy(newAcc->email, token, 29);
        token = strtok(NULL, "#\n");
        if(token) strncpy(newAcc->city, token, 49);

        char *front_str = strtok(NULL, "#\n");
        char *rear_str = strtok(NULL, "#\n");
        char *top_str = strtok(NULL, "#\n");
        int front = front_str ? atoi(front_str) : -1;
        int rear = rear_str ? atoi(rear_str) : -1;
        int completedTop = top_str ? atoi(top_str) : -1;

        ExerciseType items[50];
        for (int j = 0; j < 50; j++) {
            char *token = strtok(NULL, "#\n");
            items[j] = token ? atoi(token) : 0;
        }

        if (front != -1 && rear != -1) {
            for (int idx = front; idx <= rear; idx++) {
                ExerciseType ex = items[idx];
                if (ex >= bench && ex <= squatPR) {
                    enqueueWorkout(newAcc, ex);
                }
            }
        }

        WorkoutHistory **historyTail = &newAcc->completedTop;
        while (1) {
            char *exToken = strtok(NULL, "#\n");
            if (!exToken || strcmp(exToken, "#") == 0) {
                break;
            }

            Workout w;
            w.exercise = atoi(exToken);

            if (w.exercise < bench || w.exercise > squatPR) {
                strtok(NULL, "#\n");
                strtok(NULL, "#\n");
                strtok(NULL, "#\n");
                continue;
            }

            char *setsToken = strtok(NULL, "#\n");
            if (!setsToken) {
                break;
            }
            w.sets = atoi(setsToken);

            char *repsToken = strtok(NULL, "#\n");
            if (!repsToken) {
                break;
            }
            w.reps = atoi(repsToken);

            char *weightToken = strtok(NULL, "#\n");
            if (!weightToken) {
                break;
            }
            w.weight = atof(weightToken);

            WorkoutHistory *newNode = (WorkoutHistory*)malloc(sizeof(WorkoutHistory));
            newNode->workout = w;
            newNode->next = NULL;

            if (*historyTail) {
                (*historyTail)->next = newNode;
                historyTail = &(*historyTail)->next;
            } else {
                *historyTail = newNode;
            }
        }

        for (int i = 0; i < 7; i++) {
            char *exToken = strtok(NULL, "#\n");
            if (!exToken || exToken[0] == '\0') {
                break;
            }
            newAcc->personalRecords[i].exercise = benchPR + i;

            char *weightToken = strtok(NULL, "#\n");
            if (weightToken) {
                newAcc->personalRecords[i].maxWeight = atof(weightToken);
            } else {
                newAcc->personalRecords[i].maxWeight = 0.0f;
            }

            char *setsToken = strtok(NULL, "#\n");
            if (setsToken) {
                newAcc->personalRecords[i].maxSets = atoi(setsToken);
            } else {
                newAcc->personalRecords[i].maxSets = 0;
            }

            char *repsToken = strtok(NULL, "#\n");
            if (repsToken) {
                newAcc->personalRecords[i].maxReps = atoi(repsToken);
            } else {
                newAcc->personalRecords[i].maxReps = 0;
            }
        }

        if(!head) {
            head = tail = newAcc;
        } else {
            tail->next = newAcc;
            tail = newAcc;
        }
    }
    fclose(file);
    return head;
}

const char* exerciseName(ExerciseType ex) {
    const char* names[] = {
        "Placeholder",
        "Bench", "Crunches", "Curls", "Deadlift", "Leg Press", "Pull Ups", "Squat",
        "Bench PR", "Crunches PR", "Curls PR", "Deadlift PR", "Leg Press PR", "Pull Ups PR", "Squat PR"
    };
    return (ex >= 1 && ex <= 14) ? names[ex] : names[0];
}

void freeWorkoutData(Account *acc) {
    WorkoutQueue *wnode = acc->plannedFront;
    while (wnode) {
        WorkoutQueue *temp = wnode;
        wnode = wnode->next;
        free(temp);
    }

    WorkoutHistory *hnode = acc->completedTop;
    while (hnode) {
        WorkoutHistory *temp = hnode;
        hnode = hnode->next;
        free(temp);
    }
}

void freeAccounts(Account *head) {
    Account *temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main(){
    Account *head = NULL, *tail = NULL, *node;
    Account *currentAccount = NULL;
    int choice, jumlahData = 0, a = 0, isValid, picks;
    char user[50], pass[17], temp[17], name[50];
    Queue loginQueue;
    initializeQueue(&loginQueue);

    head = loadAccounts();

    int isLoggedIn = 0;
    while(1){
        isValid = 0;
        if (isLoggedIn == 0){
            picks = 0;
            printf("You have not logged in yet, please login or Sign-Up!\n");
            printf("1. Login\n");
            printf("2. Sign-Up\n");
            printf("3. Exit\n");
            while (1) {
                printf("Choice: ");
                if (scanf("%d", &choice) == 1) {
                    while (getchar() != '\n');
                    break;
                } else {
                    printf("Invalid input! Please enter a valid number.\n");
                    while (getchar() != '\n');
                }
            }
        } else {
            choice = 0;
            printf("\nWelcome, %s.\n", user);
            printf("1. LogOut\n");
            printf("2. Login History\n");
            printf("3. Delete Login History\n");
            printf("4. Account Details\n");
            printf("5. Edit Account Details\n");
            printf("6. Plan Workout\n");
            printf("7. Track Workout\n");
            printf("8. Unfinished Workouts\n");
            printf("9. Workout History\n");
            printf("10. Leaderboards\n");
            printf("11. Exit\n");
            while (1) {
                printf("Choice: ");
                if (scanf("%d", &picks) == 1) {
                    while (getchar() != '\n');
                    break;
                } else {
                    printf("Invalid input! Please enter a valid number.\n");
                    while (getchar() != '\n');
                }
            }
        }

        if (isLoggedIn == 0){
            if (choice == 1){
                printf("===================================\n");
                printf("               Login            \n");
                printf("===================================\n");
                printf("Enter -1 to exit\n");

                do {
                    printf("Username: ");
                    scanf(" %[^\n]", user);

                    if ((strcmp(user, "-1")) == 0){
                        printf("Exiting...\n\n");
                        break;
                    }

                    printf("Password: ");
                    scanf(" %[^\n]", pass);

                    node = head;

                    while(node){
                        if (strcmp(node->username, user) == 0 && strcmp(node->password, pass) == 0) {
                            isValid = 1;
                            currentAccount = node;
                            enqueue(&loginQueue, user, node->city);
                            break;
                        }
                        node = node->next;
                    }

                    if (!isValid) {
                        printf("Something Went Wrong... Incorrect Username or Password!\n");
                        continue;
                    } else {
                        printf("Login Successful!\n");
                        isLoggedIn++;
                        break;
                    }
                    printf("===================================\n");
                } while(1);
            } else {
                if (choice == 2){
                    FILE *signUp = fopen("accLL.txt", "a");
                    Account *newNode = (Account *)malloc(sizeof(Account));
                    memset(newNode, 0, sizeof(Account));

                    node = head;
                    while(1){
                        printf("===================================\n");
                        printf("               Sign Up            \n");
                        printf("===================================\n");
                        printf("Enter -1 to exit\n");

                        int exitFlag = 0;

                        do {
                            int isDuplicate = 0;
                            printf("Username\t: ");
                            scanf(" %[^\n]", name);

                            if ((strcmp(name, "-1")) == 0){
                                printf("Exiting...\n\n");
                                exitFlag = 1;
                                break;
                            }

                            while(node){
                                if (strcmp(name, node->username) == 0) {
                                    printf("Sorry, Username has been used, please pick a different username!\n");
                                    isDuplicate = 1;
                                    break;
                                }
                                node = node->next;
                            }

                            if (!isDuplicate) {
                                strcpy(newNode->username, name);
                                fprintf(signUp, "%s#", newNode->username);
                                break;
                            }
                        } while(1);

                        if (exitFlag) {
                            fclose(signUp);
                            break;
                        }

                        do {
                            node = head;
                            printf("Password\t: ");
                            scanf(" %s", temp);
                            if (strlen(temp) > 16) {
                                printf("Password is too long! Try again.\n");
                            } else if (strlen(temp) < 5) {
                                printf("Password is too short! Try again.\n");
                            } else {
                                strcpy(newNode->password, temp);
                                fprintf(signUp, "%s#", newNode->password);
                                break;
                            }
                        } while (1);

                        printf("Your Birthdate in numbers (DD MM YYYY)\n");

                        while (1) {
                            printf("day\t: ");
                            if (scanf(" %d", &newNode->day) == 1) {
                                while (getchar() != '\n');
                                break;
                            } else {
                                printf("Invalid input! Please enter a valid number.\n");
                                while (getchar() != '\n');
                            }
                        }
                        fprintf(signUp, "%d#", newNode->day);

                        while (1) {
                            printf("month\t: ");
                            if (scanf(" %d", &newNode->month) == 1) {
                                while (getchar() != '\n');
                                break;
                            } else {
                                printf("Invalid input! Please enter a valid number.\n");
                                while (getchar() != '\n');
                            }
                        }
                        fprintf(signUp, "%d#", newNode->month);

                        while (1) {
                            printf("year\t: ");
                            if (scanf(" %d", &newNode->year) == 1) {
                                while (getchar() != '\n');
                                break;
                            } else {
                                printf("Invalid input! Please enter a valid number.\n");
                                while (getchar() != '\n');
                            }
                        }
                        fprintf(signUp, "%d#", newNode->year);

                        printf("Phone Number\t: ");
                        scanf(" %[^\n]", newNode->phoneNum);
                        fprintf(signUp, "%s#", newNode->phoneNum);

                        printf("Email\t: ");
                        scanf(" %[^\n]", newNode->email);
                        fprintf(signUp, "%s#", newNode->email);

                        printf("City\t: ");
                        scanf(" %[^\n]", newNode->city);
                        fprintf(signUp, "%s#\n", newNode->city);

                        fclose(signUp);
                        jumlahData++;

                        if (!head){
                            head = tail = newNode;
                        } else {
                            tail->next = newNode;
                            tail = tail->next;
                            tail->next = NULL;
                        }

                        printf("===================================\n\n");
                        break;
                    }
                } else {
                    if (choice == 3){
                        freeAccounts(head);
                        printf("Program Exiting...");
                        break;
                    } else {
                        printf("Invalid input, please try again...\n\n");
                    }
                }
            }
        } else {
            if (picks == 1){
                isLoggedIn = 0;
                printf("Successfully logged Out!\n\n");
            } else {
                if (picks == 2){
                    displayQueue(&loginQueue);
                } else {
                    if (picks == 3){
                        deleteLoginHistory(&loginQueue);
                    } else {
                        if (picks == 4){
                            int found = 0;

                            node = head;

                            while(node){
                                if (strcmp(node->username, user) == 0) {
                                    found = 1;
                                    break;
                                } else {
                                    node = node->next;
                                }
                            }

                            if (found) {
                                printf("\n============= Account Details ===============\n");
                                printf("Username        : %s\n", node->username);
                                printf("Birthdate       : %d-%d-%d\n", node->day, node->month, node->year);
                                printf("Phone Number    : %s\n", node->phoneNum);
                                printf("Email           : %s\n", node->email);
                                printf("City            : %s\n", node->city);
                                printf("==============================================\n");
                            } else {
                                printf("Error: Account not found!\n");
                            }
                        } else {
                            if (picks == 5){
                                modifyAccount(head, user);
                            } else {
                                if (picks == 6){
                                    int workoutChoice;
                                    do {
                                        printf("\n=== Plan Workout ===\n");
                                        printf("Current Planned Workouts:\n");
                                        WorkoutQueue *current = currentAccount->plannedFront;
                                        if(!current) {
                                            printf("None\n");
                                        } else {
                                            int pos = 1;
                                            while(current) {
                                            printf("%d. %s\n", pos++, exerciseName(current->exercise));
                                            current = current->next;
                                            }
                                        }

                                        printf("\nExercise Types:\n");
                                        for (int ex = 1; ex <= 14; ex++) {
                                            printf("%2d. %s\n", ex, exerciseName(ex));
                                        }
                                        printf("15. Finish planning\nChoice: ");
                                        if (scanf("%d", &workoutChoice) != 1) {
                                            while(getchar() != '\n');
                                            continue;
                                        }

                                        if (workoutChoice == 15) {
                                            break;
                                        }
                                        if (workoutChoice < 1 || workoutChoice > 14) {
                                            printf("Invalid choice!\n");
                                            continue;
                                        }

                                        WorkoutQueue *check = currentAccount->plannedFront;
                                        while (check) {
                                            if (check->exercise == workoutChoice) {
                                                printf("%s is already in your plan!", exerciseName(workoutChoice));
                                                break;
                                            }
                                            check = check->next;
                                        }
                                        if (check) {
                                            continue;
                                        }

                                        enqueueWorkout(currentAccount, workoutChoice);
                                        saveAccounts(head);
                                        printf("%s added!\n", exerciseName(workoutChoice));
                                    } while(1);
                                } else {
                                    if (picks == 7) {
                                        int tracking = 1;
                                        while (tracking) {
                                            printf("\n=== Track Workout ===\n");
                                            WorkoutQueue *current = currentAccount->plannedFront;
                                            if (!current) {
                                                printf("No workouts to track!\n");
                                                tracking = 0;
                                                break;
                                            }

                                            int pos = 1;
                                            WorkoutQueue *nodes[50];
                                            while (current) {
                                                printf("%d. %s\n", pos, exerciseName(current->exercise));
                                                nodes[pos-1] = current;
                                                pos++;
                                                current = current->next;
                                            }

                                            printf("\n0. Finish tracking\n");
                                            printf("Enter workout entry to track: ");
                                            int choice;
                                            scanf("%d", &choice);
                                            getchar();

                                            if (choice == 0) {
                                                tracking = 0;
                                                break;
                                            }

                                            if (choice > 0 && choice <= pos-1) {
                                                WorkoutQueue *target = nodes[choice-1];
                                                Workout newWorkout;
                                                newWorkout.exercise = target->exercise;

                                                printf("Enter sets done: ");
                                                scanf("%d", &newWorkout.sets);
                                                getchar();

                                                printf("Enter reps done: ");
                                                scanf("%d", &newWorkout.reps);
                                                getchar();

                                                printf("Enter weight applied (kg): ");
                                                scanf("%f", &newWorkout.weight);
                                                getchar();

                                                if (target == currentAccount->plannedFront) {
                                                    currentAccount->plannedFront = target->next;
                                                    if (!currentAccount->plannedFront) {
                                                        currentAccount->plannedRear = NULL;
                                                    }
                                                } else {
                                                    WorkoutQueue *prev = currentAccount->plannedFront;
                                                    while (prev->next != target) {
                                                        prev = prev->next;
                                                    }
                                                    prev->next = target->next;
                                                    if (target == currentAccount->plannedRear) {
                                                        currentAccount->plannedRear = prev;
                                                    }
                                                }
                                                free(target);
                                                addWorkout(currentAccount, newWorkout);
                                                updatePR(currentAccount, newWorkout);
                                                saveAccounts(head);
                                                printf("%s tracked successfully!\n", exerciseName(newWorkout.exercise));
                                            }
                                        }
                                    } else {
                                        if (picks == 8) {
                                            int choice;
                                            do {
                                                printf("\n=== Unfinished Workouts ===\n");
                                                WorkoutQueue *current = currentAccount->plannedFront;
                                                if (!current) {
                                                    printf("No planned workouts!\n");
                                                    break;
                                                }
                                                int pos = 1;
                                                while (current) {
                                                    printf("%d. %s\n", pos++, exerciseName(current->exercise));
                                                    current = current->next;
                                                }
                                                printf("\nEnter 0 to finish viewing or...\nEnter an entry's number to delete: ");
                                                if (scanf("%d", &choice) != 1) {
                                                    while (getchar() != '\n');
                                                    continue;
                                                }
                                                while (getchar() != '\n');

                                                if (choice == 0) {
                                                    break;
                                                } else if (choice > 0) {
                                                    WorkoutQueue *target = currentAccount->plannedFront;
                                                    WorkoutQueue *prev = NULL;
                                                    for (int i = 1; i < choice && target; i++) {
                                                        prev = target;
                                                        target = target->next;
                                                    }
                                                    if (target) {
                                                        if (prev) {
                                                            prev->next = target->next;
                                                        } else {
                                                            currentAccount->plannedFront = target->next;
                                                        }
                                                        if (!target->next) {
                                                            currentAccount->plannedRear = prev;
                                                        }
                                                        free(target);
                                                        saveAccounts(head);
                                                        printf("Workout deleted successfully!\n");
                                                    }
                                                }
                                            } while (1);
                                        } else {
                                            if (picks == 9) {
                                                printf("\n=== Workout History ===\n");
                                                WorkoutHistory *current = currentAccount->completedTop;
                                                if (!current) {
                                                    printf("No completed workouts yet!\n");
                                                }
                                                int counter = 1;
                                                while(current) {
                                                    if (current->workout.exercise >= bench && current->workout.exercise <= squatPR) {
                                                        printf("|%-4d|%-15s|%-6d|%-6d|%-8.1fkg\n", counter++, exerciseName(current->workout.exercise), current->workout.sets, current->workout.reps, current->workout.weight);
                                                    } else {
                                                        printf("|%-4d|Invalid Entry (Skipped)\n", counter++);
                                                    }
                                                    current = current->next;
                                                }
                                            } else {
                                                if (picks == 10) {
                                                    int leaderChoice;
                                                    showLeaderboardHeader("   THE GRAND FITNESS LEADERBOARDS");
                                                    printf("1. Top Total Workouts!!!\n");
                                                    printf("2. Top PR Records!!!\n");
                                                    printf("3. Return to Main Menu\n");
                                                    while (1) {
                                                        printf("\nChoose leaderboard type: ");
                                                        if (scanf("%d", &leaderChoice) == 1) {
                                                            if (leaderChoice >= 1 && leaderChoice <= 3) break;
                                                        }
                                                        printf("Please enter a valid choice!\n");
                                                        while (getchar() != '\n');
                                                    }
                                                    if (leaderChoice == 1) {
                                                        FitnessChampion *champions = NULL;
                                                        int champCount = 0;
                                                        Account *current = head;
                                                        while (current) {
                                                            int total = 0;
                                                            WorkoutHistory *wh = current->completedTop;
                                                            while (wh) {
                                                                total++;
                                                                wh = wh->next;
                                                            }
                                                            if (total > 0) {
                                                                champions = realloc(champions, (champCount + 1) * sizeof(FitnessChampion));
                                                                strcpy(champions[champCount].username, current->username);
                                                                champions[champCount].totalWorkouts = total;
                                                                champCount++;
                                                            }
                                                            current = current->next;
                                                        }

                                                        if (champCount == 0) {
                                                            showLeaderboardHeader("No Workouts Recorded Yet...");
                                                            printf("Be the first to complete a workout!!!\n");
                                                        } else {
                                                            for (int i = 0; i < champCount - 1; i++) {
                                                                for (int j = 0; j < champCount - i - 1; j++) {
                                                                    if (champions[j].totalWorkouts < champions[j+1].totalWorkouts) {
                                                                        FitnessChampion temp = champions[j];
                                                                        champions[j] = champions[j+1];
                                                                        champions[j+1] = temp;
                                                                    }
                                                                }
                                                            }
                                                            showLeaderboardHeader("Top 10 GOATs - Grinders of All Time!");
                                                            printf("%-4s %-20s %-15s\n", "Rank", "Username", "Workouts Completed");
                                                            for (int i = 0; i < (champCount > 10 ? 10 : champCount); i++) {
                                                                printf("%-4d %-20s %-15d\n", i + 1, champions[i].username, champions[i].totalWorkouts);
                                                            }
                                                        }
                                                        free(champions);
                                                    } else if (leaderChoice == 2) {
                                                        int exerciseChoice;
                                                        showExerciseChoices();
                                                        while (1) {
                                                            printf("\nYour choice: ");
                                                            if (scanf("%d", &exerciseChoice) == 1) {
                                                                if (exerciseChoice >= 1 && exerciseChoice <= 8) {
                                                                    break;
                                                                }
                                                            }
                                                            printf("Please enter a valid choice!\n");
                                                            while (getchar() != '\n');
                                                        }
                                                        if (exerciseChoice == 8) {
                                                            continue;
                                                        }
                                                        StrengthRecord *records = NULL;
                                                        int recordCount = 0;
                                                        ExerciseType targetExercise = benchPR + (exerciseChoice - 1);

                                                        Account *current = head;
                                                        while (current) {
                                                            PR *pr = &current->personalRecords[exerciseChoice - 1];
                                                            if (pr->maxWeight > 0 || pr->maxSets > 0 || pr->maxReps > 0) {
                                                                records = realloc(records, (recordCount + 1) * sizeof(StrengthRecord));
                                                                strcpy(records[recordCount].username, current->username);
                                                                records[recordCount].bestWeight = pr->maxWeight;
                                                                records[recordCount].bestSets = pr->maxSets;
                                                                records[recordCount].bestReps = pr->maxReps;
                                                                recordCount++;
                                                            }
                                                            current = current->next;
                                                        }
                                                        if (recordCount == 0) {
                                                            showLeaderboardHeader("No Records Yet!");
                                                            printf("This thing needs its first champion!\n");
                                                        } else {
                                                            for (int i = 0; i < recordCount - 1; i++) {
                                                                for (int j = 0; j < recordCount - i - 1; j++) {
                                                                    if (records[j].bestWeight < records[j+1].bestWeight || (records[j].bestWeight == records[j+1].bestWeight && records[j].bestSets < records[j+1].bestSets) || (records[j].bestWeight == records[j+1].bestWeight && records[j].bestSets == records[j+1].bestSets && records[j].bestReps < records[j+1].bestReps)) {
                                                                        StrengthRecord temp = records[j];
                                                                        records[j] = records[j+1];
                                                                        records[j+1] = temp;
                                                                    }
                                                                }
                                                            }
                                                            showLeaderboardHeader(exerciseName(targetExercise));
                                                            printf("%-4s %-20s %-8s %-6s %-6s\n", "Rank", "Username", "Weight", "Sets", "Reps");
                                                            for (int i = 0; i < recordCount; i++) {
                                                                printf("%-4d %-20s %-8.1f %-6d %-6d\n", i + 1, records[i].username, records[i].bestWeight, records[i].bestSets, records[i].bestReps);
                                                            }
                                                        }
                                                        free(records);
                                                    }
                                                } else {
                                                    if (picks == 11) {
                                                        freeAccounts(head);
                                                        printf("Program Exiting...");
                                                        break;
                                                    } else {
                                                        printf("Invalid input, please try again...\n\n");
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
