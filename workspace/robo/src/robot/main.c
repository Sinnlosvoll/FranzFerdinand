#include "main.h"
#include "path.c"

#define MOTOR_A NXT_PORT_A
#define MOTOR_B NXT_PORT_B
#define ROTATIONSPEED 90
#define NUDGESPEED 70
#define DRIVESPEED 70
#define LIGHTSENSOR NXT_PORT_S2
#define SAFETYROTATE 10

signed int difference=0;
signed int master=1;
signed int rot1=0;
signed int rot2=0;
signed char displayCurrentNode=1;
signed char tokensFound=0;
signed char lastCrossingSurveyed=0;
signed int absoluteRotation=0;
signed int currentLightValue=0;
signed int totalRotationCompareValue=0;
signed int lowestLightValue=1200;
signed int highestLightValue=0;
signed int lightThreshold=0;
signed int blackRotation=0;
signed int crossingCompare=0;
signed int debugInt=0;
signed char direction=0;

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void) {
	rot1=nxt_motor_get_count(MOTOR_A);
	rot2=nxt_motor_get_count(MOTOR_B);
	difference=rot1-rot2;
	absoluteRotation=difference*500/3200;
	totalRotationCompareValue=(rot1+rot2)/2;

	//if(isOnLine())ecrobot_sound_tone(400,1,50);

	//checkTokens();
	//displayRotation();
	displayLight(0);
}
void checkTokens()
{
	if ((ecrobot_get_touch_sensor(NXT_PORT_S1)==1)||(ecrobot_get_touch_sensor(NXT_PORT_S3)== 1)){
		tokensFound++;
		stopBoth();
		ecrobot_sound_tone(220,1000,100);
		wait(10000);
		if(tokensFound==3)
		{
			allVisited=1;
			getPathComplicated(currentPosition[0],currentPosition[1],6,6);
		}
	}
}
void displayLight(unsigned char display)
{
	currentLightValue=ecrobot_get_light_sensor(LIGHTSENSOR);
	if(currentLightValue<lowestLightValue)
		lowestLightValue=currentLightValue;
	if(currentLightValue>highestLightValue)
		highestLightValue=currentLightValue;
	if(display)
	{
		display_clear(0);
		display_goto_xy(7,3);
		display_int(currentLightValue,4);
		display_goto_xy(1,3);
		display_int(lowestLightValue,4);
		display_goto_xy(12,3);
		display_int(highestLightValue,4);
		display_goto_xy(7,4);
		display_int(lightThreshold,4);
		display_update();
	}
	if(displayCurrentNode)
		display_currentNode();

}
void endAll()
{
	//TODO: implement Stuff
}
void setAllVisited(signed char value)
{
	allVisited=value;
}
void initializeLightValues()
{
	while(absoluteRotation<=350)
	{
		nxt_motor_set_speed(MOTOR_B,-(ROTATIONSPEED+3),1);
		nxt_motor_set_speed(MOTOR_A,ROTATIONSPEED,1);
		displayRotation();
		displayLight(1);
		wait(1);
	}
	stopLeft();
	stopRight();
	lightThreshold=((lowestLightValue+highestLightValue)/2)+20;
	systick_wait_ms(100);
}
char getTokensFound()
{
	return tokensFound;
}
void displayRotation()
{
	display_clear(0);
	display_goto_xy(4,0);
	display_int(absoluteRotation,5);
	display_goto_xy(2,1);
	display_int(rot1,5);
	display_goto_xy(10,1);
	display_int(rot2,5);
	display_update();
}
void ecrobot_device_initialize(void) {
	nxt_motor_set_count(MOTOR_A,0);
	nxt_motor_set_count(MOTOR_B,0);
	ecrobot_set_light_sensor_active(LIGHTSENSOR);
}

void ecrobot_device_terminate(void) {
	ecrobot_set_light_sensor_inactive(LIGHTSENSOR);
}
void stopLeft()
{
	nxt_motor_set_speed(MOTOR_A,0,1);
}
void stopRight()
{
	nxt_motor_set_speed(MOTOR_B,0,1);
}
void stopBoth()
{
	stopLeft();
	stopRight();
}

