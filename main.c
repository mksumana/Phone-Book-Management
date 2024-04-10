#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 10
#define FILENAME "phonebook.csv"


// Original Code Section (KAM, SUM, KARI, MADH)
struct PhoneBookEntry {
    char name[50];
    char phone_number[15];
    int blocked; 
    struct PhoneBookEntry* next;
};

struct PhoneBook {
    struct PhoneBookEntry* table[TABLE_SIZE];
};

int hash(char* name) {
    int sum = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        sum += name[i];
    }
    return sum % TABLE_SIZE;
}

int isNameDuplicate(struct PhoneBook* phoneBook, char* name) {
    int key = hash(name);
    struct PhoneBookEntry* current = phoneBook->table[key];

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return 1; 
        }
        current = current->next;
    }

    return 0; 
}

int isPhoneNumberDuplicate(struct PhoneBook* phoneBook, char* phone_number) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        struct PhoneBookEntry* current = phoneBook->table[i];
        while (current != NULL) {
            if (strcmp(current->phone_number, phone_number) == 0) {
                return 1; 
            }
            current = current->next;
        }
    }
    return 0; 
}
// File I/O Operations Code Section (Save and Load)
void saveToCSV(struct PhoneBook* phoneBook) {
    FILE* file = fopen(FILENAME, "w");
    if (!file) {
        perror("Error opening file for writing");
        exit(1);
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        struct PhoneBookEntry* current = phoneBook->table[i];
        while (current != NULL) {
            fprintf(file, "%s,%s,%d\n", current->name, current->phone_number, current->blocked);
            current = current->next;
        }
    }

    fclose(file);
}

void addEntry(struct PhoneBook* phoneBook, char* name, char* phone_number) {
    int key = hash(name);

    if (isNameDuplicate(phoneBook, name)) {
        printf("Name already exists. Please choose a different name.\n");
        return;
    }

    if (isPhoneNumberDuplicate(phoneBook, phone_number)) {
        printf("Phone number already exists. Please choose a different phone number.\n");
        return;
    }

    struct PhoneBookEntry* newEntry = (struct PhoneBookEntry*)malloc(sizeof(struct PhoneBookEntry));
    if (!newEntry) {
        perror("Memory allocation failed");
        exit(1);
    }
    strcpy(newEntry->name, name);
    strcpy(newEntry->phone_number, phone_number);
    newEntry->blocked = 0; 
    newEntry->next = NULL;

    if (phoneBook->table[key] == NULL) {
        phoneBook->table[key] = newEntry;
    } else {
        struct PhoneBookEntry* current = phoneBook->table[key];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newEntry;
    }

    // Save to CSV after adding
    saveToCSV(phoneBook);
}

char* searchEntry(struct PhoneBook* phoneBook, char* name) {
    int key = hash(name);
    struct PhoneBookEntry* current = phoneBook->table[key];

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (current->blocked) {
                return "Entry is blocked";
            } else {
                return current->phone_number;
            }
        }
        current = current->next;
    }

    return "Entry not found";
}

char* searchByPhoneNumber(struct PhoneBook* phoneBook, char* phone_number) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        struct PhoneBookEntry* current = phoneBook->table[i];
        while (current != NULL) {
            if (strcmp(current->phone_number, phone_number) == 0) {
                if (current->blocked) {
                    return "Entry is blocked";
                } else {
                    return current->name;
                }
            }
            current = current->next;
        }
    }
    return "Entry not found";
}

void deleteEntry(struct PhoneBook* phoneBook, char* name) {
    int key = hash(name);
    struct PhoneBookEntry* current = phoneBook->table[key];
    struct PhoneBookEntry* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                phoneBook->table[key] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            // Save to CSV after deleting
            saveToCSV(phoneBook);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void displayPhoneBook(struct PhoneBook* phoneBook) {
    printf("Unblocked contacts:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        struct PhoneBookEntry* current = phoneBook->table[i];
        while (current != NULL) {
            if (!current->blocked) {
                printf("Name: %s, Phone: +91 %s\n", current->name, current->phone_number);
            }
            current = current->next;
        }
    }
    printf("\n");
    printf("Blocked contacts:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        struct PhoneBookEntry* current = phoneBook->table[i];
        while (current != NULL) {
            if (current->blocked) {
                printf("Name: %s, Phone: +91 %s\n", current->name, current->phone_number);
            }
            current = current->next;
        }
    }
}

void editPhoneNumber(struct PhoneBook* phoneBook, char* name, char* newPhoneNumber) {
    int key = hash(name);
    
    struct PhoneBookEntry* current = phoneBook->table[key];
    if (isPhoneNumberDuplicate(phoneBook, newPhoneNumber)) {
        printf("Phone number already exists. Please choose a different phone number.\n");
        return;
    }

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            strcpy(current->phone_number, newPhoneNumber);
            // Save to CSV after editing
            saveToCSV(phoneBook);
            return;
        }
        current = current->next;
    }
}

