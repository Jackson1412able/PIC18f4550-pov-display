//////////////////////////////////////////////////////////////////////////////////////////////////////////
//													//
//  THE ENTIRE CIRCUIT IS MOUNTED ON A PIECE OF WOOD, THE LED ARRAY WITH SOME EXTRA WOOD ARE 		//
//  PLACED AT A SIDE AND EVERYTHING ELSE IS PLACED AT THE OTHER SIDE. AN LED ARRAY CONSISTS		//
//  OF 8 LEDS, WHICH ARE CONNECTED TO THE 8 PINS OF PORTB OF PIC18F4550. THE WOOD AND CIRCUIT ARE	//
//  THEN MOUNTED ON THE SHAFT OF AN ELECTRIC MOTOR FROM AN ELECTRIC FAN. AS THE CIRCUIT			//
//  ROTATES, THE LED ARRAY CREATES INSTANCES OF LED ARRAY PATTERNS, 1 CHARACTER IS THEN MADE UP		//
//  OF FIVE INSTANCES OF LED ARRAY PATTERNS. SEE ILLUSTRATION BELOW TO GET A BETTER INSIGHT OF		//
//  HOW IT WORKS:											//
//											                //
//                          FOR EXAMPLE, TO DISPLAY A "H" CHARACTER:	                                //
//  									                                //
//									                                //
//                  PORT B PINS							                        //
//                    TO 8 LEDS								                //
//                                                      PORTB OUTPUT			                //
//                  EDGE OF WOOD                  {0x7f,0x08,0x08,0x08,0x7f}		                //
//										                        //
//                    RB0  O			        X  O  O  O  X		                        //
//                    RB1  O			        X  O  O  O  X		                        //
//                    RB2  O			        X  O  O  O  X		                        //
//                    RB3  O       ----------->         X  X  X  X  X		                        //
//                    RB4  O	      ROTATE IN         X  O  O  O  X		                        //
//                    RB5  O	      CLOCKWISE         X  O  O  O  X			                //
//                    RB6  O	      DIRECTION         X  O  O  O  X			                //
//                    RB7  O			        O  O  O  O  O			                //
//										                        //
//                  MOTOR SHAFT                    *X - LED ON, O - LED OFF		                //
//										                        //
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <18F4550.h>


#FUSES NOWDT                    //No Watch Dog Timer
#FUSES HS                       //Crystal osc <= 4mhz for PCM/PCH , 3mhz to 10 mhz for PCD
#FUSES NOPBADEN                 //PORTB pins are configured as digital I/O on RESET
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#FUSES NODEBUG
#use delay(clock=24000000)
#include <string.h>
#USE FAST_IO(A)
#USE FAST_IO(B)

int i,j,k;
int32 counter;
const int delay = 2982;
const int charWidth = 5;

//////////////////////////////////CHARACTER ARRATS TO BE DISPLAYED/////////////////////////////////////////
char textString[]  = "11          ";
char textString1[] = "11.         ";
char textString2[] = "11.10       ";
char textString3[] = "11.10.      ";
char textString4[] = "11.10.2017  ";
char textString5[] = "H           ";
char textString6[] = "Ha          ";
char textString7[] = "Hap         ";
char textString8[] = "Happ        ";
char textString9[] = "Happy       ";
char textStringA[] = "B           ";
char textStringB[] = "Bi          ";
char textStringC[] = "Bir         ";
char textStringD[] = "Birt        ";
char textStringE[] = "Birth       ";
char textStringF[] = "Birthd      ";
char textStringG[] = "Birthda     ";
char textStringH[] = "Birthday!!! ";
char textStringI[] = "        May";
char textStringJ[] = "        May ";

