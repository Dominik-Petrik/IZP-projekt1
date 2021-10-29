#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INVALID_NUMBER_OF_ARGS "Invalid number of arguments passed"
#define INVALID_ARGUMENT_PASSED "Argument number %d is invalid"
#define INVALID_PASSWORD_LENGTH "Password is over 100 long"

//password check functions
bool containsLowerUpperLetter(char string[102]);

bool charCharGroupsCount(char string[102], long param);

bool checkRepeatingChars(char string[102], long param);

bool checkIdenticalSubstring(char string[102], long param);

//helper functions
int getStringLength(char string[102]);

void getSubstring(char string[102], int start, int end, char out[]);

bool compareStrings(char string1[102], char string2[102]);

bool checkCharGroup(char c, char start, char end);

//statistics for keeping data about the passwords
typedef struct
{
    bool variousChars[255];
    int minLength;
    int totalLength;
    double passwordCount;
    double averageLength;
} Stats;

//main helper functions
void setup(Stats *pwStats);

bool validateArgc(int argc);

bool validateArgv(int argc, char *argv[]);

bool checkPasswordLength(char password[102]);

void updateCharArray(char password[]);

void updateStats(char password[], Stats *pwStats);

void checkPassword(char password[102], int level, long param);

void displayStats(Stats *pwStats);

//main
int main(int argc, char *argv[])
{
    Stats pwStats;
    setup(&pwStats);
    int level;
    long param;
    char password[102];

    if (!validateArgc(argc))
    {
        return -1;
    }

    if (validateArgv(argc, argv))
    {
        level = atoi(argv[1]);
        param = strtol(argv[2], NULL, 36);
    }
    else
    {
        return -1;
    }

    while (fgets(password, 102, stdin) != NULL)
    {

        if (!checkPasswordLength(password))
        {
            return -1;
        }
        else
        {
            updateStats(password, &pwStats);
            checkPassword(password, level, param);
        }
    }
    if (argc == 4 && compareStrings(argv[3], "--stats"))
    {
        displayStats(&pwStats);
    }

    return 0;
}

int getStringLength(char string[102])
{
    int count = 0;
    while (string[count] != '\0' && string[count] != '\n')
    {
        count++;
    }
    return count;
}

void getSubstring(char string[102], int start, int end, char out[102])
{
    for (int i = start; i <= end; i++)
    {
        out[i - start] = string[i];
    }
    out[end - start + 1] = '\0';
}

bool compareStrings(char string1[], char string2[])
{
    if (getStringLength(string1) != getStringLength(string2))
    {
        return false;
    }
    else
    {
        for (int i = 0; i < getStringLength(string1); i++)
        {
            if (string1[i] != string2[i])
            {
                return false;
            }
        }
        return true;
    }
}

bool containsLowerUpperLetter(char string[102])
{
    bool upperFlag = false;
    bool lowerFlag = false;

    for (int i = 0; string[i] != '\0' && string[i] != '\n'; i++)
    {
        if (!(upperFlag && lowerFlag))
        {
            if (string[i] >= 'a' && string[i] <= 'z')
            {
                lowerFlag = true;
            }
            else if (string[i] >= 'A' && string[i] <= 'Z')
            {
                upperFlag = true;
            }
        }
    }

    return (upperFlag && lowerFlag);
}

