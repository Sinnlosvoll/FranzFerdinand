#include <stdio.h>

#define left 0x06
#define top 0x09
#define down 0x07
#define right 0x08
#define DEBUG 0x01

char    mazeStorage[13][13];
char    pathStorage[25];//first char denotes length of current queue
char    currentPosition[2];
char    stop = 0;
char    directionOffset = 0;//number of times turned right


/* abstract:
 * setting a node seems to work for now, further investigation is advised
 * also when directionOffset is active
 *
 * NOT YET WORKING
 * - exploration
 * - executing a path
 *
 *
 */


/* PATHPLANNING THOUGHTS
 *
 * go direction best suited for your goal, minimize larger value of distances
 *
 *
 * alternative: create 2 new strucutres. One from the start and one from the end. The structures
 * contain the number of nodes needed to get there. Overlay both and get the smallest number greater 0 which exists in both structures.
 * Then create 2 path going from start to overlap and from overlap to end. RECURSIVE until special cases abort.
 * Cases which will go further and never stop:
 *              the 2 points are the same
 *              the points are next to each other and so they aren't >0 in both cases
 */

void initialize()
{
    int i,n;
    /* ALle KReuzungen AUf 0 SEtzen */
    for(i=0; i<=13;i++){
        for(n=0; n<=13;n++){
            mazeStorage[i][n]=0;
        }
    }
    /* STartkreuzung AUf 7,7 SEtzen */
    currentPosition[0] = 6;
    currentPosition[1] = 6;
    pathStorage[0]=0;

}
void addToPath(char direction)
{
    pathStorage[(int)pathStorage[0]+1]=direction;
}

