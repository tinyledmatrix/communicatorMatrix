#include "ch32fun.h"
#include <stdio.h>
#include "font5x7.h"
//#include <ch32v00x_conf.h> 
#include "flash.h"

#define COL_COUNT 30
#define ROW_COUNT 7
#define BUFFER_COL_COUNT (((32)*6)+10)   //30*8=180

#define ANODE_PIN 0
#define CATHODE_PIN 1

#define LPin1 PA1
#define LPin2 PD7
#define LPin3 PD2
#define LPin4 PD6
#define LPin5 PC3
#define LPin6 PD4
#define LPin7 PC7
#define LPin8 PD5
#define LPin9 PC0

#define LPinA PD3
#define LPinB PC4
#define LPinC PD0
#define LPinD PC5
#define LPinE PA2
#define LPinF PC6

static const uint8_t LED_MATRIX [ROW_COUNT][COL_COUNT][2]=     // row, col, LED-anode, LED-cathode - this maps the LEDs to the Matrix 
{
//    1               2               3               4               5               6               7               8               9               10              11
{{LPin2,LPin1},{LPin1,LPin2},{LPin4,LPin2},{LPin2,LPin4},{LPin6,LPin4},{LPin4,LPin6},{LPin8,LPin6},{LPin6,LPin8},{LPinA,LPin8},{LPin8,LPinA},{LPinC,LPinA},{LPinA,LPinC},{LPinE,LPinC},{LPinC,LPinE},{LPinF,LPinE},{LPin9,LPin8},{LPinA,LPin7},{LPin7,LPinA},{LPinC,LPin5},{LPin5,LPinC},{LPinE,LPin3},{LPin3,LPinE},{LPinF,LPin1},{LPin1,LPinF},{LPinD,LPin2},{LPin2,LPinD},{LPinB,LPin4},{LPin4,LPinB},{LPin9,LPin6},{LPin6,LPin9}},
{{LPin3,LPin2},{LPin4,LPin1},{LPin1,LPin4},{LPin6,LPin2},{LPin2,LPin6},{LPin8,LPin4},{LPin4,LPin8},{LPinA,LPin6},{LPin6,LPinA},{LPinC,LPin8},{LPin8,LPinC},{LPinE,LPinA},{LPinA,LPinE},{LPinF,LPinC},{LPinC,LPinF},{LPinA,LPin9},{LPin9,LPinA},{LPinC,LPin7},{LPin7,LPinC},{LPinE,LPin5},{LPin5,LPinE},{LPinF,LPin3},{LPin3,LPinF},{LPinD,LPin1},{LPin1,LPinD},{LPinB,LPin2},{LPin2,LPinB},{LPin9,LPin4},{LPin4,LPin9},{LPin7,LPin6}},
{{LPin4,LPin3},{LPin3,LPin4},{LPin6,LPin1},{LPin1,LPin6},{LPin8,LPin2},{LPin2,LPin8},{LPinA,LPin4},{LPin4,LPinA},{LPinC,LPin6},{LPin6,LPinC},{LPinE,LPin8},{LPin8,LPinE},{LPinF,LPinA},{LPinA,LPinF},{LPinD,LPinC},{LPinB,LPinA},{LPinC,LPin9},{LPin9,LPinC},{LPinE,LPin7},{LPin7,LPinE},{LPinF,LPin5},{LPin5,LPinF},{LPinD,LPin3},{LPin3,LPinD},{LPinB,LPin1},{LPin1,LPinB},{LPin9,LPin2},{LPin2,LPin9},{LPin7,LPin4},{LPin4,LPin7}},
{{LPin5,LPin4},{LPin6,LPin3},{LPin3,LPin6},{LPin8,LPin1},{LPin1,LPin8},{LPinA,LPin2},{LPin2,LPinA},{LPinC,LPin4},{LPin4,LPinC},{LPinE,LPin6},{LPin6,LPinE},{LPinF,LPin8},{LPin8,LPinF},{LPinD,LPinA},{LPinA,LPinD},{LPinC,LPinB},{LPinB,LPinC},{LPinE,LPin9},{LPin9,LPinE},{LPinF,LPin7},{LPin7,LPinF},{LPinD,LPin5},{LPin5,LPinD},{LPinB,LPin3},{LPin3,LPinB},{LPin9,LPin1},{LPin1,LPin9},{LPin7,LPin2},{LPin2,LPin7},{LPin5,LPin4}},
{{LPin6,LPin5},{LPin5,LPin6},{LPin8,LPin3},{LPin3,LPin8},{LPinA,LPin1},{LPin1,LPinA},{LPinC,LPin2},{LPin2,LPinC},{LPinE,LPin4},{LPin4,LPinE},{LPinF,LPin6},{LPin6,LPinF},{LPinD,LPin8},{LPin8,LPinD},{LPinB,LPinA},{LPinD,LPinC},{LPinE,LPinB},{LPinB,LPinE},{LPinF,LPin9},{LPin9,LPinF},{LPinD,LPin7},{LPin7,LPinD},{LPinB,LPin5},{LPin5,LPinB},{LPin9,LPin3},{LPin3,LPin9},{LPin7,LPin1},{LPin1,LPin7},{LPin5,LPin2},{LPin2,LPin5}},
{{LPin7,LPin6},{LPin8,LPin5},{LPin5,LPin8},{LPinA,LPin3},{LPin3,LPinA},{LPinC,LPin1},{LPin1,LPinC},{LPinE,LPin2},{LPin2,LPinE},{LPinF,LPin4},{LPin4,LPinF},{LPinD,LPin6},{LPin6,LPinD},{LPinB,LPin8},{LPin8,LPinB},{LPinE,LPinD},{LPinD,LPinE},{LPinF,LPinB},{LPinB,LPinF},{LPinD,LPin9},{LPin9,LPinD},{LPinB,LPin7},{LPin7,LPinB},{LPin9,LPin5},{LPin5,LPin9},{LPin7,LPin3},{LPin3,LPin7},{LPin5,LPin1},{LPin1,LPin5},{LPin3,LPin2}},
{{LPin8,LPin7},{LPin7,LPin8},{LPinA,LPin5},{LPin5,LPinA},{LPinC,LPin3},{LPin3,LPinC},{LPinE,LPin1},{LPin1,LPinE},{LPinF,LPin2},{LPin2,LPinF},{LPinD,LPin4},{LPin4,LPinD},{LPinB,LPin6},{LPin6,LPinB},{LPin9,LPin8},{LPinF,LPinE},{LPinF,LPinD},{LPinD,LPinF},{LPinD,LPinB},{LPinB,LPinD},{LPinB,LPin9},{LPin9,LPinB},{LPin9,LPin7},{LPin7,LPin9},{LPin7,LPin5},{LPin5,LPin7},{LPin5,LPin3},{LPin3,LPin5},{LPin3,LPin1},{LPin1,LPin3}}
};

