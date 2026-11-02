#include "snapshot.h"

#include <errno.h>
#include <sys/utsname.h>
#include <unistd.h>

static long	lire_compte(int nom)
{
	long	valeur;

	errno = 0;
	valeur = sysconf(nom);
	if (valeur < 1 && errno == 0)
		errno = EINVAL;
	return (valeur);
}

int	snapshot_collect(t_snapshot *out)
{
	struct utsname	raw;
	t_fields		in;

	if (out == NULL)
	{
		errno = EINVAL;
		return (-1);
	}
	if (uname(&raw) != 0)
		return (-1);
	in.page_size = lire_compte(_SC_PAGESIZE);
	if (in.page_size < 1)
		return (-1);
	in.processors = lire_compte(_SC_NPROCESSORS_ONLN);
	if (in.processors < 1)
		return (-1);
	in.sysname = raw.sysname;
	in.nodename = raw.nodename;
	in.release = raw.release;
	in.version = raw.version;
	in.machine = raw.machine;
	return (snapshot_fill(out, &in));
}
