#ifndef SIGNAL_H
#define SIGNAL_H

#include <stdlib.h>
#include "LinkedList.h"

/** ------------------------------------------------------------------------ *
 * A MFCC (Mel-frequency cepstral coefficients)
 *
 * FIELDS
 * n_coef       The number of coefficients
 * size         The number of time steps
 * mfcc         A 2D array containing the MFCC coefficients
 *              (dim: n_coef x timesteps)
 * ------------------------------------------------------------------------- */
typedef struct signal_t {
    double** mfcc;
    size_t n_coef, size;
} Signal;

/** ------------------------------------------------------------------------ *
 * A database containing a set of signal for all possible decimal digits.
 *
 * A list containing all the signals corresponding to the number i (in [0,9])
 * in the database is stored in database->samples[i].
 *
 * FIELDS
 * samples       An array of LinkedList objects containing the samples
 * ------------------------------------------------------------------------- */
typedef struct database_t {
    LinkedList* samples[10];
} Database;

/** ------------------------------------------------------------------------ *
 * Parse the file 'filepath' and generate the corresponding Signal file.
 * The file should start with two integers:
 *  1) the number of coefficients (rows, n_coef)
 *  2) the number of timesteps (columns, size)
 * The rest of the line should contain n_coef * size floating point numbers
 * representing the various MFCC coefficients.
 *
 * Signal must later be deleted using the `freeSignal` function.
 *
 * PARAMETERS
 * filepath     The path to the MFCC file.
 *
 * RETURN
 * signal       A pointer to the Signal object.
 * ------------------------------------------------------------------------- */
Signal* parseSignal(const char *filepath);

/** ------------------------------------------------------------------------ *
 * Free the memory allocated for the Signal object.
 *
 * PARAMETERS
 * signal       A pointer to the Signal object.
 * ------------------------------------------------------------------------- */
void freeSignal(Signal* signal);

/** ------------------------------------------------------------------------ *
 * Generate a Database object gathering the signals contained in the
 * directory at the given path.
 *
 * The top-level directory should contain subdirectories numbered from 0 to
 * 9 representing the digits. The subdirectory i should contain all the
 * samples of the digit i.
 * See the `refDB` for a example.
 *
 * The MFCC signal files to load should have the extension '.mfcc'.
 *
 * PARAMETERS
 * dirpath       The path to the directory
 *
 * RETURN
 * database      A pointer to the Database object.
 * ------------------------------------------------------------------------- */
Database* parseDatabase(const char* dirpath);

/** ------------------------------------------------------------------------ *
 * Free the memory allocated for the Dignal object.
 *
 * PARAMETERS
 * signal       A pointer to the Database object.
 * ------------------------------------------------------------------------- */
void freeDatabase(Database* database);

#endif //SIGNAL_H
