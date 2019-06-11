#include <reg51.h>
#include <stdio.h>
//@11.0592MHz OSC
/*
The leds look like:

|   |
  |
	  |
	|	|
|
  |

*/
unsigned char code DISP_Table[]={0x00,0x01,0x40,0x08,0x41,0x09,0x48,0x49,0x7f};
//			TO SHOW:								  000  100 010   001   110  101  011  111   8
//     This table is builded to show the cars' locations.

unsigned char code SCORE_Table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e, 0x79,0x71};
//     This table is builded to show the score numbers.

sbit adrd=P3^7;
sbit adwr=P3^6;
sbit diola=P2^5;
sbit dula=P2^6;
sbit wela=P2^7;
sbit csda=P3^2;
sbit beep=P2^3;
sbit P2_6=P2^6;
sbit P2_7=P2^7;
sbit P0_7=P0^7;

static unsigned char key;
static unsigned char cars[6]={7,7,7,7,7,7};
static char player=0;
static char pac=0;     //player and car at 0 position
static char update=0;
static char state=0;   //0 menu   1  play
static long msc=0;
static int timec=0;
static int frame=40;   //10ms a frame, 40 frames a cycle,  the cycle will be shorter and shorter
static int score=0;

void delay(int ms)  //@12MHz OSC
{
	int i=0,j=0;
	for(i=0;i<ms;i++)
	{
		for(j=0;j<123;j++);
	}
}
void calpac(){
  if(player==cars[0]){
    pac=player;
  }else{
	  pac=player+cars[0];
  }
}
void displayScore(){  //To show score
	int i=0;
	int dat=0;
  char scoreText[6]={0};
  int num=0;
  sprintf(scoreText,"%d",(int)score);
	while(scoreText[num]!=0)
		{
		 	scoreText[num]-='0';
			num++;
		}
	for(i=0;i<num;i++)
	{
	 	//P2_7 = 0;
		P0=~(1<<i);
		P2_7 = 1;
		P2_7 = 0;
		//P2_6 = 0;
		dat = scoreText[i];
		P0=SCORE_Table[dat];
		P2_6 = 1;
		P2_6 = 0;
		delay(2);
		P0=0;
		P2_6 = 1;
		P2_6 = 0;
	}
}
void display(){
  if(state==1){
	int i=0;
	int dat=0;
	for(i=0;i<6;i++)
	{
	 	//P2_7 = 0;
		P0=~(1<<i);
		P2_7 = 1;
		P2_7 = 0;
		//P2_6 = 0;
		if(i==0){
			calpac();
			dat=pac;
		}else{
			dat = cars[i];
		}
		P0=DISP_Table[dat];
		P2_6 = 1;
		P2_6 = 0;
		delay(2);
		P0=0;
		P2_6 = 1;
		P2_6 = 0;
	}
  }else{
    displayScore();
  }
}

void bb(){
		beep=0;
		delay(500);
		beep=1;
}
void InitTimer0(void)
{
    TMOD = 0x01;
    TH0 = 0x0DC;  //@11.0592MHz, 10msec
    TL0 = 0x00;
	  TF0 = 0x00;
    ET0 = 1;
    TR0 = 1;
	  EA = 1;
}