////////////////////SEQUENCE OF PORTB LED ARRAY OUTPUTS FOR DISPLAYING A CHARACTER PATTERN/////////////////
const char font[95][5] = {
         {0x00,0x00,0x00,0x00,0x00},   //   0x20 32
         {0x00,0x00,0x6f,0x00,0x00},   // ! 0x21 33
         {0x00,0x07,0x00,0x07,0x00},   // " 0x22 34
         {0x14,0x7f,0x14,0x7f,0x14},   // # 0x23 35
         {0x00,0x07,0x04,0x1e,0x00},   // $ 0x24 36
         {0x23,0x13,0x08,0x64,0x62},   // % 0x25 37
         {0x36,0x49,0x56,0x20,0x50},   // & 0x26 38
         {0x00,0x00,0x07,0x00,0x00},   // ' 0x27 39
         {0x00,0x1c,0x22,0x41,0x00},   // ( 0x28 40
         {0x00,0x41,0x22,0x1c,0x00},   // ) 0x29 41
         {0x14,0x08,0x3e,0x08,0x14},   // * 0x2a 42
         {0x08,0x08,0x3e,0x08,0x08},   // + 0x2b 43
         {0x00,0x50,0x30,0x00,0x00},   // , 0x2c 44
         {0x08,0x08,0x08,0x08,0x08},   // - 0x2d 45
         {0x00,0x60,0x60,0x00,0x00},   // . 0x2e 46
         {0x20,0x10,0x08,0x04,0x02},   // / 0x2f 47
         {0x3e,0x51,0x49,0x45,0x3e},   // 0 0x30 48
         {0x00,0x42,0x7f,0x40,0x00},   // 1 0x31 49
         {0x42,0x61,0x51,0x49,0x46},   // 2 0x32 50
         {0x21,0x41,0x45,0x4b,0x31},   // 3 0x33 51
         {0x18,0x14,0x12,0x7f,0x10},   // 4 0x34 52
         {0x27,0x45,0x45,0x45,0x39},   // 5 0x35 53
         {0x3c,0x4a,0x49,0x49,0x30},   // 6 0x36 54
         {0x01,0x71,0x09,0x05,0x03},   // 7 0x37 55
         {0x36,0x49,0x49,0x49,0x36},   // 8 0x38 56
         {0x06,0x49,0x49,0x29,0x1e},   // 9 0x39 57
         {0x00,0x36,0x36,0x00,0x00},   // : 0x3a 58
         {0x00,0x56,0x36,0x00,0x00},   // ; 0x3b 59
         {0x08,0x14,0x22,0x41,0x00},   // < 0x3c 60
         {0x14,0x14,0x14,0x14,0x14},   // = 0x3d 61
         {0x00,0x41,0x22,0x14,0x08},   // > 0x3e 62
         {0x02,0x01,0x51,0x09,0x06},   // ? 0x3f 63
         {0x3e,0x41,0x5d,0x49,0x4e},   // @ 0x40 64
         {0x7e,0x09,0x09,0x09,0x7e},   // A 0x41 65
         {0x7f,0x49,0x49,0x49,0x36},   // B 0x42 66
         {0x3e,0x41,0x41,0x41,0x22},   // C 0x43 67
         {0x7f,0x41,0x41,0x41,0x3e},   // D 0x44 68
         {0x7f,0x49,0x49,0x49,0x41},   // E 0x45 69
         {0x7f,0x09,0x09,0x09,0x01},   // F 0x46 70
         {0x3e,0x41,0x49,0x49,0x7a},   // G 0x47 71
         {0x7f,0x08,0x08,0x08,0x7f},   // H 0x48 72
         {0x00,0x41,0x7f,0x41,0x00},   // I 0x49 73
         {0x20,0x40,0x41,0x3f,0x01},   // J 0x4a 74
         {0x7f,0x08,0x14,0x22,0x41},   // K 0x4b 75
         {0x7f,0x40,0x40,0x40,0x40},   // L 0x4c 76
         {0x7f,0x02,0x0c,0x02,0x7f},   // M 0x4d 77
         {0x7f,0x04,0x08,0x10,0x7f},   // N 0x4e 78
         {0x3e,0x41,0x41,0x41,0x3e},   // O 0x4f 79
         {0x7f,0x09,0x09,0x09,0x06},   // P 0x50 80
         {0x3e,0x41,0x51,0x21,0x5e},   // Q 0x51 81
         {0x7f,0x09,0x19,0x29,0x46},   // R 0x52 82
         {0x46,0x49,0x49,0x49,0x31},   // S 0x53 83
         {0x01,0x01,0x7f,0x01,0x01},   // T 0x54 84
         {0x3f,0x40,0x40,0x40,0x3f},   // U 0x55 85
         {0x0f,0x30,0x40,0x30,0x0f},   // V 0x56 86
         {0x3f,0x40,0x30,0x40,0x3f},   // W 0x57 87
         {0x63,0x14,0x08,0x14,0x63},   // X 0x58 88
         {0x07,0x08,0x70,0x08,0x07},   // Y 0x59 89
         {0x61,0x51,0x49,0x45,0x43},   // Z 0x5a 90
         {0x3c,0x4a,0x49,0x29,0x1e},   // [ 0x5b 91
         {0x02,0x04,0x08,0x10,0x20},   // \ 0x5c 92
         {0x00,0x41,0x7f,0x00,0x00},   // ] 0x5d 93
         {0x04,0x02,0x01,0x02,0x04},   // ^ 0x5e 94
         {0x40,0x40,0x40,0x40,0x40},   // _ 0x5f 95
         {0x00,0x00,0x03,0x04,0x00},   // ` 0x60 96
         {0x20,0x54,0x54,0x54,0x78},   // a 0x61 97
         {0x7f,0x48,0x44,0x44,0x38},   // b 0x62 98
         {0x38,0x44,0x44,0x44,0x20},   // c 0x63 99
         {0x38,0x44,0x44,0x48,0x7f},   // d 0x64 100
         {0x38,0x54,0x54,0x54,0x18},   // e 0x65 101
         {0x08,0x7e,0x09,0x01,0x02},   // f 0x66 102
         {0x0c,0x52,0x52,0x52,0x3e},   // g 0x67 103
         {0x7f,0x08,0x04,0x04,0x78},   // h 0x68 104
         {0x00,0x44,0x7d,0x40,0x00},   // i 0x69 105
         {0x20,0x40,0x44,0x3d,0x00},   // j 0x6a 106
         {0x00,0x7f,0x10,0x28,0x44},   // k 0x6b 107
         {0x00,0x41,0x7f,0x40,0x00},   // l 0x6c 108
         {0x7c,0x04,0x18,0x04,0x78},   // m 0x6d 109
         {0x7c,0x08,0x04,0x04,0x78},   // n 0x6e 110
         {0x38,0x44,0x44,0x44,0x38},   // o 0x6f 111
         {0x7c,0x14,0x14,0x14,0x08},   // p 0x70 112
         {0x08,0x14,0x14,0x18,0x7c},   // q 0x71 113
         {0x7c,0x08,0x04,0x04,0x08},   // r 0x72 114
         {0x48,0x54,0x54,0x54,0x20},   // s 0x73 115
         {0x04,0x3f,0x44,0x40,0x20},   // t 0x74 116
         {0x3c,0x40,0x40,0x20,0x7c},   // u 0x75 117
         {0x1c,0x20,0x40,0x20,0x1c},   // v 0x76 118
         {0x3c,0x40,0x30,0x40,0x3c},   // w 0x77 119
         {0x44,0x28,0x10,0x28,0x44},   // x 0x78 120
         {0x0c,0x50,0x50,0x50,0x3c},   // y 0x79 121
         {0x44,0x64,0x54,0x4c,0x44},   // z 0x7a 122
         {0x00,0x08,0x36,0x41,0x41},   // { 0x7b 123
         {0x00,0x00,0x7f,0x00,0x00},   // | 0x7c 124
         {0x41,0x41,0x36,0x08,0x00},   // } 0x7d 125
         {0x04,0x02,0x04,0x08,0x04},   // ~ 0x7e 126
};

