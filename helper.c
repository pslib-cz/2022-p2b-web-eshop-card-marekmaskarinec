#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fatal(char *msg) {
	fprintf(stderr, "fatal: %s\n", msg);
	exit(1);
}

char *readall(char *fn) {
	FILE *f = fopen(fn, "r");
	if (f == NULL)
		return NULL;

	fseek(f, 0, SEEK_END);
	size_t fsz = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *b = malloc(fsz + 1);

	fread(b, fsz, 1, f);

	fclose(f);

	return b;
}

enum keys {
	keys_img,
	keys_code,
	keys_name,
	keys_desc,
	keys_price,
	keys_avail,
	keys_label,
	keys_max,
};

int keyidx(char *key) {
	if (strcmp(key, "img") == 0)
		return keys_img;
	else if (strcmp(key, "code") == 0)
		return keys_code;
	else if (strcmp(key, "name") == 0)
		return keys_name;
	else if (strcmp(key, "desc") == 0)
		return keys_desc;
	else if (strcmp(key, "price") == 0)
		return keys_price;
	else if (strcmp(key, "avail") == 0)
		return keys_avail;
	else if (strcmp(key, "label") == 0)
		return keys_label;

	return -1;
}

int main(int argc, char *argv[]) {
	char *page;
	if ((page = readall("index.html")) == NULL)
		fatal("could not read index.html.");

	char *end = strstr(page, "</main>");

	#define STR_SIZ 4096
	char *vals[keys_max][STR_SIZ] = {0};

	char key[1024], value[STR_SIZ];
	int r;
	while ((r = scanf("%s = %[^\n]\n", key, value)) != EOF) {
		if (r != 2)
			fatal("add: error in ini config.");

		int keyi = keyidx(key);
		if (keyi < 0)
			fatal("add: invalid key.");

		strncpy((char *)vals[keyi], (char *)value, sizeof(value));
	}

	char *card = readall("karta.html");
	if (card == NULL)
		fatal("could not read karta.html");

	FILE *of = fopen("index.html", "w");
		fprintf(of, "%.*s", end - page, page);
		fprintf(of, card, vals[keys_label], vals[keys_img], vals[keys_code],
			vals[keys_name], vals[keys_desc], vals[keys_price], vals[keys_avail]);
		fprintf(of, "%s", end);
	fclose(of);

	free(card);
	free(page);
}
