int readconfig(FILE *config, char **files) {
	char c = 0, temp[64];
	int ci, fi;

	while (c != EOF) {
		while ((c = fgetc(config)) != '\n' || c != EOF) {
			temp[ci] = c;
			ci++;
			if (ci > MAXPATH) {
				return ERR_MP;
			}
		}
		strncpy(files[fi], temp, ci);
		ci = 0;
		fi++;
		if (fi > MAXFILES) {
			return ERR_MF;
		}
	}

	return 0;
}

void save(FILE *config, char *pkgname) {

}

void apply(FILE *config, char *pkgname) {

}

void rm(char *pkgname) {

}