/////////////////////////////////////STARTUP SEQUENCE OF LED ARRAY PATTERNS////////////////////////////////
const char fixed1[8][6] = {
 {0x80,0x80,0x80,0x80,0x80,0x80},	//1000 0000
 {0xc0,0xc0,0xc0,0xc0,0xc0,0xc0},	//1100 0000
 {0xe0,0xe0,0xe0,0xe0,0xe0,0xe0},	//1110 0000
 {0xf0,0xf0,0xf0,0xf0,0xf0,0xf0},	//1111 0000
 {0xf8,0xf8,0xf8,0xf8,0xf8,0xf8},	//1111 1000
 {0xfc,0xfc,0xfc,0xfc,0xfc,0xfc},	//1111 1100
 {0xfe,0xfe,0xfe,0xfe,0xfe,0xfe},	//1111 1110
 {0xff,0xff,0xff,0xff,0xff,0xff},	//1111 1111
};


////////////////////////////SEQUENCE OF LED ARRAY PATTERNS FOR DISPLAYING A LOVE SYMBOL////////////////////
const char love[1][5] = {
   {0x1c,0x22,0x44,0x22,0x1c}
};

///////////////////////////////// FUNCTION PROTOTYPES /////////////////////////////////////////////////////
void printLetter(char ch);
void printStart();
void displaytxt(char[] txtstr, int delayCnst, int Cnst);
void cleardisplay(int delayCnst2, int Cnst2);

