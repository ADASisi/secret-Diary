#include<stdio.h>
#include<string.h>

#define NUMBER_OF_SYMBOLS 100//max number of symbols stored in a file
#define HEADING_SUMBOLS 30

void print_menu(FILE* menu){
    char list[NUMBER_OF_SYMBOLS];
    fseek(menu, 0, SEEK_SET);
    printf("    LIST\n");
    printf("------------------------\n");
    while(fgets(list, NUMBER_OF_SYMBOLS, menu)){
        printf("%s\n", list);
    }
    printf("------------------------\n");
}

void make_capital(char* heading){  //to upper
    int i = 0;
    while(heading[i] != '\0'){//\n when using gets
        if(heading[i] > 96 && heading[i] < 123){
            heading[i] += -32;
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

void extract_name(char* filename){
    int i = 0;
    while(filename[i] != '@'){
        i++;
    }
    filename[i-1] = '\0';
}

int earlier_story_date(char* text, int day, int month, int year){
    int tday = 0, tmonth = 0, tyear = 0, i = 0, j = 0, add = 0;
   while(text[i] != '\0'){
        i++;
   }
   i--;
   while(text[i] != '.'){
        add = text[i] - 48;
        for(int k = 0; k < j; k++){
            add *= 10;
        }
        tyear += add;
        j++;
        i--;
   }
   i--;
   j = 0;
   while(text[i] != '.'){
        add = text[i] - 48;
        for(int k = 0; k < j; k++){
            add *= 10;
        }
        tmonth += add;
        j++;
        i--;
   }
   i--;
   j = 0;
   while(text[i] != ' '){
        add = text[i] - 48;
        for(int k = 0; k < j; k++){
            add *= 10;
        }
        tday += add;
        j++;
        i--;
   }
    if(tyear > year){
        return 0;
    }  
    if(tyear == year){
        if(tmonth > month){
            return 0;
        }
        if(tmonth == month){
            if(tday > day){
                return 0;
            }
        }
    }
    return 1;
}

void add_story(FILE* menu){
    char name[HEADING_SUMBOLS], text[NUMBER_OF_SYMBOLS], filename[HEADING_SUMBOLS + 4];
    int day, month, year, i = -1;
    do{
        printf("\nENTER NAME OF STORY: ");
        getchar();
        gets(name);
        strcpy(filename, name);
        strcat(filename, ".txt");
    }while(check_if_file_exists(filename) == 1);

    printf("ENTER ON WHICH DAY DID IT HAPPEN: ");
    scanf("%d", &day);
    printf("ENTER IN WHICH MONTH DID IT HAPPEN: ");
    scanf("%d", &month);
    printf("ENTER WHICH YEAR DID IT HAPPEN: ");
    scanf("%d", &year);

    make_capital(name);
    FILE *temp;
    int s = 0;
    temp = fopen("temp.txt", "w+");
    fflush(menu);
    fseek(menu, 0, SEEK_SET);
    while(fgets(text, 50, menu) != NULL){//HEADING_SYMBOLS + 4 + 5(' ', '@', '.') + 3 int ~ 50
            printf("\n%s\n", text);
            if(earlier_story_date(text, day, month, year) == 0 && s == 0){
                fprintf(temp, "%s @ %d.%d.%d\n", name, day, month, year);
                s = 1;
            }
            fprintf(temp, "%s", text);
        }//something stops the loop 
    if(s == 0){
        fprintf(temp, "%s @ %d.%d.%d\n", name, day, month, year);
    }
    fclose(menu);
    menu = fopen("menu.txt", "a");
    fseek(temp, 0, SEEK_SET);
    while(fgets(text, 50, temp) != NULL){
        fprintf(menu, "%s", text);
    }
    fclose(temp);

    FILE *story;
    story = fopen(filename,"w");
    printf("\nENTER THE STORY (FOR NEW LINES TYPE @)\n");
    printf("\n");
    getchar();
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
                fprintf(story, "%c", text[i]);
            }
        }
    }
    printf("\n");
    fclose(story);
}

void read_story(FILE* menu){
    char text[NUMBER_OF_SYMBOLS], name[HEADING_SUMBOLS], filename[HEADING_SUMBOLS + 4];
    FILE *read;
    printf("READ ALL(type \"read all\")\n");
    getchar();
    while(1){
        printf("\nENTER THE NAME OF THE ONE YOU WANT TO READ: ");
        gets(name);
        if(strcmp(name,"read all") == 0){
            printf("\n");
            fseek(menu, 0, SEEK_SET);
            while(fgets(filename, 50, menu) != NULL){//HEADING_SYMBOLS + 4 + 5(' ', '@', '.') + 3 int ~ 50
                extract_name(filename);
                printf("    %s\n", filename);
                strcat(filename, ".txt");
                read = fopen(filename, "r");
                while(fgets(text, NUMBER_OF_SYMBOLS, read)){
                    printf("%s\n", text);
                }
                printf("\n");
            }
            break;
        }
        else{
            strcpy(filename, name);
            strcat(filename, ".txt");
            if((read = fopen(filename, "r")) != NULL){
                printf("\n");
                make_capital(name);
                printf("    %s\n", name);

                while(fgets(text, NUMBER_OF_SYMBOLS, read)){
                    printf("%s\n", text);
                }
                printf("\n");
                break;
            }
            else{
                printf("ERROR WITH OPENING THE FILE\n");
            }
        }
    };
    fclose(read);
    char stay[4];
    printf("\nDO YOU WANT TO KEEP READING?: ");
    scanf("%s", stay);
    if(strcmp(stay,"yes") == 0){
        print_menu(menu);
        read_story(menu);
    }
}

int main(){
    FILE *menu = fopen("menu.txt","a");
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
            read_story(menu);
        }

    }while(choice != 3);
    fclose(menu);
    return 0;
}
