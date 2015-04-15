#include "polka-dot.h"

void printUsage(char *str) {
	fprintf(stderr, "%s: usage: polka-dot <command> <package-name>\n\ncommands:\n"
			"    save\tSave the current configuration as a package\n"
			"    apply\tApply the package\n"
			"    rm\tRemove the package\n", str);	
}

int getcmd(int argc, char **argv) {
	enum command cmd;
	
	if (argc != 3) {
		printUsage(argv[0]);
		exit(EXIT_FAILURE);
	}
	if (strncmp(argv[1], "save", 4) == 0) {
		cmd = SAVE;
	}
	else if (strncmp(argv[1], "apply", 6) == 0) {
		cmd = APPLY;
	}
	else if (strncmp (argv[1], "rm", 2) == 0) {
		cmd = REMOVE;
	}
	else {
		cmd = CMD_ERROR;
	}
	return cmd;
}

int readConfig(struct cfile *config, char (*paths)[MAXFILES]) {
	char c = 0, temp[MAXPATH], *home;
	int ci = 0, fi = 0;

	while (c != EOF) {
		while ((c = fgetc(config->file)) != '\n' && c != EOF) {
			if (c == '~') {
				if (ci != 0) {
					fprintf(stderr, "polka-dot: config formatting error: config.%d.%d", fi, ci);
					exit(EXIT_FAILURE);
				}
				home = getenv("HOME");
				home[strlen(home)] = '\0';
				strcpy(temp, home);
				ci = strlen(temp);
				temp[ci] = '\0';
			}
			else {
				temp[ci++] = c;
			}
		}
		if (c == EOF) {
			break;
		}
		temp[ci] = '\0';
#ifdef DEBUG
		printf("54: temp: '%s'\n", temp);
#endif
		strncpy(paths[fi], temp, ci);
		ci = 0;
		fi++;
		if (fi > MAXFILES) {
			return ERR_MF;
		}
	}
	config->filecount = fi;
#ifdef DEBUG
	printf("config->filecount at 66: %d\n", config->filecount);
#endif
	return 0;
}

void save(struct cfile *config, char (*paths)[MAXFILES], char *pkgname) {
	FILE *pkg = fopen(pkgname, "w");
	FILE *temp;
	char c;

#ifdef DEBUG
	for (int i = 0; i < config->filecount; i++) {
		printf("config->filecount: %d\npaths[%d] at 78: %s\n", config->filecount, i, paths[i]);
	}
#endif
	for (int i = 0; i < config->filecount; i++) {
		temp = fopen(paths[i], "r");
		fprintf(pkg, "{{ file: %s }}\n", paths[i]);
		while ((c = fgetc(temp)) != EOF) {
			fputc(c, pkg);
		}
		fprintf(pkg, "\n{{ end file }}\n\n");
	}
}

void apply(struct cfile *config, char (*paths)[MAXFILES], char *pkgname) {

}

void rm(char *pkgname) {

}


