#include "suite.h"

#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	int	passes;
	int	echecs;

	passes = 0;
	echecs = 0;
	lancer_bornes(&passes, &echecs);
	lancer_seuil(&passes, &echecs);
	lancer_comptes(&passes, &echecs);
	lancer_champs(&passes, &echecs);
	lancer_format(&passes, &echecs);
	lancer_erreurs(&passes, &echecs);
	printf("cas: %d passes, %d echecs\n", passes, echecs);
	if (echecs != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
