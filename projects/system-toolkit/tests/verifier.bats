#!/usr/bin/env bats

bats_require_minimum_version 1.5.0

setup() {
	ROOT="$(CDPATH='' cd -- "$BATS_TEST_DIRNAME/.." && pwd)"
	VERIFIER="$ROOT/tests/verifier.sh"
}

@test "usage sans argument" {
	run -2 "$VERIFIER"
	[[ "$output" == *"usage:"* ]]
}

@test "gdb usage sans argument" {
	run -2 "$ROOT/tests/verifier-gdb.sh"
	[[ "$output" == *"usage:"* ]]
}

@test "deux arguments" {
	run -2 "$VERIFIER" /bin/true /bin/true
	[[ "$output" == *"usage:"* ]]
}

@test "programme absent" {
	run -2 "$VERIFIER" "$ROOT/tests/fixtures/absent"
	[[ "$output" == *"programme non executable"* ]]
}

@test "programme non executable" {
	run -2 "$VERIFIER" "$ROOT/tests/fixtures/pas-executable"
	[[ "$output" == *"programme non executable"* ]]
}

@test "sortie conforme" {
	run -0 "$VERIFIER" "$ROOT/tests/fixtures/conforme.sh"
}

@test "chemin avec espace" {
	run -0 "$VERIFIER" "$ROOT/tests/fixtures/nom avec espace/conforme.sh"
}

@test "sysname faux" {
	run -1 "$VERIFIER" "$ROOT/tests/fixtures/sysname-faux.sh"
	[[ "$output" == *"sysname:"* ]]
	[[ "$output" == *"PasLinux"* ]]
}

@test "page_size faux" {
	run -1 "$VERIFIER" "$ROOT/tests/fixtures/page-faux.sh"
	[[ "$output" == *"page_size:"* ]]
	[[ "$output" == *"attendu <"* ]]
}

@test "ligne en trop" {
	run -1 "$VERIFIER" "$ROOT/tests/fixtures/ligne-en-trop.sh"
	[[ "$output" == *"attendu 7 lignes"* ]]
}

@test "trop court" {
	run -1 "$VERIFIER" "$ROOT/tests/fixtures/trop-court.sh"
	[[ "$output" == *"attendu 7 lignes"* ]]
}

@test "stderr non vide" {
	run -1 "$VERIFIER" "$ROOT/tests/fixtures/stderr-bruit.sh"
	[[ "$output" == *"stderr non vide"* ]]
}

@test "code de sortie 1" {
	run -1 "$VERIFIER" "$ROOT/tests/fixtures/echec.sh"
	[[ "$output" == *"le programme a echoue"* ]]
}

@test "sans saut de ligne final" {
	run -1 "$VERIFIER" "$ROOT/tests/fixtures/sans-saut.sh"
	[[ "$output" == *"saut de ligne"* ]]
}
