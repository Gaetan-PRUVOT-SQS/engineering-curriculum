#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <stddef.h>

#define SNAPSHOT_TEXT_MAX 256

struct system_snapshot {
    char sysname[SNAPSHOT_TEXT_MAX];
    char nodename[SNAPSHOT_TEXT_MAX];
    char release[SNAPSHOT_TEXT_MAX];
    char version[SNAPSHOT_TEXT_MAX];
    char machine[SNAPSHOT_TEXT_MAX];
    long page_size;
    long processors;
};

int snapshot_fill(struct system_snapshot *out, const char *sysname,
                  const char *nodename, const char *release,
                  const char *version, const char *machine, long page_size,
                  long processors);

int snapshot_collect(struct system_snapshot *out);

int snapshot_format(const struct system_snapshot *snap, char *dst,
                    size_t dst_len);

#endif
