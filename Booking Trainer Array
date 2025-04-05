#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char username[50];
    char trainer[50];
    char specialty[50];
    char date[6];
    int hour;
    int duration;
} Booking;

typedef struct {
    char name[50];
    char specialty[50];
} Trainer;

void trainerMenu(const char* username);
void showBookings(const char* username, Booking bookings[], int count);
void addBooking(const char* username, Booking bookings[], int* count);
void cancelBooking(const char* username, Booking bookings[], int* count);
void cleanOldBookings(const char* username, Booking bookings[], int* count);
int loadBookings(Booking bookings[]);
void saveBookings(Booking bookings[], int count);
int dateToValue(const char* date);
int hasConflict(Booking bookings[], int count, const char* trainer, const char* date, int hour, int duration);

void showBookings(const char* username, Booking bookings[], int count) {
    printf("\n=== Your Bookings ===\n");
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(bookings[i].username, username) == 0) {
            printf("- Trainer: %s\n", bookings[i].trainer);
            printf("  Date: %s, Time: %02d:00-%02d:00\n", 
                  bookings[i].date, bookings[i].hour, bookings[i].hour + bookings[i].duration);
            found = 1;
        }
    }
    
    if (!found) printf("No bookings found\n");
}

void cancelBooking(const char* username, Booking bookings[], int* count) {
    char date[6];
    int hour;

    printf("Enter booking date to cancel (dd/mm): ");
    fgets(date, sizeof(date), stdin);
    getchar();
    
    printf("Enter start hour: ");
    scanf("%d", &hour);
    getchar();

    int found = -1;
    for (int i = 0; i < *count; i++) {
        if (strcmp(bookings[i].username, username) == 0 &&
            strcmp(bookings[i].date, date) == 0 &&
            bookings[i].hour == hour) {
            found = i;
            break;
        }
    }

    if (found != -1) {
        for (int i = found; i < *count - 1; i++) {
            bookings[i] = bookings[i + 1];
        }
        (*count)--;
        saveBookings(bookings, *count);
        printf("Booking cancelled!\n");
    } else {
        printf("Booking not found\n");
    }
}

void cleanOldBookings(const char* username, Booking bookings[], int* count) {
    char today[6];
    printf("Enter today's date (dd/mm): ");
    fgets(today, sizeof(today), stdin);
    getchar();

    int todayValue = dateToValue(today);
    int cleaned = 0;
    int i = 0;

    while (i < *count) {
        int bookingDate = dateToValue(bookings[i].date);
        
        if (strcmp(bookings[i].username, username) == 0 && 
            bookingDate < todayValue) {

            for (int j = i; j < *count - 1; j++) {
                bookings[j] = bookings[j + 1];
            }
            (*count)--;
            cleaned++;
        } else {
            i++;
        }
    }

    if (cleaned > 0) {
        saveBookings(bookings, *count);
        printf("Cleaned %d old bookings\n", cleaned);
    } else {
        printf("No old bookings found\n");
    }
}

void trainerMenu(const char* username) {
    Booking bookings[100];
    int bookingCount = loadBookings(bookings);

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
            case 1: showBookings(username, bookings, bookingCount); break;
            case 2: addBooking(username, bookings, &bookingCount); break;
            case 3: cancelBooking(username, bookings, &bookingCount); break;
            case 4: cleanOldBookings(username, bookings, &bookingCount); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice\n");
        }
    } while(choice != 5);
}

void addBooking(const char* username, Booking bookings[], int* count) {
    const Trainer trainers[5] = {
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
    
    printf("Date (dd/mm): ");
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

    if(*count >= 100) {
        printf("Booking system full!\n");
        return;
    }

    if(hasConflict(bookings, *count, trainers[choice-1].name, date, hour, duration)) {
        printf("Time conflict!\n");
        return;
    }

    strcpy(bookings[*count].username, username);
    strcpy(bookings[*count].trainer, trainers[choice-1].name);
    strcpy(bookings[*count].specialty, trainers[choice-1].specialty);
    strcpy(bookings[*count].date, date);
    bookings[*count].hour = hour;
    bookings[*count].duration = duration;
    (*count)++;
    
    saveBookings(bookings, *count);
    printf("Booking added!\n");
}

int loadBookings(Booking bookings[]) {
    FILE* file = fopen("trainer-bookings.txt", "r");
    if (!file) return 0;

    int count = 0;
    char line[128];

    while (fgets(line, sizeof(line), file) && count < 100) {

        if (strlen(line) < 3) continue;

        char* copy = strdup(line);
        char* token;
        int field = 0;

        token = strtok(copy, "#\n");
        while (token != NULL && field < 6) {
            switch(field) {
                case 0: strncpy(bookings[count].username, token, 49); break;
                case 1: strncpy(bookings[count].trainer, token, 49); break;
                case 2: strncpy(bookings[count].specialty, token, 49); break;
                case 3: strncpy(bookings[count].date, token, 5); break;
                case 4: bookings[count].hour = atoi(token); break;
                case 5: bookings[count].duration = atoi(token); break;
            }
            token = strtok(NULL, "#\n");
            field++;
        }

        free(copy);

        if (field == 6) {
            count++;
        } else {
            printf("Invalid booking data: %s", line);
        }
    }

    fclose(file);
    return count;
}

void saveBookings(Booking bookings[], int count) {
    FILE* file = fopen("trainer-bookings.txt", "w");
    for(int i = 0; i < count; i++) {
        fprintf(file, "%s#%s#%s#%s#%d#%d\n",
               bookings[i].username, bookings[i].trainer, bookings[i].specialty,
               bookings[i].date, bookings[i].hour, bookings[i].duration);
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

int hasConflict(Booking bookings[], int count, const char* trainer, const char* date, int hour, int duration) {
    int newEnd = hour + duration;
    for(int i = 0; i < count; i++) {
        if(strcmp(bookings[i].trainer, trainer) == 0 && 
           strcmp(bookings[i].date, date) == 0) {
            int currEnd = bookings[i].hour + bookings[i].duration;
            if(!(newEnd <= bookings[i].hour || hour >= currEnd)) return 1;
        }
    }
    return 0;
}

int main() {
    char username[50];
    printf("Enter username: ");
    fgets(username, 50, stdin);
    username[strcspn(username, "\n")] = '\0';

    trainerMenu(username);
    return 0;
}
