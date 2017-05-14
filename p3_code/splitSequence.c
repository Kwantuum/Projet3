#include "splitSequence.h"
#include "predictDigit.h"

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
// typedef struct digit_sequence_t {
//     size_t nDigits;
//     double score;
//     int* digits;
//     size_t* splits;
// } DigitSequence;

static Signal* subsignal(Signal *, size_t, size_t);

static size_t condition(size_t , size_t , size_t );

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
                        size_t locality, size_t lMin, size_t lMax){
	if(signal == NULL || database == NULL || signal->size < lMin)
		return (DigitSequence){0, 0, NULL, NULL};
	

	
	
	// Initialise the DigitSequence structure
	DigitSequence* bestSplit = malloc(sizeof(DigitSequence));
	if(!bestSplit)
		return (DigitSequence){0, 0, NULL, NULL};
	
	bestSplit->splits[0] = 0; // start
	
	// Decompose the source signal into sub_signals
	for(size_t i = 0 ; i < signal->size - 1 ; i++)
	{
		bestSplit->splits[i+1] = condition(lMin, lMax, bestSplit->splits[i]); // stop
		Signal* sub_signal = subsignal(signal, bestSplit->splits[i], bestSplit->splits[i+1]);
		DigitScore digtScore = predictDigit(sub_signal, database, locality);
		++bestSplit->nDigits;
		bestSplit->score += digtScore.score;
		bestSplit->digits[i] = digtScore.digit;	
	}
	
	return *bestSplit;
}

/* Takes a pointer to a source signal and puts a section of it into sub.
 * The subsignal should already be initialized and its mfcc field have enough
 * allocated memory to contain the subsignal. */
static Signal* subsignal(Signal *src, size_t start, size_t stop){
	// Check args
	if(src == NULL || start > stop
		|| start > src->size || stop > src->size)
		return NULL;

	Signal* sub_signal = malloc(sizeof(Signal));
	sub_signal->size = stop - start + 1;
	for(size_t i = 0; i < sub_signal->size; i++)
		for(size_t j = 0; j < 13; j++)
			sub_signal->mfcc[j][i] = src->mfcc[j][i+start];
		
	return sub_signal;
}

static size_t condition(size_t lMin, size_t lMax, size_t start)
{
	size_t i = start;
	while(i)
	{
		if(lMin + start <= i && lMax + start >= i)
			break;
		i++;
	}
	return i;
}
		