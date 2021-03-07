#include <xc.h>

#define _XTAL_FREQ 20000000

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// defining macros

#define RS PORTDbits.RD0
#define RW PORTDbits.RD1
#define EN PORTDbits.RD2

#define C1 PORTBbits.RB0 //custom name basically
#define C2 PORTBbits.RB1
#define C3 PORTBbits.RB2
#define R1 PORTBbits.RB4
#define R2 PORTBbits.RB5
#define R3 PORTBbits.RB6
#define R4 PORTBbits.RB7

void lcd_data(unsigned char data) // to pass character data to lcd
{
    PORTC = data; // passing data to port C
    RS=1; //this means that data passing will go to data register
    RW=0; //to write data, this has to be 0
    EN=1; //clock has to be enabled irrespective of process
    __delay_ms(5);
    EN=0; //disabling clock after 5 ms
}

void lcd_command(unsigned char cmd) //passing instruction
{
    PORTC = cmd; // passing instruction to port C
    RS=0; //instruction passed to instruction register
    RW=0; //to write data, this has to be 0
    EN=1; //clock has to be enabled irrespective of process
    ___delay_ms(5);
    EN=0;  //disabling clock after 5 ms
}

void lcd_string(const unsigned char str, unsigned char num) //num=length of string
{
    unsigned char i;
    for(i=0; i<num; i++){
        lcd_data(str(i));
    }
}

void lcd_initialise()
{
    lcd_command(0x38);//setting lcd in 16 columns and 2 rows model
    lcd_command(0x06);//auto increment cursor of lcd to next element when current element is printed
    lcd_command(0x0C); //lcd on and cursor off
    lcd_command(0x01); //clear screen
}
void keypad()
{
    //Column 1 sequence
    C1=1;
    C2=0;
    C3=0;
    
    if(R1==1){ //Column 1 = 1 and if Row 1=1 it means 1 is pressed
        lcd_data('1');
        while(R1==1); // this condition will be true until we release the button
    }
    if(R2==1){
        lcd_data('4');
        while(R2==1);
    }
    if(R3==1){
        lcd_data('7');
        while(R3==1);
    }
    if(R4==1){
        lcd_data('*');
        while(R4==1);
    }
    
    //Column 2 sequence
    C1=0;
    C2=1;
    C3=0;
    
    if(R1==1){ 
        lcd_data('2');
        while(R1==1); 
    }
    if(R2==1){
        lcd_data('5');
        while(R2==1);
    }
    if(R3==1){
        lcd_data('8');
        while(R3==1);
    }
    if(R4==1){
        lcd_data('0');
        while(R4==1);
    }
    
    //Column 3 sequence
    C1=0;
    C2=0;
    C3=1;
    
    if(R1==1){ 
        lcd_data('3');
        while(R1==1); 
    }
    if(R2==1){
        lcd_data('6');
        while(R2==1);
    }
    if(R3==1){
        lcd_data('9');
        while(R3==1);
    }
    if(R4==1){
        lcd_data('#');
        while(R4==1);
    }
}

void main(void)
{
    TRISC = 0x00; 
    TRISD = 0x00;
    TRISB = 0xF0;
    //we are implementing row scan technique
    lcd_initialise();
    lcd_command(0x80); // first row first column of lcd
    lcd_string("KEYPAD:", 7); //printing keypad
    lcd_command(0xC0);  //second row first column of lcd
    while(1)
    {
     keypad();
    }
}