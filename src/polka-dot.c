#include "polka-dot.h"

void printUsage(char *str, enum command cmd) {
	switch (cmd) {
		case SAVE ... DELETE:
			fprintf(stderr, "usage: %s %s <package-name>\n", str, cmd == SAVE ? "save" : cmd == APPLY ? "apply" : "del");
			break;
		case LIST ... QUEUE:
			fprintf(stderr, "usage: %s %s\n", str, cmd == LIST ? "list" : "queue");
			break;
		case ADD:
			fprintf(stderr, "usage: %s add <path-to-file>\n", str);
			break;
		case STATUS ... RESET:
			fprintf(stderr, "usage: %s %s\n", str, cmd == STATUS ? "status" : "reset");
			break;
		default:
			fprintf(stderr, "\nusage: %s <command> [<package-name>]\n\ncommands:\n"
					"    save <package-name>  \tSave the files in queue to a package\n"
					"    apply <package-name> \tApply the package\n"
					"    del <package-name>   \tRemove the package\n"
					"    list                 \tList saved packages\n"
					"    queue                \tList files queued to be saved\n"
					"    add <path-to-file>   \tAdd a path to the queue\n"
					"    status <package-name>\tList files saved in package\n"
					"    reset                \tClear the queue\n", str);
	}
	exit(EXIT_FAILURE);
}

//finds string str in file fp, returns location at end of string
int findString(FILE *fp, char *str, fpos_t *strloc) {
	char c;
	int len, i = 0, ret_val = 1;			// ret_val: 1 = notfound, 0 = found

	len = strlen(str);
	while ((c = fgetc(fp)) != EOF) {
		if (c == str[0]) {				// Start comparing if first char is found
			for (i = 1; i < len; i++) {
				if ((c = fgetc(fp)) == str[i]) {
					if (i == len - 1) {
						fgetpos(fp, strloc);
						ret_val = 0;
						break;
					}
				} else {
					ret_val = 1;
					break;
				}
			}
			if (ret_val = 0)
				break;
		}
	}
	return ret_val;
}

