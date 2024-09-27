#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef PLAYERCTL
#define PLAYERCTL

// Opens new pipe and executes command
char* get_playerctl_info(const char* command) {
    FILE *fp;
    char buffer[128];
    char *result = NULL;
    size_t result_len = 0;

    // Open pipe
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("popen() failed");
        return NULL;
    }

    // Read command output
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t buffer_len = strlen(buffer);
        char *new_result = realloc(result, result_len + buffer_len + 1);
        if (new_result == NULL) {
            perror("realloc() failed");
            free(result);
            pclose(fp);
            return NULL;
        }
        result = new_result;
        strcpy(result + result_len, buffer);
        result_len += buffer_len;
    }

    // Close pipe
    pclose(fp);

    if (result != NULL && result[result_len - 1] == '\n') {
        result[result_len - 1] = '\0';
    }

    return result;
}

// Decodes unicode chars
char* url_decode(const char *str) {
    char *decoded = malloc(strlen(str) + 1);
    char *pstr = (char *)str, *pbuf = decoded;
    while (*pstr) {
        if (*pstr == '%' && isxdigit(*(pstr + 1)) && isxdigit(*(pstr + 2))) {

            char hex[3] = { *(pstr + 1), *(pstr + 2), '\0' };
            *pbuf++ = (char)strtol(hex, NULL, 16);
            pstr += 3;
        } else {
            *pbuf++ = *pstr++;
        }
    }
    *pbuf = '\0';
    return decoded;
}

// Removes white spaces from start and from end of the string 
char* trim_whitespace(char *str) {
    char *start = str;
    char *end;

    // Remove white spaces from start
    while (isspace((unsigned char)*start)) start++;
    if (*start == 0) {
        return strdup("");
    }

    // Remove white spaces from end
    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
    return strdup(start);
}

#endif
