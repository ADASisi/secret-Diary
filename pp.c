#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"search.c"
#include"file_encryption.h"

#define MAX_FILE_LINE_LENGTH 100
#define MAX_MENU_LINE_LENGTH 100
#define HEADING_SYMBOLS 30
#define MAX_FILENAME_LENGTH 50
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
//#define STRINGSIZE 40

struct node{
    char* name, *filename;
    int day, month, year;
    struct node* next;
};

struct user{
    char* username, *password;
    struct node* diary;
    struct user* next;
};

struct node* init_node(){
    struct node* node = malloc(sizeof(struct node));
    node->next = NULL;
    node->name = NULL;
    node->filename = NULL;
    node->day = 0;
    node->month = 0;
    node->year = 0;
    return node;
}

struct user* init_user(){
    struct user* user = malloc(sizeof(struct user));
    user->next = NULL;
    user->username = NULL;
    user->password = NULL;
    user->diary = NULL;
    return user;
}

void get_data(char* text, int* day, int* month, int* year){
    int i = 0, add = 0;

    while (text[i] >= '0' && text[i] <= '9'){
        add = add * 10 + (text[i] - '0');
        i++;
    }
    *day = add;

    i++;
    add = 0;

    while (text[i] >= '0' && text[i] <= '9'){
        add = add * 10 + (text[i] - '0');
        i++;
    }
    *month = add;

    i++;
    add = 0;

    while (text[i] >= '0' && text[i] <= '9'){
        add = add * 10 + (text[i] - '0');
        i++;
    }
    *year = add;
}

void make_uppercase(char* heading){
    int i = 0;
    while(heading[i] != '\0'){
        if(heading[i] > 96 && heading[i] < 123){
            heading[i] += -32;
        }
        i++;
    }
}

void generate_filename(char* filename){
    int i = time(NULL), j = 0;
    while(j < MAX_FILENAME_LENGTH - 6){
        i = (((j+i)*i)%26) + 97;
        filename[j] = i;
        j++;
    }
    filename[j+1] = '\0';
    strcat(filename, ".txt");
}

