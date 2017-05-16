#include <stddef.h>
#include <float.h>
#include "Signal.h"
#include "predictDigit.h"
#include "dynamicTimeWarping.h"
#include "LinkedList.h"

#include <stdio.h>

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
	for(int i = 0; i < 10; i++){
		printf("Calculating DTWs with digit %d\n", i);
		LLNode *reference = database->samples[i]->head;
		while(reference != NULL){
			double score = dtw(signal, (Signal *)reference->value, locality);
			if(score < digitScore.score){
				digitScore.score = score;
				digitScore.digit = i;
			}
			reference = reference->next;
		}
	}
	return digitScore;
}