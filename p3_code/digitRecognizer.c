
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>

#include "predictDigit.h"
#include "Signal.h"

/* ------------------------------------------------------------------------- *
 * This program attempts to recognize a digit.
 *
 * PARAMETERS
 * mfcc_file        The file containing the mfcc transform of the digit
 * database_dir     The path to the directory containing the reference samples
 * locality         The locality constraint (unsigned int) [OPTIONAL]
 * ------------------------------------------------------------------------- */
int main(int argc, char** argv) {
    if (argc != 3 && argc != 4) {
        fprintf(stderr, "USAGE: %s <mfcc_file> <database_dir> [<locality>]\n",
                argv[0]);
        return EXIT_FAILURE;
    }

    Signal* unknownSignal = parseSignal(argv[1]);
    if (!unknownSignal) {
        fprintf(stderr, "Cannot load the signal from file '%s'... Exit!\n", argv[1]);
        return EXIT_FAILURE;
    }

    Database* database = parseDatabase(argv[2]);
    if (!database) {
        fprintf(stderr, "Cannot load the database from directory '%s'... Exit!\n", argv[2]);
        return EXIT_FAILURE;
    }

    size_t locality = SIZE_MAX;
    if (argc == 4)
    {
        if(sscanf(argv[3], "%zu", &locality) != 1)
        {
            fprintf(stderr, "Cannot parse the locality '%s'... Exit!\n", argv[3]);
            return EXIT_FAILURE;
        }
    }


    printf("Computing...\n");

    DigitScore digtScore = predictDigit(unknownSignal, database, locality);

    printf("The detected digit is '%d'!\n", digtScore.digit);
    printf("The corresponding score is %lf\n", digtScore.score);
    printf(" (signal  : %s)\n", argv[1]);
    printf(" (database: %s)\n", argv[2]);

    freeSignal(unknownSignal);
    freeDatabase(database);
    return EXIT_SUCCESS;
}
