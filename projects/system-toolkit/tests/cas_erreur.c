#include "suite.h"

#include <errno.h>
#include <string.h>

static void	format_page_zero(int *ko)
{
	t_snapshot	snap;
	char		buf[512];
	int			st;

	remplir(&snap, &st);
	if (st != 0)
	{
		suite_echec(ko, "preparation");
		return ;
	}
	snap.page_size = 0;
	memset(buf, 0xA5, sizeof(buf));
	errno = 0;
	if (snapshot_format(&snap, buf, sizeof(buf)) != -1 || errno != EINVAL)
		suite_echec(ko, "format page zero");
	if ((unsigned char)buf[0] != 0xA5)
		suite_echec(ko, "format page zero a ecrit");
}

static void	format_champ_vide(int *ko)
{
	t_snapshot	snap;
	char		buf[512];
	int			st;

	remplir(&snap, &st);
	if (st != 0)
	{
		suite_echec(ko, "preparation");
		return ;
	}
	snap.release[0] = '\0';
	memset(buf, 0xA5, sizeof(buf));
	errno = 0;
	if (snapshot_format(&snap, buf, sizeof(buf)) != -1 || errno != EINVAL)
		suite_echec(ko, "format champ vide");
	if ((unsigned char)buf[0] != 0xA5)
		suite_echec(ko, "format champ vide a ecrit");
}

static void	instantane_nul(int *ko)
{
	t_fields	in;
	char		buf[8];

	champs_nominaux(&in);
	errno = 0;
	if (snapshot_fill(NULL, &in) != -1 || errno != EINVAL)
		suite_echec(ko, "instantane nul");
	errno = 0;
	if (snapshot_format(NULL, buf, sizeof(buf)) != -1 || errno != EINVAL)
		suite_echec(ko, "format nul");
}

void	lancer_erreurs(int *passes, int *echecs)
{
	noter("erreur format page zero", format_page_zero, passes, echecs);
	noter("erreur format champ vide", format_champ_vide, passes, echecs);
	noter("partition instantane nul", instantane_nul, passes, echecs);
}
