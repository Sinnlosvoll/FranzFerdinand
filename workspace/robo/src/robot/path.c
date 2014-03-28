#include <stdio.h>
#include "../h/main.h"

#define none 0
#define top 1
#define right 2
#define down 4
#define left 8

#define DEBUG 0x00

unsigned char    mazeStorage[13][13];
signed char 	 getNextCalled =0;
signed char 	 mazeVisited[13][13];
signed char 	 mazeDistance[13][13];
unsigned char 	 mazeStart[13][13];
unsigned char 	 mazeEnd[13][13];
unsigned char    pathStorage[25];//first char denotes length of current queue
unsigned char    currentPosition[2];//X, Y
unsigned char 	 previousNode[2];
unsigned char 	 allVisited=0;

signed   char    directionOffset = 0;//number of times turned right

/* FUN.ction prototypes */

void initializePathStuff();
void checkIfAllVisited();
signed char currentNodeVisited();
signed char hasDirectionRelative(signed char direction);
signed char shiftBits(signed char toShift, signed char shiftAmount);
signed char shiftBitsForDriving(signed char toShift, signed char shiftAmount);
void hasMoved(unsigned char direction);
signed char getNextDirection();
signed char hasBeenVisitedRelative(signed char direction);
signed char visited(signed char x, signed char y);
void addToPath(char direction);
void getNextUnvisited();
signed char hasDirection(char direction, unsigned char x, unsigned char y);
signed char getLargestValue(char c1, char c2, char c3, char c4, char own);
void getPathComplicated(signed char x1, signed char y1, signed char xDestination, signed char yDestination);
void getPathTo(char xDestination, char yDestination);
void setNode(signed char x, signed char y, char links, char unten, char rechts, char oben, char visited);
void setCurrentNode(char links, char unten, char rechts, char oben);


/* abstract:
 * setting a node seems to work for now, further investigation is advised
 * also when directionOffset is active
 *
 * NOT YET WORKING
 * - exploration
 *
 *
 *
 */

/* TODO:
 *
 * increment visited nbr when setNode is called // done: call setCurrentNode for this
 *
 *
 * */

/* PATHPLANNING 
 *
 * 
 * NOW MAIN PATHPLANNING: create 2 new strucutres. One from the start and one from the end. The structures
 * contain the number of (50-x) steps needed to get there. Overlay both and get the highest number greaters. These mark the optimal path.
 * 
 */

void initializePathStuff()
{
    int i,n;
    /* Alle Kreuzungen auf 0 setzen */
    for(i=0; i<=12;i++){
        for(n=0; n<=12;n++){
            mazeStorage[i][n]=0;
        }
    }
    /* STartkreuzung AUf 6,6 SEtzen */
    currentPosition[0] = 6;
    currentPosition[1] = 6;

	for(i=0;i<=24;i++)
	{
		pathStorage[i]=0;
	}
	/*********************für Wettkamps ****************/
	setNode(6,5,0,0,0,1,1);
}

