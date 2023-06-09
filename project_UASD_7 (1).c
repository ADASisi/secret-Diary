#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "file_encryption.h"

#define MAX_FILE_LINE_LENGTH 100
#define MAX_MENU_LINE_LENGTH 100
#define HEADING_SYMBOLS 30
#define MAX_FILENAME_LENGTH 50
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20

struct node {
    char* name;
    char* filename;
    char* username;
    struct node* next;
};

struct user {
    char* username;
    char* password;
    struct user* next;
};

struct node* _init_() {
    struct node* node = malloc(sizeof(struct node));
    node->next = NULL;
    node->name = NULL;
    node->filename = NULL;
    node->username = NULL;
    return node;
}

struct user* init_user() {
    struct user* user = malloc(sizeof(struct user));
    user->next = NULL;
    user->username = NULL;
    user->password = NULL;
    return user;
}

void get_data(char* text, int* day, int* month, int* year){// NE PIPAJ
    int i = 0, add = 0;
    while(text[i] != '@'){
        i++;
    }
    i += 2;

    while (text[i] != '.'){
        add = add * 10 + (text[i] - '0');
        i++;
    }
    *year = add;

    i++;
    add = 0;

    while (text[i] != '.'){
        add = add * 10 + (text[i] - '0');
        i++;
    }
    *month = add;

    i++;
    add = 0;

    while (text[i] != ' '){
        add = add * 10 + (text[i] - '0');
        i++;
    }
    *day = add;
}

void extract_name(char* text, char* name){// NE PIPAJ
    int i = 0;
    while(text[i] != '@'){
        name[i] = text[i];
        i++;
    }
    name[i-1] = '\0';
}

void extract_filename(char* text, char* filename){// NE PIPAJ
    int i = 0, j = 0;
    char* c;
    while(text[i] != '@'){
        i++;
    }
    i++;
    while(text[i] != '@'){
        i++;
    }
    i += 2;
    c = text + i;
    strcpy(filename, c);
    filename[strlen(filename) - 1] = '\0';
    /*
    while(text[i] != '\0'){
        filename[j] = text[i];
        printf("%c", filename[j]);
        j++;
    }
    filename[j] = '\0';
    */
}


void make_uppercase(char* heading){// NE PIPAJ
    int i = 0;
    while(heading[i] != '\0'){//\n when using gets
        if(heading[i] > 96 && heading[i] < 123){
            heading[i] += -32;
        }
        i++;
    }
}

void generate_filename(char* filename){// NE PIPAJ
    int i = time(NULL), j = 0;
    while(j < MAX_FILENAME_LENGTH - 6){
        i = (((j+i)*i)%26) + 97;
        filename[j] = i;
        j++;
    }
    filename[j+1] = '\0';
    strcat(filename, ".txt");
}

/*
void generate_filename(char* name, int day, int month, int year, char* filename){// NE PIPAJ
    int i = 0, j = 0;

    while(j < MAX_FILENAME_LENGTH - 6){
        if(name[i] != '\0'){
            filename[i] = name[i];
            i++;
        }
        else{
            filename[j] = (((j+i)*i)%26) + 97;
        }
        if(j == day){
            filename[j] = day + 64;
        }
        if(j == day + month){
            filename[j] = month + 47;
        }
        j++;
    }
    filename[j] = (year % 26) + 97;
    filename[j+1] = '\0';
    strcat(filename, ".txt");
}
*/

void insert_node(struct node** head, char* name, char* filename, int day, int month, int year) {// NE PIPAJ
    struct node* new_node = _init_();
    new_node->name = malloc((strlen(name) + 1) * sizeof(char));
    new_node->filename = malloc((strlen(filename) + 1) * sizeof(char));
    strcpy(new_node->name, name);
    strcpy(new_node->filename, filename);
    new_node->day = day;
    new_node->month = month;
    new_node->year = year;
    make_uppercase(new_node->name);

    if (*head == NULL) {
        *head = new_node;
        return;
    }

    if ((*head)->year > year || ((*head)->year == year && (*head)->month > month) || ((*head)->year == year && (*head)->month == month && (*head)->day > day)) {
        new_node->next = *head;
        *head = new_node;
    }
    else {
        struct node* temp = *head;
        while (temp->next != NULL && (temp->next->year < year || (temp->next->year == year && temp->next->month < month) || (temp->next->year == year && temp->next->month == month && temp->next->day < day))) {
            temp = temp->next;
        }
        new_node->next = temp->next;
        temp->next = new_node;
    }
}

void getting_menu(struct node** head, char* text){// NE PIPAJ
    int day, month, year;
    char name[HEADING_SYMBOLS], filename[MAX_FILENAME_LENGTH];
    get_data(text, &day, &month, &year);
    extract_name(text, name);
    extract_filename(text, filename);
    insert_node(head, name, filename, day, month, year);
}

void print_menu(struct node* head){
    struct node* itt = head;
    if(itt->name == NULL){
        printf("THERE ARE NO STORIES WRITTEN IN THE DIARY YET!\n");
    }
    else{
        printf("    LIST\n");
        printf("------------------------\n");
        while(itt != NULL){
            printf("%s - %d.%d.%d\n", itt->name, itt->day, itt->month, itt->year);
            itt = itt->next;
        }
        printf("------------------------\n");
        printf("READ ALL(type \"read all\")\n");
    }
}

