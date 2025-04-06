#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct {
    char username[50];
    int totalWorkouts;
} FitnessChampion;

typedef struct {
    char username[50];
    float bestWeight;
    int bestSets;
    int bestReps;
} StrengthRecord;

typedef struct WorkoutQueue {
    ExerciseType items[50];
    int front, rear;
} WorkoutQueue;

typedef struct WorkoutHistory {
    Workout items[100];
    int top;
} WorkoutHistory;

typedef struct account {
    char username[50];
    char password[17];
    int day;
    int month;
    int year;
    char phoneNum[15];
    char email[30];
    char city[50];

    WorkoutQueue plannedWorkouts;
    WorkoutHistory completedWorkouts;
    PR personalRecords[7];
} Account;

typedef struct loginHistory {
    char username[50];
    char city[50];
} LoginHistory;

typedef struct queue {
    LoginHistory history[5];
    int front, rear, count;
} Queue;

void initializeQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

void enqueue(Queue *q, char username[], char city[]) {
    if (q->count == 5) {
        q->front = (q->front + 1) % 5;
        q->count--;
    }
    q->rear = (q->rear + 1) % 5;
    strcpy(q->history[q->rear].username, username);
    strcpy(q->history[q->rear].city, city);
    q->count++;
}

void deleteLoginHistory(Queue *q) {
    q->front = 0;
    q->rear = -1;
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
    int i = q->front;
    for (int j = 0; j < q->count; j++) {
        printf("Username: %s\n", q->history[i].username);
        printf("City    : %s\n", q->history[i].city);
        printf("--------------------\n");
        i = (i + 1) % 5;
    }
}

