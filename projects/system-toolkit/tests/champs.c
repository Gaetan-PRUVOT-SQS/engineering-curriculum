#include "suite.h"

#include <stdio.h>
#include <string.h>

void	champs_nominaux(t_fields *in)
{
	in->sysname = "Linux";
	in->nodename = "poste";
	in->release = "6.1.0";
	in->version = "#1 SMP";
	in->machine = "x86_64";
	in->page_size = 4096;
	in->processors = 4;
}

void	texte_attendu(char *dst, size_t cap, long page, long procs)
{
	snprintf(dst, cap,
		"sysname: Linux\nnodename: poste\nrelease: 6.1.0\n"
		"version: #1 SMP\nmachine: x86_64\n"
		"page_size: %ld\nprocessors: %ld\n",
		page, procs);
}

void	marquer_z(t_snapshot *snap)
{
	memset(snap, 'Z', sizeof(*snap));
}

void	encore_z(const t_snapshot *snap, int *reste)
{
	t_snapshot	zed;

	memset(&zed, 'Z', sizeof(zed));
	if (memcmp(snap, &zed, sizeof(zed)) == 0)
		*reste = 1;
	else
		*reste = 0;
}

void	remplir(t_snapshot *snap, int *st)
{
	t_fields	in;

	champs_nominaux(&in);
	*st = snapshot_fill(snap, &in);
}
