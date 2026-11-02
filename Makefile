SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c

.PHONY: test check

test:
	$(MAKE) -C projects/system-toolkit test

check: test