//////////////////////////////////////MAIN PROGRAM/////////////////////////////////////////////////////////
void main()
{
   set_tris_a(0x1f);	//0001 1111, RA0~RA4 INPUT, RA5~RA7 OUTPUT
   set_tris_b(0x00);	//0000 0000, RB0~RB7 OUTPUT, PORTB AS LED PINS
   set_tris_c(0x05);	//0000 0101, RC0&RC2 INPUT, RC1&RC3~RC7 OUTPUT
   set_tris_d(0x00);	//0000 0000, RD0~RD7 OUTPUT, RD6~RD7 AS LED PINS
   
   output_low(PIN_D7);	//OFF THE INNERMOST LED
   output_low(PIN_D6);	//OFF THE OUTERMOST LED

   //BLINKING SEQUENCE UPON STARTUP
   for(i=1;i<=20;i++){
   	output_b(0xff);
   	delay_ms(100);
   	output_b(0x00);
   	delay_ms(100);
   };
   
   output_high(PIN_D6);	//ON THE INNERMOST LED
   printStart();	//SLOWLY TURNING ON LED RINGS, MAINTAIN, AND TURNING OFF
   output_high(PIN_D7);	//ON THE OUTERMOST LED

   //MAIN TEXT DISPLAY LOOP
   while(1){
   output_b(0x00);
   delay_ms(500);

   displaytxt(textString, 2, 15);	//11
   displaytxt(textString1, 2, 15);	//11.
   displaytxt(textString2, 2, 15);	//11.10
   displaytxt(textString3, 2, 15);	//11.10.
   displaytxt(textString4, 5, 15);	//11.10.2017

   cleardisplay(48,15);

   displaytxt(textString5, 1, 12);	//H
   displaytxt(textString6, 1, 12);	//Ha
   displaytxt(textString7, 1, 12);	//Hap
   displaytxt(textString8, 1, 12);	//Happ
   displaytxt(textString9, 3, 12);	//Happy
   
   cleardisplay(40,12);

   displaytxt(textStringA, 1, 12);	//B
   displaytxt(textStringB, 1, 12);	//Bi
   displaytxt(textStringC, 1, 12);	//Bir
   displaytxt(textStringD, 1, 12);	//Birt
   displaytxt(textStringE, 1, 12);	//Birth
   displaytxt(textStringF, 1, 12);	//Birthd
   displaytxt(textStringG, 1, 12);	//Birthda
   displaytxt(textStringH, 4, 12);	//Birthday

   cleardisplay(40,12);

   for(k=0;k<20;k++){
	 displaytxt(textStringJ, 1, 6)	//May
      	for(counter=0;counter<=6;counter++)   //May<3
      	{
        	for(j=0; j<strlen(textStringI); j++)
        	{
         		printLetter(textStringI[j]);   
        	};
        	for(i=0;i<5;i++){
               		output_b(love[0][i]);
               		delay_us(delay);
        	};
        	output_b(0x00);
        	delay_us(delay+100);
      	};
   };
   };
}

