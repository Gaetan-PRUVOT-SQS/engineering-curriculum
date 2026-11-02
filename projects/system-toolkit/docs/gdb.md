# GDB sur system-info

Depuis `projects/system-toolkit`, après `make` :

```
gdb -batch -ex 'set confirm off' -ex 'set pagination off' -ex 'set debuginfod enabled off' -ex 'break main' -ex 'run >/dev/null' -ex 'info line' -ex continue -ex quit ./build/system-info
```

`run >/dev/null` envoie la sortie du programme à l'écart, pour ne garder que le dialogue du débogueur. On attend un arrêt sur `main`, puis la fin du processus.

Le 2026-11-02, GDB 17.1 s'est arrêté sur `Breakpoint 1, main () at src/main.c:9`, puis l'inférieur s'est terminé normalement. Le code de sortie de GDB était 0.

La commande `gdb -batch -ex 'print 1+1' -ex quit`, sans programme, est notée dans `00-environment/README.md`.
