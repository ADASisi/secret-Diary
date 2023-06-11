#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX_FILE_LINE_LENGTH 100
#define MAX_MENU_LINE_LENGTH 100
#define HEADING_SYMBOLS 30
#define MAX_FILENAME_LENGTH 50
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20

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
    filename[j] = '\0';
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

void print_menu(struct node* head){
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

void add_story(struct node** head, char* menu_filename){
    char name[MAX_FILE_LINE_LENGTH], text[MAX_FILE_LINE_LENGTH];
    int day, month, year;
    printf("Enter the story name: ");
    fgets(name, MAX_FILE_LINE_LENGTH, stdin);
    name[strlen(name)-1] = '\0';

    printf("Enter the story text (press Ctrl + D to finish):\n");
    fgets(text, MAX_FILE_LINE_LENGTH, stdin);

    printf("Enter the date (in format DD.MM.YYYY): ");
    char date[MAX_FILE_LINE_LENGTH];
    fgets(date, MAX_FILE_LINE_LENGTH, stdin);
    get_data(date, &day, &month, &year);

    char filename[MAX_FILENAME_LENGTH];
    generate_filename(filename);

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

    fprintf(story_file, "%s", text);
    fclose(story_file);

    printf("The story has been added successfully.\n");
}

void delete_story(struct node** head, char* menu_filename){
    if (!*head) {
        printf("No stories to delete.\n");
        return;
    }
    struct node* prev, *current = select_story(*head, &prev);

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

void view_story(struct node* head){
    if (!head) {
        printf("No stories to view.\n");
        return;
    }

    struct node* story = select_story(head, NULL);

    FILE* file = fopen(story->filename, "r");

    if(file == NULL){
        printf("Error opening file.\n");
        exit(1);
    }

    char line[MAX_FILE_LINE_LENGTH];

    while (fgets(line, MAX_FILE_LINE_LENGTH, file)) {
        printf("%s\n", line);
    }

    fclose(file);
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

        int choice;
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
                    add_story(head, menu_filename);
                    break;
                case 2:
                    delete_story(head, menu_filename);
                    break;
                case 3:
                    view_story(*head);
                    break;
                case 4:
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
