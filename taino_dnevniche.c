#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<windows.h>

int line_count(FILE *ptr)
{
    int line_count = 0;
    char ch;
    while((ch = fgetc(ptr)) != EOF)
    {
        if(ch == '\n')
        {
            line_count++;
        }
    }
    return line_count;
}

int character_count(FILE *ptr)
{
    int characters = 0;
    char ch;
    while ((ch = fgetc(ptr)) != EOF)
    {
        characters++;
    }
    return characters;    
}

char* encrypt(char* key, char* filename){
    FILE *ptr;
    ptr = fopen(filename,"r");
    int characters = character_count(ptr);
    char* new_str = malloc(sizeof(char) * (characters + 1));
	new_str[characters] = '\0';
    rewind(ptr);
    int lineCount = line_count(ptr);
    int k = 0;
    for(int i = 0; i < lineCount; i++)
    {
        fgets(new_str, sizeof(characters + 1), ptr);
        for (int j = strlen(key) - 1; i < strlen(new_str); k++, j--)
        {
            if(j < 0)
            {
                j = strlen(key) - 1;
            }
            new_str[k] += key[j];
        }
    }
    fclose(ptr);
    ptr = fopen(filename,"a");
    fprintf(ptr, "%s", new_str);
    fclose(ptr);
    return new_str;
}

char* decrypt(char* encrypted_str, char* key) {
    char* decrypted_str = malloc(sizeof(char) * (strlen(encrypted_str) + 1));
    strcpy(decrypted_str, encrypted_str);
    decrypted_str[strlen(encrypted_str)] = '\0';
    int i = 0;
    int j_rows = 0;
    for (int j = strlen(key) - 1; i < strlen(encrypted_str); i++, j--) {
        if (j < 0) {
            j = strlen(key) - 1;
        }
        decrypted_str[i] -= key[j];
        //decrypted_str[i] -= 1;
    }
    return decrypted_str;
}


int main()
{
    char dnevnik[10000];
    char password[20] = "1234";
    //fgets(dnevnik, 10000, stdin);
    char filename[] = "example.txt";
    char command[50];
    sprintf(command, "notepad.exe %s", filename);
    system(command);
    char* encrypted  = encrypt(password, filename);
    printf("encrypted <%s>\n", encrypted);
    //char* decrypted = decrypt(encrypted, password);
    //printf("encrypted <%s>, decrypted <%s>\n", encrypted, decrypted);
    
    return 0;
}
