# system-toolkit

`system-info` affiche le système, le nœud, le noyau, sa version, l'architecture, la taille de page et le nombre de processeurs en ligne.

## Compiler et lancer

Depuis ce dossier :

```
make
./build/system-info
```

Depuis la racine du dépôt, `make test` lance la suite. `make -C projects/system-toolkit` produit le binaire.

GCC est appelé avec `-std=c17 -D_POSIX_C_SOURCE=202405L -Wall -Wextra -Werror`. La sortie du programme est le contrat de `docs/contrat.md`.

## Tests

```
make test
```

La suite compile les tests avec ASan et UBSan, compare le binaire à `uname` et `getconf`, relance cette comparaison sous `dash` et `bash --posix`, passe Valgrind, lance bats, shellcheck, checkbashisms et shfmt, puis mesure la couverture de `snapshot.c` et `main.c`.

Pour s'arrêter dans le programme :

```
make
gdb -batch -ex 'break main' -ex 'run >/dev/null' -ex 'info line' -ex quit ./build/system-info
```

Le détail est dans `docs/gdb.md`.

## Fichiers

- `src/snapshot.c` : lecture et mise en forme
- `src/main.c` : entrée et sortie
- `tests/test_snapshot.c` : limites et partitions
- `tests/verifier.sh` : comparaison à `uname` et `getconf`
- `docs/contrat.md` : lignes, erreurs, codes de sortie
