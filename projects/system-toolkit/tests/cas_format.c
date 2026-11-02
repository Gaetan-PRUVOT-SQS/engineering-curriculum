#include "suite.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

static void	longueur_max(int *ko)
{
	t_snapshot	snap;
	t_fields	in;
	char		ok[SNAPSHOT_TEXT_MAX];
	char		buf[2048];
	char		attendu[SNAPSHOT_TEXT_MAX + 16];

	memset(ok, 'a', SNAPSHOT_TEXT_MAX - 1);
	ok[SNAPSHOT_TEXT_MAX - 1] = '\0';
	champs_nominaux(&in);
	in.sysname = ok;
	if (snapshot_fill(&snap, &in) != 0)
	{
		suite_echec(ko, "longueur max refusee");
		return ;
	}
	if (snapshot_format(&snap, buf, sizeof(buf)) != 0)
	{
		suite_echec(ko, "longueur max format");
		return ;
	}
	snprintf(attendu, sizeof(attendu), "sysname: %s\n", ok);
	if (strncmp(buf, attendu, strlen(attendu)) != 0)
		suite_echec(ko, "longueur max texte");
}

static void	nominale(int *ko)
{
	t_snapshot	snap;
	char		buf[512];
	char		attendu[128];
	int			st;

	remplir(&snap, &st);
	if (st != 0)
	{
		suite_echec(ko, "preparation");
		return ;
	}
	texte_attendu(attendu, sizeof(attendu), 4096, 4);
	if (snapshot_format(&snap, buf, sizeof(buf)) != 0
		|| strcmp(buf, attendu) != 0)
		suite_echec(ko, "nominal");
}

static void	saut_format(int *ko)
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
	snap.sysname[3] = '\n';
	memset(buf, 0xA5, sizeof(buf));
	errno = 0;
	if (snapshot_format(&snap, buf, sizeof(buf)) != -1 || errno != EINVAL)
		suite_echec(ko, "saut format");
	if ((unsigned char)buf[0] != 0xA5)
		suite_echec(ko, "saut format a ecrit");
}

static void	sans_nul(int *ko)
{
	t_snapshot	snap;
	char		buf[32];

	memset(&snap, 'A', sizeof(snap));
	snap.page_size = 4096;
	snap.processors = 4;
	memset(buf, 0xA5, sizeof(buf));
	errno = 0;
	if (snapshot_format(&snap, buf, sizeof(buf)) != -1 || errno != EINVAL)
		suite_echec(ko, "sans nul");
	if ((unsigned char)buf[0] != 0xA5)
		suite_echec(ko, "sans nul a ecrit");
}

void	lancer_format(int *passes, int *echecs)
{
	noter("partition champ longueur max", longueur_max, passes, echecs);
	noter("partition nominale", nominale, passes, echecs);
	noter("partition saut de ligne format", saut_format, passes, echecs);
	noter("partition champ sans nul", sans_nul, passes, echecs);
}
