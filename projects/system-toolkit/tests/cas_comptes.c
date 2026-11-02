#include "suite.h"

#include <errno.h>
#include <string.h>

static void	processeurs_zero(int *ko)
{
	t_fields	in;

	champs_nominaux(&in);
	in.processors = 0;
	refuse_remplissage(ko, &in, EINVAL, "processeurs zero");
}

static void	processeurs_un(int *ko)
{
	t_snapshot	snap;
	t_fields	in;
	char		buf[512];
	char		attendu[128];

	champs_nominaux(&in);
	in.processors = 1;
	if (snapshot_fill(&snap, &in) != 0)
	{
		suite_echec(ko, "processeurs un refuse");
		return ;
	}
	texte_attendu(attendu, sizeof(attendu), 4096, 1);
	if (snapshot_format(&snap, buf, sizeof(buf)) != 0
		|| strcmp(buf, attendu) != 0)
		suite_echec(ko, "processeurs un texte");
}

static void	collecte_nulle(int *ko)
{
	errno = 0;
	if (snapshot_collect(NULL) != -1 || errno != EINVAL)
		suite_echec(ko, "collecte nulle");
}

static void	entree_nulle(int *ko)
{
	t_snapshot	snap;
	int			reste;

	marquer_z(&snap);
	errno = 0;
	if (snapshot_fill(&snap, NULL) != -1 || errno != EINVAL)
		suite_echec(ko, "entree nulle");
	encore_z(&snap, &reste);
	if (reste != 1)
		suite_echec(ko, "entree nulle a ecrit");
}

void	lancer_comptes(int *passes, int *echecs)
{
	noter("limite processeurs zero", processeurs_zero, passes, echecs);
	noter("limite processeurs un", processeurs_un, passes, echecs);
	noter("partition collecte nulle", collecte_nulle, passes, echecs);
	noter("partition entree nulle", entree_nulle, passes, echecs);
}
