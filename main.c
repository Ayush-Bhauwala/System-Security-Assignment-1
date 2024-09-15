#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int isValidUsername(char username_to_check[100]);
int isCorrectPassword(char username[100], char password[25]);
void wait_counter(int seconds);
int checkValidPassword(char password[25], int attempt_ctr);

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
    while (isCorrectPassword(username, password) == 0)
    {
        ctr++;
        if (ctr == 3)
        {
            printf("Wrong password entered 3 times. Application exiting...\n");
            exit(0);
        }
        memset(password, 0, sizeof(password));
        printf("Wrong password! Enter password again: ");
        scanf("%[^\n]%*c", password);
    }

    int attempt_ctr = 1;
    int wait_time = 8;
    char new_password[25];
    while (attempt_ctr <= 4)
    {
        printf("Enter your new password (Attempt %d): ", attempt_ctr);
        scanf("%[^\n]%*c", new_password);
        if (checkValidPassword(new_password, attempt_ctr) == 1)
        {
            printf("Password changed successfully.\n");
            break;
        }
        else
        {
            if (attempt_ctr == 4)
            {
                printf("All 4 attempts failed. You need to try again later.\n");
                break;
            }
            wait_counter(wait_time);
            wait_time *= 2;
            attempt_ctr++;
        }
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

int isCorrectPassword(char username_to_check[100], char password_to_check[25])
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

void wait_counter(int seconds)
{
    while (seconds > 0)
    {
        printf("Wait for ");
        printf("%d seconds...\n", seconds);

        clock_t stop = clock() + CLOCKS_PER_SEC;
        while (clock() < stop)
        {
        }
        seconds--;
    }
}

int checkUpperCaseLetter(char string[25])
{
    int i = 0;
    int check = 0;
    for (int i = 0; i < strlen(string); i++)
    {
        if (65 <= string[i] && 90 >= string[i])
        {
            check = 1;
            return check;
        }
    }
    return check;
}

int checkLowerCaseLetter(char string[25])
{
    int i = 0;
    int check = 0;
    for (int i = 0; i < strlen(string); i++)
    {
        if (97 <= string[i] && 122 >= string[i])
        {
            check = 1;
            return check;
        }
    }
    return check;
}

int checkValidPassword(char password[25], int attempt_ctr)
{
    int is_valid_password = 1;
    if (strlen(password) < 12)
    {
        is_valid_password = 0;
        printf("Attempt %d failed.\n", attempt_ctr);
        printf("Password does not contain a minimum of 12 characters.\n");
    }
    if (checkUpperCaseLetter(password) == 0)
    {
        if (is_valid_password != 0)
        {
            is_valid_password = 0;
            printf("Attempt %d failed.\n", attempt_ctr);
        }
        printf("Password does not contain at least one uppercase letter.\n");
    }
    if (checkLowerCaseLetter(password) == 0)
    {
        if (is_valid_password != 0)
        {
            is_valid_password = 0;
            printf("Attempt %d failed.\n", attempt_ctr);
        }
        printf("Password does not contain at least one lowecase letter.\n");
    }
    return is_valid_password;
}