void hello_world() {
	ecrobot_status_monitor("Hello, World!");
}
void nudgeLeft(signed int degree)
{
	int rotationCompare=absoluteRotation;
	stopBoth();

	while(((rotationCompare-absoluteRotation)<=degree)&&!isOnLine())
	{
		nxt_motor_set_speed(MOTOR_B,NUDGESPEED+3,0);
		nxt_motor_set_speed(MOTOR_A,-NUDGESPEED,0);
		wait(1);
	}
	stopBoth();

}
void turnLeft(signed int degree)
{
	int rotationCompare=absoluteRotation;
	stopBoth();

	while((rotationCompare-absoluteRotation)<=degree)
	{
		nxt_motor_set_speed(MOTOR_B,ROTATIONSPEED+3,0);
		nxt_motor_set_speed(MOTOR_A,-ROTATIONSPEED,0);
		wait(1);
	}
	stopBoth();

}
void nudgeRight(signed int degree)
{
	int rotationCompare=absoluteRotation;
	stopBoth();

	while(((rotationCompare-absoluteRotation)>=-(degree-2))&&(!isOnLine()))
	{
		nxt_motor_set_speed(MOTOR_A,NUDGESPEED,0);
		nxt_motor_set_speed(MOTOR_B,-(NUDGESPEED+3),0);
		wait(1);
	}
	stopBoth();
}
void turnRight(signed int degree)
{
	int rotationCompare=absoluteRotation;
	stopBoth();

	while((rotationCompare-absoluteRotation)>=-(degree-2))
	{
		nxt_motor_set_speed(MOTOR_A,ROTATIONSPEED,1);
		nxt_motor_set_speed(MOTOR_B,-(ROTATIONSPEED+3),1);
		wait(1);
	}
	stopBoth();
}
void driveStraight(unsigned int time)
{

	nxt_motor_set_speed(MOTOR_A,DRIVESPEED,1);
	nxt_motor_set_speed(MOTOR_B,DRIVESPEED+3,1);
	checkTokens();
	wait(time);

	stopBoth();


}
void wait_rotation(unsigned int degree)
{
	signed int initialRotationCompareValue=totalRotationCompareValue;
	while(totalRotationCompareValue-initialRotationCompareValue<=degree)
	{
		wait(1);
	}
}

