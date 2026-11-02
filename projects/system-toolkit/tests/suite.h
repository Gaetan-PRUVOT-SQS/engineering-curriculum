#ifndef SUITE_H
# define SUITE_H

# include "snapshot.h"

void	suite_echec(int *echecs, const char *nom);
void	noter(const char *nom, void (*fn)(int *ko), int *passes, int *echecs);
void	refuse_remplissage(int *ko, const t_fields *in, int err,
			const char *nom);
void	champs_nominaux(t_fields *in);
void	texte_attendu(char *dst, size_t cap, long page, long procs);
void	marquer_z(t_snapshot *snap);
void	encore_z(const t_snapshot *snap, int *reste);
void	remplir(t_snapshot *snap, int *st);
void	lancer_bornes(int *passes, int *echecs);
void	lancer_seuil(int *passes, int *echecs);
void	lancer_comptes(int *passes, int *echecs);
void	lancer_champs(int *passes, int *echecs);
void	lancer_format(int *passes, int *echecs);
void	lancer_erreurs(int *passes, int *echecs);

#endif
