#ifndef FILE_ENCRYPTION.H
#define FILE_ENCRYPTION.H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void encrypt(char* key, char* text)
{
    int len_text = strlen(text);
    char* new_str = (char*) malloc(sizeof(char) * (strlen(text) + 1));
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
        }
        if (prime == false)
        {
            new_str[k] -= key[j];
            //new_str[k] += k;
        }
    }
    free(new_str);
}

void decrypt(char* key, char* text)
{
    int len_str = strlen(text);
    char* decrypted_str = (char*) malloc(sizeof(char) * (len_str + 1));
    strcmp(decrypted_str, text);
    decrypted_str[len_str] = '\0';
    printf("Crypted string: %s\n", decrypted_str);
    int rows = 0;
    bool prime = false;
    if (key[rows] % 2 == 0)
    {
        prime = true;
    }
    rows++;
    for (int j = strlen(key) - 1, i = 0; i < len_str; i++, j--)
    {
        if (j < 0)
        {
            j = strlen(key) - 1;
        }
        if (prime == true)
        {
            decrypted_str[i] -= key[j];
        }
        if (prime == false)
        {
            decrypted_str[i] += key[j];
            //decrypted_str[i] -= i;
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
    free(decrypted_str);
}

#endif