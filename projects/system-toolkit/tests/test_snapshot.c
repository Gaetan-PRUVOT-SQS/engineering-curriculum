#include "snapshot.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures;
static int passed;

static const char nominal_texte[] =
    "sysname: Linux\n"
    "nodename: poste\n"
    "release: 6.1.0\n"
    "version: #1 SMP\n"
    "machine: x86_64\n"
    "page_size: 4096\n"
    "processors: 4\n";

static const char page_un_texte[] =
    "sysname: Linux\n"
    "nodename: poste\n"
    "release: 6.1.0\n"
    "version: #1 SMP\n"
    "machine: x86_64\n"
    "page_size: 1\n"
    "processors: 1\n";

static void fail(const char *name)
{
    fprintf(stderr, "echec: %s\n", name);
    failures++;
}

static void run_case(const char *name, void (*fn)(void))
{
    int before;

    before = failures;
    fn();
    if (failures == before) {
        passed++;
        printf("ok %s\n", name);
    } else {
        printf("ko %s\n", name);
    }
}

static int fill_nominal(struct system_snapshot *snap)
{
    return snapshot_fill(snap, "Linux", "poste", "6.1.0", "#1 SMP", "x86_64",
                         4096, 4);
}

static void mark_z(struct system_snapshot *snap)
{
    memset(snap, 'Z', sizeof *snap);
}

static int still_z(const struct system_snapshot *snap)
{
    struct system_snapshot zed;

    memset(&zed, 'Z', sizeof zed);
    return memcmp(snap, &zed, sizeof zed) == 0;
}

static void limite_tampon_nul(void)
{
    struct system_snapshot snap;

    if (fill_nominal(&snap) != 0) {
        fail("preparation");
        return;
    }
    errno = 0;
    if (snapshot_format(&snap, NULL, 32) != -1 || errno != EINVAL) {
        fail("tampon nul");
    }
}

static void limite_taille_zero(void)
{
    struct system_snapshot snap;
    char buf[8];

    if (fill_nominal(&snap) != 0) {
        fail("preparation");
        return;
    }
    memset(buf, 0xA5, sizeof buf);
    errno = 0;
    if (snapshot_format(&snap, buf, 0) != -1 || errno != EINVAL) {
        fail("taille zero");
    }
    if ((unsigned char)buf[0] != 0xA5) {
        fail("taille zero a ecrit");
    }
}

static void limite_taille_un(void)
{
    struct system_snapshot snap;
    char buf[8];

    if (fill_nominal(&snap) != 0) {
        fail("preparation");
        return;
    }
    memset(buf, 0xA5, sizeof buf);
    errno = 0;
    if (snapshot_format(&snap, buf, 1) != -1 || errno != ERANGE) {
        fail("taille un");
    }
    if (buf[0] != '\0' || (unsigned char)buf[1] != 0xA5) {
        fail("debordement taille un");
    }
}

static void limite_taille_exacte(void)
{
    struct system_snapshot snap;
    char buf[512];
    size_t need;

    if (fill_nominal(&snap) != 0) {
        fail("preparation");
        return;
    }
    need = strlen(nominal_texte) + 1;
    memset(buf, 0xA5, sizeof buf);
    if (snapshot_format(&snap, buf, need) != 0) {
        fail("taille exacte");
        return;
    }
    if (strcmp(buf, nominal_texte) != 0) {
        fail("texte exact");
    }
}

static void limite_un_octet_sous_la_taille(void)
{
    struct system_snapshot snap;
    char buf[512];
    size_t need;

    if (fill_nominal(&snap) != 0) {
        fail("preparation");
        return;
    }
    need = strlen(nominal_texte) + 1;
    memset(buf, 0xA5, sizeof buf);
    errno = 0;
    if (snapshot_format(&snap, buf, need - 1) != -1 || errno != ERANGE) {
        fail("un sous la taille");
    }
    if (buf[0] != '\0' || (unsigned char)buf[need - 1] != 0xA5) {
        fail("debordement un sous");
    }
}

static void limite_page_zero(void)
{
    struct system_snapshot snap;

    mark_z(&snap);
    errno = 0;
    if (snapshot_fill(&snap, "Linux", "poste", "6.1.0", "#1 SMP", "x86_64", 0,
                      4)
            != -1
        || errno != EINVAL || !still_z(&snap)) {
        fail("page zero");
    }
}

