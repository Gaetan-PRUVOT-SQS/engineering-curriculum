#include "snapshot.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

static int	champ_invalide(const char *text, size_t cap)
{
	size_t	n;

	n = strnlen(text, cap);
	if (n == 0 || n == cap)
		return (1);
	if (memchr(text, '\n', n) != NULL)
		return (1);
	return (0);
}

static int	ecrire_lignes(const t_snapshot *snap, char *dst, size_t dst_len)
{
	int	written;

	written = snprintf(dst, dst_len,
			"sysname: %s\nnodename: %s\nrelease: %s\nversion: %s\n"
			"machine: %s\npage_size: %ld\nprocessors: %ld\n",
			snap->sysname, snap->nodename, snap->release, snap->version,
			snap->machine, snap->page_size, snap->processors);
	if (written < 0 || (size_t)written >= dst_len)
	{
		dst[0] = '\0';
		errno = ERANGE;
		if (written < 0)
			errno = EIO;
		return (-1);
	}
	return (0);
}

int	snapshot_format(const t_snapshot *snap, char *dst, size_t dst_len)
{
	if (snap == NULL || dst == NULL || dst_len == 0
		|| snap->page_size < 1 || snap->processors < 1)
	{
		errno = EINVAL;
		return (-1);
	}
	if (champ_invalide(snap->sysname, sizeof(snap->sysname)) != 0
		|| champ_invalide(snap->nodename, sizeof(snap->nodename)) != 0
		|| champ_invalide(snap->release, sizeof(snap->release)) != 0
		|| champ_invalide(snap->version, sizeof(snap->version)) != 0
		|| champ_invalide(snap->machine, sizeof(snap->machine)) != 0)
	{
		errno = EINVAL;
		return (-1);
	}
	return (ecrire_lignes(snap, dst, dst_len));
}
