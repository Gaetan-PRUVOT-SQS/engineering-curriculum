SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c

.PHONY: test check

test check:
	$(MAKE) -C projects/system-toolkit test
