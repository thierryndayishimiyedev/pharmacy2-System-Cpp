#ifndef UI_H
#define UI_H

#include <stdbool.h>
#include <stddef.h>

void printHeader(const char *title);
void printSuccessFmt(const char *format, ...);
void printErrorFmt(const char *format, ...);
void printInfoFmt(const char *format, ...);
void pauseForUser(void);
void setActiveUserDisplay(const char *name);
bool isValidPersonName(const char *value);
bool isValidBusinessName(const char *value);
bool isValidUsername(const char *value);
bool isValidPassword(const char *value);
bool isValidPhone(const char *value);
bool isValidEmail(const char *value);
bool isValidText(const char *value, size_t maxLength);
bool isValidDate(const char *value);
bool isPositiveAmount(float value);
bool isNonNegativeNumber(int value);
bool isValidRole(const char *value);

#endif