void insert_node(struct node** head, char* name, int day, int month, int year, char* filename){
    struct node* new_node = init_node();
    new_node->name = malloc(strlen(name)+1);
    strcpy(new_node->name, name);
    new_node->day = day;
    new_node->month = month;
    new_node->year = year;
    new_node->filename = malloc(strlen(filename)+1);
    strcpy(new_node->filename, filename);

    if (*head == NULL || (year < (*head)->year) || (year == (*head)->year && month < (*head)->month) ||
        (year == (*head)->year && month == (*head)->month && day < (*head)->day)){
        new_node->next = *head;
        *head = new_node;
    } else {
        struct node* current = *head;
        while (current->next != NULL && (year > current->next->year || (year == current->next->year && month > current->next->month) ||
               (year == current->next->year && month == current->next->month && day > current->next->day))){
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

struct user* insert_user(struct user** head, char* username, char* password){
    struct user* new_user = init_user();
    new_user->username = malloc(strlen(username)+1);
    strcpy(new_user->username, username);
    new_user->password = malloc(strlen(password)+1);
    strcpy(new_user->password, password);

    if (*head == NULL){
        new_user->next = *head;
        *head = new_user;
    } else {
        struct user* current = *head;
        while (current->next != NULL){
            current = current->next;
        }
        new_user->next = current->next;
        current->next = new_user;
    }

    return new_user;
}

struct node* select_story(struct node* head, struct node** prev) {
    struct node* curr = head;
    int i = 0;
    int opt;

    while (curr) {
        i++;
        printf("%d. %s\n", i, curr->name);
        curr = curr->next;
    }

    while (1) {
        printf("Select a story (1 - %d): ", i);

        scanf("%d", &opt);
        opt--;

        if (opt < 0 || opt >= i) {
            printf("Invalid index!\n");
        }
        else break;
    }

    i = 0;
    curr = head;
    if (prev) *prev = NULL;
    while (curr) {
        if (opt == i) break;
        if (prev) *prev = curr;
        curr = curr->next;
        i++;
    }
    return curr;
}

char* search_story(struct node* head, struct node** prev, struct  hashtable* table_for_titles, struct  hashtable* table_for_dates) {
    struct node* curr = head;
    int i = 0;
    char* filename;
    char secondValue[STRINGSIZE];
    char sourceForHash[STRINGSIZE];
    int option;

    while (curr) {
        i++;
        printf("%d. %s - %d.%d.%d\n", i, curr->name, curr->day, curr->month, curr->year);
        curr = curr->next;
    }

    printf("Choose to enter date or title of the story:\n");
    printf("1) DATE\n");
    printf("2) TITLE\n");
    printf("Option: ");
    scanf("%d", &option);
    getchar();
    if(option == 1){
        printf("\nEnter the date of the story you want to read(dd.mm.yyyy): ");
        fgets(sourceForHash, STRINGSIZE, stdin);
        sourceForHash[strlen(sourceForHash) - 1] = '\0';
        
        if(printSecondValues(table_for_dates, sourceForHash)){
            printf("Enter the title of the story: ");
            fgets(secondValue, STRINGSIZE, stdin);
            secondValue[strlen(secondValue) - 1] = '\0';

            filename = hashtable_contains(table_for_dates, sourceForHash, secondValue);
        }
        else return NULL;

    }
    else if(option == 2){
        printf("\nEnter the title of the story you want to read: ");
        fgets(sourceForHash, STRINGSIZE, stdin);
        sourceForHash[strlen(sourceForHash) - 1] = '\0';
        
        if(printSecondValues(table_for_titles, sourceForHash)){
            printf("Enter the date of the story: ");
            fgets(secondValue, STRINGSIZE, stdin);
            secondValue[strlen(secondValue) - 1] = '\0';

            filename = hashtable_contains(table_for_titles, sourceForHash, secondValue);
        }
        else return NULL;
    }
    else {
        printf("There is no such option!\n");
        return NULL;
    }

    if(filename == NULL) {
        printf("There is not such story!\n");
    }

    return filename;
}

void add_story(struct node** head, char* menu_filename, struct  hashtable* table_for_titles, struct  hashtable* table_for_dates, char* password){
    char name[MAX_FILE_LINE_LENGTH], text[MAX_FILE_LINE_LENGTH];
    int day, month, year, i = 0, space_printed = 0;
    printf("Enter the story name: ");
    fgets(name, MAX_FILE_LINE_LENGTH, stdin);
    name[strlen(name)-1] = '\0';

    printf("Enter the date (in format DD.MM.YYYY): ");
    char date[MAX_FILE_LINE_LENGTH];
    fgets(date, MAX_FILE_LINE_LENGTH, stdin);
    get_data(date, &day, &month, &year);

    char filename[MAX_FILENAME_LENGTH];
    generate_filename(filename);

    //printf("Table: title: %s, date: %s, filename: %s\n", name, dateToString(day, month, year), filename);
    table_for_titles = hashtable_add(table_for_titles, name, dateToString(day, month, year), filename);
    table_for_dates = hashtable_add(table_for_dates, dateToString(day, month, year), name, filename);

    insert_node(head, name, day, month, year, filename);

    FILE* menu_file = fopen(menu_filename, "a");
    if(menu_file == NULL){
        printf("Error opening file.\n");
        exit(1);
    }
    fprintf(menu_file, "%s\n%s\n%d %d %d\n", name, filename, day, month, year);
    fclose(menu_file);

    FILE* story_file = fopen(filename, "w");

    if(story_file == NULL){
        printf("Error opening file.\n");
        exit(1);
    }

    printf("Enter the story text (for new lines type @):\n");
    while(1){
        if(i == MAX_FILE_LINE_LENGTH - 1) break;
        text[i] = getchar();
        if(text[i] == '\n') break;
        if (text[i] == '@') {
            fprintf(story_file, "\n");
            space_printed = 0;
        }
        else {
            if (text[i] != ' ') {
                if (space_printed) {
                    fprintf(story_file, " ");
                    space_printed = 0;
                }
                fprintf(story_file, "%c", text[i]);
            }
            else if (!space_printed) {
                if(text[i-1] != '@'){
                    space_printed = 1;
                }
            }
            
        }
        i++;
    }
    if(text[i-1] != '!' && text[i-1] != '?' && text[i-1] != '.' && text[i-1] != ';'){
        fprintf(story_file, ".");
    }
    fclose(story_file);
    encrypt(password, filename);

    printf("The story has been added successfully.\n");
}

void delete_story(struct node** head, char* menu_filename, struct  hashtable* table_for_titles, struct  hashtable* table_for_dates){
    if (!*head) {
        printf("No stories to delete.\n");
        return;
    }
    struct node* prev, *current = select_story(*head, &prev);

    hashtable_delete(table_for_titles, current->name, dateToString(current->day, current->month, current->year));
    hashtable_delete(table_for_dates, dateToString(current->day, current->month, current->year), current->name);

    if(prev == NULL){
        *head = current->next;
    } else {
        prev->next = current->next;
    }

    FILE* menu_file = fopen(menu_filename, "w");

    prev = current;
    current = *head;
    while (current != NULL) {
        fprintf(menu_file, "%s\n%s\n%d %d %d\n", current->name, current->filename, current->day, current->month, current->year);
        current = current->next;
    }

    remove(prev->filename);
    free(prev->name);
    free(prev->filename);
    free(prev);
    printf("The story has been deleted successfully.\n");
}

void view_story(struct node* head, struct  hashtable* table_for_titles, struct  hashtable* table_for_dates, char* password){
    if (!head) {
        printf("No stories to view.\n");
        return;
    }

    char* story_filename = search_story(head, NULL, table_for_titles, table_for_dates);
    if(story_filename == NULL) return;
    FILE* file = fopen(story_filename, "r");

    if(file == NULL){
        printf("Error opening file.\n");
        exit(1);
    }

    fclose(file);
    decrypt(story_filename, password);
    

    /*char line[MAX_FILE_LINE_LENGTH];

    while (fgets(line, MAX_FILE_LINE_LENGTH, file)) {
        printf("%s", line);
    }*/
    printf("\n");

    
}

void fillingHashtables(struct node* head, struct hashtable* table_for_dates, struct hashtable* table_for_titles){
    struct node* itt = head;
    if(itt != NULL){
        while(itt != NULL){
            printf("%s\n", itt->name);
            printf("%s\n", dateToString(itt->day, itt->month, itt->year));
            printf("%s\n", itt->filename);
            table_for_titles = hashtable_add(table_for_titles, itt->name, dateToString(itt->day, itt->month, itt->year), itt->filename);
            table_for_dates = hashtable_add(table_for_dates, dateToString(itt->day, itt->month, itt->year), itt->name, itt->filename);
            itt = itt->next;
        }
        for(int i = 0; i < table_for_titles->num_of_buckets; i++){
            for(struct linkedlist_node* curr = table_for_titles->buckets[i]->head; curr != NULL; curr = curr->next){
                for(int j = 0; j < curr->secondValue->count; j++){
                    printf("%d: %s, %s, %s\n", i, curr->sourceForHash, curr->secondValue->buff[j], curr->thirdValue->buff[j]);
                }
            }
        }
    }
}

void login(struct user* users, char* user_filename){
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    printf("Enter your username: ");
    fgets(username, MAX_USERNAME_LENGTH, stdin);
    username[strlen(username)-1] = '\0';

    struct user* current = users;
    while(current != NULL){
        if(strcmp(current->username, username) == 0){
            break;
        }
        current = current->next;
    }

    if(current == NULL){
        printf("Invalid username.\n");
        return;
    }

    printf("Enter your password: ");
    fgets(password, MAX_PASSWORD_LENGTH, stdin);
    password[strlen(password)-1] = '\0';

    if(strcmp(current->password, password) == 0){
        printf("Login successful.\n");
        char menu_filename[MAX_MENU_LINE_LENGTH];
        struct node** head = &current->diary;
        strcpy(menu_filename, username);
        strcat(menu_filename, ".txt");

        
        struct  hashtable* table_for_titles = hashtable_init(10, 5);
        struct  hashtable* table_for_dates = hashtable_init(10, 5);

        //fillingHashtables(*head, table_for_dates, table_for_titles);
        struct node* itt = *head;
        if(itt != NULL){
            while(itt != NULL){
                table_for_titles = hashtable_add(table_for_titles, itt->name, dateToString(itt->day, itt->month, itt->year), itt->filename);
                table_for_dates = hashtable_add(table_for_dates, dateToString(itt->day, itt->month, itt->year), itt->name, itt->filename);
                itt = itt->next;
            }
        }

        int choice = 0;
        while(1){
            printf("\nMenu:\n");
            printf("1. Add a story\n");
            printf("2. Delete a story\n");
            printf("3. View my stories\n");
            printf("4. Logout\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            getchar(); // To consume the newline character

            switch(choice){
                case 1:
                    add_story(head, menu_filename, table_for_titles, table_for_dates, current->password);
                    break;
                case 2:
                    delete_story(head, menu_filename, table_for_titles, table_for_dates);
                    break;
                case 3:
                    view_story(*head, table_for_titles, table_for_dates, current->password);
                    break;
                case 4:
                    free_hashtable(table_for_titles);
                    free_hashtable(table_for_dates);
                    return;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        }
    } else {
        printf("Invalid password.\n");
    }
}

void register_user(struct user** users,  char* user_filename){
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    printf("Enter your desired username: ");
    fgets(username, MAX_USERNAME_LENGTH, stdin);
    username[strlen(username)-1] = '\0';

    struct user* current = *users;
    while(current != NULL){
        if(strcmp(current->username, username) == 0){
            printf("Username \"%s\" is already taken. Please choose a different username.\n", username);
            return;
        }
        current = current->next;
    }

    printf("Enter your desired password: ");
    fgets(password, MAX_PASSWORD_LENGTH, stdin);
    password[strlen(password)-1] = '\0';

    insert_user(users, username, password);

    FILE* user_file = fopen(user_filename, "a");
    if(user_file == NULL){
        printf("Error opening file.\n");
        exit(1);
    }
    fprintf(user_file, "%s\n%s\n", username, password);
    fclose(user_file);

    printf("Registration successful.\n");
}

int main(){
    struct user* users = NULL;
    char user_filename[] = "users.txt";

    FILE* user_file = fopen(user_filename, "r");

    if (user_file != NULL) {
        while (1) {
            char username[MAX_MENU_LINE_LENGTH], password[MAX_MENU_LINE_LENGTH], menu_filename[MAX_MENU_LINE_LENGTH];

            if (!fgets(username, MAX_MENU_LINE_LENGTH, user_file)) break;
            if (!fgets(password, MAX_MENU_LINE_LENGTH, user_file)) break;

            username[strlen(username) - 1] = '\0';
            password[strlen(password) - 1] = '\0';

            struct user* user = insert_user(&users, username, password);

            strcpy(menu_filename, username);
            strcat(menu_filename, ".txt");


            FILE* menu_file = fopen(menu_filename, "r");

            if (menu_file != NULL) {
                while (1) {
                    char name[MAX_MENU_LINE_LENGTH];
                    char filename[MAX_MENU_LINE_LENGTH];
                    int day, month, year;

                    if (!fgets(name, MAX_MENU_LINE_LENGTH, menu_file)) break;
                    if (!fgets(filename, MAX_MENU_LINE_LENGTH, menu_file)) break;
                    fscanf(menu_file, "%d %d %d", &day, &month, &year);
                    getc(menu_file);

                    name[strlen(name) - 1] = '\0';
                    filename[strlen(filename) - 1] = '\0';

                    insert_node(&user->diary, name, day, month, year, filename);
                }
                fclose(menu_file);
            }
        }
        fclose(user_file);
    }

    int choice;
    while(1){
        printf("\nMenu:\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // To consume the newline character

        switch(choice){
            case 1:
                login(users, user_filename);
                break;
            case 2:
                register_user(&users, user_filename);
                break;
            case 3:
                printf("Exiting the program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}