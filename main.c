#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SOLUTION "METHINKS IT IS LIKE A WEASEL"
#define NUM_OFFSPRING 50

char random_char() {
	char new_char;
	int random = (int) (((float) rand() / RAND_MAX) * 27);

	if (random == 26) {
		new_char = ' ';
	} else {
		new_char = (char) (random + 65);
	}

	return new_char;
}

char * random_string(int length) {
	int i;
	char * new_string = malloc(sizeof(char[length + 1]));

	if (!new_string) {
		printf("Failed to allocate memory\n");
		exit(1);
	}

	for (i = 0; i < length; i++) {
		new_string[i] = random_char();
	}

	new_string[length] = '\0';

	return new_string;
}

float compare(char * string1, char * string2) {
	int i, sum, minlen;

	minlen = fmin(strlen(string1), strlen(string2));

	sum = 0;
	for (i = 0; i < minlen; i++) {
		if (string1[i] == string2[i]) {
			sum++;
		}
	}

	return (float) sum / minlen;
}

// Actually mutates string. Like, seriously.
void mutate(char * child) {
	int len = strlen(child);
	int index = ((float) rand() / RAND_MAX) * len;
	char new_char = random_char();

	child[index] = new_char;
}

char ** reproduce(int num_children, char * parent, int length) {
	int i;
	char ** children = malloc(sizeof(char * [num_children]));

	if (!children) {
		printf("Could not allocate memory\n");
		exit(1);
	}

	for (i = 0; i < num_children; i++) {
		children[i] = malloc(sizeof(char[length + 1]));

		if (!children[i]) {
			printf("Could not allocate memory\n");
			exit(1);
		}

		strncpy(children[i], parent, length + 1);
		mutate(children[i]);
	}

	return children;
}

int main() {
	int i, finished = 0, generation = 1;
	int length = strlen(SOLUTION);
	char *parent = random_string(length);
	char **children;

	srand(time(0));

	while (!finished) {
		int top_index = 0;
		float top_score = 0.0;

		children = reproduce(NUM_OFFSPRING, parent, length);

		printf("Generation #%i\n", generation);
		printf("Children:\n");
		for (i = 0; i < NUM_OFFSPRING; i++) {
			float this_score = compare(children[i], SOLUTION);

			printf("%s\t", children[i]);

			if (this_score > top_score) {
				top_score = this_score;
				top_index = i;
			}
		}

		strncpy(parent, children[top_index], length + 1);
		printf("\n\nNew parent: %s\n\n", parent);

		for (i = 0; i < NUM_OFFSPRING; i++) {
			free(children[i]);
		}

		if (compare(parent, SOLUTION) == 1.0) {
			finished = 1;
		}

		generation++;
	}

	return 0;
}