/* internal Pathplanning funtions */
signed char shiftBits(signed char toShift, signed char shiftAmount)
{
	//secret stuff I need to rotate my direction clockwise(if I remember correctly)
    signed char temp=0;
    signed char cnt = 0;
    for(cnt=0;cnt<shiftAmount;cnt++)
    {
        if((toShift&8)==8){temp=1;}else{temp=0;};
        toShift= toShift << 1;
        toShift= toShift % 16;
        toShift = toShift + temp;
    }

    return toShift;
}
signed char shiftBitsForDriving(signed char toShift, signed char shiftAmount)
{
	//secret stuff I need to rotate my direction clockwise(if I remember correctly)
    signed char temp=0;
    signed char cnt = 0;
    for(cnt=0;cnt<shiftAmount;cnt++)
    {
        if((toShift&1)==1){temp=8;}else{temp=0;};
        toShift= toShift >> 1;

        toShift = toShift + temp;
    }

    return toShift;
}
void display_currentNode(){
	//display_clear(1);
	display_goto_xy(5,6);
	display_int((int)previousNode[0],1);
	display_goto_xy(8,6);
	display_int((int)previousNode[1],1);
	display_goto_xy(6,5);
	display_int(directionOffset,1);
	display_goto_xy(6,7);
	display_int((int)mazeStorage[previousNode[0]][previousNode[1]],1);
	display_update();
}
void hasMoved(unsigned char direction)
{
    //changes currentPosition according to direction WITH directional offset
    //direction = shiftBitsForDriving(direction,directionOffset);

	//ecrobot_sound_tone(100,80, 100);
	//ecrobot_sound_tone(200,80, 100);
	//ecrobot_sound_tone(400,80, 100);
	previousNode[0]=currentPosition[0];
	previousNode[1]=currentPosition[1];

    if(direction == top )
        currentPosition[1]++;

    if(direction == right )
        currentPosition[0]++;

    if(direction == down )
        currentPosition[1]--;

    if(direction == left)
        currentPosition[0]--;
//*/

}
void addToPath(char direction)
{
    pathStorage[0]++;
    pathStorage[(int)pathStorage[0]]=direction;
}
signed char hasDirection(char direction,unsigned char x,unsigned char y)
{
    char temp = mazeStorage[x][y];

    if((direction == top)  && ((temp & 1)==0x01) )
    {
        return 1;
    }
    if((direction == right)&& ((temp & 2)==0x02) )
    {
        return 1;
    }
    if((direction == down) && ((temp & 4)==0x04) )
    {
        return 1;
    }
    if((direction == left) && ((temp & 8)==0x08) )
    {
        return 1;
    }
    return 0; // FAlls DAs PAssieren SOllte: WUT? FAlscher KNoten oder pfadabfrage und richtige richtung gewählt
}
signed char currentNodeVisited()
{
	return visited(currentPosition[0],currentPosition[1]);
}
signed char getLargestValue(char c1, char c2, char c3, char c4, char own){
    if((c1>=0) && (c1>=c2) && (c1>=c3) && (c1>=c4) && (c1>=own))
    {return c1;}
    if((c2>=0) && (c2>=c1) && (c2>=c3) && (c2>=c4) && (c2>=own))
    {return c2;}
    if((c3>=0) && (c3>=c2) && (c3>=c1) && (c3>=c4) && (c3>=own))
    {return c3;}
    if((c4>=0) && (c4>=c2) && (c4>=c3) && (c4>=c1) && (c4>=own))
    {return c4;}
    if((own>=0) && (own>=c2) && (own>=c3) && (own>=c4) && (own>=c1))
    {return own;}
    return 0;

}

