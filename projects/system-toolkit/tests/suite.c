#include "suite.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

void	suite_echec(int *echecs, const char *nom)
{
	fprintf(stderr, "echec: %s\n", nom);
	*echecs += 1;
}

void	noter(const char *nom, void (*fn)(int *ko), int *passes, int *echecs)
{
	int	avant;

	avant = *echecs;
	fn(echecs);
	if (*echecs == avant)
	{
		*passes += 1;
		printf("ok %s\n", nom);
	}
	else
		printf("ko %s\n", nom);
}

void	refuse_remplissage(int *ko, const t_fields *in, int err,
				const char *nom)
{
	t_snapshot	snap;
	int			reste;

	marquer_z(&snap);
	errno = 0;
	if (snapshot_fill(&snap, in) != -1 || errno != err)
		suite_echec(ko, nom);
	else
	{
		encore_z(&snap, &reste);
		if (reste != 1)
			suite_echec(ko, nom);
	}
}