static void limite_page_un(void)
{
    struct system_snapshot snap;
    char buf[512];

    if (snapshot_fill(&snap, "Linux", "poste", "6.1.0", "#1 SMP", "x86_64", 1,
                      1)
        != 0) {
        fail("page un refusee");
        return;
    }
    if (snapshot_format(&snap, buf, sizeof buf) != 0
        || strcmp(buf, page_un_texte) != 0) {
        fail("page un texte");
    }
}

static void limite_page_negative(void)
{
    struct system_snapshot snap;

    mark_z(&snap);
    errno = 0;
    if (snapshot_fill(&snap, "Linux", "poste", "6.1.0", "#1 SMP", "x86_64", -1,
                      4)
            != -1
        || errno != EINVAL || !still_z(&snap)) {
        fail("page negative");
    }
}

static void limite_processeurs_zero(void)
{
    struct system_snapshot snap;

    mark_z(&snap);
    errno = 0;
    if (snapshot_fill(&snap, "Linux", "poste", "6.1.0", "#1 SMP", "x86_64",
                      4096, 0)
            != -1
        || errno != EINVAL || !still_z(&snap)) {
        fail("processeurs zero");
    }
}

static void limite_processeurs_un(void)
{
    struct system_snapshot snap;
    char buf[512];

    if (snapshot_fill(&snap, "Linux", "poste", "6.1.0", "#1 SMP", "x86_64",
                      4096, 1)
        != 0) {
        fail("processeurs un refuse");
        return;
    }
    if (snapshot_format(&snap, buf, sizeof buf) != 0
        || strstr(buf, "processors: 1\n") == NULL
        || strstr(buf, "page_size: 4096\n") == NULL) {
        fail("processeurs un texte");
    }
}

static void partition_instantane_nul(void)
{
    errno = 0;
    if (snapshot_fill(NULL, "Linux", "poste", "6.1.0", "#1 SMP", "x86_64",
                      4096, 4)
            != -1
        || errno != EINVAL) {
        fail("instantane nul");
    }
    {
        char buf[8];

        errno = 0;
        if (snapshot_format(NULL, buf, sizeof buf) != -1 || errno != EINVAL) {
            fail("format nul");
        }
    }
}

static void partition_champ_vide(void)
{
    struct system_snapshot snap;

    mark_z(&snap);
    errno = 0;
    if (snapshot_fill(&snap, "", "poste", "6.1.0", "#1 SMP", "x86_64", 4096, 4)
            != -1
        || errno != EINVAL || !still_z(&snap)) {
        fail("champ vide");
    }
}

static void partition_champ_nul(void)
{
    struct system_snapshot snap;

    mark_z(&snap);
    errno = 0;
    if (snapshot_fill(&snap, NULL, "poste", "6.1.0", "#1 SMP", "x86_64", 4096,
                      4)
            != -1
        || errno != EINVAL || !still_z(&snap)) {
        fail("champ nul");
    }
}

static void partition_champ_trop_long(void)
{
    struct system_snapshot snap;
    char bad[SNAPSHOT_TEXT_MAX + 1];

    memset(bad, 'b', SNAPSHOT_TEXT_MAX);
    bad[SNAPSHOT_TEXT_MAX] = '\0';
    mark_z(&snap);
    errno = 0;
    if (snapshot_fill(&snap, bad, "poste", "6.1.0", "#1 SMP", "x86_64", 4096,
                      4)
            != -1
        || errno != ERANGE || !still_z(&snap)) {
        fail("champ trop long");
    }
}

static void partition_champ_longueur_max(void)
{
    struct system_snapshot snap;
    char ok[SNAPSHOT_TEXT_MAX];
    char buf[2048];
    char attendu[SNAPSHOT_TEXT_MAX + 16];

    memset(ok, 'a', SNAPSHOT_TEXT_MAX - 1);
    ok[SNAPSHOT_TEXT_MAX - 1] = '\0';
    if (snapshot_fill(&snap, ok, "poste", "6.1.0", "#1 SMP", "x86_64", 4096, 4)
        != 0) {
        fail("longueur max refusee");
        return;
    }
    if (snapshot_format(&snap, buf, sizeof buf) != 0) {
        fail("longueur max format");
        return;
    }
    snprintf(attendu, sizeof attendu, "sysname: %s\n", ok);
    if (strncmp(buf, attendu, strlen(attendu)) != 0) {
        fail("longueur max texte");
    }
}

