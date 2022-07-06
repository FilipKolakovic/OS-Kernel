#include "kernel.h"

int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {

	Kernel :: startUp();
	int value = userMain(argc, argv);
	Kernel :: terminate();
	return value;
}