/* semi-internal functions. Call if you know what you are doing */
void setNode(signed char x, signed char y,char links, char unten, char rechts, char oben, char visited)
{
    char tempStorage;
    char tempCount;
    char switchCount =0;
    for(tempCount=0; tempCount < directionOffset; tempCount++)
    {
        tempStorage = rechts;
        rechts = oben;
        oben = links;
        links = unten;
        unten = tempStorage;
        switchCount++;
    }
    char temp = 0;
    temp = temp +(visited <<4);
    if(links   ==0x01)
    {
        if(x>0)
        {
            mazeStorage[(int)x-1][(int)y] = mazeStorage[(int)x-1][(int)y] | 2;
        }
        temp += 8;
    }
    if(unten   ==0x01)
    {
        if(y>0)
        {
            mazeStorage[x][y-1] = mazeStorage[x][y-1] | 1;
        }
        temp += 4;
    }
    if(rechts  ==0x01)
    {
        if(x<12)
        {
            mazeStorage[(int)x+1][(int)y] = mazeStorage[(int)x+1][(int)y] | 8;
        }
        temp += 2;
    }
    if(oben    ==0x01)
    {
        if(y<12)
        {
            mazeStorage[(int)x][(int)y+1] = mazeStorage[(int)x][(int)y+1] | 4;
        }
        temp += 1;
    }

    mazeStorage[x][y] = temp;
}
void getPathComplicated(signed char x1, signed char y1, signed char xDestination, signed char yDestination)
{
	if((x1==xDestination)&&(y1==yDestination))
	{
		stopBoth();
		ecrobot_sound_tone(100,500,100);
		systick_wait_ms(2000);
		endAll();
		addToPath(none);
		return;
	}
	//display_currentNode();
	//systick_wait_ms(1000);
	/*
	display_clear(1);
	display_goto_xy(0,0);
	display_string("gPC called");
	display_goto_xy(0,1);
	display_int((int)x1,1);
	display_goto_xy(3,1);
	display_int((int)y1,1);
	display_goto_xy(0,2);
	display_int((int)xDestination,1);
	display_goto_xy(3,2);
	display_int((int)yDestination,1);
	display_update();
	*/
	//systick_wait_ms(1000);
    signed char xCurrent;
    signed char yCurrent;
    signed char startReached = 0;
    signed char endReached = 0;
    signed char xDistance= xDestination - x1;
    signed char yDistance= yDestination - y1;
    signed char yD2=0;
    signed char xD2=0;
    if(yDistance<0){yD2=-yDistance;}else{yD2=yDistance;}
    if(xDistance<0){xD2=-xDistance;}else{xD2=xDistance;}
    char maxIterations = (xD2+yD2)/2+40;//40 is safety padding
    char cntLoops;

    if(maxIterations<0)
        maxIterations=-maxIterations;

    //************************************** MAIN BODY ******
    int x,y;
    for(x=0;x<=12;x++)
    {
        for(y=0;y<=12;y++)
        {
            mazeStart[x][y]=0;
            mazeEnd[x][y]=0;
        }
    }
    mazeStart[x1][y1]=50;
    int colX=0;
    int rowY=0;
    mazeEnd[xDestination][yDestination]=50;
    signed char l,r,t,d;
    for(cntLoops=0; cntLoops<=maxIterations*2; cntLoops++)
    {
        if((startReached==1) && (endReached==1))
        {
            cntLoops = (maxIterations*2)+1;
        }
        //***************************set mazeStart's values         checks now if values overflow


        for(colX=0;colX<=12;colX++)
        {
            for(rowY=12;rowY>=0;rowY--)
            {
                if(mazeStart[xDestination][yDestination]!=0){endReached = 1;break;};
                //check if the field has the path to the adjecant field
                l=r=t=d=0;
                if((hasDirection(left,colX,rowY)==1)  && (mazeStart[colX-1][rowY] != 0))
                {
                    if(colX>0)
                        l=mazeStart[colX-1][rowY]-1;
                }
                if((hasDirection(right,colX,rowY)==1) && (mazeStart[colX+1][rowY] != 0))
                {
                    if(colX<12)
                        r=mazeStart[colX+1][rowY]-1;
                }
                if((hasDirection(top,colX,rowY)==1)   && (mazeStart[colX][rowY+1] != 0))
                {
                    if(rowY<12)
                        t=mazeStart[colX][rowY+1]-1;
                }
                if((hasDirection(down,colX,rowY)==1)  && (mazeStart[colX][rowY-1] != 0))
                {
                    if(rowY>0)
                        d=mazeStart[colX][rowY-1]-1;
                }
                mazeStart[colX][rowY]=getLargestValue(l,t,r,d,mazeStart[colX][rowY]);

            }
        }
        //*************************** set mazeEnd's values
        for(colX=0;colX<=12;colX++)
        {
            for(rowY=12;rowY>=0;rowY--)
            {
                if((mazeEnd[x1][y1]!=0)){startReached = 1;break;};
                l=r=t=d=0;
                if((hasDirection(left,colX,rowY)==1)  && (mazeEnd[colX-1][rowY] != 0))
                {
                    if(colX>0)
                        l=mazeEnd[colX-1][rowY]-1;
                }
                if((hasDirection(right,colX,rowY)==1) && (mazeEnd[colX+1][rowY] != 0))
                {
                    if(colX<12)
                        r=mazeEnd[colX+1][rowY]-1;
                }
                if((hasDirection(top,colX,rowY)==1)   && (mazeEnd[colX][rowY+1] != 0))
                {
                    if(rowY<12)
                        t=mazeEnd[colX][rowY+1]-1;
                }
                if((hasDirection(down,colX,rowY)==1)  && (mazeEnd[colX][rowY-1] != 0))
                {
                    if(rowY>0)
                        d=mazeEnd[colX][rowY-1]-1;
                }
                mazeEnd[colX][rowY]=getLargestValue(l,t,r,d,mazeEnd[colX][rowY]);

            }
        }
    }
    //prnt values both matrixes now added into mazeStart
    int ty, tx;
    tx=ty=0;
    for(ty=12; ty>=0; ty--)
    {
        for(tx=0; tx<=12; tx++)
        {
            mazeStart[tx][ty]=mazeStart[tx][ty]+mazeEnd[tx][ty];
            mazeEnd[tx][ty]=0;
        }
    }
    //now add way of highest numbers to path so it can be printed or driven

    xCurrent = x1;
    yCurrent = y1;
    signed char xDisLeft = xDistance;
    signed char yDisLeft = yDistance;
    signed char pathValue = mazeStart[x1][y1];
    mazeEnd[xCurrent][yCurrent]=1;                 //repurposing mazeE to store already visited Nodes

    if(endReached==1)
        {
            while((xCurrent!=xDestination) || (yCurrent != yDestination))
            {
                if((xDisLeft>0)||(((mazeEnd[xCurrent-1][yCurrent]==1)||(mazeStart[xCurrent-1][yCurrent]!=pathValue||(hasDirection(left,xCurrent,yCurrent)==0)))&&((mazeEnd[xCurrent][yCurrent+1]==1)||(mazeStart[xCurrent][yCurrent+1]!=pathValue)||(hasDirection(top,xCurrent,yCurrent)==0))&&((mazeEnd[xCurrent][yCurrent-1]==1)||(mazeStart[xCurrent][yCurrent-1]!=pathValue)||(hasDirection(down,xCurrent,yCurrent)==0))))
                {
                    if(hasDirection(right,xCurrent,yCurrent) && (mazeStart[xCurrent+1][yCurrent]==pathValue) && (mazeEnd[xCurrent+1][yCurrent]==0))
                    {
                        addToPath(right);
                        mazeEnd[xCurrent][yCurrent]=1;
                        xCurrent++;
                        xDisLeft--;
                    }
                }
                if((xCurrent==xDestination) && (yCurrent == yDestination))
                    break;
                if(xDisLeft<0||(((mazeEnd[xCurrent+1][yCurrent]==1)||(mazeStart[xCurrent+1][yCurrent]!=pathValue)||(hasDirection(right,xCurrent,yCurrent)==0))&&((mazeEnd[xCurrent][yCurrent+1]==1)||(mazeStart[xCurrent][yCurrent+1]!=pathValue)||(hasDirection(top,xCurrent,yCurrent)==0))&&((mazeEnd[xCurrent][yCurrent-1]==1)||(mazeStart[xCurrent][yCurrent-1]!=pathValue)||(hasDirection(down,xCurrent,yCurrent)==0))))
                {
                    if(hasDirection(left,xCurrent,yCurrent) && (mazeStart[xCurrent-1][yCurrent]==pathValue) && (mazeEnd[xCurrent-1][yCurrent]==0))
                    {
                        addToPath(left);
                        mazeEnd[xCurrent][yCurrent]=1;
                        xCurrent--;
                        xDisLeft++;
                    }
                }
                if((xCurrent==xDestination) && (yCurrent == yDestination))
                    break;
                if(yDisLeft>0||(((mazeEnd[xCurrent+1][yCurrent]==1)||(mazeStart[xCurrent+1][yCurrent]!=pathValue)||(hasDirection(right,xCurrent,yCurrent)==0))&&((mazeEnd[xCurrent][yCurrent-1]==1)||(mazeStart[xCurrent][yCurrent-1]!=pathValue)||(hasDirection(down,xCurrent,yCurrent)==0))&&((mazeEnd[xCurrent-1][yCurrent]==1)||(mazeStart[xCurrent-1][yCurrent]!=pathValue)||(hasDirection(left,xCurrent,yCurrent)==0))))
                {
                    if(hasDirection(top,xCurrent,yCurrent) && (mazeStart[xCurrent][yCurrent+1]==pathValue) && (mazeEnd[xCurrent][yCurrent+1]==0))
                    {
                        addToPath(top);
                        mazeEnd[xCurrent][yCurrent]=1;
                        yCurrent++;
                        yDisLeft--;
                    }
                }
                if((xCurrent==xDestination) && (yCurrent == yDestination))
                    break;
                if(yDisLeft<0||(((mazeEnd[xCurrent+1][yCurrent]==1)||(mazeStart[xCurrent+1][yCurrent]!=pathValue)||(hasDirection(right,xCurrent,yCurrent)==0))&&((mazeEnd[xCurrent][yCurrent+1]==1)||(mazeStart[xCurrent][yCurrent+1]!=pathValue)||(hasDirection(top,xCurrent,yCurrent)==0))&&((mazeEnd[xCurrent-1][yCurrent]==1)||(mazeStart[xCurrent-1][yCurrent]!=pathValue)||(hasDirection(left,xCurrent,yCurrent)==0))))
                {
                    if(hasDirection(down,xCurrent,yCurrent) && (mazeStart[xCurrent][yCurrent-1]==pathValue) && (mazeEnd[xCurrent][yCurrent-1]==0))
                        {
                            addToPath(down);
                            mazeEnd[xCurrent][yCurrent]=1;
                            yCurrent--;
                            yDisLeft++;
                        }
                }
                if((xCurrent==xDestination) && (yCurrent == yDestination))
                    break;
            }
        }
/*
    display_goto_xy(0,3);
    display_string("ret:");
    display_goto_xy(4,3);
    display_int(pathStorage[1],1);
    display_update();*/
    //systick_wait_ms(2000);
}
void update_counter(){
	display_goto_xy(7,13);
	display_int((int)getNextCalled,3);
	display_update();
}

