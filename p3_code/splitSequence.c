#include "splitSequence.h"

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

static void subsignal(Signal *, Signal *, size_t, size_t);

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
		// return NULL           instead of your return ???
	

	
	
	// Initialise the DigitSequence structure
	DigitSequence bestSplit = malloc(sizeof(DigitSequence));
	if(!bestSplit)
		return NULL;
	// Decompose the source signal into sub_signals
	
	// For each i sub_signal
	DigitScore digtScore = predictDigit(sub_signal, database, locality);
	++bestSplit->nDigits;
	bestSplit->score += digtScore.score;
	bestSplit->digits[i] = digtScore.digit;
	bestSplit->splits[i] = 
	//
	
	
}

/* Takes a pointer to a source signal and puts a section of it into sub.
 * The subsignal should already be initialized and its mfcc field have enough
 * allocated memory to contain the subsignal. */
static void subsignal(Signal *sub, Signal *src, size_t start, size_t stop){
	// Check args
	if(sub == NULL || src == NULL || start > stop
		|| start > src->size || stop > src->size)
		return;

	sub->size = stop - start + 1;
	for(int i = 0; i < sub->size; i++)
		for(j = 0; j < 13; j++)
			sub->mfcc[j][i] = src[j][i+start];
}