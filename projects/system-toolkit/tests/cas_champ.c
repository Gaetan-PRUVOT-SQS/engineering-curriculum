#include "suite.h"

#include <errno.h>
#include <string.h>

static void	champ_vide(int *ko)
{
	t_fields	in;

	champs_nominaux(&in);
	in.sysname = "";
	refuse_remplissage(ko, &in, EINVAL, "champ vide");
}

static void	champ_nul(int *ko)
{
	t_fields	in;

	champs_nominaux(&in);
	in.sysname = NULL;
	refuse_remplissage(ko, &in, EINVAL, "champ nul");
}

static void	champ_trop_long(int *ko)
{
	t_fields	in;
	char		bad[SNAPSHOT_TEXT_MAX + 1];

	memset(bad, 'b', SNAPSHOT_TEXT_MAX);
	bad[SNAPSHOT_TEXT_MAX] = '\0';
	champs_nominaux(&in);
	in.sysname = bad;
	refuse_remplissage(ko, &in, ERANGE, "champ trop long");
}

static void	saut_remplissage(int *ko)
{
	t_fields	in;

	champs_nominaux(&in);
	in.sysname = "Lin\nux";
	refuse_remplissage(ko, &in, EINVAL, "saut remplissage");
}

void	lancer_champs(int *passes, int *echecs)
{
	noter("partition champ vide", champ_vide, passes, echecs);
	noter("partition champ nul", champ_nul, passes, echecs);
	noter("partition champ trop long", champ_trop_long, passes, echecs);
	noter("partition saut de ligne remplissage", saut_remplissage, passes,
		echecs);
}