int getcmd(int argc, char **argv) {
	enum command cmd;

	if (argc != 3 && argc != 2) {
		printUsage(argv[0], 0);
		exit(EXIT_FAILURE);
	}

	if ((strncmp(argv[1], "list", 4)) == 0) {
		cmd = LIST;
	}
	else if ((strncmp(argv[1], "queue", 5)) == 0) {
		cmd = QUEUE;
	}
	else if ((strncmp(argv[1], "reset", 5)) == 0) {
		cmd = RESET;
	}
	else if ((strncmp(argv[1], "status", 6)) == 0) {
		cmd = STATUS;
	}
	else if ((strncmp(argv[1], "save", 4)) == 0) {
		cmd = SAVE;
	}
	else if ((strncmp(argv[1], "apply", 6)) == 0) {
		cmd = APPLY;
	}
	else if ((strncmp(argv[1], "del", 2)) == 0) {
		cmd = DELETE;
	}
	else if ((strncmp(argv[1], "add", 3)) == 0) {
		cmd = ADD;
	}
	
	switch (cmd) {
		case SAVE ... DELETE:
			if (argc != 3) {
				printUsage(argv[0], cmd);
			}
			break;
		case LIST ... QUEUE:
			if (argc != 2) {
				printUsage(argv[0], cmd);
			}
			break;
		case ADD ... STATUS:
			if (argc != 3) {
				printUsage(argv[0], cmd);
			}
			break;
		case RESET:
			if (argc != 2) {
				printUsage(argv[0], cmd);
			}
			break;
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
					fprintf(stderr, "Config fodelatting error: config.%d.%d", fi, ci);
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

void save(struct cfile *config, char (*paths)[MAXFILES], char *pkgname, char *name) {
	FILE *pkg = fopen(pkgname, "w");
	FILE *temp;
	char c;

#ifdef DEBUG
	for (int i = 0; i < config->filecount; i++) {
		printf("config->filecount: %d\npaths[%d] at 78: %s\n", config->filecount, i, paths[i]);
	}
#endif
	for (int i = 0; i < config->filecount; i++) {
		if ((temp = fopen(paths[i], "r")) == NULL) {
			fprintf(stderr, "Error: file '%s' not found.\n", paths[i]);
			exit(EXIT_FAILURE);
		}
		fprintf(pkg, "---file: %s---\n", paths[i]);
		while ((c = fgetc(temp)) != EOF) {
			fputc(c, pkg);
		}
		fprintf(pkg, "---end file---\n\n");
		fclose(temp);
	}
	printf("Files packaged into %s.\n", name);
}

void apply(char (*paths)[MAXFILES], char *pkgname, char *name) {
	FILE *pkg;
	FILE *temp;
	char c;
	fpos_t floc, endloc, curloc;				// Location of file tag, location of endfile tag, current location
	char *filetag;
	char endbuf[15];
	char *endfiletag = "---end file---";
	int endflag = 0;							// 0: endfile tag not found, 1: found
	int i, eblen, filecount = 0;				// endbuf length

	if ((pkg = fopen(pkgname, "r")) == NULL) {
		fprintf(stderr, "Package '%s' not found\n", name);
		exit(EXIT_FAILURE);
	}
	while (c != EOF) {				// from end of beginfile tag to beginning of endfile tag
		c = fgetc(pkg);
		if (c == endfiletag[0]) {				// Start comparing if first char is found
			for (int n = 1; n < 14; n++) {
				if ((c = fgetc(pkg)) == endfiletag[n]) {
					if (n == 13) {
						endflag = 1;
						break;
					}
				}
				else {
					endflag = 0;
					break;
				}
			}
			if (endflag == 1) {
				filecount++;
			}
		}
	}
	rewind(pkg);
	endflag = 0;
	for (int i = 0; i < filecount; i++) {
		filetag = (char *) malloc(13 + strlen(paths[i]));
		strcat(filetag, "---file: ");
		strcat(filetag, paths[i]);				// filetag = "---file: <paths[i]>---\n"
		strcat(filetag, "---\n");
		findString(pkg, filetag, &floc);		// floc is at the end of beginfile tag
		fsetpos(pkg, &floc);
		temp = fopen(paths[i], "w");
		while (endflag == 0) {					// from end of beginfile tag to beginning of endfile tag
			c = fgetc(pkg);
			if (c == endfiletag[0]) {			// Start comparing if first char is found
				endbuf[0] = c;
				for (int n = 1; n < 14; n++) {
					c = fgetc(pkg);
					endbuf[n] = c;
					if (c == endfiletag[n]) {
						if (n == 13) {
							endflag = 1;
							break;
						}
					}
					else {
						endflag = 0;
						eblen = n;
						break;
					}
				}
				endbuf[eblen + 1] = '\0';
				if (endflag == 1) {
					break;
				}
				else {
					fputs(endbuf, temp);
				}
			}
			else {
				fputc(c, temp);
			}
		}
#ifdef DEBUG
		printf("loopi: %d\n", i);
#endif
		endflag = 0;
		fclose(temp);
		free(filetag);
		rewind(pkg);
	}
	printf("Package %s applied.\n", name);
}

void del(char *pkgname) {
	if (remove(pkgname) != 0) {
		fprintf(stderr, "Error removing file '%s'\n", pkgname);
		exit(EXIT_FAILURE);
	}
}

void list(char *home) {
	DIR *dir;
	struct dirent *item;
	char dirname[64], pkgname[128];
	char *ft;
	
	puts("Saved packages:");
	strcpy(dirname, home);
	strcat(dirname, "/.polka-dot/");
	if ((dir = opendir(dirname)) != NULL) {
		while ((item = readdir(dir)) != NULL) {
			if (item->d_name[0] == '.') {
				continue;
			}
			if ((ft = strstr(item->d_name, ".pd")) != NULL) {
				strncpy(pkgname, item->d_name, 128);
				pkgname[strlen(pkgname) - 3] = '\0';
				printf("\t%s\n", pkgname);
			}
		}
		closedir(dir);
	}
	else {
		fprintf(stderr, "Could not open package directory.\nDid you delete '%s/.polka-dot/'?\n", home);
		exit(EXIT_FAILURE);
	}
}

void queue(int count, char (*paths)[MAXFILES]) {
	if (count == 0) {
		puts("No files in queue!");
	}
	else {
		puts("Files to be saved:");
		for (int i = 0; i < count; i++) {
			printf("\t%s\n", paths[i]);
		}
	}
}

void add(char *path, char (*paths)[MAXFILES], struct cfile *config) {
	if (path[0] == '\0') {
		fprintf(stderr, "Error adding file.\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < config->filecount; i++) {
		if ((strcmp(paths[i], path)) == 0) {
			printf("File %s already queued.\n", path);
			exit(EXIT_FAILURE);
		}
	}
	fputs(path, config->file);
	fputc('\n', config->file);
	printf("File '%s' added to queue.\n", path);
}

void status(char *pkgname, char *pkgpath) {
	FILE *pkg = fopen(pkgpath, "r");
	fpos_t loc;
	char endbuf[5], filepath[64];
	char c = 0;

	rewind(pkg);	
	printf("Files in package %s:\n", pkgname);
	findString(pkg, "---file:", &loc);
	fsetpos(pkg, &loc);
	while (c != EOF) {
		putchar('\t');
		c = fgetc(pkg);
		do {
			putchar(c);
			c = fgetc(pkg);
		} while (c != '-');
		putchar('\n');
		if (findString(pkg, "---file:", &loc) == 1) {
			fsetpos(pkg, &loc);
		}
		else {
			break;
		}
	}
}

