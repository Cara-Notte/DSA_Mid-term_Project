#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Structures that will hold the data

typedef struct account{
    char username[50];
    char password[17];
    int day;
    int month;
    int year;
    char phoneNum[15];
    char email[50];
    char city[50];
}Account;

typedef struct Admin{
    char username[50];
    char password[17];
}Admins;

typedef struct Facility {
    char name[50];
    char description[100];
    int popularity;
}Facility;

typedef struct Class {
    char name[50];
    char day[10];
    int startHour;
    int key;
    char instructor[50];

    struct Class *left, *right;
}Class;

typedef struct plan {
    char name[50];
    int popularity;
    int duration;
    char difficulty[15];
}Plan;

// Account related functions

void login (Account acc[], int *log, int a, char *user[], Admins adm[], int *mod){
    char pass[17];
    int Valid;
    printf("===================================\n");
    printf("               Login            \n");
    printf("===================================\n");
    printf("Enter -1 to exit\n");

    do {
        printf("Username: ");
        scanf(" %[^\n]", user);

        if ((strcmp(user, "-1")) == 0){
            printf("Exiting...\n\n");
            return;
        }

        printf("Password: ");
        scanf(" %[^\n]", pass);

        for (int i = 0; i < a; i++) {
            if (strcmp(acc[i].username, user) == 0 && strcmp(acc[i].password, pass) == 0) {
                Valid = 1;
                break;
            }
            if (strcmp(adm[i].username, user) == 0 && strcmp(adm[i].password, pass) == 0) {
                Valid = 1;
                (*mod) = 1;
                break;
            }
        }

        if (!Valid) {
            printf("Something Went Wrong... Incorrect Username or Password!\n");
            continue;
        } else {
            printf("Login Successful!\n");
            (*log)++;
            return;
        }
        printf("===================================\n");
    } while(1);
}

void signUp(Account acc[], int *data){
    char temp[50];
    FILE *signUp = fopen("acc.txt", "a");
    printf("===================================\n");
    printf("               Sign Up            \n");
    printf("===================================\n");
    printf("Enter -1 to exit\n");

    do {
        int isDuplicate = 0;
        printf("Username\t: ");
        scanf(" %[^\n]", temp);

        if ((strcmp(temp, "-1")) == 0){
            printf("Exiting...\n\n");
            fclose(signUp);
            return;
        }

        for (int i = 0; i < *data; i++) {
            if (strcmp(temp, acc[i].username) == 0) {
                printf("Sorry, Username has been used, please pick a different username!\n");
                isDuplicate = 1;
                break;
            }
        }

        if (!isDuplicate) {
            strcpy(acc[*data].username, temp);
            fprintf(signUp, "%s#", acc[*data].username);
            break;
        }
    } while(1);

    do {
        printf("Password (Don't add any space)\t: ");
        scanf(" %s", temp);
        if (strlen(temp) > 16) {
            printf("Password is too long! Try again.\n");
        } else if (strlen(temp) < 5) {
            printf("Password is too short! Try again.\n");
        } else {
            strcpy(acc[*data].password, temp);
            fprintf(signUp, "%s#", acc[*data].password);
            break;
        }
    } while (1);

    printf("Your Birthdate in numbers (DD MM YYYY)\n");
    while (1) {
        printf("day\t: ");
        if (scanf(" %d", &acc[*data].day) == 1) {
            while (getchar() != '\n');
            break;
        } else {
            printf("Invalid input! Please enter a valid number.\n");
            while (getchar() != '\n');
        }
    }
    fprintf(signUp, "%d#", acc[*data].day);

    while (1) {
        printf("month\t: ");
        if (scanf(" %d", &acc[*data].month) == 1) {
            while (getchar() != '\n');
            break;
        } else {
            printf("Invalid input! Please enter a valid number.\n");
            while (getchar() != '\n');
        }
    }
    fprintf(signUp, "%d#", acc[*data].month);

    while (1) {
        printf("year\t: ");
        if (scanf(" %d", &acc[*data].year) == 1) {
            while (getchar() != '\n');
            break;
        } else {
            printf("Invalid input! Please enter a valid number.\n");
            while (getchar() != '\n');
        }
    }
    fprintf(signUp, "%d#", acc[*data].year);

    printf("Phone Number\t: ");
    scanf(" %[^\n]", acc[*data].phoneNum);
    fprintf(signUp, "%s#", acc[*data].phoneNum);

    printf("Email\t: ");
    scanf(" %[^\n]", acc[*data].email);
    fprintf(signUp, "%s#", acc[*data].email);

    printf("City\t: ");
    scanf(" %[^\n]", acc[*data].city);
    fprintf(signUp, "%s\n", acc[*data].city);

    fclose(signUp);
    (*data)++;
    printf("===================================\n\n");
}