volatile uint8_t MatrixBuffer[ROW_COUNT][BUFFER_COL_COUNT];      // yes one byte per LED - for future Dimming or so 

static uint8_t activeCol = 0;
static uint8_t activeRow = 0;
static uint8_t oldCol    = 0;
static uint8_t oldRow    = 0;
static uint8_t bufferOffset =0;

void doMatrixStuff(void)
{
    oldCol=activeCol;
    oldRow=activeRow;
    
    activeCol++;                                // next Col
    if (activeCol>(COL_COUNT-1)) activeCol=0;

    activeRow++;                                // next Row 
    if (activeRow>(ROW_COUNT-1)) activeRow=0;

    //reset last LED
    funPinMode( LED_MATRIX[oldRow][oldCol][ANODE_PIN],   GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD ); 
    funPinMode( LED_MATRIX[oldRow][oldCol][CATHODE_PIN], GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD ); 
    Delay_Us(10);

    //set new LED
    if ((MatrixBuffer[activeRow][activeCol+bufferOffset])>0)
    {
        funPinMode( LED_MATRIX[activeRow][activeCol][ANODE_PIN],   GPIO_Speed_10MHz | GPIO_CNF_OUT_PP ); 
        funDigitalWrite( LED_MATRIX[activeRow][activeCol][ANODE_PIN], FUN_HIGH );

        funPinMode( LED_MATRIX[activeRow][activeCol][CATHODE_PIN],   GPIO_Speed_10MHz | GPIO_CNF_OUT_PP ); 
        funDigitalWrite( LED_MATRIX[activeRow][activeCol][CATHODE_PIN], FUN_LOW);
    }
}

void initMatrixBufferWithDummyData(void)
{
    uint8_t alternater=1;
    uint8_t row=0;
    uint8_t col=0;

    while (row<ROW_COUNT)
    {
        while(col<COL_COUNT)
        {
             MatrixBuffer[row][col]=1;
            //MatrixBuffer[row][col]=(alternater++)&1;    //this should activate every secon LED and create a checkerboard pattern on a 11 collum mA trix  
            col++;
        }
        col=0;
        row++;
    }
}

