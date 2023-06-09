#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#define MAX_USERS 10
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_FILENAME_LENGTH 100

struct User {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int ID;
};

struct User users[MAX_USERS];
int numUsers = 0;

void addUser(const char *username, const char *password, int ID) {
    if (numUsers < MAX_USERS) {
        strcpy(users[numUsers].username, username);
        strcpy(users[numUsers].password, password);
        users[numUsers].ID = ID;
        numUsers++;
    }
}

int findUser(const char *username) {
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

int authenticateUser(const char *username, const char *password) {
    int userIndex = findUser(username);
    if (userIndex != -1 && strcmp(users[userIndex].password, password) == 0) {
        return users[userIndex].ID;
    }
    return -1;
}

void listUserFiles(const char *username) {
    char dirname[MAX_USERNAME_LENGTH + 1];
    strcpy(dirname, username);

    DIR *dir = opendir(dirname);
    if (dir != NULL) {
        printf("Списък на файловете в папката '%s':\n", dirname);

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG) {
                printf("%s\n", entry->d_name);
            }
        }

        closedir(dir);
    } else {
        printf("Грешка при отваряне на папката: %s\n", dirname);
    }
}

void openUserFile(const char *username) {
    char dirname[MAX_USERNAME_LENGTH + 1];
    strcpy(dirname, username);

    DIR *dir = opendir(dirname);
    if (dir != NULL) {
        printf("Списък на файловете в папката '%s':\n", dirname);

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG) {
                printf("%s\n", entry->d_name);
            }
        }

        closedir(dir);

        char filename[MAX_FILENAME_LENGTH];
        printf("Моля, въведете името на файла, който искате да отворите: ");
        scanf("%s", filename);

        char filepath[MAX_USERNAME_LENGTH + 1 + MAX_FILENAME_LENGTH];
        sprintf(filepath, "%s/%s", dirname, filename);

        FILE *file = fopen(filepath, "r");
        if (file != NULL) {
            printf("Отварям файл: %s\n", filepath);
            fclose(file);
        } else {
            printf("Грешка при отваряне на файла: %s\n", filepath);
        }
    } else {
        printf("Грешка при отваряне на папката: %s\n", dirname);
    }
}

int main() {
    // Добавяне на няколко примерни потребители
    addUser("user1", "pass1", 1);
    addUser("user2", "pass2", 2);
    addUser("user3", "pass3", 3);

    char inputUsername[MAX_USERNAME_LENGTH];
    char inputPassword[MAX_PASSWORD_LENGTH];

    printf("Моля, въведете потребителско име: ");
    scanf("%s", inputUsername);

    printf("Моля, въведете парола: ");
    scanf("%s", inputPassword);

    int userID = authenticateUser(inputUsername, inputPassword);
    if (userID != -1) {
        printf("Успешно влизане! Потребителски идентификатор: %d\n", userID);
        openUserFile(inputUsername);
    } else {
        printf("Невалидно потребителско име или парола.\n");
    }

    return 0;
}