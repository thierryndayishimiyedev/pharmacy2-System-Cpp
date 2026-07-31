#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "ui.h"

static char activeUserDisplay[128] = "Guest";

void setActiveUserDisplay(const char *name)
{
    if(name == NULL || name[0] == '\0')
        snprintf(activeUserDisplay, sizeof(activeUserDisplay), "Guest");
    else
        snprintf(activeUserDisplay, sizeof(activeUserDisplay), "%s", name);
}

void printHeader(const char *title)
{
    printf("\n+----------------------------------------------------------+\n");
    printf("| MOTHERLAND PHARMACY                                     |\n");
    printf("| %-56.56s |\n", title);
    printf("| Signed in: %-45.45s |\n", activeUserDisplay);
    printf("+----------------------------------------------------------+\n");
}

void printSuccessFmt(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    printf("\n[ SUCCESS ] ");
    vprintf(format, args);
    printf("\n");
    va_end(args);
}

void printErrorFmt(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    printf("\n[  ERROR  ] ");
    vprintf(format, args);
    printf("\n");
    va_end(args);
}

void printInfoFmt(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    printf("\n[   INFO  ] ");
    vprintf(format, args);
    printf("\n");
    va_end(args);
}

void pauseForUser(void)
{
    printf("\nPress Enter to continue...");
    fflush(stdout);
    while (getchar() != '\n');
}

static bool hasUnsafeSqlCharacter(const char *value)
{
    return strchr(value, '\'') != NULL || strchr(value, '"') != NULL || strchr(value, '`') != NULL;
}

bool isValidText(const char *value, size_t maxLength)
{
    size_t len;
    if(value == NULL) return false;
    len = strlen(value);
    return len > 0 && len <= maxLength && !hasUnsafeSqlCharacter(value);
}

bool isValidPersonName(const char *value)
{
    size_t i, len = value ? strlen(value) : 0;
    if(len < 2 || len > 100 || hasUnsafeSqlCharacter(value)) return false;
    for(i = 0; i < len; i++)
        if(!isalpha((unsigned char)value[i]) && value[i] != ' ' && value[i] != '-' && value[i] != '.') return false;
    return true;
}

bool isValidBusinessName(const char *value)
{
    size_t i, len = value ? strlen(value) : 0;
    if(len < 2 || len > 100 || hasUnsafeSqlCharacter(value)) return false;
    for(i = 0; i < len; i++)
        if(!isalnum((unsigned char)value[i]) && value[i] != ' ' && value[i] != '-' && value[i] != '.' && value[i] != '&' && value[i] != ',') return false;
    return true;
}

bool isValidUsername(const char *value)
{
    size_t i, len = value ? strlen(value) : 0;
    if(len < 3 || len > 30 || !isalnum((unsigned char)value[0])) return false;
    for(i = 0; i < len; i++) if(!isalnum((unsigned char)value[i]) && value[i] != '_') return false;
    return true;
}

bool isValidPassword(const char *value)
{
    size_t i, len = value ? strlen(value) : 0;
    if(len < 4 || len > 64 || hasUnsafeSqlCharacter(value)) return false;
    for(i = 0; i < len; i++) if(isspace((unsigned char)value[i])) return false;
    return true;
}

bool isValidPhone(const char *value)
{
    size_t i, len = value ? strlen(value) : 0;
    int digits = 0;
    if(len < 7 || len > 20) return false;
    for(i = 0; i < len; i++)
    {
        if(isdigit((unsigned char)value[i])) digits++;
        else if(value[i] != '+' && value[i] != '-' && value[i] != ' ' && value[i] != '(' && value[i] != ')') return false;
    }
    return digits >= 7;
}

bool isValidEmail(const char *value)
{
    const char *at;
    if(!isValidText(value, 100) || strchr(value, ' ') != NULL) return false;
    at = strchr(value, '@');
    return at != NULL && at != value && strchr(at + 1, '@') == NULL && strchr(at + 1, '.') != NULL;
}

bool isValidDate(const char *value)
{
    int year, month, day, maxDay;
    if(value == NULL || strlen(value) != 10 || sscanf(value, "%4d-%2d-%2d", &year, &month, &day) != 3) return false;
    if(value[4] != '-' || value[7] != '-' || year < 1900 || month < 1 || month > 12) return false;
    maxDay = (month == 2) ? (((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) ? 29 : 28) : (month == 4 || month == 6 || month == 9 || month == 11 ? 30 : 31);
    return day >= 1 && day <= maxDay;
}

bool isPositiveAmount(float value) { return value > 0.0f; }
bool isNonNegativeNumber(int value) { return value >= 0; }
bool isValidRole(const char *value) { return strcmp(value, "Admin") == 0 || strcmp(value, "Pharmacist") == 0; }