void putASCIIatPos(char index, uint16_t pos)
{
    uint8_t colCounter =0;
    if ((pos+5)>BUFFER_COL_COUNT)  return;   // don't allow clipped Letters for now  
	if ((index-=32)>96)     return;   // if ascii>127
        	
    while (colCounter<5)
    {
        for(uint8_t i=0; i<ROW_COUNT; i++)
        {
            if ( (uint8_t)Font5x7[index][colCounter] & (1<<i) )
                MatrixBuffer[i][pos+colCounter]=0xFF;
        }
        colCounter++;
    }
}

void putStringToBuffer(char* inString)
{
    uint16_t i=0;
    while ( (inString[i]!='/0') && (i<BUFFER_COL_COUNT/6) )
    {
        putASCIIatPos(inString[i], COL_COUNT+i*6);
        i++;
    }
}

uint16_t counter=1;
uint16_t t=1;



int main (void)
{
   //FLASH_Unlock();
   // FLASH_EraseOptionBytes();
   // FLASH_UserOptionByteConfig(OB_STOP_NoRST, OB_STDBY_NoRST, OB_RST_NoEN, OB_PowerON_Start_Mode_USER);
   // FLASH_Unlock();
    //unlockD7();
    SystemInit();
    //unlockD7();
    Delay_Ms(100);
    funGpioInitAll();

/*
    FLASH_Unlock();
    FLASH_EraseOptionBytes();
    FLASH_UserOptionByteConfig(OB_STOP_NoRST, OB_STDBY_NoRST, OB_RST_NoEN, OB_PowerON_Start_Mode_USER);
    FLASH_Unlock();*/

    //Init IOs
    funPinMode( PA1, GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD );     // on port A
    funPinMode( PA2, GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD );

    funPinMode( PC0, GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD );     // on port C 
    funPinMode( PC3, GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD );
    funPinMode( PC4, GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD );
    funPinMode( PC5, GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD );
    funPinMode( PC6, GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD );
    funPinMode( PC7, GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD );

    funPinMode( PD0, GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD );     // on port D
    funPinMode( PD2, GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD ); 
    funPinMode( PD3, GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD ); 
    funPinMode( PD4, GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD ); 
    funPinMode( PD5, GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD ); 
    funPinMode( PD6, GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD ); 
    funPinMode( PD7, GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD ); 

    //funDigitalWrite(PD7, FUN_LOW);

    //initMatrixBufferWithDummyData();
    //MatrixBuffer[0][0]=1;
    //MatrixBuffer[1][0]=1;
    //MatrixBuffer[0][1]=1;
    //MatrixBuffer[1][1]=1;
    //MatrixBuffer[3][3]=1;
    
    //putASCIIatPos('T',1);
    /*
    putASCIIatPos(' ',0);
    putASCIIatPos(' ',6);
    putASCIIatPos('H',11);
    putASCIIatPos('e',11+6);
    putASCIIatPos('l',11+6*2);
    putASCIIatPos('l',11+6*3);
    putASCIIatPos('o',11+6*4);
    putASCIIatPos(' ',11+6*5);
    putASCIIatPos('f',11+6*6);
    putASCIIatPos('r',11+6*7);
    putASCIIatPos('o',11+6*8);
    putASCIIatPos('m',11+6*9);
    putASCIIatPos(' ',11+6*10);
    putASCIIatPos('@',11+6*11);
    putASCIIatPos('t',11+6*12);
    putASCIIatPos('i',11+6*13);
    putASCIIatPos('n',11+6*14);
    putASCIIatPos('y',11+6*15);
    putASCIIatPos('l',11+6*16);
    putASCIIatPos('e',11+6*17);
    putASCIIatPos('d',11+6*18);
    putASCIIatPos('m',11+6*19);
    putASCIIatPos('a',11+6*20);
    putASCIIatPos('t',11+6*21);
    putASCIIatPos('r',11+6*22);
    putASCIIatPos('i',11+6*23);
    putASCIIatPos('x',11+6*24);
    putASCIIatPos(' ',11+6*25);
    putASCIIatPos(' ',11+6*26); //167
    */
   putStringToBuffer("Hello from @tinyledmatrix    ");
   //putStringToBuffer("DAH DAH DAH DAVEDARKO        ");

    bufferOffset=28*6;

    //while(counter)


    while(1)
    {
        doMatrixStuff();
        counter++;
        if (counter==4000)
        {
            counter=0;
            bufferOffset++;
            if (bufferOffset>(BUFFER_COL_COUNT-COL_COUNT))
            {
                bufferOffset=0;
                //putStringToBuffer("Hello from @tinyledmatrix       ");
            }
        }
            
        //Delay_Us(100);
        
    }
}   
