#include <stdio.h>

void onload() __attribute__((constructor));

void onload() {
	printf("Opened bad so\n");
}
