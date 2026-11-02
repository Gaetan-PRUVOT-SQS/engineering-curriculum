#!/bin/sh
set -eu
printf 'sysname: %s\n' PasLinux
printf 'nodename: %s\n' "$(uname -n)"
printf 'release: %s\n' "$(uname -r)"
printf 'version: %s\n' "$(uname -v)"
printf 'machine: %s\n' "$(uname -m)"
printf 'page_size: %s\n' "$(getconf PAGE_SIZE)"
printf 'processors: %s\n' "$(getconf _NPROCESSORS_ONLN)"
