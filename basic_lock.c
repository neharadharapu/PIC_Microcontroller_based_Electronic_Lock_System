#include <xc.h>
#include<htc.h>
#define _XTAL_FREQ   20000000

// BEGIN CONFIG
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON // Brown-out Reset Enable bit (BOR enabled)
#pragma config CPD = OFF // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF // Flash  Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)
//END CONFIG

// Configuration word for PIC16F877A
//__CONFIG( FOSC_HS & WDTE_OFF & PWRTE_ON & CP_OFF & BOREN_ON
 //         & LVP_OFF & CPD_OFF & WRT_OFF & DEBUG_OFF);


// Define Pins
#define LCD_E        RC1   // Enable pin for LCD
#define LCD_RS       RC0   // RS pin for LCD
#define LCD_Data_Bus   PORTB // Data bus for LCD
// Define Pins direction register
#define LCD_E_Dir         TRISC1
#define LCD_RS_Dir        TRISC0
#define LCD_Data_Bus_Dir   TRISB
// Constants
#define E_Delay       500

#define C1 PORTDbits.RD0 //custom name basically
#define C2 PORTDbits.RD1
#define C3 PORTDbits.RD2
#define R1 PORTDbits.RD3
#define R2 PORTDbits.RD4
#define R3 PORTDbits.RD5
#define R4 PORTDbits.RD6


// Function Declarations
void WriteCommandToLCD(unsigned char);
void WriteDataToLCD(char);
void InitLCD(void);
void WriteStringToLCD(const char*);
void ClearLCDScreen(void);
void keypad();

//Program start from here
int main(void)
{
    CMCON = 0x07;            // Turn comparator off
    InitLCD();                // Initialize LCD in 8bit mode
    
    ClearLCDScreen();          // Clear LCD screen
    //WriteStringToLCD(msg);  // Write Hello World on LCD
    while(1)
    {
        keypad();
    }
    return 0;
}

void keypad()
{
    //Column 1 sequence
    C1=1;
    C2=0;
    C3=0;
    
    if(R1==1){ //Column 1 = 1 and if Row 1=1 it means 1 is pressed
        WriteStringToLCD("1");
        while(R1==1); // this condition will be true until we release the button
    }
    if(R2==1){
        WriteStringToLCD("4");
        while(R2==1);
    }
    if(R3==1){
        WriteStringToLCD("7");
        while(R3==1);
    }
    if(R4==1){
        WriteStringToLCD("*");
        while(R4==1);
    }
    
    //Column 2 sequence
    C1=0;
    C2=1;
    C3=0;
    
    if(R1==1){ 
        WriteStringToLCD("2");
        while(R1==1); 
    }
    if(R2==1){
        WriteStringToLCD("5");
        while(R2==1);
    }
    if(R3==1){
        WriteStringToLCD("8");
        while(R3==1);
    }
    if(R4==1){
        WriteStringToLCD("0");
        while(R4==1);
    }
    
    //Column 3 sequence
    C1=0;
    C2=0;
    C3=1;
    
    if(R1==1){ 
        WriteStringToLCD("3");
        while(R1==1); 
    }
    if(R2==1){
        WriteStringToLCD("6");
        while(R2==1);
    }
    if(R3==1){
        WriteStringToLCD("9");
        while(R3==1);
    }
    if(R4==1){
        WriteStringToLCD("#");
        while(R4==1);
    }
}

void ToggleEpinOfLCD(void)
{
    LCD_E = 1;                // Give a pulse on E pin
    __delay_us(E_Delay);      // so that LCD can latch the
    LCD_E = 0;                // data from data bus
    __delay_us(E_Delay);
}
void WriteCommandToLCD(unsigned char Command)
{
    LCD_RS = 0;               // It is a command
    LCD_Data_Bus = Command;   // Write Command value on data bus
    ToggleEpinOfLCD();
}
void WriteDataToLCD(char LCDChar)
{
    LCD_RS = 1;               // It is data
    LCD_Data_Bus = LCDChar;   // Write Data value on data bus
    ToggleEpinOfLCD();
}
void InitLCD(void)
{
    // Firstly make all pins output
    LCD_E          = 0;      // E = 0
    LCD_RS           = 0;      // D = 0
    LCD_Data_Bus     = 0;      // CLK = 0
    LCD_E_Dir        = 0;      // Make Output
    LCD_RS_Dir       = 0;      // Make Output
    LCD_Data_Bus_Dir = 0;      // Make Output
    
    WriteCommandToLCD(0x38);    //function set
    WriteCommandToLCD(0x0c);    //display on,cursor off,blink off
    WriteCommandToLCD(0x01);    //clear display
    WriteCommandToLCD(0x06);    //entry mode, set increment
}
void WriteStringToLCD(const char *s)
{
    while(*s)
    {
        WriteDataToLCD(*s++);   // print first character on LCD
    }
}
void ClearLCDScreen(void)
{
    WriteCommandToLCD(0x01);    // Clear the screen
    __delay_ms(2);              // Delay for cursor to return at zero position
}