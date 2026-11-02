#!/bin/sh
set -eu
here=$(CDPATH='' cd -- "$(dirname -- "$0")" && pwd)
text=$("$here/conforme.sh")
printf '%s' "$text"