void add_story(struct node** head, char* password){
    char name[HEADING_SYMBOLS], text[MAX_FILE_LINE_LENGTH], filename[MAX_FILENAME_LENGTH];
    int day, month, year, i = 0;
    printf("\nENTER NAME OF STORY: ");
    getchar();
    gets(name);
    do{
        printf("ENTER ON WHICH DAY DID IT HAPPEN: ");
        scanf("%d", &day);
        getchar();
    }while(day < 1 || day > 31);
    do{
        printf("ENTER IN WHICH MONTH DID IT HAPPEN: ");
        scanf("%d", &month);
        getchar();
    }while(month < 1 || month > 12);
    do{
        printf("ENTER WHICH YEAR DID IT HAPPEN: ");
        scanf("%d", &year);
        getchar();
    }while(year > 2023);
    
    generate_filename(filename);

    printf("%s", filename);
    
    insert_node(head, name, filename, day, month, year);
    printf("\nafter insert\n");

    FILE *story;
    story = fopen(filename,"w");
    printf("\nENTER THE STORY (FOR NEW LINES TYPE @)\n");
    printf("\n");
    while(1){
        text[i] = getchar();
        if(text[i] == '\n' && text[i-1] != '!' && text[i-1] != '?' && text[i-1] != '.' && text[i-1] != ';'){
            fprintf(story, ".");
            printf(".");
            break;
        }
        if(text[i] == '@'){
            fprintf(story, "\n");
            printf("\n");
        }
        else{
            if(text[i] != ' ' && text[i-1] != '@'){
                if(text[i] == ' '){
                    fprintf(story, " ");// it doesn't print spaces! 
                    printf(" ");
                }
                fprintf(story, "%c", text[i]);
                printf("%c", text[i]);
            }
        }
        i++;
    }
    fclose(story);
    encrypt(password, filename);
    printf("\n");
}

void read_story(struct node* head, char* password){
    if(head->name != NULL){
        char text[MAX_FILE_LINE_LENGTH], name[HEADING_SYMBOLS];
        FILE *read;
        getchar();
        while(1){
            printf("\nENTER THE NAME OF THE ONE YOU WANT TO READ: ");
            gets(name);
            if(strcmp(name,"read all") == 0){
                struct node* temp = head;
                printf("\n");
                while(temp != NULL){
                    printf("    %s\n", temp->name);
                    decrypt(temp->filename, password);
                    /*read = fopen(temp->filename, "r");
                    while(fgets(text, MAX_FILE_LINE_LENGTH, read)){
                        printf("%s\n", text);
                    }*/
                    printf("\n");
                    fclose(read);
                    temp = temp->next;
                }
                break;
            }
            else{
                struct node* curr;
                for (curr = head; head != NULL; head = head->next) {
                    if (!strcmp(curr->name, name))break;
                }
                if((read = fopen(curr->filename, "r")) != NULL){
                    printf("\n");
                    make_uppercase(name);
                    printf("    %s\n", name);
                    decrypt(temp->filename, password);
                    /*while(fgets(text, MAX_FILE_LINE_LENGTH, read)){
                        printf("%s\n", text);
                    }*/
                    printf("\n");
                    fclose(read);
                    break;
                }
                else{
                    fclose(read);
                    printf("ERROR WITH OPENING THE FILE\n");
                }
            }
        };
    }
/*
        char stay[4];
        printf("\nDO YOU WANT TO KEEP READING?: ");
        scanf("%s", stay);
        if(strcmp(stay,"yes") == 0){
            print_menu(head);
            read_story(head);
        }
    }
*/
}

void register_user(struct user** head) {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter a username: ");
    scanf("%s", username);
    printf("Enter a password: ");
    scanf("%s", password);

    struct user* new_user = init_user();
    new_user->username = malloc((strlen(username) + 1) * sizeof(char));
    new_user->password = malloc((strlen(password) + 1) * sizeof(char));
    strcpy(new_user->username, username);
    strcpy(new_user->password, password);

    if (*head == NULL) {
        *head = new_user;
    } else {
        struct user* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_user;
    }
}

struct user* login(struct user* head) {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter your username: ");
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);

    struct user* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->username, username) == 0 && strcmp(temp->password, password) == 0) {
            return temp;
        }
        temp = temp->next;
    }

    printf("Invalid username or password. Please try again.\n");
    return NULL;
}

int main() {
    struct node* head = NULL;
    struct user* user_head = NULL;
    struct user* current_user = NULL;

    FILE* users_file = fopen("Users.txt", "r");
    if (users_file != NULL) {
        char line[MAX_FILE_LINE_LENGTH];
        while (fgets(line, sizeof(line), users_file)) {
            char username[MAX_USERNAME_LENGTH];
            char password[MAX_PASSWORD_LENGTH];

            sscanf(line, "%s %s", username, password);

            struct user* new_user = init_user();
            new_user->username = malloc((strlen(username) + 1) * sizeof(char));
            new_user->password = malloc((strlen(password) + 1) * sizeof(char));
            strcpy(new_user->username, username);
            strcpy(new_user->password, password);

            if (user_head == NULL) {
                user_head = new_user;
            } else {
                struct user* temp = user_head;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = new_user;
            }
        }
        fclose(users_file);
    }

    int choice;
    while (1) {
        printf("\n--- Diary Menu ---\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Add Story\n");
        printf("4. Read Story\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Consume the newline character

        switch (choice) {
            case 1:
                register_user(&user_head);
                break;
            case 2:
                current_user = login(user_head);
                break;
            case 3:
                if (current_user != NULL) {
                    add_story(&head, current_user->password);
                } else {
                    printf("Please log in first.\n");
                }
                break;
            case 4:
                if (current_user != NULL) {
                    read_story(head, current_user->password);
                } else {
                    printf("Please log in first.\n");
                }
                break;
            case 5:
                printf("Exiting the program...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}