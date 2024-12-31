#include "contact.h"
#include "file.h"
//#include "populate.h"

/*void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    populateAddressBook(addressBook);
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}*/
Contact contacts[MAX_CONTACTS];
int contact_count=0;

int validate_name(char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        // Check if the character is not an alphabet or space
        if (!isalpha(name[i]) && !isspace(name[i])) {
            return 0;// Return 0 if invalid character is found
        }
    }
    return 1;
}

int validate_phone(char *phone) {
    int len = strlen(phone);
    if (len != 10) return 0;
    // Verify that all characters in the string are digits
    for (int i = 0; i < len; i++) {
        if (!isdigit(phone[i])) {
            return 0;
        }
    }
     // Check if the phone number already exists in the contacts list
    for (int i = 0; i < contact_count; i++) {
        if (strcmp(contacts[i].phone, phone) == 0) {
            return 0;  
        }
    }
    
    return 1;
}

int validate_email(char *email) {
    int at_count = 0, dot_count = 0;
    const char *at = NULL;
    const char *dot = NULL;
    for (const char *p = email; *p; ++p) {
         if (*p == '@') { // Check for '@' character
            at_count++; // Increment '@' counter
            if (at_count == 1) at = p; // Store pointer to first '@'
        } else if (*p == '.') { // Check for '.' character
            dot_count++; // Increment '.' counter
            if (dot_count == 1) dot = p; // Store pointer to first '.'
        }
    }

    return (at_count == 1 && dot_count == 1 &&
            at && dot && at < dot &&
            at != email && dot != at + 1 &&
            *(dot + 1) != '\0'); 
}

//CREATE FUNCTION
void create_contact() {
    if (contact_count >= MAX_CONTACTS) {
        printf("Address book is full!\n");
        return;
    }

    Contact new_contact;

    // Name input and validation
    do {
        printf(BOLD"Enter name: "RESET);
        scanf(" %[^\n]", new_contact.name);
        if (!validate_name(new_contact.name)) {
            printf(RED"Invalid name! Use only alphabets.\n"RESET);
        }
    } while (!validate_name(new_contact.name));

    // Phone input and validation
    do {
    printf(BOLD"Enter phone number: "RESET);
    scanf(" %[^\n]", new_contact.phone);
    if (!validate_phone(new_contact.phone)) {
        if (strlen(new_contact.phone) != 10) {
            printf(RED"Invalid phone number! Must be 10 digits.\n"RESET);
        } else {
            printf(RED"Contact with this phone number already exists! Please try again.\n"RESET);
        }
    }
} while (!validate_phone(new_contact.phone));
    // Email input and validation
    do {
        printf(BOLD"Enter email: "RESET);
        scanf(" %[^\n]", new_contact.email);
        if (!validate_email(new_contact.email)) {
            printf(RED"Invalid email format!\n"RESET);
        }
    } while (!validate_email(new_contact.email));

    contacts[contact_count++] = new_contact;
    printf(GREEN"Contact added successfully!\n"RESET);
}

//SEARCH FUNCTION

