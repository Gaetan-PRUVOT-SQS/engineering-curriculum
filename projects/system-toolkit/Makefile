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

LIB_NAMES := champ collecte format
TEST_NAMES := suite champs cas_bornes cas_seuil cas_comptes cas_champ \
	cas_format cas_erreur lancer

LIB_OBJS := $(addprefix $(BUILD)/,$(addsuffix .o,$(LIB_NAMES)))
LIB_SAN := $(addprefix $(BUILD)/,$(addsuffix -san.o,$(LIB_NAMES)))
LIB_COV := $(addprefix $(BUILD)/,$(addsuffix -cov.o,$(LIB_NAMES)))
TEST_OBJS := $(addprefix $(BUILD)/t-,$(addsuffix .o,$(TEST_NAMES)))
TEST_COV_OBJS := $(addprefix $(BUILD)/t-,$(addsuffix -cov.o,$(TEST_NAMES)))

.PHONY: all test clean debug

all: $(PROG)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: src/%.c src/snapshot.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(BUILD)/%-san.o: src/%.c src/snapshot.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(SAN_CFLAGS) -c -o $@ $<

$(BUILD)/%-cov.o: src/%.c src/snapshot.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(COV_CFLAGS) -c -o $@ $<
	rm -f $(patsubst %.o,%.gcda,$@)

$(PROG): $(BUILD)/main.o $(LIB_OBJS)
	$(CC) $(CFLAGS) -o $@ $(BUILD)/main.o $(LIB_OBJS) $(LDFLAGS)

$(BUILD)/system-info-san: $(BUILD)/main-san.o $(LIB_SAN)
	$(CC) $(SAN_CFLAGS) -o $@ $(BUILD)/main-san.o $(LIB_SAN) $(LDFLAGS)

$(BUILD)/system-info-cov: $(BUILD)/main-cov.o $(LIB_COV)
	$(CC) $(COV_CFLAGS) -o $@ $(BUILD)/main-cov.o $(LIB_COV) $(LDFLAGS)

$(BUILD)/t-%.o: tests/%.c src/snapshot.h tests/suite.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(SAN_CFLAGS) -Isrc -c -o $@ $<

$(BUILD)/t-%-cov.o: tests/%.c src/snapshot.h tests/suite.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(COV_CFLAGS) -Isrc -c -o $@ $<
	rm -f $(patsubst %.o,%.gcda,$@)

$(BUILD)/test-systeme: $(TEST_OBJS) $(LIB_SAN)
	$(CC) $(SAN_CFLAGS) -o $@ $(TEST_OBJS) $(LIB_SAN) $(LDFLAGS)

$(BUILD)/test-systeme-cov: $(TEST_COV_OBJS) $(LIB_COV)
	$(CC) $(COV_CFLAGS) -o $@ $(TEST_COV_OBJS) $(LIB_COV) $(LDFLAGS)

test: $(BUILD)/test-systeme $(PROG) $(BUILD)/system-info-san \
		$(BUILD)/test-systeme-cov $(BUILD)/system-info-cov
	$(BUILD)/test-systeme
	$(BUILD)/test-systeme-cov
	./tests/verifier.sh $(PROG)
	./tests/verifier.sh $(BUILD)/system-info-san
	dash ./tests/verifier.sh $(PROG)
	bash --posix ./tests/verifier.sh $(PROG)
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all --quiet \
		--log-file=$(BUILD)/valgrind.log -- $(PROG) >$(BUILD)/valgrind.out
	bats tests/verifier.bats
	./tests/verifier-gdb.sh $(PROG)
	shellcheck -S info -x -e SC1091 tests/verifier.sh tests/verifier-gdb.sh \
		tests/fixtures/*.sh "tests/fixtures/nom avec espace/conforme.sh"
	checkbashisms -p tests/verifier.sh tests/verifier-gdb.sh \
		tests/fixtures/*.sh "tests/fixtures/nom avec espace/conforme.sh"
	shfmt -ln posix -fn -d tests/verifier.sh tests/verifier-gdb.sh \
		tests/fixtures/*.sh "tests/fixtures/nom avec espace/conforme.sh"
	python3 tests/forme.py src/*.c src/*.h tests/*.c tests/*.h
	if python3 tests/forme.py tests/fixtures/forme-ko.c \
			>$(BUILD)/forme-ko.out 2>&1; then \
		printf '%s\n' 'forme: le cas interdit est passe' >&2; \
		exit 1; \
	fi
	grep -q FORBIDDEN_CS $(BUILD)/forme-ko.out
	$(BUILD)/system-info-cov >$(BUILD)/cov.out
	gcov -b -g -o $(BUILD) $(LIB_COV) $(BUILD)/main-cov.o

debug:
	$(MAKE) clean
	$(MAKE) OPT=-O0 all

clean:
	rm -rf $(BUILD)
	rm -f src/*.gcov tests/*.gcov *.gcov
