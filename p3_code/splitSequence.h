//
// Created by Romain on 01-12-16.
//

#ifndef SPLITSEQUENCE_H
#define SPLITSEQUENCE_H

#include "Signal.h"

/**
 * A DigitSequence object contains all the information about the splitting of a sequence.
 *
 * FIELDS
 * nDigits      The number of digits found in the sequence (k)
 * score        The optimal score resulting from the splitting
 * digits       An array containing the digits found (size: nDigits)
 * splits       An array containing the start indexes of each sequence (size: k).
 *              Typical array: splits[0]   = 0  (start index of the first sequence)
 *                             splits[i]   = .. (start index of the (i+1)th sequence)
 *                             splits[k-1] = .. (start index of the last sequence)
 *
 */
typedef struct digit_sequence_t {
    size_t nDigits;
    double score;
    int* digits;
    size_t* splits;
} DigitSequence;

/** ------------------------------------------------------------------------ *
 * Given a signal containing a sequence of digits, find the best split of
 * the signal isolating each digit in the sequence.
 *
 * Returns the optimal digit sequence, its score, its corresponding split
 * indexes as a structure.
 *
 * PARAMETERS
 * signal           A pointer to the Signal object.
 * database         A database of samples to use for comparing the current signal
 * locality         The locality constraint
 * lMin             The minimum length of a digit
 * lMax             The maximum length of a digit
 *
 * RETURN
 * digitSequence    The optimal sequence information (see DigitSequence doc)
 *                  In case of error, `digits` and `splits` arrays will be
 *                  NULL
 * ------------------------------------------------------------------------- */
DigitSequence bestSplit(Signal* signal, Database* database,
                        size_t locality, size_t lMin, size_t lMax);


#endif //SPLITSEQUENCE_H