int isValidPhoneNumber(char* phone_number) {
    int len = strlen(phone_number);
    if (len == 10) {
        if (phone_number[0] >= '6' && phone_number[0] <= '9') {
            for (int i = 0; i < 10; i++) {
                if (!isdigit(phone_number[i])) {
                    return 0;
                }
            }
            return 1;
        }
    }
    return 0; 
}

void blockPerson(struct PhoneBook* phoneBook, char* name, int block) {
    int key = hash(name);
    struct PhoneBookEntry* current = phoneBook->table[key];

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            current->blocked = block;
            // Save to CSV after blocking/unblocking
            saveToCSV(phoneBook);
            return;
        }
        current = current->next;
    }
}


void loadFromCSV(struct PhoneBook* phoneBook) {
    FILE* file = fopen(FILENAME, "r");
    if (!file) {
        perror("Error opening file for reading");
        return;
    }

    char name[50];
    char phone_number[15];
    int blocked;

    while (fscanf(file, "%49[^,],%14[^,],%d\n", name, phone_number, &blocked) == 3) {
        addEntry(phoneBook, name, phone_number);
        struct PhoneBookEntry* entry = phoneBook->table[hash(name)];
        while (entry != NULL && strcmp(entry->name, name) != 0) {
            entry = entry->next;
        }
        if (entry != NULL) {
            entry->blocked = blocked;
        }
    }

    fclose(file);
}

// Main Function
int main() {
    struct PhoneBook phoneBook;
    for (int i = 0; i < TABLE_SIZE; i++) {
        phoneBook.table[i] = NULL;
    }

    loadFromCSV(&phoneBook);  // Load from CSV at the beginning

    int choice;
    char name[50];
    char phone_number[15];

    while (1) {
        printf("\nPhone Book Menu:\n");
        printf("1. Add an entry\n");
        printf("2. Search for an entry by name\n");
        printf("3. Search for an entry by phone number\n");
        printf("4. Delete an entry\n");
        printf("5. Display phone book\n");
        printf("6. Edit phone number\n");
        printf("7. Block an entry\n");
        printf("8. Unblock an entry\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter name: ");
                scanf("%s", name);
                do {
                    printf("Enter phone number (10 digits, starting with 6-9): +91 ");
                    scanf("%s", phone_number);
                } while (!isValidPhoneNumber(phone_number));
                addEntry(&phoneBook, name, phone_number);
                break;
            case 2:
                printf("Enter name to search: ");
                scanf("%s", name);
                printf("%s\n", searchEntry(&phoneBook, name));
                break;
            case 3:
                printf("Enter phone number to search: ");
                scanf("%s", phone_number);
                printf("%s\n", searchByPhoneNumber(&phoneBook, phone_number));
                break;
            case 4:
                printf("Enter name to delete: ");
                scanf("%s", name);
                deleteEntry(&phoneBook, name);
                break;
            case 5:
                printf("Phone Book:\n");
                displayPhoneBook(&phoneBook);
                break;
            case 6:
                printf("Enter name to edit phone number: ");
                scanf("%s", name);
                if(!isNameDuplicate(&phoneBook, name)) {
                    printf("Name does not exist.\n");
                    break;
                }
                printf("Enter new phone number: ");
                scanf("%s", phone_number);
                editPhoneNumber(&phoneBook, name, phone_number);
                break;
            case 7:
                printf("Enter name to block: ");
                scanf("%s", name);
                blockPerson(&phoneBook, name, 1); 
                break;
            case 8:
                printf("Enter name to unblock: ");
                scanf("%s", name);
                blockPerson(&phoneBook, name, 0); 
                break;
            case 9:
                printf("Exiting program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    }

    return 0;
}
