#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"search.c"

#define MAX_FILE_LINE_LENGTH 100//max number of symbols stored in a file
#define MAX_MENU_LINE_LENGTH 100
#define HEADING_SYMBOLS 30
#define MAX_FILENAME_LENGTH 50

struct node{
    char* name,* filename;
    int day, month, year;
    struct node* next;
};

struct node* _init_(){
    struct node* node = malloc(sizeof(struct node));
    node->next = NULL;
    node->name = NULL;
    node->filename = NULL;
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

    while (text[i] != ' '){
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

void extract_filename(char* text, char* filename){
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

void insert_node(struct node** head, char* name, char* filename, int day, int month, int year) {
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

void getting_menu(struct node** head, char* text){
    int day, month, year;
    char name[HEADING_SYMBOLS], filename[MAX_FILENAME_LENGTH];
    get_data(text, &day, &month, &year);
    extract_name(text, name);
    extract_filename(text, filename);
    insert_node(head, name, filename, day, month, year);
}

void print_menu(struct node* head, int *isEmpty){
    struct node* itt = head;
    if(itt == NULL){
        printf("THERE ARE NO STORIES WRITTEN IN THE DIARY YET!\n");
        *isEmpty = 1;
    }
    else{
        printf("    LIST\n");
        printf("------------------------\n");
        while(itt != NULL){
            printf("%s - %d.%d.%d\n", itt->name, itt->day, itt->month, itt->year);
            itt = itt->next;
        }
        printf("------------------------\n");
        //printf("READ ALL(type \"read all\")\n");
        *isEmpty = 0;
    }
}

void add_story(struct node** head, struct hashtable* table_for_dates, struct hashtable* table_for_titles){
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

    table_for_titles = hashtable_add(table_for_titles, name, dateToString(day, month, year), filename);
    table_for_dates = hashtable_add(table_for_dates, dateToString(day, month, year), name, filename);

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
                    fprintf(story, " "); 
                    printf(" ");
                }
                fprintf(story, "%c", text[i]);
                printf("%c", text[i]);
            }
        }
        i++;
    }
    printf("\n");
    fclose(story);
}

void fillingHashtables(struct node* head, struct hashtable* table_for_dates, struct hashtable* table_for_titles){
    struct node* itt = head;
    printf("HERE\n");
    if(itt != NULL){
        while(itt != NULL){
            printf("%s\n", itt->name);
            table_for_titles = hashtable_add(table_for_titles, itt->name, dateToString(itt->day, itt->month, itt->year), itt->filename);
            table_for_dates = hashtable_add(table_for_dates, dateToString(itt->day, itt->month, itt->year), itt->name, itt->filename);
            itt = itt->next;
        }
    }
    else printf("NULL\n");
}

void read_story(struct node* head, struct hashtable* table_for_titles, struct hashtable* table_for_dates){
    char text[MAX_FILE_LINE_LENGTH], name[HEADING_SYMBOLS];
    char* filename;
    FILE *read;
    char secondValue[STRINGSIZE];
    char sourceForHash[STRINGSIZE];
    int option;
    getchar();
    printf("\n");
    for(int i = 0; i < table_for_dates->num_of_buckets; i++){
        for(struct linkedlist_node* curr = table_for_dates->buckets[i]->head; curr != NULL; curr = curr->next){
            for(int j = 0; j < curr->secondValue->count; j++){
                printf("%d: %s, %s, %s\n", i, curr->sourceForHash, curr->secondValue->buff[j], curr->thirdValue->buff[j]);
            }
        }
    }
    printf("\n");
    while(1){
        printf("CHOOSE TO ENTER DATE OR TITLE OF THE STORY\n");
        printf("1) DATE\n");
        printf("2) TITLE\n");
        printf("Option: ");
        scanf("%d", &option);
        getchar();
        if(option == 1){
            printf("\nENTER THE DATE OF THE STORY YOU WANT TO READ(dd.mm.yyyy): ");
            fgets(sourceForHash, STRINGSIZE, stdin);
            printf("source for hash: %s|", sourceForHash);
            sourceForHash[strlen(sourceForHash) - 1] = '\0';
            
            if(printSecondValues(table_for_dates, sourceForHash)){
                printf("Enter which one to open: ");
                fgets(secondValue, STRINGSIZE, stdin);
                secondValue[strlen(secondValue) - 1] = '\0';

                filename = hashtable_contains(table_for_dates, sourceForHash, secondValue);
            }
            else break;

        }
        else if(option == 2){
            printf("\nENTER THE TITLE OF THE STORY YOU WANT TO READ: ");
            fgets(sourceForHash, STRINGSIZE, stdin);
            printf("source for hash: %s|", sourceForHash);
            sourceForHash[strlen(sourceForHash) - 1] = '\0';
            
            if(printSecondValues(table_for_titles, sourceForHash)){
                printf("Enter which one to open: ");
                fgets(secondValue, STRINGSIZE, stdin);
                secondValue[strlen(secondValue) - 1] = '\0';

                filename = hashtable_contains(table_for_titles, sourceForHash, secondValue);
            }
            else break;
        }
        else {
            printf("There is no such option!\n");
            break;
        }

        if(filename == NULL) {
            printf("There is not such story!\n");
            break;
        }
        if((read = fopen(filename, "r")) != NULL){
            printf("\n");
            make_uppercase(name);
            printf("    %s\n", name);

            while(fgets(text, MAX_FILE_LINE_LENGTH, read)){
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
}
int main(){
    int choice;
    char text[MAX_MENU_LINE_LENGTH];
    struct node* head = NULL;
    FILE* menu;
    if((menu = fopen("menu.txt", "r")) != NULL){
        while (fgets(text, MAX_MENU_LINE_LENGTH, menu) != NULL)
        {
            if (*text == '\n') continue;
            getting_menu(&head, text);
        }
        fclose(menu);
    }  
    else   fprintf(stderr, "tyk\n");

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
            add_story(&head, table_for_dates, table_for_titles);
        }

        if(choice == 2){
            int isEmpty = 0;
            print_menu(head, &isEmpty);
            if(!isEmpty) read_story(head, table_for_titles, table_for_dates);
        }

    }while(choice != 3);
    menu = fopen("menu.txt", "w");
    struct node* temp1 = head,* temp2 = head;
    while(temp1 != NULL){
        make_uppercase(head->name);
        fprintf(menu, "%s @ %d.%d.%d @ %s\n", temp1->name, temp1->day, temp1->month, temp1->year, temp1->filename);
        temp1 = temp1->next;
    }
    fclose(menu);
    temp1 = head;
    while(temp1 != NULL){
        temp1 = temp1->next;
        free(temp2->name);
        free(temp2);
        temp2 = temp1;
    }

    //free_hashtable(table_for_titles);
    //free_hashtable(table_for_dates);
    return 0;
}
