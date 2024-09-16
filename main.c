#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

int isValidUsername(char username_to_check[100]);
int isCorrectPassword(char username[100], char password[25]);
void wait_counter(int seconds);
int checkValidPassword(char password[25], int attempt_ctr, char username[100]);
void updatePasswordFile(char password[25], char username[100]);

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
        if (checkValidPassword(new_password, attempt_ctr, username) == 1)
        {
            printf("\nPassword changed successfully.\n");
            updatePasswordFile(new_password, username);
            break;
        }
        else
        {
            printf("\n");
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
        printf("Wait for %d seconds...\r", seconds);
        fflush(stdout);
        clock_t stop = clock() + CLOCKS_PER_SEC;
        while (clock() < stop)
        {
        }
        seconds--;
    }
}

void updatePasswordFile(char new_password[25], char username_to_update[100])
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
        if (strcmp(username_to_update, username) == 0)
        {
            fclose(f);
            FILE *file_pass = fopen(password_file, "r");
            if (file_pass == NULL)
            {
                printf("ERROR: Not able to open password file.");
                exit(1);
            }

            fseek(file_pass, 0, SEEK_END);
            long fileSize = ftell(file_pass);
            rewind(file_pass);
            char *passwords = (char *)malloc(fileSize + 1);
            if (passwords == NULL)
            {
                printf("Error: Memory allocation failed.\n");
                fclose(file_pass);
                exit(1);
            }
            size_t bytesRead = fread(passwords, 1, fileSize, file_pass);
            passwords[bytesRead] = '\0';
            fclose(file_pass);

            file_pass = fopen(password_file, "w");
            fprintf(file_pass, "%s\n", new_password);

            int pass_count = 1;
            char old_password[25];
            int index = 0;
            for (int i = 0; i < strlen(passwords); i++)
            {
                if (pass_count >= 10)
                    break;

                if (passwords[i] == '\r' || passwords[i] == '\n')
                {
                    old_password[index] = '\0';
                    if (pass_count == 9)
                        fprintf(file_pass, "%s", old_password);
                    else
                        fprintf(file_pass, "%s\n", old_password);
                    memset(old_password, 0, sizeof(old_password));
                    pass_count++;
                    index = 0;
                }
                else
                {
                    old_password[index] = passwords[i];
                    index++;
                }
            }
            if (pass_count < 10)
            {
                old_password[index] = '\0';
                fprintf(file_pass, "%s", old_password);
            }

            fclose(file_pass);
        }
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

int hasSpecialCharacter(char string[25])
{
    int hasSpecial = 0;
    char special_chars[11] = {'.', '@', '!', '#', '$', '%', '^', '&', '*', '-', '_'};
    for (int i = 0; i < strlen(string); i++)
    {
        int checkSpecial = 0;
        for (int j = 0; j < 11; j++)
        {
            if (string[i] == special_chars[j])
            {
                hasSpecial = 1;
                checkSpecial = 1;
            }
        }
        if (!isalnum(string[i]) && checkSpecial == 0)
        {
            return 0;
        }
    }
    return hasSpecial;
}

int checkNameInPassword(char password[25], char name[50])
{
    int name_len = (int)strlen(name);
    for (int i = 0; i <= ((int)strlen(password) - name_len); i++)
    {
        int check = 1;
        for (int j = 0; j < name_len; j++)
        {
            if (tolower((unsigned char)password[i + j]) != tolower((unsigned char)name[j]))
            {
                check = 0;
                break;
            }
        }
        if (check == 1)
            return 1;
    }
    return 0;
}

int hasNameinPassword(char password[25], char username[100])
{
    char first_name[50];
    char sur_name[50];
    int first = 1;
    int index_period = 0;
    int i = 0;
    for (; i < strlen(username); i++)
    {
        if (username[i] != '.')
        {
            if (first)
            {
                first_name[i] = username[i];
            }
            else
            {
                sur_name[i - index_period - 1] = username[i];
            }
        }
        else
        {
            first_name[i] = '\0';
            index_period = i;
            first = 0;
        }
    }
    sur_name[i - index_period - 1] = username[i];

    int has_first_name = checkNameInPassword(password, first_name);
    int has_sur_name = checkNameInPassword(password, sur_name);

    if (has_first_name == 0 && has_sur_name == 0)
    {
        return 0;
    }
    if (has_first_name == 0 && has_sur_name == 1)
    {
        return 1;
    }
    if (has_first_name == 1 && has_sur_name == 0)
    {
        return 2;
    }
    else
    {
        return 3;
    }
}

int checkValidPassword(char password[25], int attempt_ctr, char username[100])
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

    // TODO R4

    if (hasSpecialCharacter(password) == 0)
    {
        if (is_valid_password != 0)
        {
            is_valid_password = 0;
            printf("Attempt %d failed.\n", attempt_ctr);
        }
        printf("Password does not contain at least one of the allowed special characters.\n");
    }

    // TODO R6

    int name_in_password = hasNameinPassword(password, username);
    if (name_in_password != 0)
    {
        if (is_valid_password != 0)
        {
            is_valid_password = 0;
            printf("Attempt %d failed.\n", attempt_ctr);
        }
        switch (name_in_password)
        {
        case 1: // only surname present in password
            printf("Password contains surname portion of username.\n");
            break;
        case 2: // only first name present in password
            printf("Password contains name portion of username.\n");
            break;
        case 3: // both first name and second name present in password
            printf("Password  contains  name  and  surname  portions  of  username.\n");
            break;
        }
    }

    // TODO R8

    return is_valid_password;
}