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

field()
{
	key=$1
	file=$2
	awk -v k="$key" '
		index($0, k ": ") == 1 {
			print substr($0, length(k) + 3)
			n++
		}
		END { if (n != 1) exit 1 }
	' "$file"
}

check_field()
{
	key=$1
	exp=$2
	got=$(field "$key" "$out") || die "champ ${key} illisible" 1
	if [ "$got" != "$exp" ]; then
		printf '%s: %s: attendu <%s> obtenu <%s>\n' \
			"${0##*/}" "$key" "$exp" "$got" >&2
		exit 1
	fi
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
	out=build/verifier.out
	err=build/verifier.err
	if ! "$prog" >"$out" 2>"$err"; then
		die "le programme a echoue" 1
	fi
	if [ -s "$err" ]; then
		die "stderr non vide" 1
	fi
	end=$(tail -c 1 "$out")
	if [ "$end" != "$(printf '\n')" ]; then
		die "la sortie ne finit pas par un saut de ligne" 1
	fi
	n=$(awk 'END { print NR }' "$out")
	if [ "$n" -ne 7 ]; then
		die "attendu 7 lignes" 1
	fi
	check_field sysname "$(uname -s)"
	check_field nodename "$(uname -n)"
	check_field release "$(uname -r)"
	check_field version "$(uname -v)"
	check_field machine "$(uname -m)"
	check_field page_size "$(getconf PAGE_SIZE)"
	check_field processors "$(getconf _NPROCESSORS_ONLN)"
}

main "$@"
