#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//https://github.com/ADASisi/secret-Diary.git

int line_count(FILE* ptr)
{
    int curr_line = 1;
    char ch;
    do
    {
        ch = fgetc(ptr);
        if (ch == '\n')
        {
            curr_line++;
        }
    }
    while (ch != EOF);
    fseek(ptr, 0, SEEK_SET);
    return curr_line;
}

int character_count(FILE* ptr)
{
    int characters = 0;
    char ch;
    while ((ch = fgetc(ptr)) != EOF)
    {
        characters++;
    }
    fseek(ptr, 0, SEEK_SET);
    return characters;
}

void encrypt(char* key, char* filename)
{
    FILE *ptr, *ptr_1, *ptr_2;
    ptr = fopen(filename, "r");

    int lineCount = line_count(ptr);
    //printf("lineCount: %d\n", lineCount);

    rewind(ptr);
    int characters = character_count(ptr);
    printf("characters: %d\n", characters);


    char* new_str = malloc(sizeof(char) * (characters + 1));
    for(int i = 0; i < characters + 1; i++)
    {
        char ch;
        ch = fgetc(ptr);
        new_str[i] = ch;
    }
    new_str[characters] = '\0';

    printf("new_str: %s\n", new_str);

    for (int k = 0, j = strlen(key) - 1; k < characters; k++, j--)
    {
        if (j < 0)
        {
            j = strlen(key) - 1;
        }
        new_str[k] += key[j];
        new_str[k] += k;
    }
    //ptr_1 = ptr;
    ptr = freopen(filename, "w+", stdout);
    fprintf(ptr, "%s", new_str);
    fclose(ptr);
}

void decrypt(char* filename, char* key)
{
    printf("maika tiiiiiiiii \n");
    FILE* ptr;
    ptr = fopen(filename, "r");

    int lineCount = line_count(ptr);
    printf("lineCount: %d\n", lineCount);

    //rewind(ptr);
    int characters = character_count(ptr);
    printf("characters: %d\n", characters);

    char* decrypted_str = malloc(sizeof(char) * (characters + 1));

    int i = 0;
    int j_rows = 0;
    //rewind(ptr);
    for(int i = 0; i < characters + 1; i++)
    {
        char ch;
        ch = fgetc(ptr);
        decrypted_str[i] = ch;
    }
    decrypted_str[characters] = '\0';
    printf("Before decrypting: %s\n", decrypted_str);
    for (int j = strlen(key) - 1, i = 0; i < characters; i++, j--)
    {
        if (j < 0)
        {
            j = strlen(key) - 1;
        }
        decrypted_str[i] -= key[j];
        decrypted_str[i] -= i;
    }

    printf("Decrypted string: %s\n", decrypted_str);
    fclose(ptr);
}

int main()
{
    char dnevnik[10000];
    char password[20] = "obicham_kotki";
    char filename[] = "example.txt";
    //encrypt(password, filename);

    //printf("encrypted <%s>\n", encrypted);

    decrypt(filename, password);
    //printf("encryp<%s>\n", new_str);

    //free(decrypted);

    return 0;
}
