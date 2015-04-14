#define MAXPATH  64
#define MAXFILES 64
#define ERR_MP   1
#define ERR_MF   2

int readconfig(FILE *config, char **files);

void save(FILE *config, char *pkgname);

void apply(FILE *config, char *pkgname);

void rm(char *pkgname);
