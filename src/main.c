#define DEBUG

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

	if ((cmd = getcmd(argc, argv)) == CMD_ERROR) {
		printUsage(argv[0]);
		exit(EXIT_FAILURE);
	}

	home = getenv("HOME");
	strncpy(conf, home, strlen(home));
	strncat(conf, "/.polka-dot/config", 18);
#ifdef DEBUG
	printf("conf: %s\n", conf);
#endif
	config.file = fopen(conf, "r");

	strcpy(pkgname, home);
	strncat(pkgname, "/.polka-dot/", 12);
	strcat(pkgname, argv[2]);
	strncat(pkgname, ".pd", 3);
#ifdef DEBUG
	printf("pkgname: %s\n", pkgname);
#endif

	readConfig(&config, paths);
#ifdef DEBUG
	for (int i = 0; i < config.filecount; i++) {
		printf("main at 40: paths[%d]: %s\n", i, paths[i]);
	}
#endif
	
	switch (cmd) {
		case SAVE:
			save(&config, paths, pkgname);
			printf("Package %s saved.\n", pkgname);
			break;
		case APPLY:
			apply(&config, paths, pkgname);
			printf("Package %s applied.\n", pkgname);
			break;
		case REMOVE:
			rm(pkgname);
			printf("Package %s removed.\n", pkgname);
			break;
		default:
			fprintf(stderr, "%s: Internal error. Check arguments?", argv[0]);
			exit(EXIT_FAILURE);
	}

	return 0;
}
