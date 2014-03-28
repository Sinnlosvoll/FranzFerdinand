#include "Configuration.h"
#include "Algo.h"

int main(void) {
	//implement your data structures and algorithms in extra c and h files!
	//only use them here for testing
	posX = 0;
	int nextX;
	//avoid printf() in code used by your robot
	//instead use the debugger
	printf("Test");
	nextX = calculateWay();
	printf("Token: %d\n", Robot_Move(nextX, 0));
	printf("Intersection: %d\n", Robot_GetIntersections());
	nextX = calculateWay();
	printf("Token: %d\n", Robot_Move(nextX, 0));
	printf("Intersection: %d\n", Robot_GetIntersections());
	nextX = calculateWay();
	printf("Token: %d\n", Robot_Move(nextX, 0));

	return 0;
}
