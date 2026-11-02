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

La suite compile les tests avec ASan et UBSan, compare le binaire à `uname` et `getconf`, relance cette comparaison sous `dash` et `bash --posix`, passe Valgrind, lance bats, arrête GDB sur `main`, lance shellcheck, checkbashisms et shfmt, puis mesure la couverture de `champ.c`, `collecte.c`, `format.c` et `main.c`.

Pour s'arrêter dans le programme :

```
make
gdb -batch -ex 'break main' -ex 'run >/dev/null' -ex 'info line' -ex quit ./build/system-info
```

Le détail est dans `docs/gdb.md`.

## Fichiers

- `src/champ.c` : copie des champs
- `src/collecte.c` : lecture `uname` et `sysconf`
- `src/format.c` : mise en forme des sept lignes
- `src/snapshot.h` : structures et prototypes
- `src/main.c` : entrée et sortie
- `tests/` : limites, partitions, comparaison à `uname`
- `docs/contrat.md` : lignes, erreurs, codes de sortie
