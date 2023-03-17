#include <iostream>
#include <cassert>
#include"main.h"
#include"io.h"
#include"graph.h"

int main() {
	FILE *input = fopen("test.in", "r");
	assert(input != nullptr);
	Map mp(input);
	mp.merge_hole();
	print_svg(mp);


}