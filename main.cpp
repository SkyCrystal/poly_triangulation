#include <iostream>
#include <cassert>
#include"main.h"
#include"io.h"
#include"graph.h"

int main() {
	FILE *input = fopen("test2.in", "r");
	assert(input != nullptr);
	Map mp(input);
	print_svg(mp, "input.svg");
	mp.merge_hole();
	print_svg(mp, "pass1.svg");
	mp.cut();
	print_svg(mp, "output.svg");

}