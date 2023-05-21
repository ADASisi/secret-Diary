#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <windows.h>

char* encrypt(char* source, char* key, char* filename){
    FILE *ptr;
    ptr = fopen(filename,"r");
    char* new_str = malloc(sizeof(char) * (strlen(source) + 1));
	strcpy(new_str, source);
	new_str[strlen(source)] = '\0';
    int i = 0;
    int j_rows = 0;
    for(int j = strlen(key) - 1; i < strlen(source); i++, j--)
    {
        if(j < 0)
        {
            j = strlen(key) - 1;
        }
        new_str[i] += key[j];// + key[j] + 1;
        
        /*if(new_str[i] == '\n')
        {
            if(key[j_rows] % 2 != 0)
            {

            }
        }*/
    }
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
    fgets(dnevnik, 10000, stdin);
    char filename[] = "example.txt";
    char command[50];
    sprintf(command, "notepad.exe %s", filename);
    system(command);
    char* encrypted  = encrypt(dnevnik, password, filename);
    printf("origunal <%s>, encrypted <%s>\n", dnevnik, encrypted);
    char* decrypted = decrypt(encrypted, password);
    printf("encrypted <%s>, decrypted <%s>\n", encrypted, decrypted);
    
    return 0;
}
