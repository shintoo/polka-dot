#ifndef _POLKA_DOT_H_
#define _POLKA_DOT_H_

#define MAXPATH  64
#define MAXFILES 64
#define ERR_MP   1
#define ERR_MF   2

/* Constants for the command provided by the second argument */
enum command {CMD_ERROR, SAVE, APPLY, DELETE, STATUS, ADD, LIST, QUEUE, RESET};

/* Config file: pointer to the file and the number of paths in the file */
struct cfile {
	FILE *file;
	int filecount;
};

/* Print the usage when there is a user error*/
void printUsage(char *str, enum command cmd);

/* Compare a 'string' in a file with a given string str */
int match(FILE *fp, const char *str);

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
 * in the package
 */
void apply(char (*paths)[MAXFILES], char *pkgname, char *name);

/* Remove the package */
void del(char *pkgname);

/* Show files in config */
void queue(int count, char (*paths)[MAXFILES]);

/* Add a path to the config */
void add(char *path, char (*paths)[MAXFILES], struct cfile *config);

/* print the files in a package */
void status(char *pkgname, char *pkgpath);

/* list saved packages */
void list(char *home);

#endif
