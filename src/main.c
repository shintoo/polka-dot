//#define DEBUG

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "polka-dot.c"

int main(int argc, char **argv) {
	enum command cmd;
	char *home, conf[64];
	char paths[MAXFILES][MAXPATH];
	char pkgname[128];
	struct cfile config = {NULL, 0};
	int rCerr;

	if ((cmd = getcmd(argc, argv)) == CMD_ERROR) {
		printUsage(argv[0], 0);
		exit(EXIT_FAILURE);
	}

	home = getenv("HOME");
	strncpy(conf, home, strlen(home));
	strncat(conf, "/.polka-dot/config", 18);
#ifdef DEBUG
	printf("conf: %s\n", conf);
#endif
	if ((config.file = fopen(conf, "r")) == NULL) {
		fprintf(stderr, "%s: config file not found\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (argc == 3) {
		strcpy(pkgname, home);
		strncat(pkgname, "/.polka-dot/", 12);
		strcat(pkgname, argv[2]);
		strncat(pkgname, ".pd", 3);
	}
#ifdef DEBUG
	printf("pkgname: %s\n", pkgname);
#endif

	if ((rCerr = readConfig(&config, paths)) != 0) {
		switch(rCerr) {
			case ERR_MF:
				fprintf(stderr, "%s: file limit exceeded\n", argv[0]);
				exit(EXIT_FAILURE);
		}
	}

#ifdef DEBUG
	for (int i = 0; i < config.filecount; i++) {
		printf("main at 40: paths[%d]: %s\n", i, paths[i]);
	}
#endif
	
	switch (cmd) {
		case SAVE:
			save(&config, paths, pkgname, argv[2]);
			break;
		case APPLY:
			apply(&config, paths, pkgname, argv[2]);
			break;
		case REMOVE:
			rm(pkgname);
			break;
		case LIST:
			list(home);
			break;
		default:
			fprintf(stderr, "%s: Internal error. Check arguments?", argv[0]);
			exit(EXIT_FAILURE);
	}

	return 0;
}
