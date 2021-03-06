#include "stdlib.h"
#include <stdio.h>

#define top 0x01
#define right 0x02
#define down 0x04
#define left 0x08

#define DEBUG 0x01
unsigned char    mazeStorage[13][13];
unsigned char    pathStorage[25];//first char denotes length of current queue
unsigned char    currentPosition[2];//X, Y
int              dfsPointer;
char    stop = 0;
signed   char    directionOffset = 0;//number of times turned right

/* FUN.ction prototypes */

void initialize();
signed char shiftbits(signed char toShift, signed char shiftAmount);
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
void printPathStorage();
void printNodeHelp();
void printNode(char x, char y);
void printEVERYTHINGASBITMAP_FORCOMMANDLINE_OMGTHISNAMEISINALLCAPSHOWLONGDOESTHISFUNCTIONCONTINUETOBEEEEEEE();
void printFieldAsNumbers();

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

void initialize()
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
}

/* internal Pathplanning funtions */
signed char shiftbits(signed char toShift, signed char shiftAmount)
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
void hasMoved(unsigned char direction)
{
    //changes currentPosition according to direction WITH directional offset
    //direction = shiftbits(direction,directionOffset);

    if(direction == top )
        currentPosition[1]++;

    if(direction == right )
        currentPosition[0]++;

    if(direction == down )
        currentPosition[1]--;

    if(direction == left)
        currentPosition[0]--;


}
void addToPath(char direction)
{
    if(DEBUG){printf("\n added %i to path",direction);};
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
            if(DEBUG){printf("\n setting %i:%i to %i",x,y-1,mazeStorage[x][y-1] | 1);};
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
    if(DEBUG){printf("\nswitched %i times  setting %i:%i to (%iv:%io:%ir:%iu:%il):%i",switchCount,x,y,visited,oben,rechts,unten,links,temp);};

    mazeStorage[x][y] = temp;
}
void getPathComplicated(signed char x1, signed char y1, signed char xDestination, signed char yDestination)
{
    if(DEBUG){printf("\ngot called to calculate path to %i:%i from %i:%i",xDestination,yDestination,x1,y1);};
    if((x1==xDestination)&&(y1==yDestination))
    {
        if((x1==6)&&(y1==6))
        {
            yDestination=5;
        };
        xDestination =6;
        yDestination =6;
    }
    char mazeStart[14][14];
    char mazeEnd[14][14];
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
    char maxIterations = (xD2+yD2)/2+4;//4 is safety padding
    if(DEBUG){printf("\n\n\n\nmaxIterations=%i\n\n\n",maxIterations);};
    char cntLoops;
    /*if(xDistance*xDistance > yDistance*yDistance)
        {maxIterations = xDistance;}
    else
        {maxIterations = yDistance;}*///basically obsolete
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
    if(DEBUG){printf("\nset both mazes to 0");};
    mazeStart[x1][y1]=50;
    int colX=0;
    int rowY=0;
    mazeEnd[xDestination][yDestination]=50;
    signed char l,r,t,d;
    int xCounter1=0;
    int yCounter1=0;
    for(cntLoops=0; cntLoops<=maxIterations*2; cntLoops++)
    {
        if((startReached==1) && (endReached==1))
        {
            cntLoops = (maxIterations*2)+1;
        }
        //***************************set mazeStart's values ///         checks now if values overflow :D


        if(DEBUG){printf("\nS:                                          E:\n");};
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
        if(DEBUG)
        {
            for(yCounter1=12;yCounter1>=0;yCounter1--)
            {
                //print maze Start
                for(xCounter1=0;xCounter1<=12;xCounter1++)
                    {
                        if(mazeStart[xCounter1][yCounter1]==0)
                        {
                            printf(" %2c",250);
                        }else{
                        printf(" %2i",mazeStart[xCounter1][yCounter1]);
                        }
                    }
                    printf("  ");
                    //pritn maze end
                    for(xCounter1=0;xCounter1<=12;xCounter1++)
                    {
                        if(mazeEnd[xCounter1][yCounter1]==0)
                        {
                            printf(" %2c",250);
                        }else
                        {
                            printf(" %2i",mazeEnd[xCounter1][yCounter1]);
                        }
                    }
                printf("\n");
            }
        }
    }
    //print values both matrixes now added into mazeStart
    int ty, tx;
    tx=ty=0;
    if(DEBUG){printf("\n");}
    for(ty=12; ty>=0; ty--)
    {
        for(tx=0; tx<=12; tx++)
        {
            mazeStart[tx][ty]=mazeStart[tx][ty]+mazeEnd[tx][ty];
            mazeEnd[tx][ty]=0;
            if(DEBUG){
                if(mazeStart[tx][ty]==0)
                {
                    printf(" %2c",250);
                }else
                {
                    printf(" %2i",mazeStart[tx][ty]);
                }
            };
        }
        if(DEBUG){printf("\n");};
    }
    //now add way of highest numbers to path so it can be printed or driven

    xCurrent = x1;
    yCurrent = y1;
    signed char xDisLeft = xDistance;
    signed char yDisLeft = yDistance;
    signed char pathValue = mazeStart[x1][y1];
    mazeEnd[xCurrent][yCurrent]=1;                 //repurposing mazeEnd to store already visited Nodes

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
}

