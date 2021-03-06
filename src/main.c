//#define DEBUG

/* polka-dot: The simple configuration manager.
 * 
 * See README.md for installation and usage.
 *
 * TODO:
 *		- Make apply() not use the config file, but the files			********
 *		  in the package
 *		- Ask user to confirm if when saving a package,					***
 *		  there is already a package with that name.
 *		- Add option to 'apply' that allows user to apply the			*
 *		  package but excludes a specified filename(s), i.e.
 *		  `polka-dot apply --exclude file1 file2`
 *		- Add rm:  	  takes filename argument, removes it from config	**
 *		- Improve portability of packages (handling of home and ~ in save and apply)
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "polka-dot.h"

int main(int argc, char **argv) {
	enum command cmd;
	char *home, conf[64];
	char paths[MAXFILES][MAXPATH];
	char pkgname[128];
	struct cfile config = {NULL, 0};
	int rCerr;
	
	FILE *dummy;

	if ((cmd = getcmd(argc, argv)) == CMD_ERROR) {
		printUsage(argv[0], 0);
		exit(EXIT_FAILURE);
	}

	home = getenv("HOME");
	strncpy(conf, home, strlen(home));
	strncat(conf, "/.polka-dot/config", 18);
	if ((config.file = fopen(conf, "a+")) == NULL) {
		fprintf(stderr, "%s: config file not found\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (argc == 3) {
		strcpy(pkgname, home);
		strncat(pkgname, "/.polka-dot/", 12);
		strcat(pkgname, argv[2]);
		strncat(pkgname, ".pd", 3);
	}
	if ((rCerr = readConfig(&config, paths)) != 0) {
		switch(rCerr) {
			case ERR_MF:
				fprintf(stderr, "%s: file limit exceeded\n", argv[0]);
				exit(EXIT_FAILURE);
			case ERR_MP:
				fprintf(stderr, "%s: path size limit exceeded\n", argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	switch (cmd) {
		case SAVE:
			save(&config, paths, pkgname, argv[2]);
			break;
		case APPLY:
			apply(paths, pkgname, argv[2]);
			break;
		case DELETE:
			del(pkgname);
			printf("Package %s deleted.\n", argv[2]);
			break;
		case LIST:
			list(home);
			break;
		case QUEUE:
			queue(config.filecount, paths);
			break;
		case ADD:
			add(argv[2], paths, &config);
			break;
		case STATUS:
			status(argv[2], pkgname);
			break;
		case RESET:
			dummy =fopen(conf, "w");
			fclose(dummy);
			break;
		default:
			fprintf(stderr, "%s: Internal error. Check arguments?", argv[0]);
			exit(EXIT_FAILURE);
	}

	fclose(config.file);
	return 0;
}
