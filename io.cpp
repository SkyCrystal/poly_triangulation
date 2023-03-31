#include"io.h"
#include <cassert>

void print_svg(const Map &mp, const char *file) {
	fprintf(stderr, "print svg ");
	FILE *output = fopen(file, "w");
	fprintf(output, R"(<svg version="1.1" width="%d" height="%d" xmlns="http://www.w3.org/2000/svg">
)", mp.width, mp.height);
	fprintf(output, R"(<rect width="%d" height="%d" fill="white"/>
)", mp.width, mp.height);
	for (const auto &i: mp.polygons) {
//		if (i->node_count == 0)continue;
		if (i->p == nullptr)continue;
		fprintf(output, R"(<polygon stroke="black" stroke-width="0.5" fill="gray" points=")");
		const Link *now = i->p;
		do {
			fprintf(output, "%f %f,", now->node.x, now->node.y);
//			assert(now->pLink[0]->pLink[1]==now);
			now = now->pLink[1];
		} while (now != i->p);
		fprintf(output, "\"/>\n");
	}
	fprintf(output, "</svg>");
	fclose(output);
	fprintf(stderr, "done!\n");
}

namespace IO {
	FILE *output;

	void init(const char name[], int width, int height) {
		output = fopen(name, "w");
		init(width, height);
	}

	void init(const char name[]) {
		output = fopen(name, "w");
	}

	void close() {
		fprintf(output, "</svg>");
		fclose(output);
	}

	void init(int width, int height) {
		fprintf(output, R"(<svg version="1.1" width="%d" height="%d" xmlns="http://www.w3.org/2000/svg">
)", width, height);
		fprintf(output, R"(<rect width="%d" height="%d" fill="white"/>
)", width, height);
	}

	void print(const Node &a, const Node &b) {
		fprintf(output, R"(<line x1="%f" y1="%f" x2="%f" y2="%f" stroke="black" />
)", a.x, a.y, b.x, b.y);
	}

	void print(const Polygon &i) {
		fprintf(output, R"(<polygon stroke="black" stroke-width="0.5" fill="none" points=")");
		const Link *now = i.p;
		do {
			fprintf(output, "%f %f,", now->node.x, now->node.y);
			now = now->pLink[1];
		} while (now != i.p);
		fprintf(output, "\"/>\n");
	}

	void flush() {
		fflush(output);
	}
}