void modifyAccount(Account accounts[], int jumlahData, char user[]) {
    int index = -1;

    for (int i = 0; i < jumlahData; i++) {
        if (strcmp(accounts[i].username, user) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Error: Account not found!\n");
        return;
    }

    int choice;
    printf("\n============= Modify Account Details ===============\n");
    printf("1. Change Password\n");
    printf("2. Change Birthdate\n");
    printf("3. Change Phone Number\n");
    printf("4. Change Email\n");
    printf("5. Change City\n");
    printf("6. Cancel\n");
    while (1) {
        printf("Choose an option: ");
        if (scanf("%d", &choice) == 1) {
            break;
        } else {
            printf("Invalid input! Please enter a valid number.\n");
            while (getchar() != '\n');
        }
    }

    switch (choice) {
        case 1:
            printf("Enter new password: ");
            scanf("%s", accounts[index].password);
            break;
        case 2:
            printf("Enter new birthdate (DD MM YYYY): ");
            scanf("%d %d %d", &accounts[index].day, &accounts[index].month, &accounts[index].year);
            break;
        case 3:
            printf("Enter new phone number: ");
            scanf("%s", accounts[index].phoneNum);
            break;
        case 4:
            printf("Enter new email: ");
            scanf("%s", accounts[index].email);
            break;
        case 5:
            printf("Enter new city: ");
            scanf("%s", accounts[index].city);
            break;
        case 6:
            printf("Modification canceled.\n");
            return;
        default:
            printf("Invalid choice!\n");
            return;
    }

    FILE *file = fopen("accA.txt", "w");
    if (!file) {
        printf("Error: Unable to update account file!\n");
        return;
    }

    for (int i = 0; i < jumlahData; i++) {
        fprintf(file, "%s#%s#%d#%d#%d#%s#%s#%s\n", accounts[i].username, accounts[i].password, accounts[i].day, accounts[i].month, accounts[i].year, accounts[i].phoneNum, accounts[i].email, accounts[i].city);
    }
    fclose(file);

    printf("Account details successfully updated!\n");
}

void initWorkoutQueue(WorkoutQueue *q) {
    q->front = -1;
    q->rear = -1;
}

void enqueueWorkout(WorkoutQueue *q, ExerciseType ex) {
    if ((q->rear + 1) % 50 == q->front) {
        return;
    }
    if (q->front == -1) {
        q->front = 0;
    }
    q->rear = (q->rear + 1) % 50;
    q->items[q->rear] = ex;
}

ExerciseType dequeueWorkout(WorkoutQueue *q) {
    if (q->front == -1 || q->items[q->front] < 1) {
        return -1;
    }
    if (q->front == -1) {
        return -1;
    }
    int ex = q->items[q->front];
    if (q->front == q->rear) {
        q->front = q->rear = -1;
    } else {
        q->front = (q->front + 1) % 50;
    }
    return ex;
}

void initWorkoutHistory(WorkoutHistory *h) {
    h->top = -1;
}

void addWorkout(WorkoutHistory *h, Workout w) {
    if (h->top == 99) {
        return;
    }
    h->items[++h->top] = w;
}

void updatePR(Account *acc, Workout w) {
    if (w.exercise >= benchPR && w.exercise <= squatPR) {
        int prIndex = w.exercise - benchPR;
        if (prIndex >= 0 && prIndex < 7) {
            if (w.weight > acc->personalRecords[prIndex].maxWeight || (w.weight == acc->personalRecords[prIndex].maxWeight && w.sets > acc->personalRecords[prIndex].maxSets) || (w.weight == acc->personalRecords[prIndex].maxWeight && w.sets == acc->personalRecords[prIndex].maxSets && w.reps > acc->personalRecords[prIndex].maxReps)) {
                acc->personalRecords[prIndex].maxWeight = w.weight;
                acc->personalRecords[prIndex].maxSets = w.sets;
                acc->personalRecords[prIndex].maxReps = w.reps;
            }
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

void saveAccounts(Account accounts[], int count) {
    FILE *file = fopen("accA.txt", "w");
    if (!file) {
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s#%s#%d#%d#%d#%s#%s#%s#%d#%d#%d#", accounts[i].username, accounts[i].password, accounts[i].day, accounts[i].month, accounts[i].year, accounts[i].phoneNum, accounts[i].email, accounts[i].city, accounts[i].plannedWorkouts.front, accounts[i].plannedWorkouts.rear, accounts[i].completedWorkouts.top);
        for(int j = 0; j < 50; j++) {
            fprintf(file, "%d#", accounts[i].plannedWorkouts.items[j]);
        }
        for (int j = 0; j <= accounts[i].completedWorkouts.top; j++) {
            fprintf(file, "%d#%d#%d#%.2f#", accounts[i].completedWorkouts.items[j].exercise, accounts[i].completedWorkouts.items[j].sets, accounts[i].completedWorkouts.items[j].reps, accounts[i].completedWorkouts.items[j].weight);
        }
        for (int j = 0; j < 7; j++) {
            fprintf(file, "%d#%.2f#%d#%d#", accounts[i].personalRecords[j].exercise, accounts[i].personalRecords[j].maxWeight, accounts[i].personalRecords[j].maxSets, accounts[i].personalRecords[j].maxReps);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void cleanWorkoutQueue(WorkoutQueue *q) {
    WorkoutQueue temp;
    temp.front = -1;
    temp.rear = -1;
    memset(temp.items, 0, sizeof(ExerciseType) * 50);

    if (q->front == -1) {
        *q = temp;
        return;
    }

    int current = q->front;
    do {
        if (q->items[current] >= 1 && q->items[current] <= 14) {
            if (temp.front == -1) {
                temp.front = 0;
            }
            temp.rear = (temp.rear + 1) % 50;
            temp.items[temp.rear] = q->items[current];
        }
        if (current == q->rear) {
            break;
        }
        current = (current + 1) % 50;
    } while(1);

    *q = temp;
}

int loadAccounts(Account accounts[]) {
    FILE *file = fopen("accA.txt", "r");
    if (!file) {
        return 0;
    }

    int count = 0;
    char buffer[2048];
    while (fgets(buffer, sizeof(buffer), file) && count < 100) {
        memset(&accounts[count], 0, sizeof(Account));
        initWorkoutQueue(&accounts[count].plannedWorkouts);
        initWorkoutHistory(&accounts[count].completedWorkouts);

        char *ptr = strtok(buffer, "#\n");
        if (!ptr) continue;

        strncpy(accounts[count].username, ptr, 49);
        ptr = strtok(NULL, "#\n");

        strncpy(accounts[count].password, ptr, 16);
        ptr = strtok(NULL, "#\n");
        accounts[count].day = ptr ? atoi(ptr) : 1;
        ptr = strtok(NULL, "#\n");
        accounts[count].month = ptr ? atoi(ptr) : 1;
        ptr = strtok(NULL, "#\n");
        accounts[count].year = ptr ? atoi(ptr) : 2000;
        ptr = strtok(NULL, "#\n");
        strncpy(accounts[count].phoneNum, ptr ? ptr : "", 14);
        ptr = strtok(NULL, "#\n");
        strncpy(accounts[count].email, ptr ? ptr : "", 29);
        ptr = strtok(NULL, "#\n");
        strncpy(accounts[count].city, ptr ? ptr : "", 49);
        ptr = strtok(NULL, "#\n");

        accounts[count].plannedWorkouts.front = ptr ? atoi(ptr) : -1;
        ptr = strtok(NULL, "#\n");
        accounts[count].plannedWorkouts.rear = ptr ? atoi(ptr) : -1;
        ptr = strtok(NULL, "#\n");
        accounts[count].completedWorkouts.top = ptr ? atoi(ptr) : -1;
        ptr = strtok(NULL, "#\n");

        for (int j = 0; j < 50; j++) {
            if (ptr) {
                int val = atoi(ptr);
                accounts[count].plannedWorkouts.items[j] = (val >= 1 && val <= 14) ? (ExerciseType)val : 0;
            } else {
                accounts[count].plannedWorkouts.items[j] = 0;
            }
            ptr = strtok(NULL, "#\n");
        }

        for (int j = 0; j <= accounts[count].completedWorkouts.top; j++) {
            if (j >= 100) {
                break;
            }
            if (ptr) {
                int ex = atoi(ptr);
                accounts[count].completedWorkouts.items[j].exercise = (ex >= 1 && ex <= 14) ? (ExerciseType)ex : 0;
                ptr = strtok(NULL, "#\n");
            }
            if (ptr) {
                accounts[count].completedWorkouts.items[j].sets = atoi(ptr);
                ptr = strtok(NULL, "#\n");
            }
            if (ptr) {
                accounts[count].completedWorkouts.items[j].reps = atoi(ptr);
                ptr = strtok(NULL, "#\n");
            }
            if (ptr) {
                accounts[count].completedWorkouts.items[j].weight = atof(ptr);
                ptr = strtok(NULL, "#\n");
            }
        }

        if (accounts[count].completedWorkouts.top >= 0) {
            int validCount = 0;
            Workout validWorkouts[100];
            for (int j = 0; j <= accounts[count].completedWorkouts.top; j++) {
                if (accounts[count].completedWorkouts.items[j].exercise >= bench && accounts[count].completedWorkouts.items[j].exercise <= squatPR) {
                    validWorkouts[validCount++] = accounts[count].completedWorkouts.items[j];
                }
            }

            for (int j = 0; j < validCount; j++) {
                accounts[count].completedWorkouts.items[j] = validWorkouts[j];
            }
            accounts[count].completedWorkouts.top = validCount - 1;

            if (validCount == 0) {
                accounts[count].completedWorkouts.top = -1;
            }
        }

        for (int j = 0; j < 7; j++) {
            if (ptr) {
                int ex = atoi(ptr);
                accounts[count].personalRecords[j].exercise = (ex >= 1 && ex <= 14) ? (ExerciseType)ex : 0;
                ptr = strtok(NULL, "#\n");
            }
            if (ptr) {
                accounts[count].personalRecords[j].maxWeight = atof(ptr);
                ptr = strtok(NULL, "#\n");
            }
            if (ptr) {
                accounts[count].personalRecords[j].maxSets = atoi(ptr);
                ptr = strtok(NULL, "#\n");
            }
            if (ptr) {
                accounts[count].personalRecords[j].maxReps = atoi(ptr);
                ptr = strtok(NULL, "#\n");
            }
        }

        if (accounts[count].plannedWorkouts.front < -1 || accounts[count].plannedWorkouts.front >= 50) {
            accounts[count].plannedWorkouts.front = -1;
        }
        if (accounts[count].plannedWorkouts.rear < -1 || accounts[count].plannedWorkouts.rear >= 50) {
            accounts[count].plannedWorkouts.rear = -1;
        }
        if (accounts[count].completedWorkouts.top < -1 || accounts[count].completedWorkouts.top >= 100) {
            accounts[count].completedWorkouts.top = -1;
        }
        count++;
    }
    fclose(file);
    return count;
}

const char* exerciseName(ExerciseType ex) {
    const char* names[] = {
        "Placeholder",
        "Bench", "Crunches", "Curls", "Deadlift", "Leg Press", "Pull Ups", "Squat",
        "Bench PR", "Crunches PR", "Curls PR", "Deadlift PR", "Leg Press PR", "Pull Ups PR", "Squat PR"
    };
    return (ex >= 1 && ex <= 14) ? names[ex] : names[0];
}

int main(){
    Account accounts[100];
    int choice, jumlahData = 0, a = 0, isValid, picks;
    char user[50], pass[17], temp[17];
    Queue loginQueue;
    initializeQueue(&loginQueue);

    FILE *file = fopen("accA.txt", "r");
    if (!file) {
        file = fopen("accA.txt", "w");
        if (file) {
            fclose(file);
        }
        file = fopen("accA.txt", "r");
    }

    jumlahData = loadAccounts(accounts);

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

                    for (int i = 0; i < jumlahData; i++) {
                        if (strcmp(accounts[i].username, user) == 0 && strcmp(accounts[i].password, pass) == 0) {
                            isValid = 1;
                            enqueue(&loginQueue, user, accounts[i].city);
                            break;
                        }
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
                    FILE *signUp = fopen("accA.txt", "a");

                    while(1){
                        printf("===================================\n");
                        printf("               Sign Up            \n");
                        printf("===================================\n");
                        printf("Enter -1 to exit\n");

                        int exitFlag = 0;

                        do {
                            int isDuplicate = 0;
                            printf("Username\t: ");
                            scanf(" %[^\n]", temp);

                            if ((strcmp(temp, "-1")) == 0){
                                printf("Exiting...\n\n");
                                exitFlag = 1;
                                break;
                            }

                            for (int i = 0; i < jumlahData; i++) {
                                if (strcmp(temp, accounts[i].username) == 0) {
                                    printf("Sorry, Username has been used, please pick a different username!\n");
                                    isDuplicate = 1;
                                    break;
                                }
                            }

                            if (!isDuplicate) {
                                strcpy(accounts[jumlahData].username, temp);
                                fprintf(signUp, "%s#", accounts[jumlahData].username);
                                break;
                            }
                        } while(1);

                        if (exitFlag) {
                            fclose(signUp);
                            break;
                        }

                        do {
                            printf("Password\t: ");
                            scanf(" %s", temp);
                            if (strlen(temp) > 16) {
                                printf("Password is too long! Try again.\n");
                            } else if (strlen(temp) < 5) {
                                printf("Password is too short! Try again.\n");
                            } else {
                                strcpy(accounts[jumlahData].password, temp);
                                fprintf(signUp, "%s#", accounts[jumlahData].password);
                                break;
                            }
                        } while (1);

                        printf("Your Birthdate in numbers (DD MM YYYY)\n");

                        while (1) {
                            printf("day\t: ");
                            if (scanf(" %d", &accounts[jumlahData].day) == 1) {
                                while (getchar() != '\n');
                                break;
                            } else {
                                printf("Invalid input! Please enter a valid number.\n");
                                while (getchar() != '\n');
                            }
                        }
                        fprintf(signUp, "%d#", accounts[jumlahData].day);

                        while (1) {
                            printf("month\t: ");
                            if (scanf(" %d", &accounts[jumlahData].month) == 1) {
                                while (getchar() != '\n');
                                break;
                            } else {
                                printf("Invalid input! Please enter a valid number.\n");
                                while (getchar() != '\n');
                            }
                        }
                        fprintf(signUp, "%d#", accounts[jumlahData].month);

                        while (1) {
                            printf("year\t: ");
                            if (scanf(" %d", &accounts[jumlahData].year) == 1) {
                                while (getchar() != '\n');
                                break;
                            } else {
                                printf("Invalid input! Please enter a valid number.\n");
                                while (getchar() != '\n');
                            }
                        }
                        fprintf(signUp, "%d#", accounts[jumlahData].year);

                        printf("Phone Number\t: ");
                        scanf(" %[^\n]", accounts[jumlahData].phoneNum);
                        fprintf(signUp, "%s#", accounts[jumlahData].phoneNum);

                        printf("Email\t: ");
                        scanf(" %[^\n]", accounts[jumlahData].email);
                        fprintf(signUp, "%s#", accounts[jumlahData].email);

                        printf("City\t: ");
                        scanf(" %[^\n]", accounts[jumlahData].city);
                        fprintf(signUp, "%s#-1#-1#-1#", accounts[jumlahData].city);

                        for (int j = 0; j < 50; j++) {
                            fprintf(signUp, "0#");
                        }
                        fprintf(signUp, "0#0#0#0.00#");
                        for (int j = 0; j < 7; j++) {
                            fprintf(signUp, "%d#0.00#0#0#", benchPR + j);
                        }
                        fprintf(signUp, "\n");

                        initWorkoutQueue(&accounts[jumlahData].plannedWorkouts);
                        memset(accounts[jumlahData].plannedWorkouts.items, 0, 50*sizeof(int));
                        accounts[jumlahData].plannedWorkouts.front = -1;
                        accounts[jumlahData].plannedWorkouts.rear = -1;
                        initWorkoutHistory(&accounts[jumlahData].completedWorkouts);

                        for (int j = 0; j < 7; j++) {
                            accounts[jumlahData].personalRecords[j].exercise = benchPR + j;
                            accounts[jumlahData].personalRecords[j].maxWeight = 0.0f;
                            accounts[jumlahData].personalRecords[j].maxSets = 0;
                            accounts[jumlahData].personalRecords[j].maxReps = 0;
                        }
                        jumlahData++;
                        printf("===================================\n\n");
                        fclose(signUp);
                        break;
                    }
                } else {
                    if (choice == 3){
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
                            int found = 0, index;

                            for (int i = 0; i < jumlahData; i++) {
                                if (strcmp(accounts[i].username, user) == 0) {
                                    index = i;
                                    found = 1;
                                    break;
                                }
                            }
                            if (found) {
                                printf("\n============= Account Details ===============\n");
                                printf("Username        : %s\n", accounts[index].username);
                                printf("Birthdate       : %d-%d-%d\n", accounts[index].day, accounts[index].month, accounts[index].year);
                                printf("Phone Number    : %s\n", accounts[index].phoneNum);
                                printf("Email           : %s\n", accounts[index].email);
                                printf("City            : %s\n", accounts[index].city);
                                printf("==============================================\n");
                            } else {
                                printf("Error: Account not found!\n");
                            }
                        } else {
                            if (picks == 5){
                                modifyAccount(accounts, jumlahData, user);
                            } else {
                                if (picks == 6){
                                    int workoutChoice;
                                    do {
                                        printf("\n=== Plan Workout ===\n");
                                        printf("Current Planned Workouts:\n");
                                        for (int i = 0; i < jumlahData; i++) {
                                            if (strcmp(accounts[i].username, user) == 0) {
                                                if (accounts[i].plannedWorkouts.front == -1) {
                                                    printf("None\n");
                                                } else {
                                                    int j = accounts[i].plannedWorkouts.front;
                                                    while (1) {
                                                        printf("- %s\n", exerciseName(accounts[i].plannedWorkouts.items[j]));
                                                        if (j == accounts[i].plannedWorkouts.rear) break;
                                                        j = (j + 1) % 50;
                                                    }
                                                }
                                                break;
                                            }
                                        }
                                        printf("\nExercise Types:\n");
                                        for (int ex = 1; ex <= 14; ex++) {
                                            printf("%2d. %s\n", ex, exerciseName(ex));
                                        }
                                        printf(" 15. Finish planning!\n");
                                        printf("Choose exercise: ");
                                        if (scanf("%d", &workoutChoice) != 1) {
                                            while(getchar() != '\n');
                                            continue;
                                        }
                                        while(getchar() != '\n');
                                        if (workoutChoice == 15) {
                                            break;
                                        }
                                        if (workoutChoice < 1 || workoutChoice > 14) {
                                            printf("Invalid choice!\n");
                                            continue;
                                        }
                                        int duplicate = 0;
                                        for (int i = 0; i < jumlahData; i++) {
                                            if (strcmp(accounts[i].username, user) == 0) {
                                                if (accounts[i].plannedWorkouts.front != -1) {
                                                    int j = accounts[i].plannedWorkouts.front;
                                                    do {
                                                        if (accounts[i].plannedWorkouts.items[j] == workoutChoice) {
                                                            duplicate = 1;
                                                            break;
                                                        }
                                                        if (j == accounts[i].plannedWorkouts.rear) {
                                                            break;
                                                        }
                                                        j = (j + 1) % 50;
                                                    } while (1);
                                                }
                                                break;
                                            }
                                        }
                                        if (duplicate) {
                                            printf("%s is already in your plan!\n", exerciseName(workoutChoice));
                                            continue;
                                        }
                                        for (int i = 0; i < jumlahData; i++) {
                                            if (strcmp(accounts[i].username, user) == 0) {
                                                enqueueWorkout(&accounts[i].plannedWorkouts, workoutChoice);
                                                saveAccounts(accounts, jumlahData);
                                                printf("%s added!\n", exerciseName(workoutChoice));
                                                break;
                                            }
                                        }
                                    } while(1);
                                } else {
                                    if (picks == 7) {
                                        for (int i = 0; i < jumlahData; i++) {
                                            if (strcmp(accounts[i].username, user) == 0) {
                                                int tracking = 1;
                                                while (tracking) {
                                                    printf("\n=== Track Workout ===\n");
                                                    if (accounts[i].plannedWorkouts.front == -1) {
                                                        printf("No workouts to track!\n");
                                                        break;
                                                    }
                                                    int pos = 1;
                                                    int current = accounts[i].plannedWorkouts.front;
                                                    int validExercises[50];
                                                    int validIndexes[50];
                                                    int validCount = 0;

                                                    while (1) {
                                                        if (current == -1) {
                                                            break;
                                                        }
                                                        validExercises[validCount] = accounts[i].plannedWorkouts.items[current];
                                                        validIndexes[validCount] = current;
                                                        printf("%2d. %s\n", pos, exerciseName(validExercises[validCount]));
                                                        validCount++;
                                                        pos++;
                                                        if (current == accounts[i].plannedWorkouts.rear) {
                                                            break;
                                                        }
                                                        current = (current + 1) % 50;
                                                    }
                                                    printf("\n0. Finish tracking\n");
                                                    printf("Enter workout entry to track: ");
                                                    int trackChoice;
                                                    scanf("%d", &trackChoice);

                                                    if (trackChoice == 0) {
                                                        tracking = 0;
                                                    } else if (trackChoice > 0 && trackChoice <= validCount) {
                                                        int targetIndex = validIndexes[trackChoice-1];
                                                        ExerciseType ex = accounts[i].plannedWorkouts.items[targetIndex];

                                                        if (targetIndex == accounts[i].plannedWorkouts.front) {
                                                            dequeueWorkout(&accounts[i].plannedWorkouts);
                                                        } else {
                                                            int prev = accounts[i].plannedWorkouts.front;
                                                            while (prev != targetIndex) {
                                                                prev = (prev + 1) % 50;
                                                            }
                                                            int next = (targetIndex + 1) % 50;
                                                            while (next != (accounts[i].plannedWorkouts.rear + 1) % 50) {
                                                                accounts[i].plannedWorkouts.items[prev] = accounts[i].plannedWorkouts.items[next];
                                                                prev = next;
                                                                next = (next + 1) % 50;
                                                            }
                                                            accounts[i].plannedWorkouts.rear = (accounts[i].plannedWorkouts.rear - 1 + 50) % 50;
                                                        }

                                                        Workout w;
                                                        w.exercise = ex;
                                                        printf("Enter sets done: ");
                                                        scanf("%d", &w.sets);
                                                        printf("Enter reps done: ");
                                                        scanf("%d", &w.reps);
                                                        printf("Enter weight applied (kg): ");
                                                        scanf("%f", &w.weight);

                                                        addWorkout(&accounts[i].completedWorkouts, w);
                                                        updatePR(&accounts[i], w);
                                                        saveAccounts(accounts, jumlahData);
                                                        printf("%s tracked successfully!\n", exerciseName(ex));
                                                    } else {
                                                    printf("Invalid selection!\n");
                                                    }
                                                }
                                                break;
                                            }
                                        }
                                    } else {
                                        if (picks == 8) {
                                            printf("\n=== Unfinished Workouts ===\n");
                                            for (int i = 0; i < jumlahData; i++) {
                                                if (strcmp(accounts[i].username, user) == 0) {
                                                    if (accounts[i].plannedWorkouts.front == -1) {
                                                        printf("No planned workouts!\n");
                                                        break;
                                                    }
                                                    int pos = 1;
                                                    int current = accounts[i].plannedWorkouts.front;
                                                    int validExercises[50];
                                                    int validIndexes[50];
                                                    int validCount = 0;
                                                    while(1) {
                                                        if (accounts[i].plannedWorkouts.items[current] >= 1) {
                                                            validExercises[validCount] = accounts[i].plannedWorkouts.items[current];
                                                            validIndexes[validCount] = current;
                                                            printf("%2d. %s\n", pos, exerciseName(validExercises[validCount]));
                                                            validCount++;
                                                            pos++;
                                                        }
                                                        if (current == accounts[i].plannedWorkouts.rear) {
                                                            break;
                                                        }
                                                        current = (current + 1) % 50;
                                                    }
                                                    if (validCount == 0) {
                                                        printf("No valid workouts!\n");
                                                        break;
                                                    }
                                                    printf("\nEnter 0 to finish viewing or...\nEnter an entry's number to delete: ");
                                                    int delChoice;
                                                    if (scanf("%d", &delChoice) == 1 && delChoice > 0 && delChoice <= validCount) {
                                                        int targetIndex = validIndexes[delChoice-1];
                                                        int newFront = accounts[i].plannedWorkouts.front;
                                                        int newRear = accounts[i].plannedWorkouts.rear;
                                                        if (targetIndex == newFront) {
                                                            if (newFront == newRear) {
                                                                newFront = newRear = -1;
                                                            } else {
                                                                newFront = (newFront + 1) % 50;
                                                            }
                                                        } else {
                                                            int prev = newFront;
                                                            while (prev != targetIndex) {
                                                                prev = (prev + 1) % 50;
                                                            }
                                                            int next = (targetIndex + 1) % 50;
                                                            while (next != (newRear + 1) % 50) {
                                                                accounts[i].plannedWorkouts.items[prev] = accounts[i].plannedWorkouts.items[next];
                                                                prev = next;
                                                                next = (next + 1) % 50;
                                                            }
                                                            newRear = (newRear - 1 + 50) % 50;
                                                        }
                                                        accounts[i].plannedWorkouts.front = newFront;
                                                        accounts[i].plannedWorkouts.rear = newRear;
                                                        saveAccounts(accounts, jumlahData);
                                                        printf("Workout deleted successfully!\n");
                                                    }
                                                    break;
                                                }
                                            }
                                        } else {
                                            if (picks == 9) {
                                                printf("\n=== Workout History ===\n");
                                                for (int i = 0; i < jumlahData; i++) {
                                                    if (strcmp(accounts[i].username, user) == 0) {
                                                        if (accounts[i].completedWorkouts.top == -1) {
                                                            printf("No completed workouts yet!\n");
                                                        } else {
                                                            printf("|%-4s|%-15s|%-6s|%-6s|%-8s\n", "No.", "Exercise", "Sets", "Reps", "Weight");
                                                            for (int j = 0; j <= accounts[i].completedWorkouts.top; j++) {
                                                                printf("|%-4d|%-15s|%-6d|%-6d|%-8.1fkg\n", j+1, exerciseName(accounts[i].completedWorkouts.items[j].exercise), accounts[i].completedWorkouts.items[j].sets, accounts[i].completedWorkouts.items[j].reps, accounts[i].completedWorkouts.items[j].weight);
                                                            }
                                                        }
                                                        break;
                                                    }
                                                }
                                            } else {
                                                if (picks == 10) {
                                                    int leaderChoice;
                                                    showLeaderboardHeader("   THE GRAND FITNESS LEADERBOARDS");
                                                    printf("1. Top Total Workouts!!!\n");
                                                    printf("2. Top PR Records!!!\n");
                                                    printf("3. Return to Main Menu\n");

                                                    while(1) {
                                                        printf("\nChoose leaderboard type: ");
                                                        if (scanf("%d", &leaderChoice) == 1) {
                                                            if (leaderChoice >=1 && leaderChoice <=3) break;
                                                        }
                                                        printf("Please enter a valid choice!\n");
                                                        while(getchar() != '\n');
                                                    }

                                                    if (leaderChoice == 1) {
                                                        FitnessChampion champions[100];
                                                        int champCount = 0;

                                                        for (int i = 0; i < jumlahData; i++) {
                                                            int total = accounts[i].completedWorkouts.top + 1;
                                                            if (total > 0) {
                                                                strcpy(champions[champCount].username, accounts[i].username);
                                                                champions[champCount].totalWorkouts = total;
                                                                champCount++;
                                                            }
                                                        }

                                                        if (champCount == 0) {
                                                            showLeaderboardHeader("No Workouts Recorded Yet...");
                                                            printf("Be the first to complete a workout!!!\n");
                                                            continue;
                                                        }
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
                                                            printf("%-4d %-20s %-15d\n", i+1, champions[i].username, champions[i].totalWorkouts);
                                                        }
                                                    } else if(leaderChoice == 2) {
                                                        int exerciseChoice;
                                                        showExerciseChoices();

                                                        while(1) {
                                                            printf("\nYour choice: ");
                                                            if (scanf("%d", &exerciseChoice) == 1) {
                                                                if (exerciseChoice >=1 && exerciseChoice <=8) {
                                                                    break;
                                                                }
                                                            }
                                                            printf("Please enter a valid choice!\n");
                                                            while(getchar() != '\n');
                                                        }
                                                        if (exerciseChoice == 8) {
                                                            continue;
                                                        }
                                                        StrengthRecord records[100];
                                                        int recordCount = 0;
                                                        ExerciseType targetExercise = benchPR + (exerciseChoice - 1);

                                                        for (int i = 0; i < jumlahData; i++) {
                                                            PR *pr = &accounts[i].personalRecords[exerciseChoice-1];
                                                            if(pr->maxWeight > 0 || pr->maxSets > 0 || pr->maxReps > 0) {
                                                                strcpy(records[recordCount].username, accounts[i].username);
                                                                records[recordCount].bestWeight = pr->maxWeight;
                                                                records[recordCount].bestSets = pr->maxSets;
                                                                records[recordCount].bestReps = pr->maxReps;
                                                                recordCount++;
                                                            }
                                                        }
                                                        if (recordCount == 0) {
                                                            showLeaderboardHeader("No Records Yet!");
                                                            printf("This thing needs its first champion!\n");
                                                            continue;
                                                        }
                                                        for (int i = 0; i < recordCount - 1; i++) {
                                                            for (int j = 0; j < recordCount - i - 1; j++) {
                                                                int swap = 0;
                                                                if (records[j].bestWeight < records[j+1].bestWeight) {
                                                                    swap = 1;
                                                                } else if (records[j].bestWeight == records[j+1].bestWeight) {
                                                                    if (records[j].bestSets < records[j+1].bestSets) {
                                                                        swap = 1;
                                                                    } else if (records[j].bestSets == records[j+1].bestSets) {
                                                                        if (records[j].bestReps < records[j+1].bestReps) {
                                                                            swap = 1;
                                                                        }
                                                                    }
                                                                }
                                                                if (swap) {
                                                                    StrengthRecord temp = records[j];
                                                                    records[j] = records[j+1];
                                                                    records[j+1] = temp;
                                                                }
                                                            }
                                                        }
                                                        showLeaderboardHeader(exerciseName(targetExercise));
                                                        printf("%-4s %-20s %-8s %-6s %-6s\n", "Rank", "Username", "Weight", "Sets", "Reps");
                                                        for (int i = 0; i < recordCount; i++) {
                                                            printf("%-4d %-20s %-8.1f %-6d %-6d\n", i+1, records[i].username, records[i].bestWeight, records[i].bestSets,  records[i].bestReps);
                                                        }
                                                    }
                                                } else {
                                                    if (picks == 11) {
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