int hasDirection(char direction,unsigned char x,unsigned char y)
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
int getLargestGreaterZero(char c1, char c2, char c3, char c4, char own){
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

int getPathComplicated(signed char x1, signed char y1, signed char xDestination, signed char yDestination)
{
    char mazeStart[14][14];
    char mazeEnd[14][14];
    signed char xCurrent;
    signed char yCurrent;
    signed char xDistance= xDestination - x1;
    signed char yDistance= yDestination - y1;
    char maxIterations = (xDistance+yDistance)/2+4;//4 is safety padding
    char cntLoops;
    /*if(xDistance*xDistance > yDistance*yDistance)
        {maxIterations = xDistance;}
    else
        {maxIterations = yDistance;}*///basically obsolete
    if(maxIterations<0)
        maxIterations=-maxIterations;

    //************************************** MAIN BODY ******
    int x,y;
    for(x=0;x<=13;x++)
    {
        for(y=0;y<=13;y++)
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
        //***************************set mazeStart's values ///         checks now if values overflow :D


        if(DEBUG){printf("\nS:                                          E:\n");};
        for(colX=0;colX<=13;colX++)
        {
            for(rowY=13;rowY>=0;rowY--)
            {
                //check if the field has the path to the adjecant field
                l=r=t=d=0;
                if((hasDirection(left,colX,rowY)==1)  && (mazeStart[colX-1][rowY] != 0))
                {
                    if(colX>0)
                        l=mazeStart[colX-1][rowY]-1;
                }
                if((hasDirection(right,colX,rowY)==1) && (mazeStart[colX+1][rowY] != 0))
                {
                    if(colX<13)
                        r=mazeStart[colX+1][rowY]-1;
                }
                if((hasDirection(top,colX,rowY)==1)   && (mazeStart[colX][rowY+1] != 0))
                {
                    if(rowY<13)
                        t=mazeStart[colX][rowY+1]-1;
                }
                if((hasDirection(down,colX,rowY)==1)  && (mazeStart[colX][rowY-1] != 0))
                {
                    if(rowY>0)
                        d=mazeStart[colX][rowY-1]-1;
                }
                mazeStart[colX][rowY]=getLargestGreaterZero(l,t,r,d,mazeStart[colX][rowY]);
            }
        }
        //*************************** set mazeEnd's values
        for(colX=0;colX<=13;colX++)
        {
            for(rowY=13;rowY>=0;rowY--)
            {
                l=r=t=d=0;
                if((hasDirection(left,colX,rowY)==1)  && (mazeEnd[colX-1][rowY] != 0))
                {
                    if(colX>0)
                        l=mazeEnd[colX-1][rowY]-1;
                }
                if((hasDirection(right,colX,rowY)==1) && (mazeEnd[colX+1][rowY] != 0))
                {
                    if(colX<13)
                        r=mazeEnd[colX+1][rowY]-1;
                }
                if((hasDirection(top,colX,rowY)==1)   && (mazeEnd[colX][rowY+1] != 0))
                {
                    if(rowY<13)
                        t=mazeEnd[colX][rowY+1]-1;
                }
                if((hasDirection(down,colX,rowY)==1)  && (mazeEnd[colX][rowY-1] != 0))
                {
                    if(rowY>0)
                        d=mazeEnd[colX][rowY-1]-1;
                }
                mazeEnd[colX][rowY]=getLargestGreaterZero(l,t,r,d,mazeEnd[colX][rowY]);

            }
        }
        for(yCounter1=13;yCounter1>=0;yCounter1--)
        {
            //print maze Start
            for(xCounter1=0;xCounter1<=13;xCounter1++)
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
            for(xCounter1=0;xCounter1<=13;xCounter1++)
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
    //print values both matrixes now pushed into mazeStart
    int ty, tx;
    tx=ty=0;
    printf("\n");
    for(ty=13; ty>=0; ty--)
    {
        for(tx=0; tx<=13; tx++)
        {
            mazeStart[tx][ty]=mazeStart[tx][ty]+mazeEnd[tx][ty];
            mazeEnd[tx][ty]=0;
            if(mazeStart[tx][ty]==0)
            {
                printf(" %2c",250);
            }else
            {
                printf(" %2i",mazeStart[tx][ty]);
            }
        }
        printf("\n");
    }
    //now add way of highest numbers to path so it can be printed or driven

    xCurrent = x1;
    yCurrent = y1;
    signed char xDisLeft = xDistance;
    signed char yDisLeft = yDistance;
    signed char pathValue = mazeStart[x1][y1];
    mazeEnd[xCurrent][yCurrent]=1;                 //repurposing mazeEnd to store already visited Nodes

    /*while((xCurrent!=xDestination) && (yCurrent != yDestination))
    {
        if(xDisLeft>0)
        {
            if(hasDirection(right,xCurrent,yCurrent) && (mazeStart[xCurrent+1][yCurrent]==pathValue))
            {
                addToPath(right);
                mazeEnd[xCurrent][yCurrent]=1;
                xCurrent++;
                xDisLeft--;
            }
        }else
            if(xDisLeft<0)
            {
                if(hasDirection(left,xCurrent,yCurrent) && (mazeStart[xCurrent-1][yCurrent]==pathValue))
                {
                    addToPath(left);
                    mazeEnd[xCurrent][yCurrent]=1;
                    xCurrent--;
                    xDisLeft++;
                }
            }
        if(yDisLeft>0)
        {
            if(hasDirection(top,xCurrent,yCurrent) && (mazeStart[xCurrent][yCurrent+1]==pathValue))
            {
                addToPath(top);
                mazeEnd[xCurrent][yCurrent]=1;
                yCurrent++;
                yDisLeft--;
            }
        }else
            if(xDisLeft<0)
            {
                if(hasDirection(down,xCurrent,yCurrent) && (mazeStart[xCurrent][yCurrent-1 ]==pathValue))
                {
                    addToPath(left);
                    mazeEnd[xCurrent][yCurrent]=1;
                    yCurrent--;
                    yDisLeft++;
                }
            }
    }*/

    return 0; //aka no path found
}


int getPathTo(char xDestination, char yDestination)
{
    signed char x1 = currentPosition[0];
    signed char y1 = currentPosition[1];
    char xCurrent = x1; char yCurrent = y1;
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


    //single line cases
    if(distanceX == 0)
    {
        if(distanceY>=0)
        {
            char counter = 0;
            char cntMax = distanceY;
            char possiblePath = 1;
            for(counter=0; counter <= cntMax; counter++)
            {
                yCurrent = y1 + counter;

                if(hasDirection(top,xCurrent,yCurrent)==0)
                {
                    if(DEBUG){printf("\nfound error for straight line at %i:%i:top", xCurrent, yCurrent);};
                    possiblePath = 0;
                }
            }
            if(possiblePath==1)
            {
                for(counter=0; counter <= cntMax; counter++)
                {
                    addToPath(top);
                }
                return 1;
            }
        }
        if(distanceY<0)
        {
            char counter = 0;
            char cntMax = -distanceY;
            char possiblePath = 1;
            for(counter=0; counter <= cntMax; counter++)
            {
                yCurrent = y1 + counter;

                if(hasDirection(down,xCurrent,yCurrent)==0)
                {
                    if(DEBUG){printf("\nfound error for straight line at %i:%i:down", xCurrent, yCurrent);};
                    possiblePath = 0;
                }
            }
            if(possiblePath==1)
            {
                for(counter=0; counter <= cntMax; counter++)
                {
                    addToPath(down);
                }
                return 1;
            }
        }


    }
    if(distanceY == 0)
    {
        if(distanceX>=0)
        {
            char counter = 0;
            char cntMax = distanceX;
            char possiblePath = 1;
            for(counter=0; counter <= cntMax; counter++)
            {
                yCurrent = y1 + counter;

                if(hasDirection(right,xCurrent,yCurrent)==0)
                {
                    if(DEBUG){printf("\nfound error for straight line at %i:%i:right", xCurrent, yCurrent);};
                    possiblePath = 0;
                }
            }
            if(possiblePath==1)
            {
                for(counter=0; counter <= cntMax; counter++)
                {
                    addToPath(right);
                }
                return 1;
            }
        }
        if(distanceX<0)
        {
            char counter = 0;
            char cntMax = -distanceX;
            char possiblePath = 1;
            for(counter=0; counter <= cntMax; counter++)
            {
                yCurrent = y1 + counter;

                if(hasDirection(left,xCurrent,yCurrent)==0)
                {
                    possiblePath = 0;
                    if(DEBUG){printf("\nfound error for straight line at %i:%i:left", xCurrent, yCurrent);};
                }
            }
            if(possiblePath==1)
            {
                for(counter=0; counter <= cntMax; counter++)
                {
                     addToPath(left);
                }
                return 1;
            }
        }


    }
    //the REST
    if(getPathComplicated(x1,y1,xDestination,yDestination)==1)
    {
        return 1;
    }
    else
    {
        return 0; //aka no way found
    }


}


void setNode(unsigned char x, unsigned char y,char links, char unten, char rechts, char oben, char visited)
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
            mazeStorage[(int)x][(int)y-1] = mazeStorage[(int)x][(int)y-1] | 1;
        }
        temp += 4;
    }
    if(rechts  ==0x01)
    {
        if(x<13)
        {
            mazeStorage[(int)x+1][(int)y] = mazeStorage[(int)x+1][(int)y] | 8;
        }
        temp += 2;
    }
    if(oben    ==0x01)
    {
        if(y<13)
        {
            mazeStorage[(int)x][(int)y+1] = mazeStorage[(int)x][(int)y+1] | 4;
        }
        temp += 1;
    }
    if(DEBUG){printf("\nswitched %i times  setting %i:%i to (%iv:%io:%ir:%iu:%il):%i",switchCount,x,y,visited,oben,rechts,unten,links,temp);};

    mazeStorage[x][y] = temp;
}

