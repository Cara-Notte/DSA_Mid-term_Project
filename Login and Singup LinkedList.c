// Login And SignUp Feature using LINKED LIST //

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct account{
    char username[50];
    char password[17];
    int day;
    int month;
    int year;
    char phoneNum[15];
    char email[20];
    char city[50];
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
    while (current) { // Standard linked list traversal
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

            FILE *file = fopen("acc.txt", "w");
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
    int choice, jumlahData = 0, a = 0, isValid, picks;
    char user[50], pass[17], temp[17], name[50];
    Queue loginQueue;
    initializeQueue(&loginQueue);

    FILE *file = fopen("acc.txt", "r");
    if (!file) {
        printf("File not found!\n");
        return 1;
    }

    char username[50], password[17], phoneNum[15], email[20], city[50];
    int day, month, year;

    while (fscanf(file, "%[^#]#%[^#]#%d#%d#%d#%[^#]#%[^#]#%[^\n]\n", username, password, &day, &month, &year, phoneNum, email, city) != EOF) {
        Account *newNode = (Account *)malloc(sizeof(Account));
        strcpy(newNode->username, username);
        strcpy(newNode->password, password);
        newNode->day = day;
        newNode->month = month;
        newNode->year = year;
        strcpy(newNode->phoneNum, phoneNum);
        strcpy(newNode->email, email);
        strcpy(newNode->city, city);

        newNode->next = NULL;

        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    fclose(file);

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
            printf("6. Exit\n");
            printf("Choice: ");
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
                    FILE *signUp = fopen("acc.txt", "a");
                    Account *newNode = (Account *)malloc(sizeof(Account));

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
                        fprintf(signUp, "%s\n", newNode->city);

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
