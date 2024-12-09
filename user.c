#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

#define FILENAME "users.txt"

 User users[100];   // Déclaration du tableau d'utilisateurs
 int userCount =0;           // Déclaration du compteur d'utilisateurs

// Signup function
void signup() {
    User user;

    printf("Enter your full name: ");
    getchar();
    fgets(user.fullname, sizeof(user.fullname), stdin);
    user.fullname[strcspn(user.fullname, "\n")] = '\0';

    printf("Enter your age: ");
    scanf("%d", &user.age);

    printf("Enter your email: ");
    scanf("%s", user.email);

    if (!validateEmail(user.email)) {
        printf("Invalid email format. Signup failed.\n");
    }
    else if (strstr(user.email, "@admin.com") != NULL){
        printf("si vous etes un admin connectez vous directement ");
    }
    else{
        printf("Enter your password: ");
        scanf("%s", user.password);
        printf("Signup successful! You can now login.\n");
    }
    if (strstr(user.email, "@admin.com") != NULL){
        saveUserToFile(user);
    }


}


int login(User* loggedInUser) {
    char email[100], password[100];
    User user;
    FILE *file;


    printf("Enter your email: ");
    scanf("%s", email);

    printf("Enter your password: ");
    scanf("%s", password);
    file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return -2;
    }


        while (fscanf(file, "%99[^\n]\n%d\n%99[^\n]\n%99[^\n]\n",
                      user.fullname, &user.age, user.email, user.password) != EOF) {
            if (strcmp(email, user.email) == 0 && strcmp(password, user.password) == 0) {
                fclose(file);
                *loggedInUser = user;


                if (strstr(user.email, "@admin.com") != NULL) {
                    return 1;

                }
                else {
                    return 2;
                }
            }
        }

    fclose(file);
    return 0;
}


int validateEmail(const char* email) {
    const char* at = strchr(email, '@');
    const char* dot = strchr(email, '.');
    return at && dot && at < dot;
}


void saveUserToFile(User user) {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "%s\n%d\n%s\n%s\n", user.fullname, user.age, user.email, user.password);
    fclose(file);
}


void listUsers() {
    printf("ID | Fullname | Email | Role\n");
    for (int i = 0; i < userCount; i++) {
        printf("%d | %s | %s | %d\n", users[i].id, users[i].fullname, users[i].email, users[i].role);
    }
}


void loadUsers() {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        return;
    }

    while (fscanf(file, "%d\n%99[^\n]\n%99[^\n]\n%99[^\n]\n%d\n",
                  &users[userCount].id, users[userCount].fullname,
                  users[userCount].email, users[userCount].password,
                  &users[userCount].role) != EOF) {
        userCount++;
    }

    fclose(file);
}
