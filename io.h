#ifndef IO_H
#define IO_H

#include"graph.h"

void print_svg(const Map &mp, const char *file = "output.svg");

struct IO{
	IO(char *name, int width, int height);

	FILE* output;
	IO(char* name);

};

#endif