/* normal use funtions. These are the ones you should be using */
signed char getNextDirection()
{//returns relative Path

   signed char temp=0;
   signed char counter=0 ;
   if(pathStorage[0]!=0)
   {
        temp = pathStorage[1];
        pathStorage[0]--;

        /*
        if((temp==2)&&(directionOffset==1))
        {
        	temp=1;
        }else{
            temp = shiftBits(temp,directionOffset);
        }*/
        /*
        display_clear(1);
        display_goto_xy(0,0);
        display_int(directionOffset,1);
        display_goto_xy(3,0);
        display_int(pathStorage[1],1);
        display_goto_xy(1,6);
        display_int(temp,1);*/
        temp = shiftBitsForDriving(temp,directionOffset);
        /*
        display_goto_xy(3,7);
        display_int(temp,1);
        */
        switch(pathStorage[1])
        {
            case 1: hasMoved(top);directionOffset  = 0;break;
            case 2: hasMoved(right);directionOffset= 1;break;
            case 4: hasMoved(down);directionOffset = 2;break;
            case 8: hasMoved(left);directionOffset = 3;break;
            case 0: break;
            default: break;
        }
        /*
        display_goto_xy(1,1);
        display_int(directionOffset,1);
        display_update();*/
        //systick_wait_ms(1200);
        directionOffset = directionOffset % 4;
        //ecrobot_sound_tone(100,100, 100);
		//*/

        for(counter = 1; counter <=23;counter++){pathStorage[counter]=pathStorage[counter+1];}pathStorage[24]=0;
        return temp;
   }else{

        getNextUnvisited();
    	//ecrobot_sound_tone(900,80, 100);
        /*
        display_goto_xy(13,0);
        display_int(pathStorage[0],1);
        display_update();*/
        //systick_wait_ms(500);
        return getNextDirection();
   }
   return 0;
}
void checkIfAllVisited(){
	char allHaveBeenVisited=1;
	signed char counter1=0;
	signed char counter2=0;
	for(counter1=0;counter1<=12;counter1++)
	{
		for(counter2=0;counter2<=12;counter2++)
		{
			if(mazeStorage[counter1][counter2]!=0)
			{
				if(visited(counter1,counter2)==0)
				{
					allHaveBeenVisited=0;
				}
			}
		}
	}
	if(allHaveBeenVisited)
	{
		allVisited=1;
		//throwError(12);
	}
}
void getNextUnvisited()
{
	checkIfAllVisited();
    signed char x1 = currentPosition[0];
    signed char y1 = currentPosition[1];
    signed char cnt =0;
    signed char colX=0;
    signed char compare;
    signed char rowY=0;
    signed char foundOne=0;
    //ecrobot_sound_tone(900,90,100);

    if((allVisited)&&(getTokensFound()!=3))
    {
    	addToPath(none);
    	return;
    }
    if(getTokensFound()==3)
    {
    	getPathComplicated(x1,y1,6,6);
    	return;
    }

    signed char l,t,r,d;
    for(colX=0; colX<=12;colX++){
        for(rowY=0; rowY<=12;rowY++){
            mazeVisited[colX][rowY]=visited(colX,rowY);
            mazeDistance[colX][rowY]=0;
        }
    }

    //now start calculatin'
    mazeDistance[x1][y1]=50;
    for(cnt=0; cnt<=35 ; cnt++)//nah, don't calculate how much: just do it ;)
    {
        for(colX=0;colX<=12;colX++)
        {
            for(rowY=12;rowY>=0;rowY--)
            {
                //if((mazeDistance[x1][y1]!=0)){foundOne !=0 ;break;};
                l=r=t=d=0;
                if((hasDirection(left,colX,rowY)==1)  && (mazeDistance[colX-1][rowY] != 0))
                {
                    if(colX>0)
                        l=mazeDistance[colX-1][rowY]-1;
                }
                if((hasDirection(right,colX,rowY)==1) && (mazeDistance[colX+1][rowY] != 0))
                {
                    if(colX<12)
                        r=mazeDistance[colX+1][rowY]-1;
                }
                if((hasDirection(top,colX,rowY)==1)   && (mazeDistance[colX][rowY+1] != 0))
                {
                    if(rowY<12)
                        t=mazeDistance[colX][rowY+1]-1;
                }
                if((hasDirection(down,colX,rowY)==1)  && (mazeDistance[colX][rowY-1] != 0))
                {
                    if(rowY>0)
                        d=mazeDistance[colX][rowY-1]-1;
                }
                mazeDistance[colX][rowY]=getLargestValue(l,t,r,d,mazeDistance[colX][rowY]);

            }
        }
    }

    for(colX=0; colX<=12;colX++){
        for(rowY=0; rowY<=12;rowY++){
            mazeDistance[colX][rowY]=50-mazeDistance[colX][rowY];
        }
    }

    for(compare=0; compare<=35;compare++)
    {
        if(foundOne){return;};
        for(rowY=12;rowY>=0;rowY--)
        {
            if(foundOne){return;};
            for(colX=0;colX<=12;colX++)
            {
                if((mazeVisited[colX][rowY]==0)&&(mazeDistance[colX][rowY]==compare))
                {
                    getPathComplicated(x1,y1,colX,rowY);
                    foundOne =1;
                    return;
                }
            }
        }
    }
    //alle besucht
    ecrobot_sound_tone(400,250,100);
    systick_wait_ms(250);
    ecrobot_sound_tone(900,250,100);
    systick_wait_ms(250);
    if(!allVisited)
    	getPathComplicated(x1,y1,6,6);
    setAllVisited(1);
}
signed char hasBeenVisitedRelative(signed char direction)
{//TODO: test, srsly test the fuck out of that stuff

    direction=shiftBits(direction,directionOffset);//seems to work so far :D

	switch (direction){
    case 0x01: return (mazeStorage[currentPosition[0]][currentPosition[1]+1] >> 4);
    case 0x02: return (mazeStorage[currentPosition[0]+1][currentPosition[1]] >> 4);
    case 0x04: return (mazeStorage[currentPosition[0]][currentPosition[1]-1] >> 4);
    case 0x08: return (mazeStorage[currentPosition[0]-14][currentPosition[1]] >> 4);
	default: return(0);//stupid conventions. I know what I do -.-
	}

    
}
signed char hasDirectionRelative(signed char direction)
{
	return hasDirection(shiftBitsForDriving(direction,directionOffset), currentPosition[0],currentPosition[1]);
}
signed char visited(signed char x, signed char y){
	if(allVisited)
		return 1;
    return (mazeStorage[x][y]>>4);
}
void getPathTo(char xDestination, char yDestination)//more or less outdated, still usable for a fewer arguments call
{
    signed char x1 = currentPosition[0];
    signed char y1 = currentPosition[1];



    getPathComplicated(x1,y1,xDestination,yDestination);
}
void setCurrentNode(char links, char unten, char rechts, char oben)
{

    setNode(currentPosition[0], currentPosition[1], links, unten, rechts, oben, (mazeStorage[currentPosition[0]][currentPosition[1]]>>4)+1);

}
