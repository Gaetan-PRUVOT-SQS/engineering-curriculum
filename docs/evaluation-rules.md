# Règles d'évaluation

Un palier est accepté quand les cinq points tiennent en même temps.

| Critère | Preuve |
| --- | --- |
| Compilation sans erreur | `make -C projects/system-toolkit` retourne 0, avec `-Wall -Wextra -Werror` |
| Code propre et documenté | le comportement est dans `projects/system-toolkit/docs/contrat.md`, les noms portent le sens |
| README reproductible | un clone suivi des commandes du README racine rebuild et relance la suite |
| Historique Git cohérent | `git log --oneline` : une idée par commit, message court en français |
| Démonstration | `./projects/system-toolkit/build/system-info` affiche les sept champs, et `make test` les compare à la machine |

Un seul point manquant suffit à refuser le palier. Une suite verte ne remplace pas la lecture du contrat : le test doit échouer si un champ est renommé ou tronqué.