void search_contact() {
    char search_term[MAX_NAME_LENGTH], phone_filter[15];
    int search_choice, found_contacts[MAX_CONTACTS] = {0}, found_count = 0;

    printf(BOLD"\nSearch by:\n1. Name (Partial Match)\n2. Phone Number\n3. Email\nEnter your choice: ");
    scanf("%d", &search_choice);
    getchar();
    printf("Enter search term: "RESET);
    scanf(" %[^\n]", search_term);

    for (int i = 0; i < contact_count; i++) {
        int match = 0;
        if (search_choice == 1 && strcasecmp(contacts[i].name, search_term)==0) match = 1;
        else if (search_choice == 2 && strstr(contacts[i].phone, search_term)) match = 1;
        else if (search_choice == 3 && strcasecmp(contacts[i].email, search_term) == 0) match = 1;
         // If a match is found, save the index
        if (match) found_contacts[found_count++] = i;
    }

    if (found_count == 0) {
        printf(RED"No contacts found.\n"RESET);
    }else{
    printf(BOLD"\nContacts Found:\n" RESET ); 
    printf("--------------------------------------\n");
    for (int i = 0; i < found_count; i++) {
        int idx = found_contacts[0];
        printf(BOLD"%d. Name: %s, Phone: %s, Email: %s\n"RESET, i + 1, 
               contacts[found_contacts[i]].name, 
               contacts[found_contacts[i]].phone, 
               contacts[found_contacts[i]].email );
    }
    // If multiple contacts are found, offer filtering by phone number
    if (found_count > 1) {
        printf(BOLD"Enter phone number to filter results: "RESET);
        scanf(" %[^\n]", phone_filter);
        found_count = 0;
        // Filter results based on phone number
        for (int i = 0; i < contact_count; i++) {
            if (strstr(contacts[i].phone, phone_filter)) {
                found_contacts[found_count++] = i;
            }
        }
        // If no matches after filtering, notify user
        if (found_count == 0) {
            printf(RED"No contacts found matching the phone number filter.\n"RESET);
        } else {
             // Display filtered contacts
            printf(BOLD"\nFiltered Contact(s):\n");
            for (int i = 0; i < found_count; i++) {
                int idx = found_contacts[i];
                printf("%d\n", i + 1);
                printf(" Name:%s\n Phone:%s\n Email:%s\n"RESET, contacts[idx].name, contacts[idx].phone, contacts[idx].email );
            }
        }
    }
}
}

//EDIT CONTACT

void edit_contact() {
    char search_name[MAX_NAME_LENGTH];
    int found_contacts[MAX_CONTACTS] = {0}, found_count = 0;
    
    printf(BOLD"Enter name of contact to edit: "RESET);
    scanf(" %[^\n]", search_name);
    
    for (int i = 0; i < contact_count; i++) {
        if (strstr(contacts[i].name, search_name)) {
            found_contacts[found_count++] = i;
        }
    }
    
    if (found_count == 0) {
        printf(RED"No contacts found!\n"RESET);
        return;
    }
    
    int idx;
    if (found_count == 1) {
        idx = found_contacts[0];
    } else {
        printf(BOLD"\nMultiple contacts found:\n"RESET);
        for (int i = 0; i < found_count; i++) {
            printf(BOLD"%d. Name: %s, Phone: %s, Email: %s\n"RESET, 
                   i + 1, 
                   contacts[found_contacts[i]].name, 
                   contacts[found_contacts[i]].phone, 
                   contacts[found_contacts[i]].email );
        }
        
        printf(BOLD"Choose a contact to edit (1-%d): "RESET, found_count);
        int choice;
        scanf("%d", &choice);
        
        if (choice < 1 || choice > found_count) {
            printf(RED"Invalid choice!\n"RESET);
            return;
        }
        // Get the selected contact index
        idx = found_contacts[choice - 1];
    }
    
    printf(BOLD"\nSelect field to edit:\n");
    printf("1. Name\n2. Phone\n3. Email\nEnter your choice: "RESET);
    
    int field_choice;
    scanf("%d", &field_choice);
    
    switch (field_choice) {
        case 1: {
            char new_name[MAX_NAME_LENGTH];
            int valid_name = 0;
             // Loop until a valid name is entered
            while (!valid_name) {
                printf(BOLD"Enter new name: "RESET);
                scanf(" %[^\n]", new_name);
                
                if (validate_name(new_name)) {
                    strcpy(contacts[idx].name, new_name);
                    valid_name = 1;
                } else {
                    printf(RED"Invalid name. Use only alphabets and spaces.\n"RESET);
                }
            }
            break;
        }
        
        case 2: {
            char new_phone[MAX_NAME_LENGTH];
            int valid_phone = 0;
            // Loop until a valid phone number is entered
            while (!valid_phone) {
                printf(BOLD"Enter new phone number: "RESET);
                scanf(" %[^\n]", new_phone);
                
                if (validate_phone(new_phone)) {
                    strcpy(contacts[idx].phone, new_phone);
                    valid_phone = 1;
                } else {
                    printf(RED"Invalid phone number. Must be 10 digits and unique.\n"RESET);
                }
            }
            break;
        }
        
        case 3: {
            char new_email[MAX_EMAIL_LENGTH];
            int valid_email = 0;
            // Loop until a valid email is entered
            while (!valid_email) {
                printf(BOLD"Enter new email: "RESET);
                scanf(" %[^\n]", new_email);
                
                if (validate_email(new_email)) {
                    strcpy(contacts[idx].email, new_email);
                    valid_email = 1;
                } else {
                    printf(RED"Invalid email. Must contain '@' and '.' in correct order.\n"RESET);
                }
            }
            break;
        }
        
        default:
            printf(RED"Invalid choice!\n"RESET);
            return;
    }
    
    printf(GREEN"Contact updated successfully!\n"RESET);
}
//DELETE CONTACT

