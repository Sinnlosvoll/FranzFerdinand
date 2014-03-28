#ifndef MAIN_H_
#define MAIN_H_

#include <stdlib.h>
#include "kernel.h"

/* Will be generated: */
#include "kernel_id.h"
#include "ecrobot_interface.h"

/* add user headers here: */
#include "Algo.h"		//this is a header file from the simulator

void hello_world();
void endAll();
void checkTokens();
void wait_rotation(unsigned int degree);
void setAllVisited(signed char);
void nudgeLeft(signed int);
void nudgeRight(signed int);
char getTokensFound();
void displayRotation();
void driveStraight();
void followLine();
void initializeLightValues();
void stopLeft();
void crossingRight();
void crossingLeft();
void wait(signed int time);
void throwError(unsigned char);
void crossingStraight();
void surveyCrossing();
void stopRight();
void stopBoth();
void turnLeft(signed int);
void turnRight(signed int);
void displayLight(unsigned char);
unsigned char isOnLine();
void followLine();


#endif /* MAIN_H_ */
