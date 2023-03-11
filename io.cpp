#include"io.h"

void print_svg(const Map &mp, const char *file) {
	fprintf(stderr, "print svg\n");
	FILE *output = fopen(file, "w");
	fprintf(output, R"(<svg version="1.1" width="%d" height="%d" xmlns="http://www.w3.org/2000/svg">
)", mp.width, mp.height);
	fprintf(output, R"(<rect width="%d" height="%d" fill="white"/>
)", mp.width, mp.height);
	for (const auto &i: mp.polygons) {
		fprintf(output, R"(<polygon stroke="black" stroke-width="0.5" fill="none" points=")");
		const Link *now = i->p;
		do {
			fprintf(output, "%f %f,", now->node->x, now->node->y);
			now = now->pLink[1];
		} while (now != i->p);
		fprintf(output, "\"/>\n");
	}
	fprintf(output, "</svg>");
	fclose(output);
	fprintf(stderr, "done!");
}