#include <float.h>
#include "splitSequence.h"
#include "predictDigit.h"

// typedef struct digit_sequence_t {
//     size_t nDigits;
//     double score;
//     int* digits;
//     size_t* splits;
// } DigitSequence;

static Signal* subsignal(Signal *, size_t, size_t);
static DigitSequence bestSplitWrapper(Signal*, Database*, size_t, size_t, size_t, DigitScore **, size_t, size_t);
static DigitScore splitScore(Signal *, Database *, size_t, size_t, size_t, DigitScore **);
static DigitSequence unite(DigitScore, DigitSequence, size_t);


DigitSequence bestSplit(Signal* signal, Database* database,
                        size_t locality, size_t lMin, size_t lMax){
	lMax = lMax > signal->size ? signal->size : lMax;
	if(signal == NULL || database == NULL || signal->size < lMin)
		return (DigitSequence){0, DBL_MAX, NULL, NULL};

	// Make sure that there exists at least one way to split the sequence
	if(!((signal->size / lMin) >= ((double)signal->size / lMax)))
		return (DigitSequence){0, DBL_MAX, NULL, NULL};

	DigitScore **splitScores = malloc(sizeof(*splitScores)*(signal->size));
	for(size_t i = 0; i < signal->size; i++){
		splitScores[i] = malloc(sizeof(*splitScores[i])*(signal->size));
		for(size_t j = 0; j < signal->size; j++)
			splitScores[i][j].digit = -1;

	}

	
	DigitSequence returnSequence = bestSplitWrapper(signal, database, locality, lMin, lMax,
							splitScores, 0, signal->size - 1);
	for(size_t i = 0; i < signal->size; i++)
		free(splitScores[i]);
	free(splitScores);

	return returnSequence;
}

static DigitSequence bestSplitWrapper(Signal* signal, Database* db,
                        size_t locality, size_t lMin, size_t lMax,
                        DigitScore **splitScores, size_t start,
                        size_t stop){
	size_t size = stop - start;
	// If the segment is too small or can't be split
	if(size < lMin || !((size / lMin) >= ((double)size / lMax)))
		return (DigitSequence){0, DBL_MAX, NULL, NULL};
	// Else
	DigitSequence bSplit = {0, DBL_MAX, NULL, NULL};
	if(size >= 2*lMin){
		for(size_t i = start + lMin; i <= start + lMax && i < signal->size; i++){
			size_t rightSize = stop - i;
			// Check that right side can be split
			if((rightSize / lMin) >= ((double)rightSize / lMax) && rightSize >= lMin){
			   	DigitScore leftDigit;
			   	if(splitScores[start][i].digit == -1)
					leftDigit = splitScore(signal, db, locality, start, i, splitScores);
				else{
					leftDigit = splitScores[start][i];
				}
				DigitSequence bestRightSplit = bestSplitWrapper(signal, db, locality, lMin, lMax, splitScores, i, stop);
				DigitSequence newSplit = unite(leftDigit, bestRightSplit, start);
				free(bestRightSplit.digits);
				free(bestRightSplit.splits);
				if(newSplit.score < bSplit.score){
					free(bSplit.digits);
					free(bSplit.splits);
					bSplit = newSplit;
				} else {
					free(newSplit.digits);
					free(newSplit.splits);
				}
			}
		}
	}
	if(size < lMax){
		DigitScore seqScore;
		if(splitScores[start][stop].digit == -1)
			seqScore = splitScore(signal, db, locality, start, stop, splitScores);
		else{
			seqScore = splitScores[start][stop];
		}
		if(bSplit.score > seqScore.score){
			bSplit.nDigits = 1;
			bSplit.score = seqScore.score;
			bSplit.digits = malloc(sizeof(*bSplit.digits));
			bSplit.splits = malloc(sizeof(*bSplit.splits));
			bSplit.digits[0] = seqScore.digit;
			bSplit.splits[0] = start;
		}
	}
	return bSplit;
}

static DigitScore splitScore(Signal *signal, Database *db, size_t locality, size_t start, size_t stop, DigitScore **splitScores){
	if(splitScores[start][stop].digit == -1){
		Signal *sub_signal = subsignal(signal, start, stop);
		splitScores[start][stop] = predictDigit(sub_signal, db, locality);
		freeSignal(sub_signal);
	}
	return splitScores[start][stop];
}

/* Takes a pointer to a source signal and returns a section of it*/
static Signal* subsignal(Signal *src, size_t start, size_t stop){
	// Check args
	if(src == NULL || start > stop
		|| start > src->size || stop > src->size)
		return NULL;

	Signal *sub_signal = malloc(sizeof(*sub_signal));
	if(sub_signal == NULL)
		return NULL;
	sub_signal->size = stop - start + 1;
	sub_signal->n_coef = src->n_coef;
	sub_signal->mfcc = malloc(sizeof(*sub_signal->mfcc) * src->n_coef);
	for(size_t i = 0; i < src->n_coef; i++)
		sub_signal->mfcc[i] = malloc(sizeof(**sub_signal->mfcc) * sub_signal->size);

	for(size_t i = 0; i < sub_signal->size; i++)
		for(size_t j = 0; j < src->n_coef; j++)
			sub_signal->mfcc[j][i] = src->mfcc[j][i+start];
		
	return sub_signal;
}

static DigitSequence unite(DigitScore dscore, DigitSequence seq, size_t start){
	DigitSequence retSeq;
	retSeq.nDigits = seq.nDigits + 1;
	retSeq.score = dscore.score + seq.score;
	retSeq.digits = malloc(sizeof(*retSeq.digits) * retSeq.nDigits);
	retSeq.splits = malloc(sizeof(*retSeq.splits) * retSeq.nDigits);
	retSeq.digits[0] = dscore.digit;
	retSeq.splits[0] = start;
	for(size_t i = 1; i < retSeq.nDigits; i++){
		retSeq.digits[i] = seq.digits[i-1];
		retSeq.splits[i] = seq.splits[i-1];
	}
	return retSeq;
}