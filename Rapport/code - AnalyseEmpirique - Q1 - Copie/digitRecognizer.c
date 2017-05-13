
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

	
	Signal* signal_tab[50];
		
    signal_tab[0] = parseSignal("testDB/0/zeropg6.mfcc");
	signal_tab[1] = parseSignal("testDB/0/zeropg7.mfcc");
	signal_tab[2] = parseSignal("testDB/0/zeropg8.mfcc");
	signal_tab[3] = parseSignal("testDB/0/zeropg9.mfcc");
	signal_tab[4] = parseSignal("testDB/0/zeropg10.mfcc");
	
	signal_tab[5] = parseSignal("testDB/1/unpg6.mfcc");
	signal_tab[6] = parseSignal("testDB/1/unpg7.mfcc");
	signal_tab[7] = parseSignal("testDB/1/unpg8.mfcc");
	signal_tab[8] = parseSignal("testDB/1/unpg9.mfcc");
	signal_tab[9] = parseSignal("testDB/1/unpg10.mfcc");
	
	signal_tab[10] = parseSignal("testDB/2/deuxpg6.mfcc");
	signal_tab[11] = parseSignal("testDB/2/deuxpg7.mfcc");
	signal_tab[12] = parseSignal("testDB/2/deuxpg8.mfcc");
	signal_tab[13] = parseSignal("testDB/2/deuxpg9.mfcc");
	signal_tab[14] = parseSignal("testDB/2/deuxpg10.mfcc");
	
	signal_tab[15] = parseSignal("testDB/3/troispg6.mfcc");
	signal_tab[16] = parseSignal("testDB/3/troispg7.mfcc");
	signal_tab[17] = parseSignal("testDB/3/troispg8.mfcc");
	signal_tab[18] = parseSignal("testDB/3/troispg9.mfcc");
	signal_tab[19] = parseSignal("testDB/3/troispg10.mfcc");
	
	signal_tab[20] = parseSignal("testDB/4/quatrepg6.mfcc");
	signal_tab[21] = parseSignal("testDB/4/quatrepg7.mfcc");
	signal_tab[22] = parseSignal("testDB/4/quatrepg8.mfcc");
	signal_tab[23] = parseSignal("testDB/4/quatrepg9.mfcc");
	signal_tab[24] = parseSignal("testDB/4/quatrepg10.mfcc");
	
	signal_tab[25] = parseSignal("testDB/5/cinqpg6.mfcc");
	signal_tab[26] = parseSignal("testDB/5/cinqpg7.mfcc");
	signal_tab[27] = parseSignal("testDB/5/cinqpg8.mfcc");
	signal_tab[28] = parseSignal("testDB/5/cinqpg9.mfcc");
	signal_tab[29] = parseSignal("testDB/5/cinqpg10.mfcc");
	
	signal_tab[30] = parseSignal("testDB/6/sixpg6.mfcc");
	signal_tab[31] = parseSignal("testDB/6/sixpg7.mfcc");
	signal_tab[32] = parseSignal("testDB/6/sixpg8.mfcc");
	signal_tab[33] = parseSignal("testDB/6/sixpg9.mfcc");
	signal_tab[34] = parseSignal("testDB/6/sixpg10.mfcc");
	
	signal_tab[35] = parseSignal("testDB/7/septpg6.mfcc");
	signal_tab[36] = parseSignal("testDB/7/septpg7.mfcc");
	signal_tab[37] = parseSignal("testDB/7/septpg8.mfcc");
	signal_tab[38] = parseSignal("testDB/7/septpg9.mfcc");
	signal_tab[39] = parseSignal("testDB/7/septpg10.mfcc");
	
	signal_tab[40] = parseSignal("testDB/8/huitpg6.mfcc");
	signal_tab[41] = parseSignal("testDB/8/huitpg7.mfcc");
	signal_tab[42] = parseSignal("testDB/8/huitpg8.mfcc");
	signal_tab[43] = parseSignal("testDB/8/huitpg9.mfcc");
	signal_tab[44] = parseSignal("testDB/8/huitpg10.mfcc");
	
	signal_tab[45] = parseSignal("testDB/9/neufpg6.mfcc");
	signal_tab[46] = parseSignal("testDB/9/neufpg7.mfcc");
	signal_tab[47] = parseSignal("testDB/9/neufpg8.mfcc");
	signal_tab[48] = parseSignal("testDB/9/neufpg9.mfcc");
	signal_tab[49] = parseSignal("testDB/9/neufpg10.mfcc");

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
		printf("The locality is %zu \n", locality);
    }


    printf("Computing...\n");

	int nb_errors = 0;
	for(int i = 0; i < 50 ; i++)
	{
		DigitScore digitscore = predictDigit(signal_tab[i], database, locality);
		printf("The detected digit is '%d'!\n", digitscore.digit);
		if(i < 5)
			if(digitscore.digit != 0)
			{
				printf("error \n");
				nb_errors++;
			}
		if(i >= 5 && i < 10)
			if(digitscore.digit != 1)
			{
				printf("error \n");
				nb_errors++;
			}
		if(i >= 10 && i < 15)
			if(digitscore.digit != 2)
			{
				printf("error \n");
				nb_errors++;
			}
		if(i >= 15 && i < 20)
			if(digitscore.digit != 3)
			{
				printf("error \n");
				nb_errors++;
			}
		if(i >= 20 && i < 25)
			if(digitscore.digit != 4)
			{
				printf("error \n");
				nb_errors++;
			}
		if(i >= 25 && i < 30)
			if(digitscore.digit != 5)
			{
				printf("error \n");
				nb_errors++;
			}
		if(i >= 30 && i < 35)
			if(digitscore.digit != 6)
			{
				printf("error \n");
				nb_errors++;
			}
		if(i >= 35 && i < 40)
			if(digitscore.digit != 7)
			{
				printf("error \n");
				nb_errors++;
			}
		if(i >= 40 && i < 45)
			if(digitscore.digit != 8)
			{
				printf("error \n");
				nb_errors++;
			}
		if(i >= 45 && i < 50)
			if(digitscore.digit != 9)
			{
				printf("error \n");
				nb_errors++;
			}			
		freeSignal(signal_tab[i]);
	}		

	printf("Nombre d'erreurs : %d \n", nb_errors);
	printf("The locality was %zu \n", locality);
    freeDatabase(database);
    return EXIT_SUCCESS;
}
