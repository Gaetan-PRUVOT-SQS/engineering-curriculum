#include "snapshot.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void	dire_erreur(const char *action)
{
	int	err;

	err = errno;
	fprintf(stderr, "système: %s: %s\n", action, strerror(err));
}

int	main(void)
{
	t_snapshot	snap;
	char		text[2048];

	if (snapshot_collect(&snap) != 0)
	{
		dire_erreur("lecture impossible");
		return (EXIT_FAILURE);
	}
	if (snapshot_format(&snap, text, sizeof(text)) != 0)
	{
		dire_erreur("mise en forme impossible");
		return (EXIT_FAILURE);
	}
	if (fputs(text, stdout) == EOF || fflush(stdout) == EOF)
	{
		dire_erreur("écriture impossible");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