static void partition_saut_de_ligne_remplissage(void)
{
    struct system_snapshot snap;

    mark_z(&snap);
    errno = 0;
    if (snapshot_fill(&snap, "Lin\nux", "poste", "6.1.0", "#1 SMP", "x86_64",
                      4096, 4)
            != -1
        || errno != EINVAL || !still_z(&snap)) {
        fail("saut remplissage");
    }
}

static void partition_saut_de_ligne_format(void)
{
    struct system_snapshot snap;
    char buf[512];

    if (fill_nominal(&snap) != 0) {
        fail("preparation");
        return;
    }
    snap.sysname[3] = '\n';
    memset(buf, 0xA5, sizeof buf);
    errno = 0;
    if (snapshot_format(&snap, buf, sizeof buf) != -1 || errno != EINVAL) {
        fail("saut format");
    }
    if ((unsigned char)buf[0] != 0xA5) {
        fail("saut format a ecrit");
    }
}

static void partition_champ_sans_nul(void)
{
    struct system_snapshot snap;
    char buf[32];

    memset(&snap, 'A', sizeof snap);
    snap.page_size = 4096;
    snap.processors = 4;
    memset(buf, 0xA5, sizeof buf);
    errno = 0;
    if (snapshot_format(&snap, buf, sizeof buf) != -1 || errno != EINVAL) {
        fail("sans nul");
    }
    if ((unsigned char)buf[0] != 0xA5) {
        fail("sans nul a ecrit");
    }
}

static void partition_nominale(void)
{
    struct system_snapshot snap;
    char buf[512];

    if (fill_nominal(&snap) != 0) {
        fail("preparation");
        return;
    }
    if (snapshot_format(&snap, buf, sizeof buf) != 0
        || strcmp(buf, nominal_texte) != 0) {
        fail("nominal");
    }
}

static void partition_collecte_nulle(void)
{
    errno = 0;
    if (snapshot_collect(NULL) != -1 || errno != EINVAL) {
        fail("collecte nulle");
    }
}

static void erreur_format_page_zero(void)
{
    struct system_snapshot snap;
    char buf[512];

    if (fill_nominal(&snap) != 0) {
        fail("preparation");
        return;
    }
    snap.page_size = 0;
    memset(buf, 0xA5, sizeof buf);
    errno = 0;
    if (snapshot_format(&snap, buf, sizeof buf) != -1 || errno != EINVAL) {
        fail("format page zero");
    }
    if ((unsigned char)buf[0] != 0xA5) {
        fail("format page zero a ecrit");
    }
}

static void erreur_format_champ_vide(void)
{
    struct system_snapshot snap;
    char buf[512];

    if (fill_nominal(&snap) != 0) {
        fail("preparation");
        return;
    }
    snap.release[0] = '\0';
    errno = 0;
    if (snapshot_format(&snap, buf, sizeof buf) != -1 || errno != EINVAL) {
        fail("format champ vide");
    }
}

int main(void)
{
    run_case("limite tampon nul", limite_tampon_nul);
    run_case("limite taille zero", limite_taille_zero);
    run_case("limite taille un", limite_taille_un);
    run_case("limite taille exacte", limite_taille_exacte);
    run_case("limite un octet sous la taille", limite_un_octet_sous_la_taille);
    run_case("limite page zero", limite_page_zero);
    run_case("limite page un", limite_page_un);
    run_case("limite page negative", limite_page_negative);
    run_case("limite processeurs zero", limite_processeurs_zero);
    run_case("limite processeurs un", limite_processeurs_un);
    run_case("partition instantane nul", partition_instantane_nul);
    run_case("partition champ vide", partition_champ_vide);
    run_case("partition champ nul", partition_champ_nul);
    run_case("partition champ trop long", partition_champ_trop_long);
    run_case("partition champ longueur max", partition_champ_longueur_max);
    run_case("partition saut de ligne remplissage",
             partition_saut_de_ligne_remplissage);
    run_case("partition saut de ligne format", partition_saut_de_ligne_format);
    run_case("partition champ sans nul", partition_champ_sans_nul);
    run_case("partition nominale", partition_nominale);
    run_case("partition collecte nulle", partition_collecte_nulle);
    run_case("erreur format page zero", erreur_format_page_zero);
    run_case("erreur format champ vide", erreur_format_champ_vide);
    printf("cas: %d passes, %d echecs\n", passed, failures);
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
