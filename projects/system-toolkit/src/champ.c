#include "snapshot.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

static int	copy_field(char *dst, size_t cap, const char *src)
{
	int	n;

	n = -1;
	if (src != NULL)
		n = snprintf(dst, cap, "%s", src);
	if (src != NULL && n >= 0 && (size_t)n < cap && n > 0
		&& memchr(src, '\n', (size_t)n) == NULL)
		return (0);
	if (n >= 0 && cap > 0)
		dst[0] = '\0';
	errno = EINVAL;
	if (src != NULL && n < 0)
		errno = EIO;
	if (src != NULL && n >= 0 && (size_t)n >= cap)
		errno = ERANGE;
	return (-1);
}

int	snapshot_fill(t_snapshot *out, const t_fields *in)
{
	t_snapshot	tmp;

	if (out == NULL || in == NULL || in->page_size < 1
		|| in->processors < 1)
	{
		errno = EINVAL;
		return (-1);
	}
	memset(&tmp, 0, sizeof(tmp));
	if (copy_field(tmp.sysname, sizeof(tmp.sysname), in->sysname) != 0
		|| copy_field(tmp.nodename, sizeof(tmp.nodename), in->nodename) != 0
		|| copy_field(tmp.release, sizeof(tmp.release), in->release) != 0
		|| copy_field(tmp.version, sizeof(tmp.version), in->version) != 0
		|| copy_field(tmp.machine, sizeof(tmp.machine), in->machine) != 0)
		return (-1);
	tmp.page_size = in->page_size;
	tmp.processors = in->processors;
	*out = tmp;
	return (0);
}
