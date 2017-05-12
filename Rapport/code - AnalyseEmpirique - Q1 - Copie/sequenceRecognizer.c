#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "splitSequence.h"
#include "Signal.h"



/* ------------------------------------------------------------------------- *
 * This program attempts to recognize a sequence of digits.
 *
 * PARAMETERS
 * mfcc_file        The file containing the mfcc transform of the sequence
 * database_dir     The path to the directory containing the reference samples
 * locality         The locality constraint (unsigned int) [OPTIONAL]
 * lMin             The minimum length of a digit (unsigned int) [OPTIONAL]
 * lMax             The maximum length of a digit (unsigned int) [OPTIONAL]
 * ------------------------------------------------------------------------- */
int main(int argc, char** argv)
{
    size_t locality = SIZE_MAX;
    size_t lMin = 1;
    size_t lMax = SIZE_MAX;

    // --------------------- Parsing the command line args ---------------------
    if (argc < 3) {
        fprintf(stderr, "USAGE: %s <mfcc_file> <database_dir> [-loc <locality>] [-lMin <lMin>] [-lMax <lMax>]\n", argv[0]);
        return EXIT_FAILURE;
    }

    Signal* unknownSignal = parseSignal(argv[1]);
    if (!unknownSignal) {
        fprintf(stderr, "Cannot load the signal from file '%s'... Exit!", argv[1]);
        return EXIT_FAILURE;
    }

    Database* database = parseDatabase(argv[2]);
    if (!database) {
        fprintf(stderr, "Cannot load the database from directory '%s'... Exit!", argv[2]);
        return EXIT_FAILURE;
    }

    // Parsing optional args
    int idx = 3;
    while (idx < argc)
    {
        if (strcmp(argv[idx], "-loc") == 0)
        {
            if(sscanf(argv[++idx], "%zu", &locality) != 1)
            {
                fprintf(stderr, "Cannot parse the locality '%s'... Exit!\n", argv[idx]);
                return EXIT_FAILURE;
            }
        }
        else if (strcmp(argv[idx], "-lMin") == 0)
        {
            if(sscanf(argv[++idx], "%zu", &lMin) != 1 || lMin == 0)
            {
                fprintf(stderr, "Cannot parse the lMin '%s'... Exit!\n", argv[idx]);
                return EXIT_FAILURE;
            }
        }
        else if (strcmp(argv[idx], "-lMax") == 0)
        {
            if(sscanf(argv[++idx], "%zu", &lMax) != 1)
            {
                fprintf(stderr, "Cannot parse the lMax '%s'... Exit!\n", argv[idx]);
                return EXIT_FAILURE;
            }
        }
        idx++;
    }
    if (lMin > lMax)
    {
        fprintf(stderr, "lMin ('%zu') should be smaller or equal to lMax ('%zu') \n", lMin, lMax );
        return EXIT_FAILURE;
    }


    // --------------------- Computing the sequence ---------------------

    printf("Computing...\n");

    DigitSequence digitSequence = bestSplit(unknownSignal, database,
                                            locality, lMin, lMax);

    if(!digitSequence.digits || !digitSequence.splits)
    {
        fprintf(stderr, "%s\n", "The splitting did not work\n");
        return EXIT_FAILURE;
    }

    printf("The following %zu digits were found: ", digitSequence.nDigits);
    for(size_t i = 0; i < digitSequence.nDigits; i++)
    {
        printf(" %d ", digitSequence.digits[i]);
    }
    printf("\nThe corresponding splits are: ");
    for(size_t i = 0; i < digitSequence.nDigits; i++)
    {
        printf(" %zu ", digitSequence.splits[i]);
    }
    printf("\n The corresponding score is %f\n", digitSequence.score);

    free(digitSequence.digits);
    free(digitSequence.splits);
    freeSignal(unknownSignal);
    freeDatabase(database);
    return EXIT_SUCCESS;

}
