# Environnement

Relevé le 2026-11-02 sur le poste qui a servi à créer ce dépôt.

| Élément | Valeur |
| --- | --- |
| Machine | MEDION Crawler E30e |
| Processeur | 12th Gen Intel Core i5-12450H |
| Processeurs en ligne | 12 (`nproc`, `getconf _NPROCESSORS_ONLN`) |
| Mémoire | `MemTotal` 15526852 kB |
| Système | Ubuntu 26.04.1 LTS (Resolute Raccoon) |
| Noyau | Linux 7.0.0-42-generic, x86_64 |
| glibc | 2.43 (Ubuntu GLIBC 2.43-2ubuntu2.4) |
| GCC | 15.2.0, paquet Ubuntu 15.2.0-16ubuntu1, cible `x86_64-linux-gnu` |
| GDB | 17.1, paquet Ubuntu 17.1-2ubuntu1 |
| Git | 2.53.0 |
| Make | GNU Make 4.4.1 |
| Bash | 5.3.9 |
| `getconf _POSIX_VERSION` | 200809 |

Le programme C est compilé avec `-std=c17 -D_POSIX_C_SOURCE=202405L`. `getconf` rapporte encore 200809. Ce poste n'est pas présenté comme une certification POSIX.

## Relancer les contrôles

```
gcc --version
gdb --version
git --version
make --version
uname -srm
nproc
getconf PAGE_SIZE
getconf _NPROCESSORS_ONLN
getconf _POSIX_VERSION
awk '/MemTotal/ { print }' /proc/meminfo
```

Le 2026-11-02, `gcc --version` commence par `gcc (Ubuntu 15.2.0-16ubuntu1) 15.2.0`. `gdb --version` commence par `GNU gdb (Ubuntu 17.1-2ubuntu1) 17.1`.

GDB exécute une expression. La commande suivante affiche `$1 = 2` et retourne 0 :

```
gdb -batch -ex 'print 1+1' -ex quit
```

Le même jour, `system-info` se compile sans avertissement (`-Wall -Wextra -Werror`) et GDB s'arrête sur `main`. La commande et la ligne observée sont dans `projects/system-toolkit/docs/gdb.md`.

## Git

Branche par défaut : `main`. Les commits de ce dépôt portent l'adresse noreply du compte GitHub qui le publie. Cette adresse n'est pas recopiée dans les fichiers. Le remote `origin` pointe vers `https://github.com/Gaetan-PRUVOT-SQS/engineering-curriculum.git`.
