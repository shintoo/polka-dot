#define MAXPATH  1
#define MAXFILEs 2 

int readconfig(FILE *config, char **files);

void save(FILE *config, char *pkgname);

void apply(FILE *config, char *pkgname);

void rm(char *pkgname);
