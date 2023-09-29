/*
 * File:   Simon.c
 * Author: Louis Nguyen
 *
 * Created on March 26, 2021, 10:27 AM
 */

// 'C' source line config statements  
#include <xc.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <time.h>
// #pragma config statements should precede project file includes.  
// Use project enums instead of #define for ON and OFF.  
// CONFIG1  
#pragma config FOSC = INTOSC // Oscillator Selection (INTOSCoscillator: I/O function on CLKIN pin)  
#pragma config WDTE = OFF // Watchdog Timer Enable (WDT disabled)  
#pragma config PWRTE = OFF // Power-up Timer Enable (PWRT disabled)  
#pragma config MCLRE = ON // MCLR Pin Function Select (MCLR/VPP pinfunction is MCLR)  
#pragma config CP = OFF // Flash Program Memory Code Protection(Program memory code protection is disabled)  
#pragma config CPD = OFF // Data Memory Code Protection (Datamemory code protection is disabled)  
#pragma config BOREN = OFF // Brown-out Reset Enable (Brown-out Resetdisabled)  
#pragma config CLKOUTEN = OFF // Clock Out Enable (CLKOUT function isdisabled. I/O or oscillator function on the CLKOUT pin)  
#pragma config IESO = OFF // Internal/External Switchover(Internal/External Switchover mode is disabled)  
#pragma config FCMEN = OFF // Fail-Safe Clock Monitor Enable (FailSafeClock Monitor is disabled)  
// CONFIG2  
#pragma config WRT = OFF // Flash Memory Self-Write Protection(Write protection off)  
#pragma config PLLEN = OFF // PLL Enable (4x PLL disabled)  
#pragma config STVREN = ON // Stack Overflow/Underflow Reset Enable(Stack Overflow or Underflow will cause a Reset)  
#pragma config BORV = LO // Brown-out Reset Voltage Selection(Brown-out Reset Voltage (Vbor), low trip point selected.)  
#pragma config LVP = ON // Low-Voltage Programming Enable (Highvoltageon MCLR/VPP must be used for programming)  
  
//These are the values to set baud rate for UART and configure some functions
#define BAUD 9600  
#define FOSC 4000000L  
#define DIVIDER ((int)(FOSC/(16UL * BAUD) -1)) // Should be 25 for9600/4MhZ  
#define NINE_BITS 0  
#define SPEED 0x4  
#define RX_PIN TRISC5  
#define TX_PIN TRISC4  
#define _XTAL_FREQ 4000000.0 /*for 4mhz*/  

//*Function Prototypes*//  
void RA5Blink(void);  
void pinConfig(void);  
void setup_comms(void);  
void putch(unsigned char);  
unsigned char getch(void);  
unsigned char getche(void);
void timer_config(void);
void clockAndpin_config(void);
void usartConfig(void);
void Display(int);
  