void displaytxt(char[] txtstr, int delayCnst, int Cnst)
{
	for(counter=0;counter<=delayCnst*Cnst;counter++)
   	{
   		for(j=0; j<strlen(txtstr); j++)
   		{
   			printLetter(txtstr[j]);   
   		};
   	};
}

void cleardisplay(int delayCnst2, int Cnst2)
{
	for(counter=0;counter<=delayCnst2*Cnst2;counter++)  //CLEAR DISPLAY
   	{
      		output_b(0x00);
      		delay_us(delay);
   	};
}

void printLetter(char ch)	//PRINTING THE CHARACTERS
{
   if(ch<32 || ch>126)	//IF DECIMAL EQUIVALENT OF ASCII CHARACTER NOT DEFINED, CONVERT IT TO A SPACE
   {
      ch = 32;
   };
   ch -= 32;	//NEGATE THE ASCII DECIMAL EQUIVALENT BY 32 TO ALIGN IT TO THE LED SEQUENCE DEFINITION OF THE CORRESPONDING CHARACTER IN FONT[][] ARRAY
   for(i=0;i<charWidth;i++)
   {
      output_b(font[ch][i]);	//OUTPUT LED SEQUENCE FOR CORRESPONDING CHARACTER
      delay_us(delay);  
   };
   output_b(0x00);	//EMPTY COLUMN BETWEEN CHARACTERS
   delay_us(delay);  
}
      
void printStart()	//STARTING LED ARRAY PATTERN THAT EXTENDS OUT
{
   //SOME DELAY BEFORE TURNING ON THE LEDS
   for(counter=0;counter<=6*12*10;counter++){
      output_b(0x00);
      delay_us(delay);
   };

   //STARTING LED ARRAY PATTERNS THAT LIGHT UP FROM INNERMOST RING TO OUTERMOST RING
   for(k=0;k<=7;k++){ 
      for(counter=0;counter<=10;counter++){
         for(j=0;j<=11;j++){
            for(i=0;i<=5;i++){
               output_b(fixed1[k][i]);	//TURNING ON LEDS ACCORDING TO SEQUENCE IN FIXED1 ARRAY
               delay_us(delay);
            };
         };
      };
   };

   //MAINTAIN ALL LED ARRAYS IN HIGH STATE
   for(counter=0;counter<=5*10;counter++){   
      for(j=0;j<=11;j++){
         for(i=0;i<=5;i++){
            output_b(fixed1[7][i]);	//ALL LEDS IN HIGH STATE
            output_high(PIN_D7);	//OUTERMOST LED IN HIGH STATE
            delay_us(delay);
         };
      };
   };

   //LED ARRAYS TO OFF STATE FROM OUTERMOST RING TO INNERMOST RING
   for(k=0;k<=7;k++){   
      for(counter=0;counter<=10-(k+3);counter++){	//INCREASES SPEED OF TURNING OFF LED AS TURNING OFF INNER RINGS
         for(j=0;j<=11;j++){
            for(i=0;i<=5;i++){
               output_b(fixed1[7-k][i]);	//TURNING OFF LEDS ACCORDING TO REVERSE SEQUENCE IN FIXED1 ARRAY
               delay_us(delay);
            };
         };
      };
   };
}  
