#include<stdio.h>
#include<string.h>

void print_menu(FILE* menu){
    char list[100];
    fseek(menu, 0, SEEK_SET);
    printf("    LIST\n");
    printf("------------------------\n");
    while(fgets(list, 100, menu)){
        printf("%s\n", list);
    }
    printf("------------------------\n");
}

void make_capital(char* heading){  //to upper
    int i = 0;
    while(heading[i] != '\0'){//\n when using gets
        if(heading[i] > 96 && heading[i] < 123){
            heading[i] += -32;
            printf("%d\n", i);
        }
        i++;
    }
}

int check_if_file_exists(char* name){
    if(fopen(name,"r") == NULL){
        return 0;//doesn't exist
    }
    else{
        printf("STORY WITH THE SAME NAME ALREADY EXIST\n");
        return 1;//exists
    }
}

void add_story(FILE* menu){
    char name[30], text[100], filename[34];
    int day, month, year;
    do{
        printf("ENTER NAME OF STORY: ");
        scanf("%s", name);
        strcpy(filename, name);
        strcat(filename, ".txt");
        printf("%s\n", filename);
    }while(check_if_file_exists(filename) == 1);

    printf("ENTER ON WHICH DAY DID IT HAPPEN: ");
    scanf("%d", &day);
    printf("ENTER IN WHICH MONTH DID IT HAPPEN: ");
    scanf("%d", &month);
    printf("ENTER WHICH YEAR DID IT HAPPEN: ");
    scanf("%d", &year);

    make_capital(name);
    printf("%s", name);
    fflush(menu);
    fprintf(menu, "%s - %d.%d.%d\n", name, day, month, year);//need to be sorted

    FILE *story;
    story = fopen(filename,"w");
    printf("\nENTER THE STORY\n");
    printf("\n");
    getchar();
    if(gets(text) == NULL){
        printf("IT RETURNED NULL\n");
    }else{
        printf("IT RETURNED: %s.", text);
    }
    printf("\n");
    rewind(story);
    fprintf(story, "%s", text);
    fclose(story);
}

void read_story(){
    char text[100], name[30], filename[30];

    printf("ENTER THE NAME OF THE ONE YOU WANT TO READ: ");
    scanf("%s", name);

    strcpy(filename, name);
    strcat(filename, ".txt");
    FILE *read;
    if((read = fopen(filename, "r")) != NULL){
        printf("\n");
        make_capital(name);
        printf("    %s\n", name);

        while(fgets(text, 100, read)){
            printf("%s\n", text);
        }
        printf("\n");
    }
    else{
        printf("ERROR WITH OPENING THE FILE\n");
    }
}

int main(){
    FILE *menu = fopen("menu.txt","a+");
    int choice;
    printf("THIS IS PERSONAL DIARY PLEASE DON'T LOOK IN IT IF IT'S NOT YOURS!\n\n");
    do{
        do{
            printf("CHOOSE AN OPTION:\n1)ADD STORY\n2)READ YOUR STORIES\n3)EXIT THE DIARY\n: ");
            scanf("%d", &choice);
        }while(choice != 1 && choice != 2 && choice != 3);

        if(choice == 1){
            add_story(menu);
            //sort_menu();
        }

        if(choice == 2){
            print_menu(menu);
            read_story();
        }

    }while(choice != 3);
    fclose(menu);
    return 0;
}