/* normal use funtions. These are the ones you should be using */
signed char getNextDirectionOld()
{//returns relative Path
   signed char temp=0;
   signed char counter =0 ;
   if(pathStorage[0])
   {
        temp = pathStorage[1];
        pathStorage[0]--;
        switch(pathStorage[1])
        {
            case 0x01: hasMoved(top);break;
            case 0x02: hasMoved(right);directionOffset++;break;
            case 0x04: hasMoved(down);directionOffset = directionOffset+2;break;
            case 0x08: hasMoved(left);directionOffset = directionOffset+3;break;
        }
        directionOffset = directionOffset % 4;
        for(counter = 1; counter <=23;counter++){pathStorage[counter]=pathStorage[counter+1];}pathStorage[24]=0;
        return temp;
   }else{
        getNextUnvisited();
        return getNextDirection();
   }
   return 0;
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

        temp = shiftBitsForDriving(temp,directionOffset);

        switch(pathStorage[1])
        {
            case 1: hasMoved(top);directionOffset  = 0;break;
            case 2: hasMoved(right);directionOffset= 1;break;
            case 4: hasMoved(down);directionOffset = 2;break;
            case 8: hasMoved(left);directionOffset = 3;break;
        }

        directionOffset = directionOffset % 4;

        //*/

        for(counter = 1; counter <=23;counter++){pathStorage[counter]=pathStorage[counter+1];}pathStorage[24]=0;
        return temp;
   }else{

        getNextUnvisited();
        //ecrobot_sound_tone(900,80, 100);

        //systick_wait_ms(500);
        return getNextDirection();
   }
   return 0;
}
void getNextUnvisited()
{
    signed char x1 = currentPosition[0];
    signed char y1 = currentPosition[1];
    signed char cnt =0;
    signed char colX=0;
    signed char compare;
    signed char rowY=0;
    signed char foundOne=0;

    signed char mazeVisited[13][13];
    signed char mazeDistance[13][13];
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
    if(DEBUG)
    {
        for(rowY=12;rowY>=0;rowY--)
        {
            for(colX=0;colX<=12;colX++)
            {
                if(mazeDistance[colX][rowY]==0)
                {
                    printf(" %2c",250);
                }else
                {
                    printf(" %2i",mazeDistance[colX][rowY]);
                }
            }
        printf("\n");
        printf("\n");
        for(rowY=12;rowY>=0;rowY--)
            {
            for(colX=0;colX<=12;colX++)
            {
                if(mazeVisited[colX][rowY]==0)
                {
                    printf(" %2c",250);
                }else
                {
                    printf(" %2i",mazeVisited[colX][rowY]);
                }
            }

            printf("\n");
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
        if(foundOne){break;};
        for(rowY=12;rowY>=0;rowY--)
        {
            if(foundOne){break;};
            for(colX=0;colX<=12;colX++)
            {
                if((mazeVisited[colX][rowY]==0)&&(mazeDistance[colX][rowY]==compare))
                {
                    getPathComplicated(x1,y1,colX,rowY);
                    foundOne =1;
                    break;
                }
            }
        }
    }
    if(DEBUG)
    {
        printf("\n");
        for(rowY=12;rowY>=0;rowY--)
        {
            for(colX=0;colX<=12;colX++)
            {
                if(mazeDistance[colX][rowY]==50)
                {
                    printf(" %2c",250);
                }else
                {
                    printf(" %2i",mazeDistance[colX][rowY]);
                }
            }
        printf("\n");
        }
    }


}
signed char hasBeenVisitedRelative(signed char direction)
{//TODO: test, srsly test the fuck out of that stuff

    direction=shiftbits(direction,directionOffset);//seems to work so far :D

	switch (direction){
    case 0x01: return (mazeStorage[currentPosition[0]][currentPosition[1]+1] >> 4);
    case 0x02: return (mazeStorage[currentPosition[0]+1][currentPosition[1]] >> 4);
    case 0x04: return (mazeStorage[currentPosition[0]][currentPosition[1]-1] >> 4);
    case 0x08: return (mazeStorage[currentPosition[0]-14][currentPosition[1]] >> 4);
	default: return(0);//stupid conventions. I know what I do -.-
	}

    
}
signed char visited(signed char x, signed char y){
    return (mazeStorage[x][y]>>4);
}
void getPathTo(char xDestination, char yDestination)//more or less outdated, still usable for a fewer arguments call
{
    signed char x1 = currentPosition[0];
    signed char y1 = currentPosition[1];
    signed char distanceX = xDestination - x1;
    signed char distanceY = yDestination - y1;

    char value = mazeStorage[(int)x1][(int)y1];


        char temp[10];

        temp[0]=(value >>4)+48;//+48 for ascii
        temp[1]=58;
        temp[2]=(value & 8)+48;
        temp[3]=58;
        temp[4]=(value & 4)+48;
        temp[5]=58;
        temp[6]=(value & 2)+48;
        temp[7]=58;
        temp[8]=(value & 1)+48;
        temp[9]='\0';

        char temp1[10];
        value = mazeStorage[(int)xDestination][(int)yDestination];
        temp1[0]=(value >>4)+48;//+48 for ascii
        temp1[1]=58;
        temp1[2]=(value & 8)+48;
        temp1[3]=58;
        temp1[4]=(value & 4)+48;
        temp1[5]=58;
        temp1[6]=(value & 2)+48;
        temp1[7]=58;
        temp1[8]=(value & 1)+48;
        temp1[9]='\0';


    printf("\nformat specified: x:y(v:t:r:d:l) to x:y(v:t:r:d:l)");
    printf("\ngetting path from %i:%i(%s) to %i:%i(%s)\n", x1,y1,temp,xDestination,yDestination,temp1);
    printf("distanceX:%i\ndistanceY:%i\n\n",distanceX, distanceY);


    getPathComplicated(x1,y1,xDestination,yDestination);
}
void setCurrentNode(char links, char unten, char rechts, char oben)
{

    setNode(currentPosition[0], currentPosition[1], links, unten, rechts, oben, (mazeStorage[currentPosition[0]][currentPosition[1]]>>4)+1);

}


/* mah console functions: PLZ DONT TATSCH */
void printCurrentPositionInfo()
{

    printf("\npos:%i:%i||Direction:",currentPosition[0],currentPosition[1]);
    switch (directionOffset){
    case 0x00: printf("North"); break;
    case 0x01: printf("East"); break;
    case 0x02: printf("South"); break;
    case 0x03: printf("West"); break;
    }
}
void printPathStorage()
{
    signed char pathLength = pathStorage[0];
    signed char cnt;
    printf("\nLength of stored path is %i", pathLength);
    for(cnt=1; cnt<=pathLength+1;cnt++)
    {
        switch(pathStorage[cnt]){
            case 1: printf("\ntop");break;
            case 2: printf("\nright");break;
            case 4: printf("\ndown");break;
            case 8: printf("\nleft");break;
        }


    }
}
void printNode(char x, char y)
{
    char temp1[10];
    char value = mazeStorage[(int)x][(int)y];
    temp1[0]=(value >>4)+48;//+48 for ascii
    temp1[1]=58;
    temp1[2]=(value & 1)+48;
    temp1[3]=58;
    temp1[4]=(value & 2)+48;
    temp1[5]=58;
    temp1[6]=(value & 4)+48;
    temp1[7]=58;
    temp1[8]=(value & 8)+48;
    temp1[9]='\0';

    //printf("\nX:Y(v:t:r:d:l)");
    printf("%i:%i(%s)", x,y,temp1);
}
void printNodeHelp()
{
    printf("\nX:Y(v:t:r:d:l)");
}
void printEVERYTHINGASBITMAP_FORCOMMANDLINE_OMGTHISNAMEISINALLCAPSHOWLONGDOESTHISFUNCTIONCONTINUETOBEEEEEEE()
{
    int t1, t2,t3;
    t1=t2=t3=0;
    printf("\n");
    for(t1=12; t1>=0; t1--)
    {
        for(t2=0; t2<=12; t2++)
        {
            t3 = mazeStorage[t2][t1]%16;

            //t3 = t3 >>4;
            if(t3==0x00){printf(" %2c",250);}
            else if(t3==0x01){printf(" %2c", 39);}
            else if(t3==0x02){printf(" %2c",175);}
            else if(t3==0x03){printf(" %2c",200);}
            else if(t3==0x04){printf(" %2c", 44);}
            else if(t3==0x05){printf(" %2c",179);}
            else if(t3==0x06){printf(" %2c",218);}
            else if(t3==0x07){printf(" %2c",195);}
            else if(t3==0x08){printf(" %2c",174);}
            else if(t3==0x09){printf(" %2c",217);}
            else if(t3==0x0a){printf(" %2c",196);}
            else if(t3==0x0b){printf(" %2c",193);}
            else if(t3==0x0c){printf(" %2c",187);}
            else if(t3==0x0d){printf(" %2c",185);}
            else if(t3==0x0e){printf(" %2c",194);}
            else if(t3==0x0f){printf(" %2c",197);}
        }
        printf("\n");
    }
}
void printFieldAsNumbers()
{
    int t1,t2,t3;
    t1=t2=t3=0;
    printf("\n");
    for(t1=12; t1>=0; t1--)
    {
        for(t2=0; t2<=12; t2++)
        {
            t3 = mazeStorage[t2][t1]%16;
           //t3 = t3 >>4;
            printf("%2i ",t3);
        }
        printf("\n");
    }
}


int main(void)
{
    initialize();

    //setNode(x,y,links,unten,rechts,oben,visited);
    //printEVERYTHINGASBITMAP_FORCOMMANDLINE_OMGTHISNAMEISINALLCAPSHOWLONGDOESTHISFUNCTIONCONTINUETOBEEEEEEE();

    setNode(6,6,0,0,0,1,1);
    currentPosition[0]=6;
    currentPosition[1]=6;
    //printf("\n%i",hasBeenVisitedRelative(right));
    printEVERYTHINGASBITMAP_FORCOMMANDLINE_OMGTHISNAMEISINALLCAPSHOWLONGDOESTHISFUNCTIONCONTINUETOBEEEEEEE();
    //directionOffset =1;//aka 1x rechts
    //printf(" ");
    //printNodeHelp();
    //printf("\n");
    //printNode(4,1);
    //printf("\n");
    //printNode(4,0);
    //printf("\n");
    //printNode(2,0);
    //printPathStorage();
    //printEVERYTHINGASBITMAP_FORCOMMANDLINE_OMGTHISNAMEISINALLCAPSHOWLONGDOESTHISFUNCTIONCONTINUETOBEEEEEEE();
    //getPathComplicated(2,1,6,4);
    //printEVERYTHINGASBITMAP_FORCOMMANDLINE_OMGTHISNAMEISINALLCAPSHOWLONGDOESTHISFUNCTIONCONTINUETOBEEEEEEE();
    //printPathStorage();
    /*printCurrentPositionInfo();
	directionOffset++;
	printCurrentPositionInfo();
	printf("\n%i\n", hasBeenVisitedRelative(right));

    printNode(6, 1);
    printf("\n");
    */
    //printPathStorage();
    //directionOffset++;
    setNode(6,6,1,1,1,1,1);

    setNode(5,6,0,1,1,0,1);
    setNode(5,5,1,0,1,1,1);
    setNode(4,5,0,0,1,0,1);
    directionOffset=3;
    currentPosition[0]=4;
    currentPosition[1]=5;
    printEVERYTHINGASBITMAP_FORCOMMANDLINE_OMGTHISNAMEISINALLCAPSHOWLONGDOESTHISFUNCTIONCONTINUETOBEEEEEEE();
    printf("\n");
    //printf("\n%i",getNextDirection());
    //getNextDirection();
    getPathComplicated(4,5,6,6);
    /*
    printPathStorage();
    printf("\n");
    printf("\n%i",getNextDirection());
    printPathStorage();
    printf("\n");
    printf("\n%i",getNextDirection());
    printPathStorage();
    printf("\n");
    printf("\n%i",getNextDirection());

    printPathStorage();
    printf("\n");
    printf("\n%i\n",getNextDirection());
    currentPosition[0]=7;
    currentPosition[1]=11;
    setNode(7,11,0,1,1,0,1);
    printPathStorage();
    printf("\n");
    printf("\n%i",getNextDirection());
    //setNode(6,7,0,1,1,1,1);
    //printEVERYTHINGASBITMAP_FORCOMMANDLINE_OMGTHISNAMEISINALLCAPSHOWLONGDOESTHISFUNCTIONCONTINUETOBEEEEEEE();
    //printf("\n%i",getNextDirection());
    //printf("\n%i",getNextDirection());
    //*/
    return 0;
}
