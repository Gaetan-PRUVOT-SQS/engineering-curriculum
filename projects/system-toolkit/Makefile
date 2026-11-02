SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c

CC := gcc
OPT := -O2
CFLAGS := -std=c17 -Wall -Wextra -Werror -Wpedantic -Wformat=2 -Wshadow \
	-Wstrict-prototypes -Wnull-dereference $(OPT) -g
CPPFLAGS := -D_POSIX_C_SOURCE=202405L
LDFLAGS :=
SANITIZE := -fsanitize=address,undefined
SAN_CFLAGS := -std=c17 -Wall -Wextra -Werror -Wpedantic -Wformat=2 -Wshadow \
	-Wstrict-prototypes -Wnull-dereference -O1 -g $(SANITIZE)
COV_CFLAGS := -std=c17 -Wall -Wextra -Werror -Wpedantic -Wformat=2 -Wshadow \
	-Wstrict-prototypes -Wnull-dereference -O0 -g --coverage \
	-fprofile-abs-path -fcondition-coverage

BUILD := build
PROG := $(BUILD)/system-info

.PHONY: all test clean debug

all: $(PROG)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/snapshot.o: src/snapshot.c src/snapshot.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ src/snapshot.c

$(BUILD)/main.o: src/main.c src/snapshot.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ src/main.c

$(PROG): $(BUILD)/main.o $(BUILD)/snapshot.o
	$(CC) $(CFLAGS) -o $@ $(BUILD)/main.o $(BUILD)/snapshot.o $(LDFLAGS)

$(BUILD)/snapshot-san.o: src/snapshot.c src/snapshot.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(SAN_CFLAGS) -c -o $@ src/snapshot.c

$(BUILD)/main-san.o: src/main.c src/snapshot.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(SAN_CFLAGS) -c -o $@ src/main.c

$(BUILD)/system-info-san: $(BUILD)/main-san.o $(BUILD)/snapshot-san.o
	$(CC) $(SAN_CFLAGS) -o $@ $(BUILD)/main-san.o $(BUILD)/snapshot-san.o $(LDFLAGS)

$(BUILD)/test-snapshot.o: tests/test_snapshot.c src/snapshot.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(SAN_CFLAGS) -Isrc -c -o $@ tests/test_snapshot.c

$(BUILD)/test-snapshot: $(BUILD)/test-snapshot.o $(BUILD)/snapshot-san.o
	$(CC) $(SAN_CFLAGS) -o $@ $(BUILD)/test-snapshot.o $(BUILD)/snapshot-san.o $(LDFLAGS)

$(BUILD)/snapshot-cov.o: src/snapshot.c src/snapshot.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(COV_CFLAGS) -c -o $@ src/snapshot.c

$(BUILD)/test-snapshot-cov.o: tests/test_snapshot.c src/snapshot.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(COV_CFLAGS) -Isrc -c -o $@ tests/test_snapshot.c

$(BUILD)/test-snapshot-cov: $(BUILD)/test-snapshot-cov.o $(BUILD)/snapshot-cov.o
	$(CC) $(COV_CFLAGS) -o $@ $(BUILD)/test-snapshot-cov.o $(BUILD)/snapshot-cov.o $(LDFLAGS)

$(BUILD)/main-cov.o: src/main.c src/snapshot.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(COV_CFLAGS) -c -o $@ src/main.c

$(BUILD)/system-info-cov: $(BUILD)/main-cov.o $(BUILD)/snapshot-cov.o
	$(CC) $(COV_CFLAGS) -o $@ $(BUILD)/main-cov.o $(BUILD)/snapshot-cov.o $(LDFLAGS)

test: $(BUILD)/test-snapshot $(PROG) $(BUILD)/system-info-san \
		$(BUILD)/test-snapshot-cov $(BUILD)/system-info-cov
	$(BUILD)/test-snapshot
	$(BUILD)/test-snapshot-cov
	./tests/verifier.sh $(PROG)
	./tests/verifier.sh $(BUILD)/system-info-san
	dash ./tests/verifier.sh $(PROG)
	bash --posix ./tests/verifier.sh $(PROG)
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all --quiet \
		--log-file=$(BUILD)/valgrind.log -- $(PROG) >$(BUILD)/valgrind.out
	bats tests/verifier.bats
	shellcheck -S info -x -e SC1091 tests/verifier.sh tests/fixtures/*.sh \
		"tests/fixtures/nom avec espace/conforme.sh"
	checkbashisms -p tests/verifier.sh tests/fixtures/*.sh \
		"tests/fixtures/nom avec espace/conforme.sh"
	shfmt -ln posix -fn -d tests/verifier.sh tests/fixtures/*.sh \
		"tests/fixtures/nom avec espace/conforme.sh"
	$(BUILD)/system-info-cov >$(BUILD)/cov.out
	gcov -b -g -o $(BUILD) $(BUILD)/snapshot-cov.o $(BUILD)/main-cov.o

debug:
	$(MAKE) clean
	$(MAKE) OPT=-O0 all

clean:
	rm -rf $(BUILD)
	rm -f src/*.gcov tests/*.gcov *.gcov
