#define MAXPATH  64
#define MAXFILES 64
#define ERR_MP   1
#define ERR_MF   2

/* Constants for the command provided by the second argument */
enum command {CMD_ERROR, SAVE, APPLY, REMOVE, LIST};

/* Config file: pointer to the file and the number of paths in the file */
struct cfile {
	FILE *file;
	int filecount;
};

/* Print the usage when there is a user error*/
void printUsage(char *str, enum command cmd);

/* Find the location of a string in a file */
int findString(FILE *fp, char *str, fpos_t *strloc);

/* Retrieve the command provided by the second argument */
int getcmd(int argc, char **args);

/* Read the config file into a cfile struct */
int readConfig(struct cfile *config, char (*paths)[MAXFILES]);

/* Save the contents of the files specified in the 
 * config file into a package
 */
void save(struct cfile *config, char (*paths)[MAXFILES], char *pkgname, char *name);

/* Apply the contents of a package to the files
 * specified in the config file
 */
void apply(struct cfile *config, char (*paths)[MAXFILES], char *pkgname, char *name);

/* Remove the package */
void rm(char *pkgname);
