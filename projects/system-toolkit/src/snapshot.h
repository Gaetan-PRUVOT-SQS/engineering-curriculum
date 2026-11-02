#ifndef SNAPSHOT_H
# define SNAPSHOT_H

# include <stddef.h>

# define SNAPSHOT_TEXT_MAX 256

typedef struct s_fields
{
	const char	*sysname;
	const char	*nodename;
	const char	*release;
	const char	*version;
	const char	*machine;
	long		page_size;
	long		processors;
}	t_fields;

typedef struct s_snapshot
{
	char	sysname[SNAPSHOT_TEXT_MAX];
	char	nodename[SNAPSHOT_TEXT_MAX];
	char	release[SNAPSHOT_TEXT_MAX];
	char	version[SNAPSHOT_TEXT_MAX];
	char	machine[SNAPSHOT_TEXT_MAX];
	long	page_size;
	long	processors;
}	t_snapshot;

int	snapshot_fill(t_snapshot *out, const t_fields *in);
int	snapshot_collect(t_snapshot *out);
int	snapshot_format(const t_snapshot *snap, char *dst, size_t dst_len);

#endif
