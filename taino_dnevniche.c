#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int line_count(FILE* ptr)
{
    int curr_line = 1;
    char ch;
    do
    {
        ch = fgetc(ptr);
        if (ch == '\n')
            curr_line++;
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
    FILE* ptr;
    ptr = fopen(filename, "r");

    int lineCount = line_count(ptr);
    printf("lineCount: %d\n", lineCount);

    int characters = character_count(ptr);

    int k = 0;
    char ch;

    char* new_str = malloc(sizeof(char) * (characters + 1));
    while (fgets(new_str, characters + 1, ptr) != NULL)
    {
        new_str[characters] = '\0';
        char* str_2 = malloc(sizeof(char) * (characters + 1));
        fgets(str_2, characters + 1, ptr);
        strcat(new_str, str_2);

        //printf("i: %d\n", i);
        printf("new_str: %s\n", new_str);
        printf("size_new_str: %d\n", strlen(new_str));

        for (int j = strlen(key) - 1; k < strlen(new_str); k++, j--)
        {
            if (j < 0)
            {
                j = strlen(key) - 1;
            }
            new_str[k] += key[j];
            new_str[k] += k;
        }
        fclose(ptr);

        printf("new_str - encrypt: %s\n", new_str);

        ptr = fopen(filename, "a");
        fprintf(ptr, "%s", new_str);
        fclose(ptr);
    }

}

char* decrypt(char* encrypted_str, char* key)
{
    char* decrypted_str = malloc(sizeof(char) * (strlen(encrypted_str) + 1));
    strcpy(decrypted_str, encrypted_str);
    decrypted_str[strlen(encrypted_str)] = '\0';

    int i = 0;
    int j_rows = 0;
    for (int j = strlen(key) - 1; i < strlen(encrypted_str); i++, j--)
    {
        if (j < 0)
        {
            j = strlen(key) - 1;
        }
        decrypted_str[i] -= key[j];
    }
    return decrypted_str;
}

int main()
{
    char dnevnik[10000];
    char password[20] = "obicham_kotki";
    char filename[] = "example.txt";
    encrypt(password, filename);
    //printf("encrypted <%s>\n", encrypted);

    //char* decrypted = decrypt(encrypted, password);
    //printf("decrypted <%s>\n", decrypted);

    //free(decrypted);

    return 0;
}
