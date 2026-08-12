#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <string>

#define LOG !(ov4::GLOBAL_DEBUG && ov4::DEBUG) ? (void)0 : ov4::LogVoidify() & std::clog

namespace ov4 {

void atomic_print(char *s);
void atomic_print(int x);
void reverse(char str[], int length);
char* itoa(int num, char* str, int base);

constexpr bool GLOBAL_DEBUG = true;
bool DEBUG = true; // verbose mode, can be enabled on demand using something like -v
struct LogVoidify { void operator&(std::ostream&) const {} };

const int DECIMAL = 10;
const int LENGTH = 20;

void atomic_print(char *s)
{
    write(STDOUT_FILENO, s, strlen(s));
}

void atomic_print(int x)
{
    char msg[LENGTH];
    itoa(x, msg, DECIMAL);
    atomic_print(msg);
}

void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}


char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitly, otherwise empty string is
     * printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled
    // only with base 10. Otherwise numbers are
    // considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}

}
