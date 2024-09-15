#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isValidUsername(char username_to_check[100]);
int isValidPassword(char username[100], char password[25]);

int main()
{
    char username[100];
    printf("Enter username: ");
    scanf("%[^\n]%*c", username);
    while (isValidUsername(username) == 0)
    {
        printf("Username does not exist.\n");
        printf("Enter username: ");
        memset(username, 0, sizeof(username));
        scanf("%[^\n]%*c", username);
    }
    printf("Enter password: ");
    char password[25];
    scanf("%[^\n]%*c", password);
    int ctr = 0;
    while (isValidPassword(username, password) == 0)
    {
        ctr++;
        if (ctr == 3)
        {
            printf("Wrong password entered 3 times. Application exiting...");
            exit(0);
        }
        memset(password, 0, sizeof(password));
        printf("Wrong password! Enter password again: ");
        scanf("%[^\n]%*c", password);
    }
    return 0;
}

int isValidUsername(char username_to_check[100])
{
    FILE *f = fopen("masterfile.txt", "r");
    if (f == NULL)
    {
        printf("\nERROR: Not able to open master file.\n");
        exit(1);
    }
    char username[100];
    char date_of_birth[11];
    char password_file[11];
    fscanf(f, "%*[^\n]\n");
    while (fscanf(f, "%s %s %s", username, date_of_birth, password_file) == 3)
    {
        if (strcmp(username_to_check, username) == 0)
        {
            fclose(f);
            return 1;
        }
    }
    return 0;
}

int isValidPassword(char username_to_check[100], char password_to_check[25])
{
    FILE *f = fopen("masterfile.txt", "r");
    if (f == NULL)
    {
        printf("\nERROR: Not able to open master file.\n");
        exit(1);
    }
    char username[100];
    char date_of_birth[11];
    char password_file[11];
    fscanf(f, "%*[^\n]\n");
    while (fscanf(f, "%s %s %s", username, date_of_birth, password_file) == 3)
    {
        if (strcmp(username_to_check, username) == 0)
        {
            fclose(f);
            FILE *file_pass = fopen(password_file, "r");
            if (file_pass == NULL)
            {
                printf("ERROR: Not able to open password file.");
                exit(1);
            }
            char curr_password[25];
            fscanf(file_pass, "%[^\n]", curr_password);

            // To check on Ubuntu
            curr_password[strcspn(curr_password, "\r")] = '\0';

            fclose(file_pass);
            if (strcmp(password_to_check, curr_password) != 0)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
    }
    printf("ERROR: Username does not exist?");
    return 0;
}