#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "search.c"
#include "file_encryption.h"

#define NUMBER_OF_SYMBOLS 100//max number of symbols stored in a file
#define SYMBOLS_IN_MENU 50
#define HEADING_SUMBOLS 30

struct node{
    char* name;
    int day, month, year;
    struct node* next;
};

struct node* _init_(){
    struct node* node = malloc(sizeof(struct node));
    node->next = NULL;
    node->name = NULL;
    node->day = 0;
    node->month = 0;
    node->year = 0;
    return node;
}

void get_data(char* text, int* day, int* month, int* year){
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

    while (text[i] != '\n'){
        add = add * 10 + (text[i] - '0');
        i++;
    }
    *day = add;
}

void extract_name(char* text, char* name){
    int i = 0;
    while(text[i] != '@'){
        name[i] = text[i];
        i++;
    }
    name[i-1] = '\0';
}

void make_uppercase(char* heading){  //to upper
    int i = 0;
    while(heading[i] != '\0'){//\n when using gets
        if(heading[i] > 96 && heading[i] < 123){
            heading[i] += -32;
        }
        i++;
    }
}

void insert_node(struct node** head, char* name, int day, int month, int year){
    struct node* new_node = _init_();
    int i = 0;
    new_node->name = malloc(strlen(name)*sizeof(char) + 1);
    strcpy(new_node->name, name);
    new_node->day = day;
    new_node->month = month;
    new_node->year = year;
    make_uppercase(new_node->name);

     if ((*head) == NULL || (*head)->year > year || ((*head)->year == year && (*head)->month > month) || ((*head)->year == year && (*head)->month == month && (*head)->day > day)){
        new_node->next = *head;
        *head = new_node;
    }
    else {
        struct node* temp = *head;
        while (temp->next != NULL && (temp->next->year < year || (temp->next->year == year && temp->next->month < month) || (temp->next->year == year && temp->next->month == month && temp->next->day < day))){
            temp = temp->next;
        }
        new_node->next = temp->next;
        temp->next = new_node;
    }
}