//*Global Vars*//  
unsigned int Pval, Temp, FVval;  
double Ctemp; 
int randNum[4];
int user[4] = {4,4,4,4};
//TOUCH SENSE
int score, track, hScore, p = 0;
 
 void Display(int delay){
     TRISA5 = 0;
     TRISA2 = 0;
     TRISC6 = 0;
    switch(delay) {
        case 0: //Case for button U
        RA5 = 0;
        __delay_ms(100);
        RA5 = 1;
        __delay_ms(100);
        if(track == 1){
            user[p]=0;
            p++;
        }
        break;
        case 1: //Case for button T
        RA2 = 0;
        __delay_ms(100);
        RA2 = 1;
        __delay_ms(100);
        if(track == 1){
            user[p]=1;
            p++;
        }
        break;
        case 2: //Case for button S
        RC6 = 0;
        __delay_ms(100);
        RC6 = 1;
        __delay_ms(100);
        if(track == 1){
            user[p]=2;
            p++;
        }
        break;
        case 3: //Case for button A
            RC6 = 0;
            RA2 = 0;
            RA5 = 0;
            __delay_ms(100);
            RC6 = 1;
            RA2 = 1;
            RA5 = 1;
            __delay_ms(100);
            if(track == 1){
            user[p]=3;
            p++;
        }
        break;  
    }
    RA5 = 1;
    RA2 = 1;
    RC6 = 1;
}
 
 
int main(int argc, char** argv) {  
 pinConfig();  
setup_comms(); // set up the USART - settings defined in usart.h  
// Set up for the Temp Sensor  and FVR
FVRCON = 0xA2;
TSEN = 1;
TSRNG = 0;
 
TRISB7 = 1;
// Get set up for A2D
RA2 = 1;
RA5 = 1;
RC6 = 1;
__delay_ms(100); 
int a, b = 0;
    b = 0;
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
            "\n\n\n\n\n\n\n\n\n\n\n\n\n"
            "\n\n\n\n\rU = Green\n\rT = Blue\n\r"
            "S = Red\n\rA = White\n\n\r");
    while(1){
 printf("\n\rpush button, wait for flash, then "
         "enter your pattern\n\r");
 printf("\n\rThe flash of blue and green at the end "
         "is the signal to start your pattern\n\r");
 printf("\n\rC score %d\n",score);
 
 while(1){
    for( a = 0; a<100; a++)
    {
        __delay_ms(10);
        if(RB7 == 0)
        {
            b = 1;
            break;
        }
        else
         continue;
 }
 if(b==1)
 {
     b = 0;
    break;
 }
 else
     continue;
 }
       int i;
       srand(a);
       int k;
       srand(a);


       for(i=0;i<4;i++)
       {
           k = rand()%4;
           randNum[i] = k;
           __delay_ms(100);
       }
       
       __delay_ms(1000);
       for(i=0;i<4;i++)
       {
           Display(randNum[i]);
           __delay_ms(750);
       }
       track = 1;
    //Touch sensing
        unsigned int Tcount, Threshold;
        //touch pad setup
        int Touch[4], mode;
        CPSCON0 = 0x8C;
        Touch[0] = 3;
        Touch[1] = 9;
        Touch[2] = 5;
        Touch[3] = 4;
        //Clock and pin
        clockAndpin_config();
        //touch threshold
        Threshold = 0x1200;
        
        while(track == 1){
            mode =4;
            for(int j=0;j<4;j++){
                CPSCON1 = Touch[j];
                timer_config();
                while(!TMR0IF) continue;
                Tcount = (TMR1H <<8) + TMR1L;
                TMR0IF =0;
                if(Tcount<Threshold) mode = j;
            }
            Display(mode);
            if(user[p] != 4){
                track = 0;
            }
        }
        
        
        for(int i = 0; i<4;i++){
            if(user[i] != randNum[i]){
                if(hScore<score){
                    hScore = score;
                }
                printf("\n\r~~~~Wrong~~~~\n\r");
                printf("high score: %d\n\r",hScore);
                for(int j = 0; j<4; j++){
                   user[j] = 4;
                }
                score=0;
                break;
            }    
        }
        if(user[3] == randNum[3]){
                score++;
                if(score>hScore){
                    hScore = score;
                }
                for(int j = 0; j<4;j++){
                   user[j] = 4;
                }
        }
       p=0;
    }
    return;
} 


void setup_comms(void)  {  
RX_PIN = 1;  
TX_PIN = 1;  
SPBRG = DIVIDER;  
RCSTA = (NINE_BITS | 0x90);  
TXSTA = (SPEED | NINE_BITS | 0x20);  
TXEN = 1;  
SYNC = 0;  
SPEN = 1;  
BRGH = 1;  
  }  

void putch(unsigned char byte)  
  {  
/* output one byte */  
while(!TXIF) /* set when register is empty */  
continue;  
TXREG = byte;  
  }  

unsigned char getch()  
  {  
/* retrieve one byte */  
while(!RCIF) /* set when register is not empty */  
continue;  
return RCREG;  
  }  

unsigned char getche(void)  
  {  
unsigned char c;  
putch(c = getch());  
return c;  
  }  
  
void pinConfig(void)  {  
 OSCCON = 0x6A; /* b6..4 = 1101 = 4MHz */  
 TXCKSEL = 1; // both bits in APFCON0 MUST BE 1 for 1829 0 for 1825  
 RXDTSEL = 1; /* makes RC4 & 5 TX & RX for USART (Allows ICSP)*/  
  
 TRISA = 0x10;  
 ANSELA =0x10;  
 TRISC = 0x0C; /* set as output */  
 ANSELC =0x0C; /* all ADC pins to digital I/O */  
 TRISB = 0xF0;
 ANSELB =0xF0;
 INTCON = 0; // purpose of disabling the interrupts.  
  }  
void clockAndpin_config(){
    OSCCON      =	0X6A;  	//set up 4MHz for Fosc
    INTCON      =   0;      // purpose of disabling the interrupts.
    OPTION_REG  =   0XC5;  	// set up timer 0, and then timer 1
    T1CON    	=	0XC1;   //TMR 1 Enable
    T1GCON   	=  	0X81;   //81 is without toggle mode A1 is toggle mode
    TRISA   	= 	0X10;
    TRISC   	=	0XFF;
    PORTA   	= 	0;
    ANSELA  	=   0X10;
}

void timer_config(){
    TMR1ON  	=   0;
    TMR0    	=   0;
    TMR1H       =   0;
    TMR1L   	=   0;
    TMR1ON      =   1;
    TMR0IF  	=   0;      	//Clear the interrupt flag for Timer 1
    TMR0    	=   0;  
}

void usartConfig(){
    APFCON0 = 0x84; //This sets pins RC5 and RC4 as RX & TX on pic16f1829
    TXCKSEL = 1;   
    RXDTSEL = 1;  //makes RC4 & 5 TX & RX for USART (Allows ICSP)
}