bool checkCharGroup(char c, char start, char end)
{
    if (c >= start && c <= end)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* helper function that returns true if string
contains char with ascii value between start and end */
bool checkStringCharGroup(char string[102], char start, char end)
{
    for (int i = 0; string[i] != '\0' && string[i] != '\n'; i++)
    {
        if (checkCharGroup(string[i], start, end))
        {
            return true;
        }
    }
    return false;
}

bool checkStringNonAlphaNumCharGroup(char string[102])
{
    {

        for (int i = 0; string[i] != '\0' && string[i] != '\n'; i++)
        {
            if ((checkCharGroup(string[i], ' ', '/') || checkCharGroup(string[i], ':', '@') || checkCharGroup(string[i], '[', '`') || checkCharGroup(string[i], '{', '~')))
            {
                return true;
            }
        }
        return false;
    }
}

bool checkCharGroupsCount(char string[102], long param)
{
    int charGroupCount = checkStringCharGroup(string, 'a', 'z') + checkStringCharGroup(string, 'A', 'Z') + checkStringCharGroup(string, '0', '9') + checkStringNonAlphaNumCharGroup(string);

    return (charGroupCount >= param || charGroupCount == 4);
}

bool checkRepeatingChars(char string[102], long param)
{
    if (param == 1)
    {
        return false;
    }

    for (int i = 0; i < getStringLength(string) - (param - 1); i++)
    {
        char checkedChar = string[i];

        for (int j = 1; j <= param - 1; j++)
        {
            if (checkedChar != string[i + j])
            {
                break;
            }
            else if (j == param - 1)
            {
                return false;
            }
        }
    }
    return true;
}

bool checkIdenticalSubstring(char string[102], long param)
{
    for (int i = 0; i <= getStringLength(string) - param - 1; i++)
    {
        char string1[102];
        getSubstring(string, i, i + param - 1, string1);
        for (int j = i + 1; j <= getStringLength(string) - param; j++)
        {
            char string2[102];
            getSubstring(string, j, j + param - 1, string2);

            if (compareStrings(string1, string2))
            {
                return false;
            }
        }
    }
    return true;
}

void setup(Stats *pwStats)
{
    pwStats->minLength = 0;
    pwStats->totalLength = 0;
    pwStats->passwordCount = 0.00;
    pwStats->averageLength = 0.00;
}

bool validateArgc(int argc)
{
    if (argc < 3 || argc > 4)
    {
        fprintf(stderr, INVALID_NUMBER_OF_ARGS);
        return false;
    }
    return true;
}

bool validateArgv(int argc, char *argv[])
{
    if (atoi(argv[1]) < 1 || atoi(argv[1]) > 4)
    {
        fprintf(stderr, INVALID_ARGUMENT_PASSED, 1);
        return false;
    }
    else if (strtol(argv[2], NULL, 36) < 1)
    {
        fprintf(stderr, INVALID_ARGUMENT_PASSED, 2);
        return false;
    }
    else if (argc == 4 && !compareStrings(argv[3], "--stats"))
    {
        fprintf(stderr, INVALID_ARGUMENT_PASSED, 3);
        return false;
    }
    return true;
}

bool checkPasswordLength(char password[102])
{
    int passLength = getStringLength(password);
    if (passLength > 100)
    {
        fprintf(stderr, INVALID_PASSWORD_LENGTH);
        return false;
    }
    return true;
}

void updateVariousChars(char password[102], bool variousChars[255])
{

    for (int i = 0; i < getStringLength(password); i++)
    {
        variousChars[(int)password[i]] = true;
    }
}

void updateStats(char password[102], Stats *pwStats)
{
    int passLength = getStringLength(password);
    pwStats->totalLength += passLength;
    pwStats->passwordCount++;
    if (passLength < pwStats->minLength || pwStats->passwordCount == 0)
    {
        pwStats->minLength = passLength;
    }

    updateVariousChars(password, pwStats->variousChars);
}

void checkPassword(char password[102], int level, long param)
{
    bool results[4] = {containsLowerUpperLetter(password), checkCharGroupsCount(password, param), checkRepeatingChars(password, param), checkIdenticalSubstring(password, param)};

    for (int i = 0; i < level; i++)
    {
        if (!results[i])
        {
            return;
        }
    }
    printf("%s", password);
}

void displayStats(Stats *pwStats)
{
    int charCount = 0;
    for (int i = 0; i < 255; i++)
    {
        if (pwStats->variousChars[i])
        {
            charCount++;
        }
    }
    printf("charCount: %d", charCount);

    double averageLength = 0.00;
    if (pwStats->passwordCount != 0)
    {
        averageLength = pwStats->totalLength / pwStats->passwordCount;
    }

    printf("Statistika:\n");
    printf("Ruznych znaku: %d\n", charCount);
    printf("Minimalni delka: %d\n", pwStats->minLength);
    printf("Prumerna delka: %.2f", averageLength);
}