#ifndef IO_H
#define IO_H

#include"graph.h"

void print_svg(const Map &mp, const char *file = "output.svg");

namespace IO{
	void init(const char [], int width, int height);

	void flush();
	void init(const char[]);
	void init(int,int);
	void close();

	void print(const Node &a, const Node &b);

	void print(const Polygon &i);
}

#endif