void AccountDetails(Account acc[], int data, char user[]) {
    int found = 0, index;

    for (int i = 0; i < data; i++) {
        if (strcmp(acc[i].username, user) == 0) {
            index = i;
            found = 1;
            break;
        }
    }

    if (found) {
        printf("\n============= Account Details ===============\n");
        printf("Username        : %s\n", acc[index].username);
        printf("Birthdate       : %d-%d-%d\n", acc[index].day, acc[index].month, acc[index].year);
        printf("Phone Number    : %s\n", acc[index].phoneNum);
        printf("Email           : %s\n", acc[index].email);
        printf("City            : %s\n", acc[index].city);
        printf("==============================================\n");
    } else {
        printf("Error: Account not found!\n");
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

    FILE *file = fopen("acc.txt", "w");
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

void AccountSet(int *log, Account acc[], int data, char user[]){
    int choice;
    while(1){
        printf("\n============= Account Setting ===============\n");
        printf("1. LogOut\n");
        printf("2. Account Details\n");
        printf("3. Edit Account Details\n");
        printf("4. Exit\n");
        printf("Choice: ");
        scanf(" %d", &choice);

        switch (choice) {
        case 1:
            (*log) = 0;
            
            return;

        case 2:
            AccountDetails(acc, data, user);
            break;

        case 3:
            modifyAccount(acc, data, user);
            break;

        case 4:
            printf("Exiting Settings...\n");
            return;

        default:
            break;
        }
    }
}

// Classes related functions (add class, show classes, ect). This uses BST logic

int dayToNumber(char *day) {
    if (strcmp(day, "Monday") == 0) return 1;
    if (strcmp(day, "Tuesday") == 0) return 2;
    if (strcmp(day, "Wednesday") == 0) return 3;
    if (strcmp(day, "Thursday") == 0) return 4;
    if (strcmp(day, "Friday") == 0) return 5;
    if (strcmp(day, "Saturday") == 0) return 6;
    if (strcmp(day, "Sunday") == 0) return 7;
    return 0;
}

void printScheduleEachDay(Class *root, char *day) {
    if (root == NULL) return;

    printScheduleEachDay(root->left, day);
    if (strcmp(root->day, day) == 0) {
        printf("  %02d:00 - %s with %s\n", root->startHour, root->name, root->instructor);
    }
    printScheduleEachDay(root->right, day);
}

void printSchedule(Class *root) {
    char *days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    for (int i = 0; i < 7; i++) {
        printf("%s:\n", days[i]);
        printScheduleEachDay(root, days[i]);
        printf("\n");
    }
}

Class* insertClass(Class *root, Class *newClass) {
    if (root == NULL) {
        Class *node = (Class*) malloc(sizeof(Class));
        *node = *newClass;
        node->left = node->right = NULL;
        return node;
    }

    if (newClass->key < root->key) {
        root->left = insertClass(root->left, newClass);
    } else {
        root->right = insertClass(root->right, newClass);
    }

    return root;
}

void freeClassTree(Class *root) {
    if (root == NULL) return;

    freeClassTree(root->left);
    freeClassTree(root->right);

    free(root);
}

Class* searchClassByKey(Class *root, int key) {
    if (root == NULL) return NULL;

    if (key == root->key)
        return root;
    else if (key < root->key)
        return searchClassByKey(root->left, key);
    else
        return searchClassByKey(root->right, key);
}

void searchClass(Class *root){
    int hour, day;
    printf("Enter start hour (0-23): ");
    scanf("%d", &hour);
    printf("Enter day (1 = Monday, ..., 7 = Sunday): ");
    scanf("%d", &day);

    int key = hour * 100 + day;
    Class *found = searchClassByKey(root, key);
    if (found) {
        printf("Found class: %s on %s with %s\n", found->name, found->day, found->instructor);
    } else {
        printf("No class found at that time.\n");
    }
}

Class *findMin(Class *node) {
    while (node && node->left != NULL) {
        node = node->left;
    }
    return node;
}

Class *deleteClass(Class *root, char *name, char *day, int key) {
    if (root == NULL) return NULL;

    if (key < root->key) {
        root->left = deleteClass(root->left, name, day, key);
    } else if (key > root->key) {
        root->right = deleteClass(root->right, name, day, key);
    } else {
        if (strcmp(root->name, name) == 0 && strcmp(root->day, day) == 0) {
            if (root->left == NULL) {
                Class* temp = root->right;
                free(root);
                return temp;
            } else if (root->right == NULL) {
                Class* temp = root->left;
                free(root);
                return temp;
            }

            Class* temp = findMin(root->right);
            strcpy(root->name, temp->name);
            strcpy(root->day, temp->day);
            root->startHour = temp->startHour;
            root->key = temp->key;
            strcpy(root->instructor, temp->instructor);
            root->right = deleteClass(root->right, temp->name, temp->day, temp->key);
        } else {
            root->left = deleteClass(root->left, name, day, key);
            root->right = deleteClass(root->right, name, day, key);
        }
    }

    return root;
}

void writeClassToFile(FILE *file, Class *root) {
    if (root == NULL) return;

    writeClassToFile(file, root->left);

    fprintf(file, "%s#%s#%d#%s\n", root->name, root->day, root->startHour, root->instructor);

    writeClassToFile(file, root->right);
}

Class *deleteAClass(Class *root){
    char name[50];
    char day[15];
    int startHour, dayNumber, key;

    printf("Enter Class name: ");
    scanf(" %[^\n]", name);
    printf("Enter the Day of the class (e.g. Monday): ");
    scanf(" %[^\n]", day);
    printf("Enter The Hour of the class (0–23): ");
    scanf(" %d", &startHour);

    dayNumber = dayToNumber(day);
    key = startHour * 100 + dayNumber;

    root = deleteClass(root, name, day, key);

    FILE *file = fopen("classes.txt", "w");
    writeClassToFile(file, root);
    fclose(file);

    printf("Class Succesfully Deleted!\n");
    return root;
}

Class *addClass(Class *root){
    Class *newClass = (Class *)malloc(sizeof(Class));
    printf("Class Name: ");
    scanf(" %[^\n]", newClass->name);
    do {
        int b = 1, day;
        while(b){
            printf("Day:\n");
            printf("1. Monday\n");
            printf("2. Tuesday\n");
            printf("3. Wednesday\n");
            printf("4. Thursday\n");
            printf("5. Friday\n");
            printf("6. Saturday\n");
            printf("7. Sunday\n");
            scanf(" %d", &day);
            switch(day){
                case 1:
                    strcpy(newClass->day, "Monday");
                    b = 0;
                    break;

                case 2:
                    strcpy(newClass->day, "Tuesday");
                    b = 0;
                    break;
                
                case 3:
                    strcpy(newClass->day, "Wednesday");
                    b = 0;
                    break;

                case 4:
                    strcpy(newClass->day, "Thursday");
                    b = 0;
                    break;

                case 5:
                    strcpy(newClass->day, "Friday");
                    b = 0;
                    break;

                case 6:
                    strcpy(newClass->day, "Saturday");
                    b = 0;
                    break;

                case 7:
                    strcpy(newClass->day, "Sunday");
                    b = 0;
                    break;

                default:
                    printf("Enter a valid input!");
            }
        }
        do{
            printf("Start Hour (0-23): ");
            scanf("%d", &newClass->startHour);
        }while ((newClass->startHour) < 0 || (newClass->startHour) > 23);

        newClass->key = newClass->startHour * 100 + day;

        if (searchClassByKey(root, newClass->key) != NULL) {
            printf("A class already exists at that day and hour. Please try again.\n");
        } else {
            break;
        }

    }while(1);

    printf("Instructor: ");
    scanf(" %[^\n]", newClass->instructor);

    root = insertClass(root, newClass);

    FILE *fp = fopen("classes.txt", "a");
    fprintf(fp, "%s#%s#%d#%s\n", newClass->name, newClass->day, newClass->startHour, newClass->instructor);
    fclose(fp);

    free(newClass);
    printf("Class Succesfully Addedd!\n");
    return root;
}

void classes(int mod){
    Class *root = NULL;
    FILE *fp = fopen("classes.txt", "r");

    if (fp != NULL) {
        Class *temp = (Class *)malloc(sizeof(Class));
        while (fscanf(fp, "%[^#]#%[^#]#%d#%[^\n]\n", temp->name, temp->day, &temp->startHour, temp->instructor) != EOF) {
            int dayNumber = dayToNumber(temp->day);
            temp->key = temp->startHour * 100 + dayNumber;
            temp->left = temp->right = NULL;
            root = insertClass(root, temp);
        }
        fclose(fp);
    }

    int choice;
    int a = 1;
    while(a){
        printf("\n============= Classes ===============\n");
        printf("1. View Schedule\n");
        printf("2. Search by Time\n");
        printf("3. Exit\n");
        if (mod == 1){
            printf("4. Add Class\n");
            printf("5. Delete Existing Class\n");
        }
        printf("Choice: ");
        scanf("%d", &choice);

        if (mod == 1){
            switch (choice){
                case 1:
                    printSchedule(root);
                    break;

                case 2:
                    searchClass(root);
                    break;

                case 3:
                    printf("Exiting Classes...\n");
                    freeClassTree(root);
                    return;

                case 4: {
                    root = addClass(root);
                    break;
                }

                case 5: {
                    root = deleteAClass(root);
                    break;
                }

                default:
                    printf("Invalid input, please try again...\n\n");
            }
        } else {
            switch (choice){
                case 1:
                    printSchedule(root);
                    break;

                case 2:
                    searchClass(root);
                    break;

                case 3:
                    printf("Exiting Classes...\n");
                    freeClassTree(root);
                    return;

                default:
                    printf("Invalid input, please try again...\n\n");
            }
        }
    }
}

// Functions about facilities feature. Uses Binary Heap logic

void displayFac(Facility fac[], int size) {
    if (size == 0) {
        printf("Heap is empty.\n");
        return;
    }

    printf("Current Facilities:\n");
    for (int i = 0; i < size; i++) {
        printf("\n");
        printf("%d. %s", i+1, fac[i].name);
        if (i == 0){
            printf(" (MOST POPULAR!!!)");
        }
        printf("\n");
        printf("Has been used %d Times\n", fac[i].popularity);
        printf("Description: %s\n", fac[i].description);
        printf("\n============================================================================\n");
    }

    printf("Total of Facilities: %d\n\n", size);
}

void swap(Facility *a, Facility *b) {
    Facility temp = *a;
    *a = *b;
    *b = temp;
}

void insertFacility(Facility fac[], int *size) {
    if ((*size) >= 50) {
        printf("Data is full!\n");
        return;
    }

    int i = (*size)++;
    printf("Facility Name: ");
    scanf(" %[^\n]", fac[i].name);
    printf("Description: ");
    scanf(" %[^\n]", fac[i].description);
    printf("Input it's popularity (How many people have used it): ");
    scanf(" %d", &fac[i].popularity);

    while (i != 0 && fac[(i - 1) / 2].popularity < fac[i].popularity) {
        swap(&fac[i], &fac[(i - 1) / 2]);
        i = (i - 1) / 2;
    }

    FILE *fp = fopen("facilities.txt", "a");
    fprintf(fp, "%s#%s#%d#%s\n", fac[i].name, fac[i].description, fac[i].popularity);
    fclose(fp);

    printf("Facility inserted successfully!\n");
}

void searchFacility(Facility fac[], int data){
    char search[50]; int found = 0;
    printf("Input a facility you want to find: ");
    scanf(" %[^\n]", search);

    for (int i = 0; i < data; i++){
        if ((strcmp(fac[i].name, search)) == 0){
            printf("%s", fac[i].name);
            if (i == 0){
                printf("(MOST POPULAR!!!)");
            }
            printf("\n");
            printf("Has been used %d Times\n", fac[i].popularity);
            printf("Description: %s\n\n", fac[i].description);
            found = 1;
        }
    }

    if (found == 0){
        printf("Fasilitas tidak ditemukan!\n");
    }
}

void deleteFacility(Facility fac[], int *size) {
    char target[50];
    printf("Enter the facility name to delete: ");
    scanf(" %[^\n]", target);

    int index = -1;
    for (int i = 0; i < *size; i++) {
        if (strcmp(fac[i].name, target) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Facility not found!\n");
        return;
    }

    printf("Deleting facility: %s\n", fac[index].name);

    fac[index] = fac[--(*size)];

    int i = index;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        if (left < *size && fac[left].popularity > fac[largest].popularity)
            largest = left;
        if (right < *size && fac[right].popularity > fac[largest].popularity)
            largest = right;

        if (largest != i) {
            swap(&fac[i], &fac[largest]);
            i = largest;
        } else {
            break;
        }
    }

    while (i != 0 && fac[(i - 1) / 2].popularity < fac[i].popularity) {
        swap(&fac[i], &fac[(i - 1) / 2]);
        i = (i - 1) / 2;
    }

    FILE *file = fopen("facilities.txt", "w");
    for (int i = 0; i < size; i++){
        fprintf(file, "%s#%s#%d#%s\n", fac[i].name, fac[i].description, fac[i].popularity);
    }
    fclose(file);


    printf("Facility deleted successfully!\n");
}

void heapifyDown(Facility facility[], int n, int i) {
    int largest = i;
    int left = 2*i + 1;
    int right = 2*i + 2;

    if (left < n && facility[left].popularity > facility[largest].popularity)
        largest = left;
    if (right < n && facility[right].popularity > facility[largest].popularity)
        largest = right;

    if (largest != i) {
        swap(&facility[i], &facility[largest]);
        heapifyDown(facility, n, largest);
    }
}

void facilities(int mod){
    Facility facility[50];
    int jumlahFasilitas = 0;
    FILE *fp = fopen("facilities.txt", "r");

    if (fp != NULL) {
        while (fscanf(fp, "%[^#]#%[^#]#%d\n", facility[jumlahFasilitas].name, facility[jumlahFasilitas].description, &facility[jumlahFasilitas].popularity) != EOF) {
            jumlahFasilitas++;
        }
        fclose(fp);
    }

    for (int i = jumlahFasilitas / 2 - 1; i >= 0; i--) {
        heapifyDown(facility, jumlahFasilitas, i);
    }

    int a = 1, choice;
    while (a){
        printf("\n============= Our Facilities ===============\n");
        printf("1. View Facilities\n");
        printf("2. Search a Facility\n");
        printf("3. Exit\n");
        if (mod == 1){
            printf("4. Add New Facility\n");
            printf("5. Delete Existing Facility\n");
        }
        printf("Choice: ");
        scanf("%d", &choice);
        if (mod == 0){
            switch (choice) {
                case 1:
                    displayFac(facility, jumlahFasilitas);
                    break;
                
                case 2:
                    searchFacility(facility, jumlahFasilitas);
                    break;

                case 3:
                    printf("Exiting Facility...\n");
                    return;

                default:
                    break;
            }
        } else {
            switch (choice) {
                case 1:
                    displayFac(facility, jumlahFasilitas);
                    break;
                
                case 2:
                    searchFacility(facility, jumlahFasilitas);
                    break;

                case 3:
                    printf("Exiting Facility...\n");
                    return;

                case 4:
                    insertFacility(facility, &jumlahFasilitas);
                    break;

                case 5:
                    deleteFacility(facility, &jumlahFasilitas);
                    break;

                default:
                    printf("Invalid input, please try again...\n\n");
            }
        }
    }
    
}

// Functions about Workout Plans (Using 2 Sorting algorithms BubbleSort and QuickSort ascending + descending) + Binary Search for the search feature

void displayPlans(Plan plans[], int data) {
    for (int i = 0; i < data; i++){
        printf("==============================\n");
        printf("%s\n", plans[i].name);
        printf("------------------------------\n");
        printf("Difficulty: %s\n", plans[i].difficulty);
        printf("Popularity: %d\n", plans[i].popularity);
        printf("Duration: %d Weeks\n", plans[i].duration);
        printf("==============================\n\n");
    }
}

void swapPlans(Plan *A, Plan *B) {
    Plan temp = *A;
    *A = *B;
    *B = temp;
}

void SortPopularDescending(Plan plans[], int data) {
    int i, j;
    for (i = 0; i < data - 1; i++) {
        for (j = 0; j < data - i - 1; j++) {
            if (plans[j].popularity < plans[j + 1].popularity) {
                swapPlans(&plans[j], &plans[j + 1]);
            }
        }
    }
}

void SortPopularAscending(Plan plans[], int data) {
    int i, j;
    for (i = 0; i < data - 1; i++) {
        for (j = 0; j < data - i - 1; j++) {
            if (plans[j].popularity > plans[j + 1].popularity) {
                swapPlans(&plans[j], &plans[j + 1]);
            }
        }
    }
}

void sortPopularity(Plan plans[], int data) {
    int choice;
    printf("\n============= Sort By Popularity ===============\n");
    printf("1. From Most to Least popular\n");
    printf("2. From Least to Most popular\n");
    printf("Choice: ");
    scanf(" %d", &choice);
    switch(choice){
        case 1:
            SortPopularDescending(plans, data);
            break;

        case 2:
            SortPopularAscending(plans, data);
            break;

        default:
            printf("Invalid input, please try again...\n\n");
    }
}

int partitionDec(Plan *plans, int l, int r) {
    int pivot = plans[r].duration;
    int i = l - 1;
    for (int j = l; j <= r - 1; j++) {
        if (plans[j].duration >= pivot) {
            i++;
            swapPlans(&plans[i], &plans[j]);
        }
    }
    swapPlans(&plans[i + 1], &plans[r]);
    return (i + 1);
}

void quickSortDec(Plan *plans, int l, int r) {
    if (l < r) {
        int pi = partitionDec(plans, l, r);
        quickSortDec(plans, l, pi - 1);
        quickSortDec(plans, pi + 1, r);
    }
}

int partitionAsc(Plan *plans, int l, int r) {
    int pivot = plans[r].duration;
    int i = l - 1;
    for (int j = l; j <= r - 1; j++) {
        if (plans[j].duration <= pivot) {
            i++;
            swapPlans(&plans[i], &plans[j]);
        }
    }
    swapPlans(&plans[i + 1], &plans[r]);
    return (i + 1);
}

void quickSortAsc(Plan *plans, int l, int r) {
    if (l < r) {
        int pi = partitionAsc(plans, l, r);
        quickSortAsc(plans, l, pi - 1);
        quickSortAsc(plans, pi + 1, r);
    }
}

void SortDurationDescending(Plan plans[], int data) {
    quickSortDec(plans, 0, data - 1);
}

void SortDurationAscending(Plan plans[], int data) {
    quickSortAsc(plans, 0, data - 1);
}

void sortDuration(Plan plans[], int data) {
    int choice;
    printf("\n============= Sort By Duration ===============\n");
    printf("1. From Longest to Shortest Duration\n");
    printf("2. From Shortest to Longest Duration\n");
    printf("Choice: ");
    scanf(" %d", &choice);
    switch(choice){
        case 1:
            SortDurationDescending(plans, data);
            break;

        case 2:
            SortDurationAscending(plans, data);
            break;

        default:
            printf("Invalid input, please try again...\n\n");
    }
}

void sortPlans(Plan plans[], int data) {
    int a = 1, choice;
    while (a){
        printf("\n============= Sort Workout Plans ===============\n");
        printf("1. Sort by popularity\n");
        printf("2. Sort by duration\n");
        printf("3. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                sortPopularity(plans, data);
                break;
            
            case 2:
                sortDuration(plans, data);
                break;

            case 3:
                printf("Exiting Sorting Plans...\n");
                return;

            default:
                printf("Invalid input, please try again...\n\n");
        }
    }
}

void searchByName(Plan plans[], int data) {
    char search[50];
    int found = 0;
    printf("Input the Plan you want to find: ");
    scanf(" %[^\n]", search);

    int mid, low, high;
    low = 0;
    high = data - 1;
    while (low <= high){
        mid = (low + high) / 2;
        if (strcmp(search, plans[mid].name) == 0) {
            printf("==============================\n");
            printf("%s\n", plans[mid].name);
            printf("------------------------------\n");
            printf("Difficulty: %s\n", plans[mid].difficulty);
            printf("Popularity: %d\n", plans[mid].popularity);
            printf("Duration: %d Weeks\n", plans[mid].duration);
            printf("==============================\n\n");
            found = 1;
            break;
        }
        if (strcmp(search, plans[mid].name) < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    if (found == 0){
        printf("Plan Not Found!\n");
    }
}

void searchByDiff(Plan plans[], int data) {
    char search[50];
    int choice;
    printf("Which Difficulty:\n");
    printf("1. Beginner\n");
    printf("2. Intermediate\n");
    printf("3. Advanced\n");
    scanf(" %d", &choice);
    switch (choice){
        case 1: {
            strcpy(search, "Beginner");
            break;
        }
        
        case 2: {
            strcpy(search, "Intermediate");
            break;
        }

        case 3: {
            strcpy(search, "Advanced");
            break;
        }

        default:
            printf("Invalid input, please try again...\n\n");
    }

    for (int i = 0; i < data; i++){
        if (strcmp(plans[i].difficulty, search) == 0){
            printf("==============================\n");
            printf("%s\n", plans[i].name);
            printf("------------------------------\n");
            printf("Difficulty: %s\n", plans[i].difficulty);
            printf("Popularity: %d\n", plans[i].popularity);
            printf("Duration: %d Weeks\n", plans[i].duration);
            printf("==============================\n\n");
        } 
    }

}

void sortPlansByName(Plan plans[], int data) {
    int i, j;
    for (i = 0; i < data - 1; i++) {
        for (j = 0; j < data - i - 1; j++) {
            if (strcmp(plans[j].name, plans[j + 1].name) > 0) {
                swapPlans(&plans[j], &plans[j + 1]);
            }
        }
    }
}

void searchPlans(Plan plans[], int data) {
    int a = 1, choice;
    while (a){
        printf("\n============= Search Workout Plans ===============\n");
        printf("1. Search by Name\n");
        printf("2. Search by Difficulty\n");
        printf("3. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                sortPlansByName(plans, data);
                searchByName(plans, data);
                break;
            
            case 2:
                searchByDiff(plans, data);
                break;

            case 3:
                printf("Exiting Sorting Plans...\n");
                return;

            default:
                printf("Invalid input, please try again...\n\n");
        }
    }
}

void addPlans(Plan plans[], int *data) {
    int i = (*data)++;
    int choice;
    printf("Workout Plan Name: ");
    scanf(" %[^\n]", plans[i].name);
    printf("Difficulty:\n");
    scanf(" %d", &choice);
    switch (choice){
        case 1: {
            strcpy(plans[i].difficulty, "Beginner");
            break;
        }
        
        case 2: {
            strcpy(plans[i].difficulty, "Intermediate");
            break;
        }

        case 3: {
            strcpy(plans[i].difficulty, "Advanced");
            break;
        }

        default:
            printf("Invalid input, please try again...\n\n");
    }
    printf("Popularity: ");
    scanf(" %d", &plans[i].popularity);
    printf("Duration (in weeks): ");
    scanf(" %d", &plans[i].duration);

    FILE *fp = fopen("workoutPlans.txt", "a");
    fprintf(fp, "%s#%d#%d#%s\n", plans[i].name, plans[i].popularity, plans[i].duration, plans[i].difficulty);
    fclose(fp);

    printf("Succesfully added a New Workout Plan!\n\n");
}

void deletePlan(Plan plans[], int *data) {
    printf("Current Plans:\n");
    for (int i = 0; i < *data; i++) {
        printf("%d. %s\n", i + 1, plans[i].name);
    }

    int deleteIndex;
    do{
        printf("Enter the number of the plan to delete: ");
        scanf("%d", &deleteIndex);
        deleteIndex--;
    }while (deleteIndex < 0 || deleteIndex >= *data);

    for (int i = deleteIndex; i < *data - 1; i++) {
        plans[i] = plans[i + 1];
    }

    (*data)--;

    FILE *fp = fopen("workoutPlans.txt", "w");
    for (int i = 0; i < *data; i++){
        fprintf(fp, "%s#%d#%d#%s\n", plans[i].name, plans[i].popularity, plans[i].duration, plans[i].difficulty);
    }
    fclose(fp);

    printf("Plan deleted successfully!\n\n");
}

void WorkoutPlans(int mod){
    Plan plans[100];
    int PlansCount = 0;
    FILE *fp = fopen("workoutPlans.txt", "r");

    if (fp != NULL) {
        while (fscanf(fp, "%[^#]#%d#%d#%[^\n]\n", plans[PlansCount].name, &plans[PlansCount].popularity, &plans[PlansCount].duration, plans[PlansCount].difficulty) != EOF) {
            PlansCount++;
        }
        fclose(fp);
    }

    int a = 1, choice;
    while (a){
        printf("\n============= Workout Plans ===============\n");
        printf("1. View Plans\n");
        printf("2. Sort Plans\n");
        printf("3. Search for Plans\n");
        printf("4. Exit\n");
        if (mod == 1){
            printf("5. Add New Plans\n");
            printf("6. Delete Existing Plans\n");
        }
        printf("Choice: ");
        scanf(" %d", &choice);
        if (mod == 0){
            switch (choice) {
                case 1:
                    displayPlans(plans, PlansCount);
                    break;
                
                case 2:
                    sortPlans(plans, PlansCount);
                    break;

                case 3:
                    searchPlans(plans, PlansCount);
                    break;

                case 4:
                    printf("Exiting Facility...\n");
                    return;

                default:
                    printf("Invalid input, please try again...\n\n");
            }
        } else {
            switch (choice) {
                case 1:
                    displayPlans(plans, PlansCount);
                    break;
                
                case 2:
                    sortPlans(plans, PlansCount);
                    break;

                case 3:
                    searchPlans(plans, PlansCount);
                    break;

                case 4:
                    printf("Exiting Facility...\n");
                    return;

                case 5:
                    addPlans(plans, &PlansCount);
                    break;

                case 6:
                    deletePlan(plans, &PlansCount);
                    break;

                default:
                    printf("Invalid input, please try again...\n\n");
            }
        }
    }
}

int main() {
    Account accounts[100];
    Admins admin[100];
    int z = 1, choice, isLoggedIn = 0, jumlahData = 0, jumlahAdmin = 0, Mod = 0;
    char user[50], pass[17];

    FILE *file = fopen("acc.txt", "r");
    if (!file) {
        printf("File not found!\n");
        return 1;
    }

    while(fscanf(file, "%[^#]#%[^#]#%d#%d#%d#%[^#]#%[^#]#%[^\n]\n", accounts[jumlahData].username, accounts[jumlahData].password, &accounts[jumlahData].day, &accounts[jumlahData].month, &accounts[jumlahData].year, accounts[jumlahData].phoneNum, accounts[jumlahData].email, accounts[jumlahData].city) != EOF) {
        jumlahData++;
    }
    fclose(file);

    FILE *mods = fopen("adm.txt", "r");
    if (!mods) {
        printf("File not found!\n");
        return 1;
    }

    while(fscanf(mods, "%[^#]#%[^\n]\n", admin[jumlahAdmin].username, admin[jumlahAdmin].password) != EOF) {
        jumlahAdmin++;
    }
    fclose(mods);

    while(z){
        if (isLoggedIn == 0){
            printf("=======================================================================\n");
            printf("  ______ _ _                         ______          __     __         \n");
            printf(" |  ____(_) |                       |  ____|         \\ \\   / /         \n");
            printf(" | |__   _| |_ _ __   ___  ___ ___  | |__ ___  _ __   \\ \\_/ /__  _   _ \n");
            printf(" |  __| | | __| '_ \\ / _ \\/ __/ __| |  __/ _ \\| '__|   \\   / _ \\| | | |\n");
            printf(" | |    | | |_| | | |  __/\\__ \\__ \\ | | | (_) | |       | | (_) | |_| |\n");
            printf(" |_|    |_|\\__|_| |_|\\___||___/___/ |_|  \\___/|_|       |_|\\___/ \\__,_|\n");
            printf("=======================================================================\n");
            printf("You have not logged in yet, please login or Sign-Up!\n");
            printf("1. Login\n");
            printf("2. Sign-Up\n");
            printf("3. Exit\n");
            printf("Choose: ");
            scanf(" %d", &choice);
            switch(choice){
                case 1:
                    login(accounts, &isLoggedIn, jumlahData, &user, admin, &Mod);
                    break;

                case 2:
                    signUp(&accounts, &jumlahData);
                    break;

                case 3:
                    printf("Program Exiting...");
                    z = 0;
                    break;

                default:
                    printf("Invalid input, please try again...\n\n");
            }
        } else {
            if (Mod == 0){
                int pick;
                printf("=======================================================================\n");
                printf("  ______ _ _                         ______          __     __         \n");
                printf(" |  ____(_) |                       |  ____|         \\ \\   / /         \n");
                printf(" | |__   _| |_ _ __   ___  ___ ___  | |__ ___  _ __   \\ \\_/ /__  _   _ \n");
                printf(" |  __| | | __| '_ \\ / _ \\/ __/ __| |  __/ _ \\| '__|   \\   / _ \\| | | |\n");
                printf(" | |    | | |_| | | |  __/\\__ \\__ \\ | | | (_) | |       | | (_) | |_| |\n");
                printf(" |_|    |_|\\__|_| |_|\\___||___/___/ |_|  \\___/|_|       |_|\\___/ \\__,_|\n");
                printf("=======================================================================\n");
                printf("Welcome, %s.\n", user);
                printf("1. Account Setting\n");
                printf("2. Facility\n");
                printf("3. Class Schedule\n");
                printf("4. Workout Plans\n");
                printf("5. Exit\n");
                printf("Choice: ");
                scanf(" %d", &pick);

                switch (pick){
                    case 1:
                        AccountSet(&isLoggedIn, &accounts, jumlahData, user);
                        break;

                    case 2:
                        facilities(Mod);
                        break;

                    case 3:
                        classes(Mod);
                        break;

                    case 4:
                        WorkoutPlans(Mod);
                        break;

                    case 5:
                        printf("Program Exiting...");
                        z = 0;
                        break;

                    default:
                        printf("Invalid input, please try again...\n\n");
                }
            } else {
                int pick;
                printf("=======================================================================\n");
                printf("  ______ _ _                         ______          __     __         \n");
                printf(" |  ____(_) |                       |  ____|         \\ \\   / /         \n");
                printf(" | |__   _| |_ _ __   ___  ___ ___  | |__ ___  _ __   \\ \\_/ /__  _   _ \n");
                printf(" |  __| | | __| '_ \\ / _ \\/ __/ __| |  __/ _ \\| '__|   \\   / _ \\| | | |\n");
                printf(" | |    | | |_| | | |  __/\\__ \\__ \\ | | | (_) | |       | | (_) | |_| |\n");
                printf(" |_|    |_|\\__|_| |_|\\___||___/___/ |_|  \\___/|_|       |_|\\___/ \\__,_|\n");
                printf("=======================================================================\n");
                printf("Welcome to Admin Panel, %s.\n", user);
                printf("1. LogOut\n");
                printf("2. Edit Facility\n");
                printf("3. Edit Classes and Schedules\n");
                printf("4. Edit Workout Plans\n");
                printf("5. Exit\n");
                printf("Choice: ");
                scanf(" %d", &pick);

                switch (pick){
                    case 1:
                        isLoggedIn = 0;
                        Mod = 0;
                        break;

                    case 2:
                        facilities(Mod);
                        break;

                    case 3:
                        classes(Mod);
                        break;
                    
                    case 4:
                        WorkoutPlans(Mod);
                        break;

                    case 5:
                        printf("Program Exiting...");
                        z = 0;
                        break;

                    default:
                        printf("Invalid input, please try again...\n\n");
                }
            }
        }
    }

    return 0;
}
