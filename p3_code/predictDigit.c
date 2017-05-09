#include <stddef.h>
#include "Signal.h"
#include "dynamicTimeWarping.h"

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
DigitScore predictDigit(Signal* signal, Database* database, size_t locality){
	DigitScore digitScore = {DBL_MAX, -1};
	for(int i = 0; i<10; i++){
		double score = dtw(signal, Database->samples[i], locality);
		if(score < digitScore.score){
			digitScore.score = score;
			digitScore.digit = i;
		}
	}
	return digitScore;
}