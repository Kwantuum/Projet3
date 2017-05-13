//
// Created by Romain on 01-12-16.
//

#ifndef  DYNAMICTIMEWARPING_H
#define  DYNAMICTIMEWARPING_H

#include <stddef.h>
#include "Signal.h"

/** ------------------------------------------------------------------------ *
 * Computer the dynamic time warping between two signals subject to a locality
 * constraint
 *
 * PARAMETERS
 * signal1       A pointer to the first Signal object.
 * signal2       A pointer to the second Signal object.
 * locality      The maximum shift between the matching
 *
 * RETURN
 * dtw           The DTW score for this pair of signals.
 *               If the locality constraint prevent the dtw from computing
 *               the score, return DBL_MAX.
 * ------------------------------------------------------------------------- */
double dtw(Signal* signal1, Signal* signal2, size_t locality);

#endif // DYNAMICTIMEWARPING_H
