#include <iostream>
#include <cassert>
#include"main.h"
#include"io.h"
#include"graph.h"
#include<chrono>
int main() {
	using namespace std::chrono;
	FILE *input = fopen("test.in", "r");
	assert(input != nullptr);
	Map mp(input);
	print_svg(mp, "input.svg");
	mp.merge_hole();
	print_svg(mp, "pass1.svg");
	auto start=std::chrono::system_clock::now();
	mp.cut();
	auto end=system_clock::now();
	auto d=duration_cast<microseconds> (end-start);
	std::cout<<d.count();
	print_svg(mp, "output.svg");

}