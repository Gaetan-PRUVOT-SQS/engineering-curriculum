#include "snapshot.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>

_Static_assert(SNAPSHOT_TEXT_MAX > 1, "champ trop court");

static int copy_field(char *dst, size_t dst_len, const char *src)
{
    int written;

    if (src == NULL) {
        errno = EINVAL;
        return -1;
    }
    written = snprintf(dst, dst_len, "%s", src);
    if (written < 0) {
        errno = EIO;
        return -1;
    }
    if ((size_t)written >= dst_len) {
        if (dst_len > 0) {
            dst[0] = '\0';
        }
        errno = ERANGE;
        return -1;
    }
    if (written == 0 || memchr(src, '\n', (size_t)written) != NULL) {
        dst[0] = '\0';
        errno = EINVAL;
        return -1;
    }
    return 0;
}

int snapshot_fill(struct system_snapshot *out, const char *sysname,
                  const char *nodename, const char *release,
                  const char *version, const char *machine, long page_size,
                  long processors)
{
    struct system_snapshot tmp;

    if (out == NULL) {
        errno = EINVAL;
        return -1;
    }
    if (page_size < 1 || processors < 1) {
        errno = EINVAL;
        return -1;
    }
    memset(&tmp, 0, sizeof tmp);
    if (copy_field(tmp.sysname, sizeof tmp.sysname, sysname) != 0
        || copy_field(tmp.nodename, sizeof tmp.nodename, nodename) != 0
        || copy_field(tmp.release, sizeof tmp.release, release) != 0
        || copy_field(tmp.version, sizeof tmp.version, version) != 0
        || copy_field(tmp.machine, sizeof tmp.machine, machine) != 0) {
        return -1;
    }
    tmp.page_size = page_size;
    tmp.processors = processors;
    *out = tmp;
    return 0;
}

int snapshot_collect(struct system_snapshot *out)
{
    struct utsname raw;
    long page_size;
    long processors;

    if (out == NULL) {
        errno = EINVAL;
        return -1;
    }
    if (uname(&raw) != 0) {
        return -1;
    }
    errno = 0;
    page_size = sysconf(_SC_PAGESIZE);
    if (page_size < 1) {
        if (errno == 0) {
            errno = EINVAL;
        }
        return -1;
    }
    errno = 0;
    processors = sysconf(_SC_NPROCESSORS_ONLN);
    if (processors < 1) {
        if (errno == 0) {
            errno = EINVAL;
        }
        return -1;
    }
    return snapshot_fill(out, raw.sysname, raw.nodename, raw.release,
                         raw.version, raw.machine, page_size, processors);
}

static int field_text_ok(const char *text, size_t cap)
{
    size_t n;

    n = strnlen(text, cap);
    if (n == 0 || n == cap) {
        return 0;
    }
    if (memchr(text, '\n', n) != NULL) {
        return 0;
    }
    return 1;
}

int snapshot_format(const struct system_snapshot *snap, char *dst,
                    size_t dst_len)
{
    int written;

    if (snap == NULL || dst == NULL || dst_len == 0) {
        errno = EINVAL;
        return -1;
    }
    if (snap->page_size < 1 || snap->processors < 1) {
        errno = EINVAL;
        return -1;
    }
    if (!field_text_ok(snap->sysname, sizeof snap->sysname)
        || !field_text_ok(snap->nodename, sizeof snap->nodename)
        || !field_text_ok(snap->release, sizeof snap->release)
        || !field_text_ok(snap->version, sizeof snap->version)
        || !field_text_ok(snap->machine, sizeof snap->machine)) {
        errno = EINVAL;
        return -1;
    }
    written = snprintf(dst, dst_len,
                       "sysname: %s\n"
                       "nodename: %s\n"
                       "release: %s\n"
                       "version: %s\n"
                       "machine: %s\n"
                       "page_size: %ld\n"
                       "processors: %ld\n",
                       snap->sysname, snap->nodename, snap->release,
                       snap->version, snap->machine, snap->page_size,
                       snap->processors);
    if (written < 0) {
        dst[0] = '\0';
        errno = EIO;
        return -1;
    }
    if ((size_t)written >= dst_len) {
        dst[0] = '\0';
        errno = ERANGE;
        return -1;
    }
    return 0;
}
