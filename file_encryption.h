#ifndef FILE_ENCRYPTION.H
#define FILE_ENCRYPTION.H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*int hash(char* str) {
    int hash = 0;
    for(int i = 1; i < strlen(str); i++) {
        hash += i * str[i] * str[i];
    }
    return hash;
}*/

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
    } while (ch != EOF);
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

    int characters = character_count(ptr);
    //printf("characters: %d\n", characters);

    char* new_str = (char *) malloc(sizeof(char) * (characters + 1));

    for (int i = 0; i < characters + 1; i++)
    {
        char ch;
        ch = fgetc(ptr);
        new_str[i] = ch;
    }
    //int len_text = strlen(text);
    //strcmp(new_str, text);
    new_str[characters] = '\0';

    //printf("new_str: %s\n", new_str);

    int rows = 0;
    bool prime = false;
    if (key[rows] % 2 == 0)
    {
        prime = true;
    }
    rows++;

    for (int k = 0, j = strlen(key) - 1; k < characters; k++, j--)
    {
        if (new_str[k] == '\n')
        {
            rows++;
            if (key[rows] % 2 == 0)
            {
                prime = true;
            }
            else
            {
                prime = false;
            }
        }

        if (j < 0)
        {
            j = strlen(key) - 1;
        }
        if (prime == true)
        {
            new_str[k] += key[j];
            //new_str[k] += k;
        }
        if (prime == false)
        {
            new_str[k] -= key[j];
            new_str[k] += k;
        }
    }
    fclose(ptr);
    ptr = fopen(filename, "w");
    //printf("%s", new_str);
    fprintf(ptr, "%s", new_str);
    fclose(ptr);
    free(new_str);
}

void decrypt(char* filename, char* key)
{
    FILE* ptr;
    ptr = fopen(filename, "r");

    int characters = character_count(ptr);
    //printf("characters: %d\n", characters);

    char* decrypted_str = (char *) malloc(sizeof(char) * (characters + 1));

    for (int i = 0; i < characters + 1; i++)
    {
        char ch;
        ch = fgetc(ptr);
        decrypted_str[i] = ch;
    }
    decrypted_str[characters] = '\0';
    //printf("Decrypted string: %s\n", decrypted_str);
    int rows = 0;
    bool prime = false;
    if (key[rows] % 2 == 0)
    {
        prime = true;
    }
    rows++;
    for (int j = strlen(key) - 1, i = 0; i < characters; i++, j--)
    {
        if (j < 0)
        {
            j = strlen(key) - 1;
        }
        if (prime == true)
        {
            decrypted_str[i] -= key[j];
            //decrypted_str[i] -= i;
        }
        if (prime == false)
        {
            decrypted_str[i] += key[j];
            decrypted_str[i] -= i;
        }
        if (decrypted_str[i] == '\n')
        {
            rows++;
            if (key[rows] % 2 == 0)
            {
                prime = true;
            }
            else
            {
                prime = false;
            }
        }


    }

    printf("%s\n", decrypted_str);
    fclose(ptr);
    free(decrypted_str);
}
#endif