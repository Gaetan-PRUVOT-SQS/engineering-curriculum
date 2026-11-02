#include "suite.h"

#include <errno.h>
#include <string.h>

static void	tampon_nul(int *ko)
{
	t_snapshot	snap;
	int			st;

	remplir(&snap, &st);
	if (st != 0)
	{
		suite_echec(ko, "preparation");
		return ;
	}
	errno = 0;
	if (snapshot_format(&snap, NULL, 32) != -1 || errno != EINVAL)
		suite_echec(ko, "tampon nul");
}

static void	taille_zero(int *ko)
{
	t_snapshot	snap;
	char		buf[8];
	int			st;

	remplir(&snap, &st);
	if (st != 0)
	{
		suite_echec(ko, "preparation");
		return ;
	}
	memset(buf, 0xA5, sizeof(buf));
	errno = 0;
	if (snapshot_format(&snap, buf, 0) != -1 || errno != EINVAL)
		suite_echec(ko, "taille zero");
	if ((unsigned char)buf[0] != 0xA5)
		suite_echec(ko, "taille zero a ecrit");
}

static void	taille_un(int *ko)
{
	t_snapshot	snap;
	char		buf[8];
	int			st;

	remplir(&snap, &st);
	if (st != 0)
	{
		suite_echec(ko, "preparation");
		return ;
	}
	memset(buf, 0xA5, sizeof(buf));
	errno = 0;
	if (snapshot_format(&snap, buf, 1) != -1 || errno != ERANGE)
		suite_echec(ko, "taille un");
	if (buf[0] != '\0' || (unsigned char)buf[1] != 0xA5)
		suite_echec(ko, "debordement taille un");
}

static void	taille_exacte(int *ko)
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
	if (snapshot_format(&snap, buf, need) != 0)
	{
		suite_echec(ko, "taille exacte");
		return ;
	}
	if (strcmp(buf, attendu) != 0)
		suite_echec(ko, "texte exact");
}

void	lancer_bornes(int *passes, int *echecs)
{
	noter("limite tampon nul", tampon_nul, passes, echecs);
	noter("limite taille zero", taille_zero, passes, echecs);
	noter("limite taille un", taille_un, passes, echecs);
	noter("limite taille exacte", taille_exacte, passes, echecs);
}
