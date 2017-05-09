//
// Created by Romain on 01-12-16.
//
#include "Signal.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <inttypes.h>
#include <dirent.h>

/** ------------------------------------------------------------------------ *
 * From the given file handle, read the next sequence of bytes until a character that cannot
 * be a digit is met and place it in the buffer. The buffer should be large enough to contain the sequence.
 * The cursor of the file handle is moved after the next sep after the execution of the
 * function.
 *
 * PARAMETERS
 * f        The file handle
 * buffer   A large enough buffer to store the null terminated read sequence
 *
 * RETURN
 * true if the sequence extraction stopped on a EOF
 * ------------------------------------------------------------------------- */
static bool nextFloat(FILE* f, char* buffer);

/** ------------------------------------------------------------------------ *
 *
 * PARAMETERS
 * c    The character
 *
 * RETURN
 * true if the next character is a float
 * ------------------------------------------------------------------------- */
static bool isFloat(char c);

/** ------------------------------------------------------------------------ *
 * PARAMETERS
 * str   A null-terminated string
 *
 * RETURN
 * A newly allocated string containing the same content as str
 * ------------------------------------------------------------------------- */
static char* __strdup(const char* str);

/** ------------------------------------------------------------------------ *
 * PARAMETERS
 * str  The string to check
 * end  The expected end sequence
 *
 * RETURN
 * true if str ends with end
 * ------------------------------------------------------------------------- */
static bool endswith(const char* str, const char* end);

/** ------------------------------------------------------------------------ *
 * PARAMETERS
 * signal   A void pointer to the Signal object to free
 * ------------------------------------------------------------------------- */
static void freeSignalVoid(void* s);

static void freeSignalVoid(void* s) {
    Signal* signal = (Signal*) s;
    for (size_t i = 0; i < signal->n_coef; ++i) {
        free(signal->mfcc[i]);
    }
    free(signal->mfcc);
    free(signal);
}

static bool endswith(const char* str, const char* end) {
    size_t n = strlen(str), s = strlen(end);
    if (n < s) {
        return false;
    }
    for (size_t i = 0; i < s; ++i) {
        if (str[n - s + i] != end[i]) {
            return false;
        }
    }
    return true;
}

static char* __strdup(const char* str) {
    if (!str) { return NULL; }
    size_t n = strlen(str);
    char* dup = malloc(sizeof(char) * (n + 1));
    if (dup) {
        strcpy(dup, str);
        dup[n] = '\0';
    }
    return dup;
}

static bool isFloat(char c) {
    return c == '.' || isdigit(c);
}

static bool nextFloat(FILE* f, char* buffer) {
    char c = getc(f);
    while (!isFloat(c) && c != EOF) {
        c = getc(f);
    }

    size_t buff_idx = 0;
    while(c != EOF && isFloat(c)) {
        buffer[buff_idx++] = c;
        c = getc(f);
    }

    buffer[buff_idx] = '\0'; // add null terminator
    return c == EOF;
}

Signal* parseSignal(const char *filepath) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        return NULL;
    }

    Signal* signal = malloc(sizeof(Signal));
    if (!signal) {
        fclose(file);
        return NULL;
    }

    char buffer[256]; // buffer for storing temporary strings

    // read number of rows (i.e. number of coefficients)
    nextFloat(file, buffer);
    signal->n_coef = strtoul(buffer, NULL, 10);
    nextFloat(file, buffer);
    signal->size = strtoul(buffer, NULL, 10);

    // allocate matrix
    signal->mfcc = malloc(sizeof(double*) * signal->n_coef);
    if (!signal->mfcc) {
        free(signal);
        fclose(file);
        return NULL;
    }

    for (size_t i = 0; i < signal->n_coef; ++i) {
        signal->mfcc[i] = malloc(sizeof(double) * signal->size);
        if (!signal->mfcc[i]) {
            // deallocate previously allocated arrays
            for(size_t j = i - 1; j < i; --j) {
                free(signal->mfcc[j]);
            }
            free(signal->mfcc);
            free(signal);
            fclose(file);
            return NULL;
        }
    }

    // fill matrix
    for (size_t i = 0; i < signal->n_coef; ++i) {
        for (size_t j = 0; j < signal->size; ++j) {
            nextFloat(file, buffer);
            signal->mfcc[i][j] = strtod(buffer, NULL);
        }
    }

    fclose(file);
    return signal;
}

void freeSignal(Signal *signal) {
    freeSignalVoid((void*)signal);
}

Database* parseDatabase(const char* dirpath) {
    Database* database = malloc(sizeof(Database));
    if (!database) {
        return NULL;
    }

    char buffer[1024] = {'\0'}; // for dirpath
    char digit_buffer[2] = {'\0'}; // for number folder name

    // Run through the different folders
    for (size_t i = 0; i < 10; ++i) {
        // Build current number's database
        database->samples[i] = newLinkedList();

        // Build the path of the current folder
        sprintf(digit_buffer, "%zu", i);
        strcpy(buffer, dirpath);
        strcat(buffer, "/");
        strcat(buffer, digit_buffer);

        DIR *dp = opendir (buffer);
        if (!dp) { // There is a directory
            continue;
        }

        // Read mfcc files
        char* base_folder = __strdup(buffer);
        if (!base_folder) {
            // free previously created lists
            for(size_t j = i; j < i + 1; --j) {
                freeLinkedList(database->samples[j], freeSignalVoid);
            }
            free(database);
            return NULL;
        }

        struct dirent *ep;
        while ((ep = readdir (dp))) {
            if (!endswith(ep->d_name, ".mfcc")) continue; // not an mfcc file
            strcpy(buffer, base_folder);
            strcat(buffer, "/");
            strcat(buffer, ep->d_name);
            insertInLinkedList(database->samples[i], parseSignal(buffer));
        }

        free(base_folder);
        closedir(dp);
    }

    return database;
}

void freeDatabase(Database *database) {
    for (size_t i = 0; i < 10; ++i) {
        freeLinkedList(database->samples[i], freeSignalVoid);
    }
    free(database);
}
