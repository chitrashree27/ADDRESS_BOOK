#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100
#define MAX_NAME_LENGTH 50
#define MAX_EMAIL_LENGTH 100
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char name[MAX_NAME_LENGTH];
    char phone[15];
    char email[MAX_EMAIL_LENGTH];
} Contact;

extern Contact contacts[MAX_CONTACTS];
extern int contact_count;

int validate_name(char *name);
int validate_phone(char *phone);
int validate_email(char *email);
void create_contact();
void search_contact();
void edit_contact();
void delete_contact();  
void list_contact(); 
void initialize();
void save_contact();

#endif
