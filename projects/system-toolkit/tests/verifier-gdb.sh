#!/bin/sh
set -eu

usage()
{
	printf 'usage: %s programme\n' "${0##*/}" >&2
}

die()
{
	printf '%s: %s\n' "${0##*/}" "$1" >&2
	exit "$2"
}

main()
{
	prog=${1-}
	root=$(CDPATH='' cd -- "$(dirname -- "$0")/.." && pwd)
	cd -- "$root"

	if [ "$#" -ne 1 ]; then
		usage
		exit 2
	fi
	if [ ! -f "$prog" ] || [ ! -x "$prog" ]; then
		die "programme non executable" 2
	fi
	mkdir -p build
	gdb -batch \
		-ex 'set confirm off' \
		-ex 'set pagination off' \
		-ex 'set debuginfod enabled off' \
		-ex 'break main' \
		-ex 'run >/dev/null' \
		-ex 'info line' \
		-ex continue \
		-ex quit \
		-- "$prog" >build/gdb.out 2>&1
	if ! grep -F 'Breakpoint 1, main ()' build/gdb.out >/dev/null; then
		die "pas d'arret sur main" 1
	fi
	if ! grep -F 'exited normally' build/gdb.out >/dev/null; then
		die "le processus ne s'est pas termine" 1
	fi
}

main "$@"