void followLine()
{
	char nodefound = 0;
	while(!nodefound)
	{
		while(isOnLine())
		{
			driveStraight(1);
			checkTokens();
		}
		driveStraight(5);
		stopBoth();
		nudgeLeft(20);
		if(!isOnLine())
			nudgeRight(40);
		if(!isOnLine())
			nudgeLeft(60);
		if(!isOnLine())
			nudgeRight(80);
		if(!isOnLine()){
			nodefound = 1;
			crossingCompare=absoluteRotation;
		}
		nudgeLeft(30);
		wait(1);
	}


}
void crossingRight()
{
	signed char hasNoTop=!hasDirectionRelative(top);
	if(absoluteRotation>(crossingCompare+90))
	{
		turnRight(30);
		signed int toRotate=360-(absoluteRotation-(crossingCompare+90));
		if(toRotate>30)
		{
			if(hasNoTop)
				turnLeft(10);
			turnRight(20);
			nudgeRight(toRotate-20);
		}else{
			//+turnLeft(30);
			nudgeRight(toRotate+(SAFETYROTATE*2));
		}
		return;
	}
	if(absoluteRotation<=(crossingCompare+90))
	{
		displayCurrentNode=0;
		/*
		display_clear(1);
		display_goto_xy(4,4);
		display_int(absoluteRotation,4);
		display_goto_xy(4,5);
		display_int(crossingCompare+90,4);
		display_goto_xy(2,6);
		display_string("C:RIGHT");
		*/
		//wait(2000);
		//throwError(1);
		turnRight(20);
		wait(1000);
		nudgeRight((crossingCompare+90)-absoluteRotation);
	}
}
void wait(signed int time)
{
	systick_wait_ms(time);
}
void throwError(unsigned char numberOfPieps)
{
	unsigned int counter=0;
	while(1)
	{
		for(counter=0;counter<numberOfPieps;counter++)
		{
			ecrobot_sound_tone(400,100,50);
			wait(200);
		}
		wait(2000);
	}
}
void crossingStraight()
{
	signed int toRotate= (absoluteRotation-crossingCompare) % 360;
	if(toRotate>=0)
	{
		nudgeLeft(20);
		nudgeRight(toRotate+SAFETYROTATE);
		return;
	}else
	{
		if(toRotate>=-50)
		{
			nudgeLeft(toRotate);
			return;
		}/*
		display_clear(1);
		display_goto_xy(11,0);
		display_int(toRotate,4);
		display_update();
		*/
		throwError(2);
		return;
	}

}
void crossingBack()
{
	signed char hasNoRight=!hasDirectionRelative(right);
	signed int toRotate=(absoluteRotation-(crossingCompare+180)) % 360;
	if((toRotate>0)&&(toRotate<270))
	{
		if(hasNoRight)
		{
			ecrobot_sound_tone(400,200,80);
			wait(200);
			turnRight(80);
			nudgeRight((toRotate-90)+(SAFETYROTATE*2));
			return;
		}
		turnRight(100);
		nudgeRight((toRotate-90)+(SAFETYROTATE*2));
		return;
	}
	/*
	display_clear(1);
	display_goto_xy(3,3);
	display_int(toRotate,4);
	display_update();
	*/
	//wait(5000);
}
void crossingLeft()
{
	if(absoluteRotation>(crossingCompare-90))
	{
		signed int toRotateLeft=(absoluteRotation-(crossingCompare-90)) % 360;
		/*
		display_clear(1);
		display_goto_xy(0,0);
		display_int(absoluteRotation,4);
		display_goto_xy(2,2);
		display_int(crossingCompare,4);
		display_goto_xy(3,3);
		display_int(toRotateLeft,4);
		display_update();
		*/
		//wait(1000);//*/
		if(toRotateLeft>30)
		{
			turnLeft(30);
			nudgeLeft(toRotateLeft-10+(SAFETYROTATE*2));
		}else{
			display_clear(1);
			display_goto_xy(7,7);
			display_int(toRotateLeft,4);
			//wait(5000);
			nudgeLeft(toRotateLeft+SAFETYROTATE);
		}
		return;
	}
	if(absoluteRotation<=(crossingCompare-90))
	{
		throwError(3);
	}
}
void surveyCrossingNew()
{
	signed char KLinks=0;
	signed char KOben=0;
	signed char KRechts=0;
	signed char KUnten=0;
	stopBoth();
	signed int rotationCompare=absoluteRotation;
	nudgeLeft(30);
	nudgeRight(30);
	while(absoluteRotation-rotationCompare<340)
	{
		nxt_motor_set_speed(MOTOR_A,ROTATIONSPEED,1);
		nxt_motor_set_speed(MOTOR_B,(-(ROTATIONSPEED+3)),1);
		if(((absoluteRotation-rotationCompare)>0)&&((absoluteRotation-rotationCompare)<30)&&isOnLine())
		{
				KOben=1;
		}
		if(((absoluteRotation-rotationCompare)>30)&&((absoluteRotation-rotationCompare)<120)&&isOnLine())
		{
				KRechts=1;
		}
		if(((absoluteRotation-rotationCompare)>130)&&((absoluteRotation-rotationCompare)<210)&&isOnLine())
		{
				KUnten=1;
		}
		if(((absoluteRotation-rotationCompare)>240)&&((absoluteRotation-rotationCompare)<300)&&isOnLine())
		{
				KLinks=1;
		}
		if(((absoluteRotation-rotationCompare)>310)&&((absoluteRotation-rotationCompare)<340)&&isOnLine())
		{
				KOben=1;
		}
	}
	stopBoth();
	display_clear(1);
	display_goto_xy(5,1);
	if(KOben)
	display_string("GERADEAUS");
	display_goto_xy(10,2);
	if(KRechts)
	display_string("RECHTS");
	display_goto_xy(7,3);
	if(KUnten)
	display_string("UNTEN");
	display_goto_xy(4,2);
	if(KLinks)
	display_string("LINKS");
	display_update();
	//wait(2000);
	setCurrentNode(KLinks, KUnten,KRechts,KOben);

}
void surveyCrossing()
{
	display_clear(1);
	signed int currentRotation=absoluteRotation;
	signed char KLinks=0;
	signed char KOben=0;
	signed char KRechts=0;
	signed char KUnten=0;
	display_goto_xy(0,0);
	display_int(currentRotation,4);
	display_update();
	//wait(500);
	turnRight(10);
	nudgeRight(130);
	if(isOnLine())
		KRechts=1;
	turnRight(10);
	nudgeRight(130);
	if(isOnLine())
		KUnten=1;
	turnRight(10);
	nudgeRight(130);
	if(isOnLine())
		KLinks=1;
	turnRight(10);
	nudgeRight(130);
	if(isOnLine())
		KOben=1;
	display_goto_xy(0,1);
	display_int(KRechts,1);
	display_goto_xy(0,2);
	display_int(KUnten,1);
	display_goto_xy(0,3);
	display_int(KLinks,1);
	display_goto_xy(0,4);
	display_int(KOben,1);
	display_update();
	//wait(800);
	setCurrentNode(KLinks,KUnten,KRechts,KOben);

}
unsigned char isOnLine()
{
	if(currentLightValue>=lightThreshold)
	{
		return 1;
	}else{
		wait(3);
		if(currentLightValue>=lightThreshold)
			return 1;
		return 0;
	}

}
TASK(OSEK_Main_Task) {
	posX = 0;
	//int nextX = calculateWay();	//use stuff from the simulation
	initializeLightValues();
	initializePathStuff();
	while (master) {
		//wait(3000);
		//turnRight(340);
		//wait(8000);

		followLine();
		driveStraight(400);
		nudgeLeft(10);
		nudgeRight(30);
		if((!currentNodeVisited())||((tokensFound==3)&&(lastCrossingSurveyed==0)))
		{
			surveyCrossingNew();
			if(tokensFound==3)
			{
				lastCrossingSurveyed=1;
			}
		}else{
			//TODO: sth here to make it work...
		}
		direction=getNextDirection();
		if(allVisited)
		{
			/*
			display_clear(1);
			display_goto_xy(3,3);
			display_int(direction,1);
			display_update();
			*/
			ecrobot_sound_tone(400,50,80);
			wait(100);
			ecrobot_sound_tone(600,50,80);
			wait(100);
			ecrobot_sound_tone(600,50,80);
			wait(100);
		}
		/*
		display_clear(1);
		display_goto_xy(8,4);
		display_int(direction,1);
		display_update();
		*/
		//wait(1000);
		switch(direction){
		case 1: crossingStraight();break;
		case 2: crossingRight();break;
		case 4: crossingBack();break;
		case 8: crossingLeft();break;
		case 0: master=0;break;
		default: throwError(4);break;
		}


	}
	//();
	while(1)
	{
		ecrobot_sound_tone(220,1000,80);
		systick_wait_ms(100000);
	}
}