void delete_contact() {
    char search_value[MAX_NAME_LENGTH];
    int search_choice;
    int found_contacts[MAX_CONTACTS] = {0}, found_count = 0;

    printf(BOLD"Choose field to search and delete by:\n");
    printf("1. Name\n");
    printf("2. Phone (preferred)\n");
    printf("3. Email\n");
    printf("Enter your choice: "RESET);
    scanf("%d", &search_choice);
    getchar();

    printf(BOLD"Enter the value to search: "RESET);
    scanf("%[^\n]%*c", search_value);

    for (int i = 0; i < contact_count; i++) {
        int match = 0;

        switch (search_choice) {
            case 1:
                match = (strcmp(contacts[i].name, search_value) == 0);
                break;
            case 2:
                match = (strcmp(contacts[i].phone, search_value) == 0);
                break;
            case 3:
                match = (strcmp(contacts[i].email, search_value) == 0);
                break;
            default:
                printf(RED"Invalid choice! Aborting.\n"RESET);
                return;
        }

        if (match) {
            found_contacts[found_count++] = i;// Save index of matching contact
        }
    }

    if (found_count == 0) {
        printf(RED"Contact not found!\n"RESET);
        return;
    }

    int idx;// Variable to store the index of the contact to delete
    if (found_count == 1) {
        idx = found_contacts[0];
    } else {
        printf(BOLD"\nMultiple contacts found:\n"RESET);
        for (int i = 0; i < found_count; i++) {
            int contact_idx = found_contacts[i];
            printf(BOLD"%d. Name: %s, Phone: %s, Email: %s\n"RESET, i + 1, 
                   contacts[contact_idx].name, 
                   contacts[contact_idx].phone, 
                   contacts[contact_idx].email );
        }
        printf(BOLD"Enter the number of the contact to delete (1-%d): "RESET, found_count );
        int choice;
        scanf("%d", &choice);

        if (choice < 1 || choice > found_count) {
            printf(RED"Invalid choice! Aborting.\n"RESET);
            return;
        }

        idx = found_contacts[choice - 1];// Retrieve the selected contact index
    }

    printf(BOLD"\nAre you sure you want to delete this contact?\n");
    printf("Name: %s\n", contacts[idx].name);
    printf("Phone: %s\n", contacts[idx].phone);
    printf("Email: %s\n", contacts[idx].email);
    printf("\nType 'yes' to confirm or 'no' to cancel: "RESET);

    char confirmation[4];
    scanf("%3s", confirmation);

    if (strcmp(confirmation, "yes") != 0) {
        printf(RED"Deletion cancelled.\n"RESET);
        return;
    }

    for (int i = idx; i < contact_count - 1; i++) {
        contacts[i] = contacts[i + 1];
    }
    contact_count--;

    printf(GREEN"Contact deleted successfully!\n"RESET);
}


//LIST FUNCTION

// Function to display all contacts
void list_contact() {
    if (contact_count == 0) {
        printf("\nNo contacts found!\n");
        return;
    }

    printf(BOLD"\n-----------------------------------------------\n");
    printf("No   Name                 Phone         Email\n");
    printf("-----------------------------------------------\n");

    for (int i = 0; i < contact_count; i++) {
        printf("%-4d %-20s %-12s %-30s\n", 
               i + 1, contacts[i].name, contacts[i].phone, contacts[i].email);
    }

    printf("-----------------------------------------------\n"RESET);
}


void intialize(){
    contact_count=0;
    load_contact();
}