void Timer0Interrupt(void) interrupt 1
{
    TH0 = 0x0DC;   //10ms
    TL0 += 0x00;
		update=1;
	  msc++;
		timec++;
}
void die(){
	int i=0;
	for(;i<6;i++){
		cars[i]=7;
	}
}
void keys()     //Run this function will get the matrix keyboard's state and key
{
	extern unsigned char message;
	unsigned char temp;
//	EA=0;
  key=16;
  if(message!=27)
  {
    P3=0xfe;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(5);
      if(temp!=0xf0)
      {
        temp=P3;
        switch(temp)
        {
          case 0xee:
               key=0;
               break;
          case 0xde:
               key=1;
               break;
          case 0xbe:
               key=2;
               break;
          case 0x7e:
               key=3;
               break;
         }
         while(temp!=0xf0)
         {
           temp=P3;
           temp=temp&0xf0;
         }
         P1=0xfe;
      }
    }
    P3=0xfd;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(5);
      if(temp!=0xf0)
      {
        temp=P3;
        switch(temp)
        {
          case 0xed:
               key=4;
               break;

          case 0xdd:
               key=5;
               break;

          case 0xbd:
               key=6;
               break;

          case 0x7d:
               key=7;
               break;
         }
         while(temp!=0xf0)
         {
           temp=P3;
           temp=temp&0xf0;
         }
         P1=0xfc;
      }
      }
    P3=0xfb;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(5);
      if(temp!=0xf0)
      {
        temp=P3;
        switch(temp)
        {
          case 0xeb:
               key=8;
               break;

          case 0xdb:
               key=9;
               break;

          case 0xbb:
               key=10;
               break;

          case 0x7b:
               key=11;
               break;
         }
         while(temp!=0xf0)
         {
           temp=P3;
           temp=temp&0xf0;
           //beep=0;
         }
        P1=0xf8;
      }
      }
    P3=0xf7;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(5);
      if(temp!=0xf0)
      {
        temp=P3;
        switch(temp)
        {
          case 0xe7:
               key=12;
               break;

          case 0xd7:
               key=13;
               break;

          case 0xb7:
               key=14;
               break;

          case 0x77:
               key=15;
               break;
         }
         while(temp!=0xf0)
         {
           temp=P3;
           temp=temp&0xf0;
         }
         P1=0xf0;
      }
    }
  }else{
    key=16;
  }
	P1=0xff;
  P0=0xff;
  }

void main()
{
		int carOut=0;
	  int i=1;
	  int ori=0;
	  InitTimer0();
		while(1)
		{
		display();
		if(state==0){
		if(update){
			keys();
			update=0;
		  //die();
		if(key==9){
			state=1;
			for(i=0;i<6;i++) cars[i]=0;
			player=2;
      score=0;
			bb();
		}
		}
	}else{
		if(update){
			keys();
			update=0;
      if(timec<(int)(frame/4.0*3)){
				// Before 3/4 cycle, you cannot move your car to other locations
				// that another car has existed.
        ori=player;
			if(player!=7-cars[0]){
         if(key==5){
		    	switch(player){
			    	case 1:break;
		    		case 2:player=1;break;
		    		case 3:player=2;break;
			}
		}
		else if(key==13){
		      	switch(player){
		      		case 1:player=2;break;
		       		case 2:player=3;break;
	      			case 3:break;
			}
		}
		if(player==cars[0]){
			player=ori;
		}
	}
      }
      else{
				// Between 3/4 cycle and 4/4 cycle, you can move your car to other locations
				// that another car has existed.
				// In other words, your car is INVINCIBLE.
          if(key==5){
		  	   switch(player){
			    	case 1:break;
		    		case 2:player=1;break;
	    			case 3:player=2;break;
			}
          }

		else if(key==13){
		    	switch(player){
		    		case 1:player=2;break;
		    		case 2:player=3;break;
			    	case 3:break;
			}
		}
        }
      }
		if(timec>frame){  // A cycle ends, other cars come to you.
			carOut=(rand()%3)+1;  // The last car will come out at random place.
			if(rand()%2==0){
			carOut=7-carOut;
			// Decide whether the last car should be |--
			// or                                    -||
			}
			if(player==cars[1]||(cars[1]>=4&&player!=7-cars[1])){
				state=0;       // YOU DIED.
				display();
				bb();
			}else{
				score++;
				if(score%5==0&&frame>=20){
					frame--;   // Make the cycle shorter.
				}
			}
			for(i=1;i<5;i++){
				cars[i]=cars[i+1];  // Move the cars.
			}
			cars[5]=carOut;
	    timec=0;
	}
    }
		}
	}
