#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Booking {
    char username[50];
    char trainer[50];
    char specialty[50];
    char date[6];
    int hour;
    int duration;
    struct Booking* next;
} Booking;

typedef struct {
    char name[50];
    char specialty[50];
} Trainer;

// these are prototye functions.. has to be at the very top! basically to give me the flexibility of placing my functions wherever i want. (EXCLUDING INT MAIN)
void trainerMenu(const char* username);
void showBookings(const char* username);
void addBooking(const char* username);
void cancelBooking(const char* username);
void cleanOldBookings(const char* username);
Booking* loadBookings();
void saveBookings(Booking* head);
void freeBookings(Booking* head);
int dateToValue(const char* date);
int hasConflict(Booking* head, const char* trainer, const char* date, int hour, int duration);


void showBookings(const char* username) {
    Booking* head = loadBookings();                     // to load all bookings into a linked list
    printf("\n=== Your Bookings ===\n");                // you know..
    
    int found = 0;
    while (head != NULL) {                             // Looping till no bookings left in the linked list
        if (strcmp(head->username, username) == 0) {   // comparing username and to retrive the data that matches the username
            printf("- Trainer: %s\n", head->trainer);   // for printing trainer name
            printf("  Date: %s, Time: %02d:00-%02d:00\n", head->date, head->hour, head->hour + head->duration); // to display time booking
            found = 1;                                  // basically our counter to see how much data retrieved
        }
        head = head->next;                             // move to next data booking
    }
    
    if (!found) printf("No bookings found\n");         // Display when theres no data retrieved
    freeBookings(head);                                // this gone, then minus 100 points by sir Dennis
}

//imma use chatgpt to explain it now. cuz its 3 AM now and i want sleep hehe

void cancelBooking(const char* username) {
    Booking* head = loadBookings();         // load all bookings from storage
    Booking* current = head;                // traversal pointer
    Booking* prev = NULL;                   // previous node tracker
    char date[6];
    int hour;

    printf("enter booking date to cancel (dd/mm): ");
    fgets(date, sizeof(date), stdin);      // read date input
    getchar();                             // consume newline
    
    printf("enter start hour: ");
    scanf("%d", &hour);                    // read hour input
    getchar();                             // consume newline

    int cancelled = 0;                     // cancellation status flag
    while (current != NULL) {              // traverse booking list
        // check if booking matches criteria
        if (strcmp(current->username, username) == 0 &&
            strcmp(current->date, date) == 0 &&
            current->hour == hour) {
            
            if (prev == NULL) {            // case: deleting head node
                head = current->next;
            } else {                       // case: deleting middle node
                prev->next = current->next;
            }
            
            free(current);                // free deleted node memory
            cancelled = 1;                 // update status
            break;                        // exit search loop
        }
        prev = current;                   // update previous pointer
        current = current->next;          // move to next node
    }

    if (cancelled) {
        saveBookings(head);               // persist changes to storage
        printf("booking cancelled\n");
    } else {
        printf("booking not found\n");
    }
    freeBookings(head);                   // clean up remaining memory
}

void cleanOldBookings(const char* username) {
    char today[6];
    printf("Enter today's date, with space inbetween day & month (dd/mm): ");
    fgets(today, sizeof(today), stdin);
    getchar();

    Booking* head = loadBookings();
    Booking* current = head;
    Booking* prev = NULL;
    int todayValue = dateToValue(today);
    int cleaned = 0;

    while (current != NULL) {
        int bookingDate = dateToValue(current->date);
        
        if (strcmp(current->username, username) == 0 && 
            bookingDate < todayValue) {
            
            if (prev == NULL) {
                head = current->next;
            } else {
                prev->next = current->next;
            }
            
            Booking* toDelete = current;
            current = current->next;
            free(toDelete);
            cleaned++;
        } else {
            prev = current;
            current = current->next;
        }
    }

    if (cleaned > 0) {
        saveBookings(head);
        printf("Cleaned %d old bookings\n", cleaned);
    } else {
        printf("No old bookings found\n");
    }
    freeBookings(head);
}

void trainerMenu(const char* username) {
    char today[6];
    printf("Enter today's date, with space inbetween day & month (dd/mm): ");
    fgets(today, sizeof(today), stdin);
    getchar();

    int choice;
    do {
        printf("\n-----=== Trainer Booking ===-----");
        printf("\n1. View Bookings");
        printf("\n2. Add Booking");
        printf("\n3. Cancel");
        printf("\n4. Clean Old");
        printf("\n5. Exit");
        printf("\nChoice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1: showBookings(username); break;
            case 2: addBooking(username); break;
            case 3: cancelBooking(username); break;
            case 4: cleanOldBookings(username); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice\n");
        }
    } while(choice != 5);
}

