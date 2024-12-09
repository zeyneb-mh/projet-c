#ifndef USER_H
#define USER_H
#define MAX_USERS 100

typedef struct {
    int id;
    char fullname[100];
    int age;
    char email[100];
    char password[100];
    int role;
} User;
extern User users[MAX_USERS];
extern int userCount;

void signup();
int login(User* loggedInUser);
int validateEmail(const char* email);
void saveUserToFile(User user);
void loadUsers();
void listUsers();

#endif
