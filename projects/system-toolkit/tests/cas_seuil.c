#include "suite.h"

#include <errno.h>
#include <string.h>

static void	un_octet_sous(int *ko)
{
	t_snapshot	snap;
	char		buf[512];
	char		attendu[128];
	size_t		need;
	int			st;

	remplir(&snap, &st);
	if (st != 0)
	{
		suite_echec(ko, "preparation");
		return ;
	}
	texte_attendu(attendu, sizeof(attendu), 4096, 4);
	need = strlen(attendu) + 1;
	memset(buf, 0xA5, sizeof(buf));
	errno = 0;
	if (snapshot_format(&snap, buf, need - 1) != -1 || errno != ERANGE)
		suite_echec(ko, "un sous la taille");
	if (buf[0] != '\0' || (unsigned char)buf[need - 1] != 0xA5)
		suite_echec(ko, "debordement un sous");
}

static void	page_zero(int *ko)
{
	t_fields	in;

	champs_nominaux(&in);
	in.page_size = 0;
	refuse_remplissage(ko, &in, EINVAL, "page zero");
}

static void	page_un(int *ko)
{
	t_snapshot	snap;
	t_fields	in;
	char		buf[512];
	char		attendu[128];

	champs_nominaux(&in);
	in.page_size = 1;
	in.processors = 1;
	if (snapshot_fill(&snap, &in) != 0)
	{
		suite_echec(ko, "page un refusee");
		return ;
	}
	texte_attendu(attendu, sizeof(attendu), 1, 1);
	if (snapshot_format(&snap, buf, sizeof(buf)) != 0
		|| strcmp(buf, attendu) != 0)
		suite_echec(ko, "page un texte");
}

static void	page_negative(int *ko)
{
	t_fields	in;

	champs_nominaux(&in);
	in.page_size = -1;
	refuse_remplissage(ko, &in, EINVAL, "page negative");
}

void	lancer_seuil(int *passes, int *echecs)
{
	noter("limite un octet sous la taille", un_octet_sous, passes, echecs);
	noter("limite page zero", page_zero, passes, echecs);
	noter("limite page un", page_un, passes, echecs);
	noter("limite page negative", page_negative, passes, echecs);
}