void addBooking(const char* username) {
    const Trainer trainers[] = {
        {"Andi Santoso", "Legs"},
        {"Budi Prasetyo", "Abs"},
        {"Cahyo Ramadhan", "Arms"},
        {"Dedi Gunawan", "Back"},
        {"Eko Setiawan", "Neck"}
    };

    printf("\n------Our Gym Trainers------\n");
    for(int i = 0; i < 5; i++) {
        printf("%d. %s (%s)\n", i+1, trainers[i].name, trainers[i].specialty);
    }

    int choice;
    printf("Select trainer (1-5): ");
    scanf("%d", &choice);
    getchar();

    if(choice < 1 || choice > 5) {
        printf("Invalid selection\n");
        return;
    }

    char date[6];
    int hour, duration;
    
    printf("Date, with space in-between day & month (dd/mm): ");
    fgets(date, sizeof(date), stdin);
    getchar();
    
    printf("Start hour (9-16): ");
    scanf("%d", &hour);
    printf("Duration (1-8h): ");
    scanf("%d", &duration);
    getchar();

    if(hour < 9 || hour > 16 || duration < 1 || duration > 8) {
        printf("Invalid time\n");
        return;
    }

    Booking* head = loadBookings();
    if(hasConflict(head, trainers[choice-1].name, date, hour, duration)) {
        printf("Time conflict!\n");
        freeBookings(head);
        return;
    }

    Booking* new = malloc(sizeof(Booking));
    strcpy(new->username, username);
    strcpy(new->trainer, trainers[choice-1].name);
    strcpy(new->specialty, trainers[choice-1].specialty);
    strcpy(new->date, date);
    new->hour = hour;
    new->duration = duration;
    new->next = head;
    
    saveBookings(new);
    freeBookings(head);
    printf("Booking added!\n");
}

Booking* loadBookings() {
    FILE* file = fopen("trainer-bookings.txt", "r");
    if (!file) return NULL;

    Booking* head = NULL;
    char line[128];

    while (fgets(line, sizeof(line), file)) {

        if (strlen(line) < 3) continue;

        Booking* b = malloc(sizeof(Booking));
        char* copy = strdup(line); 
        char* token;
        int field = 0;

        token = strtok(copy, "#\n");
        while (token != NULL && field < 6) {
            switch(field) {
                case 0: strncpy(b->username, token, 49); break;
                case 1: strncpy(b->trainer, token, 49); break;
                case 2: strncpy(b->specialty, token, 49); break;
                case 3: strncpy(b->date, token, 5); break;
                case 4: b->hour = atoi(token); break;
                case 5: b->duration = atoi(token); break;
            }
            token = strtok(NULL, "#\n");
            field++;
        }

        free(copy);


        if (field == 6) {
            b->next = head;
            head = b;
        } else {
            free(b);
            printf("Invalid booking data: %s", line);
        }
    }

    fclose(file);
    return head;
}

void saveBookings(Booking* head) {
    FILE* file = fopen("trainer-bookings.txt", "w");
    while(head) {
        fprintf(file, "%s#%s#%s#%s#%d#%d\n",
               head->username, head->trainer, head->specialty,
               head->date, head->hour, head->duration);
        head = head->next;
    }
    fclose(file);
}

int dateToValue(const char* date) {
    char day_str[3] = {0};
    char month_str[3] = {0};

    strncpy(day_str, date, 2);
    strncpy(month_str, date + 3, 2);
    
    return atoi(month_str) * 100 + atoi(day_str);
}

int hasConflict(Booking* head, const char* trainer, const char* date, int hour, int duration) {
    int newEnd = hour + duration;
    while(head) {
        if(strcmp(head->trainer, trainer) == 0 && 
           strcmp(head->date, date) == 0) {
            int currEnd = head->hour + head->duration;
            if(!(newEnd <= head->hour || hour >= currEnd)) return 1;
        }
        head = head->next;
    }
    return 0;
}

void freeBookings(Booking* head) {
    while(head) {
        Booking* temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    char username[50];
    printf("Enter username: ");
    fgets(username, 50, stdin);
    username[strcspn(username, "\n")] = '\0';

    trainerMenu(username);
    return 0;
}
