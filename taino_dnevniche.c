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

void encrypt(char* key, char* text)
{
    /*FILE* ptr;
    ptr = fopen(filename, "r");

    int characters = character_count(ptr);
    printf("characters: %d\n", characters);

    
    for (int i = 0; i < characters + 1; i++)
    {
        char ch;
        ch = fgetc(ptr);
        new_str[i] = ch;
    }*/
    int len_text = strlen(text);
    char* new_str = malloc(sizeof(char) * (strlen(text) + 1));
    strcmp(new_str, text);
    new_str[len_text] = '\0';

    printf("new_str: %s\n", new_str);

    int rows = 0;
    bool prime = false;
    if (key[rows] % 2 == 0)
    {
        prime = true;
    }
    rows++;

    for (int k = 0, j = strlen(key) - 1; k < len_text; k++, j--)
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
    /*fclose(ptr);
    ptr = fopen(filename, "w");
    fprintf(ptr, "%s", new_str);
    fclose(ptr);*/
    free(new_str);
}

void decrypt(char* filename, char* key)
{
    FILE* ptr;
    ptr = fopen(filename, "r");

    int characters = character_count(ptr);
    printf("characters: %d\n", characters);

    char* decrypted_str = malloc(sizeof(char) * (characters + 1));

    for (int i = 0; i < characters + 1; i++)
    {
        char ch;
        ch = fgetc(ptr);
        decrypted_str[i] = ch;
    }
    decrypted_str[characters] = '\0';
    printf("Decrypted string: %s\n", decrypted_str);
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

    printf("Decrypted string: %s\n", decrypted_str);
    fclose(ptr);
    free(decrypted_str);
}

int main()
{
    char password[20] = "obicham_kotki_1234";
    char filename[] = "example.txt";
    encrypt(password, filename);

    decrypt(filename, password);

    return 0;
}