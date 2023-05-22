#include<stdio.h>
#include<string.h>

void print_menu(FILE* menu){
    char list[100];
    fseek(menu, 0, SEEK_SET);
    printf("------------------------\n");
    while(fgets(list, 100, menu)){
        printf("%s\n", list);
    }
    printf("------------------------\n");
}

int check_if_file_exists(char* name){
    if(fopen(name,"r") == NULL){
        return 0;//doesn't exist
    }
    else{
        return 1;//exists
    }
}

void add_story(FILE* menu){
    char name[30], text[100];
    int day, month, year;
    do{
        printf("ENTER NAME OF STORY: ");
        scanf("%s", name);
        strcat(name, ".txt");
    }while(check_if_file_exists(name) == 1);
    
    printf("ENTER ON WHICH DAY DID IT HAPPEN: ");
    scanf("%d", &day);
    printf("ENTER IN WHICH MONTH DID IT HAPPEN: ");
    scanf("%d", &month);
    printf("ENTER WHICH YEAR DID IT HAPPEN: ");
    scanf("%d", &year);
    
    fflush(menu);
    fprintf(menu, "%s - %d.%d.%d\n", name, day, month, year);//need to be sorted

    FILE *story;
    story = fopen(name,"w");
    printf("\nENTER THE STORY\n");
    printf("\n");
    if(gets(text) == NULL){
        printf("IT RETURNED NULL");
    }else{
        printf("IT RETURNED: %s", text);
    }
    printf("\n");
    rewind(story);
    fprintf(story, "%s", text);
    fclose(story);
}

void read_story(char* name){
    FILE *read;
    read = fopen(name, "r");
}

int main(){
    FILE *menu = fopen("menu.txt","a+");
    int choice;
    char read[30];
    printf("THIS IS PERSONAL DIARY PLEASE DON'T LOOK IN IT IF IT'S NOT YOURS!");
    do{
        do{
            printf("CHOOSE AN OPTION:\n1)ADD STORY\n2)READ YOUR STORIES\n3)EXIT THE DIARY\n");
            scanf("%d", &choice);
        }while(choice != 1 && choice != 2 && choice != 3);

        if(choice == 1){
            add_story(menu);
            //sort_menu();
        }

        if(choice == 2){
            print_menu(menu);
            printf("ENTER THE NAME OF THE ONE YOU WANT TO READ: ");
            scanf("%s", read);
            //read_story();
        }

    }while(choice != 3);
    fclose(menu);
    return 0;
}