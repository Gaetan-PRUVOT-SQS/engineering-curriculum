# Contrat de system-info

Le programme écrit sept lignes sur la sortie standard, dans cet ordre, et rien sur l'erreur. Il retourne 0.

```
sysname: Linux
nodename: poste
release: 6.1.0
version: #1 SMP
machine: x86_64
page_size: 4096
processors: 4
```

Les valeurs de l'exemple sont fictives. À l'exécution, elles viennent de `uname` (`sysname`, `nodename`, `release`, `version`, `machine`), de `sysconf(_SC_PAGESIZE)` et de `sysconf(_SC_NPROCESSORS_ONLN)`.

Chaque ligne est `clé: valeur`. La valeur est le reste de la ligne. Les clés sont fixes. Un champ texte vide, trop long pour 255 octets, ou qui contient un saut de ligne, est refusé. `page_size` et `processors` doivent être au moins 1.

Si la lecture, la mise en forme ou l'écriture échoue, le programme retourne 1 et écrit une ligne sur l'erreur. Il n'écrit le rapport que lorsque la mise en forme a tenu dans le tampon.

`snapshot_fill` ne modifie pas la structure de sortie quand il échoue. `snapshot_format` n'écrit pas au-delà du tampon demandé. Si le tampon est trop court et non nul, son premier octet devient un caractère nul.
