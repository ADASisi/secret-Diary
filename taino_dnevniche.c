#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char* hash_pass(const char* str) {
    int h = 0;
    for (int i = 0; i < strlen(str); i++) {
        h += (i + 1) * str[i] * str[i];
    }
    char* pass = (char*)malloc(sizeof(char) * 12);
    snprintf(pass, 12, "%d", h);
    return pass;
}

int line_count(FILE* ptr) {
    int curr_line = 1;
    char ch;
    do {
        ch = fgetc(ptr);
        if (ch == '\n') {
            curr_line++;
        }
    } while (ch != EOF);
    fseek(ptr, 0, SEEK_SET);
    return curr_line;
}

int character_count(FILE* ptr) {
    int characters = 0;
    char ch;
    while ((ch = fgetc(ptr)) != EOF) {
        characters++;
    }
    fseek(ptr, 0, SEEK_SET);
    return characters;
}

void encrypt(const char* key, const char* filename) {
    FILE* ptr;
    ptr = fopen(filename, "r");

    int characters = character_count(ptr);
    printf("characters: %d\n", characters);

    char* new_str = (char*)malloc(sizeof(char) * (characters + 1));

    for (int i = 0; i < characters; i++) {
        char ch;
        ch = fgetc(ptr);
        new_str[i] = ch;
    }
    new_str[characters] = '\0';

    printf("new_str: %s\n", new_str);

    int rows = 0;
    bool prime = false;
    if (key[rows] % 2 == 0) {
        prime = true;
    }
    rows++;

    for (int k = 0, j = strlen(key) - 1; k < characters; k++, j--) {
        if (new_str[k] == '\n') {
            rows++;
            if (key[rows] % 2 == 0) {
                prime = true;
            } else {
                prime = false;
            }
            
        }

        if (j < 0) {
            j = strlen(key) - 1;
        }
        if (prime == true) {
            if (new_str[k] + key[j] > 31 && new_str[k] + key[j] < 127) {
                new_str[k] += key[j];
            }
            else if (new_str[k] + j > 31 && new_str[k] + j < 127) {
                new_str[k] += j;
            }
            /*else
                new_str[k] += 1;*/
        }
        if (prime == false) {
            if (new_str[k] - key[j] > 31 && new_str[k] - key[j] < 127) {
                new_str[k] -= key[j];
            }
            else if (new_str[k] + j > 31 && new_str[k] + j < 127) {
                new_str[k] += j;
            }
            /*else
                new_str[k] -= 1;*/
        }
    }
    fclose(ptr);
    ptr = fopen(filename, "w");
    printf("%s", new_str);
    fprintf(ptr, "%s", new_str);
    fclose(ptr);
    free(new_str);
}

void decrypt(const char* filename, const char* key) {
    FILE* ptr;
    ptr = fopen(filename, "r");

    int characters = character_count(ptr);

    char* decrypted_str = (char*)malloc(sizeof(char) * (characters + 1));

    for (int i = 0; i < characters; i++) {
        char ch;
        ch = fgetc(ptr);
        decrypted_str[i] = ch;
    }
    decrypted_str[characters] = '\0';

    int rows = 0;
    bool prime = false;
    if (key[rows] % 2 == 0) {
        prime = true;
    }
    rows++;
    for (int j = strlen(key) - 1, i = 0; i < characters; i++, j--) {
        
        if (decrypted_str[i] == '\n') {
            rows++;
            if (key[rows] % 2 == 0) {
                prime = true;
            } else {
                prime = false;
            }
            i++;
            j--;
        }
        if (j < 0) {
            j = strlen(key) - 1;
        }
        if (prime == true) {
            if (decrypted_str[i] - key[j] > 31 && decrypted_str[i] - key[j] < 127) {
                decrypted_str[i] -= key[j];
            }
            else if (decrypted_str[i] - j > 31 && decrypted_str[i] - j < 127) {
                decrypted_str[i] -= j;
            }
            /*else 
                decrypted_str[i] -= 1;*/
        }
        if (prime == false) {
            if (decrypted_str[i] + key[j] > 31 && decrypted_str[i] + key[j] < 127) {
                decrypted_str[i] += key[j];
            }
            else if (decrypted_str[i] - j > 31 && decrypted_str[i] - j < 127) {
                decrypted_str[i] -= j;
            }
            /*else
                decrypted_str[i] += 1;*/
        }
        
    }
    
    printf("\n \n ohhh: %s\n", decrypted_str);
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