void setCurrentNode(char links, char unten, char rechts, char oben, char visited)
{

    setNode(currentPosition[0], currentPosition[1], links, unten, rechts, oben, visited);

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

void printEVERYTHINGASBITMAP_OMGTHISNAMEISINALLCAPSHOWLONGDOESTHISFUNCTIONCONTINUETOBEEEEEEE()
{
    int t1, t2,t3;
    t1=t2=t3=0;
    printf("\n");
    for(t1=13; t1>=0; t1--)
    {
        for(t2=0; t2<=13; t2++)
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
    for(t1=13; t1>=0; t1--)
    {
        for(t2=0; t2<=13; t2++)
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
    printEVERYTHINGASBITMAP_OMGTHISNAMEISINALLCAPSHOWLONGDOESTHISFUNCTIONCONTINUETOBEEEEEEE();
    setNode(2,1,1,1,1,1,1);
    setNode(6,6,1,0,1,0,1);
    printEVERYTHINGASBITMAP_OMGTHISNAMEISINALLCAPSHOWLONGDOESTHISFUNCTIONCONTINUETOBEEEEEEE();
    //directionOffset =1;//aka 1x rechts
    setNode(8,8,1,0,0,0,1);
    setNode(5,5,1,1,1,1,1);
    setNode(4,1,1,0,1,1,1);
    setNode(4,3,0,1,1,1,1);
    setNode(4,4,0,1,1,1,1);
    setNode(9,9,1,1,1,1,1);
    setNode(9,7,0,1,0,1,1);
    setNode(8,6,1,1,1,1,1);
    printf(" ");
    printNodeHelp();
    printf("\n");
    printNode(4,1);
    printf("\n");
    printNode(4,0);
    printf("\n");
    printNode(2,0);
    printEVERYTHINGASBITMAP_OMGTHISNAMEISINALLCAPSHOWLONGDOESTHISFUNCTIONCONTINUETOBEEEEEEE();
    getPathComplicated(2,1,9,9);
    printEVERYTHINGASBITMAP_OMGTHISNAMEISINALLCAPSHOWLONGDOESTHISFUNCTIONCONTINUETOBEEEEEEE();
    printf("\n");
    return 0;
}
