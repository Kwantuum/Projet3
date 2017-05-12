//
// Created by Romain on 01-12-16.
//

#ifndef PREDICTDIGIT_H
#define PREDICTDIGIT_H

#include <stddef.h>
#include "Signal.h"

/**
 * A structure for storing a digit and its corresponding score
 */
typedef struct digit_score_t {
    double score;
    int digit;
} DigitScore;

/** ------------------------------------------------------------------------ *
 * Predict the digit represented by the signal thanks to the given database
 *
 * PARAMETERS
 * signal       A pointer to the Signal object.
 * database     A database of samples to use for comparing the current signal
 * locality     The maximum shift between the matching
 *
 * RETURN
 * digitScore   The optimal digit (in [0,9]) and its corresponding score.
 * ------------------------------------------------------------------------- */
DigitScore predictDigit(Signal* signal, Database* database, size_t locality);

#endif //PREDICTDIGIT_H
