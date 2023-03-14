#include <iostream>
#include"main.h"
#include"io.h"
#include"graph.h"

int main() {
	FILE *input= fopen("test.in","r");
	Map mp(input);
	mp.merge_hole();
	print_svg(mp);
}