void getting_menu(struct node** head, char* text){
    int day, month, year;
    char name[HEADING_SUMBOLS];
    get_data(text, &day, &month, &year);
    printf("%d, %d, %d", day, month, year);
    extract_name(text, name);
    printf("\n%s\n", name);
    insert_node(head, name, day, month, year);
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

void fillingHashtables(struct node* head, struct hashtable* table_for_dates, struct hashtable* table_for_titles){
    struct node* itt = head;
    if(itt->name != NULL){
        while(itt != NULL){
            itt = itt->next;
            //table_for_titles = hashtable_add(table_for_titles, itt->name, dateToString(itt->day, itt->month, itt->year), itt->id);
            //table_for_dates = hashtable_add(table_for_dates, dateToString(itt->day, itt->month, itt->year), itt->name, itt->id);
        }
    }
}

int check_if_file_exists(char* name){
    FILE* file;
    if((file = fopen(name,"r")) == NULL){
        fclose(file);
        return 0;//doesn't exist
    }
    else{
        printf("STORY WITH THE SAME NAME ALREADY EXIST\n");
        fclose(file);
        return 1;//exists
    }
}

void add_story(struct node** head){
    char name[HEADING_SUMBOLS], text[NUMBER_OF_SYMBOLS], filename[HEADING_SUMBOLS + 4];
    int day, month, year, i = -1;
    do{
        printf("\nENTER NAME OF STORY: ");
        getchar();
        gets(name);
        strcpy(filename, name);
        strcat(filename, ".txt");
    }while(check_if_file_exists(filename) == 1);
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
    

    insert_node(head, name, day, month, year);

    FILE *story;
    story = fopen(filename,"w");
    printf("\nENTER THE STORY (FOR NEW LINES TYPE @)\n");
    printf("\n");
    while(1){
        i++;
        text[i] = getchar();
        if(text[i] == '\n' && text[i-1] != '!' && text[i-1] != '?' && text[i-1] != '.' && text[i-1] != ';'){
            fprintf(story, ".");
            break;
        }
        if(text[i] == '@'){
            fprintf(story, "\n");
        }
        else{
            if(text[i] != ' ' && text[i-1] != '@'){
                if(text[i] == ' '){
                    fprintf(story, " ");// it doesn't print spaces! 
                }
                fprintf(story, "%c", text[i]);
            }
        }
    }
    printf("\n");
    fclose(story);
}

void read_story(struct node* head, struct hashtable* table_for_dates, struct hashtable* table_for_titles){
        int option;
        char text[NUMBER_OF_SYMBOLS], name[HEADING_SUMBOLS];
        char* filename;
        FILE *read;
        char secondValue[STRINGSIZE];
        char sourceForHash[STRINGSIZE];
        getchar();
        while(1){
            printf("CHOOSE TO ENTER DATE OR TITLE OF THE STORY\n");
            printf("1) DATE\n");
            printf("2) TITLE\n");
            printf("Option: ");
            scanf("%d", &option);
            if(option == 1){
                printf("\nENTER THE DATE OF THE STORY YOU WANT TO READ: ");
                fgets(sourceForHash, STRINGSIZE, stdin);
                printf("sourceForHash: %s123\n", sourceForHash);
                
                printSecondValues(table_for_dates, sourceForHash);

                printf("Enter which one to open: ");
                fgets(sourceForHash, STRINGSIZE, stdin);
                printf("sourceForHash: %s123\n", secondValue);

                filename = hashtable_contains(table_for_dates, sourceForHash, secondValue);
            }
            else if(option == 2){
                printf("\nENTER THE DATE OF THE STORY YOU WANT TO READ: ");
                fgets(sourceForHash, STRINGSIZE, stdin);
                printf("sourceForHash: %s123\n", sourceForHash);
                
                printSecondValues(table_for_dates, sourceForHash);

                printf("Enter which one to open: ");
                fgets(secondValue, STRINGSIZE, stdin);
                printf("sourceForHash: %s123\n", secondValue);

                filename = hashtable_contains(table_for_dates, sourceForHash, secondValue);
            }
            else {
                printf("There is no such option!\n");
                break;
            }

            if(filename == NULL) {
                printf("There is not such story!\n");
                break;
            }
            
            strcpy(filename, name);
            strcat(filename, ".txt");
            if((read = fopen(filename, "r")) != NULL){
                printf("\n");
                make_uppercase(name);
                printf("    %s\n", name);

                while(fgets(text, NUMBER_OF_SYMBOLS, read)){
                    printf("%s\n", text);
                }
                printf("\n");
                break;
            }
            else{
                printf("ERROR WITH OPENING THE FILE\n");
                break;
            }
        }

        fclose(read);
        
        char stay[4];
        printf("\nDO YOU WANT TO KEEP READING?: ");
        scanf("%s", stay);
        if(strcmp(stay,"yes") == 0){
            print_menu(head);
            read_story(head, table_for_dates, table_for_titles);
        }
}
/*

void read_story(struct node* head){
    if(head->name != NULL){
        char text[NUMBER_OF_SYMBOLS], name[HEADING_SUMBOLS], filename[HEADING_SUMBOLS + 4];
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
                    strcpy(filename, temp->name);
                    strcat(filename, ".txt");
                    read = fopen(filename, "r");
                    while(fgets(text, NUMBER_OF_SYMBOLS, read)){
                        printf("%s\n", text);
                    }
                    printf("\n");
                    fclose(read);
                    temp = temp->next;
                }
                break;
            }
            else{
                strcpy(filename, name);
                strcat(filename, ".txt");
                if((read = fopen(filename, "r")) != NULL){
                    printf("\n");
                    make_uppercase(name);
                    printf("    %s\n", name);

                    while(fgets(text, NUMBER_OF_SYMBOLS, read)){
                        printf("%s\n", text);
                    }
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
        char stay[4];
        printf("\nDO YOU WANT TO KEEP READING?: ");
        scanf("%s", stay);
        if(strcmp(stay,"yes") == 0){
            print_menu(head);
            read_story(head);
        }
    }
}
*/
int main(){
    int choice;
    char text[SYMBOLS_IN_MENU];
    struct node* head;//= _init_()

    FILE* menu;
    if((menu = fopen("menu.txt", "r")) != NULL){
        while (fgets(text, SYMBOLS_IN_MENU, menu) != NULL)
        {
            getting_menu(&head, text);
        }
    }    
    fclose(menu);

    struct  hashtable* table_for_titles = hashtable_init(10, 5);
    struct  hashtable* table_for_dates = hashtable_init(10, 5);

    fillingHashtables(head, table_for_dates, table_for_titles);
    
    printf("THIS IS PERSONAL DIARY PLEASE DON'T LOOK IN IT IF IT'S NOT YOURS!\n\n");
    do{
        do{
            printf("CHOOSE AN OPTION:\n1)ADD STORY\n2)READ YOUR STORIES\n3)EXIT THE DIARY\n: ");
            scanf("%d", &choice);
        }while(choice != 1 && choice != 2 && choice != 3);

        if(choice == 1){
            add_story(&head);
        }

        if(choice == 2){
            print_menu(head);
            read_story(head, table_for_dates, table_for_titles);
        }

    }while(choice != 3);
    menu = fopen("menu.txt", "w");
    while(head != NULL){
        make_uppercase(head->name);
        fprintf(menu, "%s @ %d.%d.%d\n", head->name, head->day, head->month, head->year);
        head = head->next;
    }
    fclose(menu);
    struct node* temp1 = head,* temp2 = head;
    while(temp1 != NULL){
        temp1 = temp1->next;
        free(temp2->name);
        free(temp2);
        temp2 = temp1;
    }
    free(temp1);
    free(temp2);
    free(head);
    return 0;
}


//masiv ot stringove - char* string[] = {"","",...};