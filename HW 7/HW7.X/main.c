#include <xc.h>           // processor SFR definitions
#include <sys/attribs.h>  // __ISR macro
#include <stdio.h>
#include "ILI9163C.h"
#include "i2c.h"

// DEVCFG0
#pragma config DEBUG = 0b11 // no debugging (disabled)
#pragma config JTAGEN = 0 // no jtag (disabled)
#pragma config ICESEL = 0b11 // use PGED1 and PGEC1
#pragma config PWP = OFF // no write protect ()
#pragma config BWP = 1 // no boot write protect (writeable)
#pragma config CP = 1 // no code protect

// DEVCFG1
#pragma config FNOSC = 0b011  // use primary oscillator with pll
#pragma config FSOSCEN = 0  // turn off secondary oscillator (disabled)
#pragma config IESO =  0 // no switching clocks (disabled)
#pragma config POSCMOD = 0b10 // high speed crystal mode
#pragma config OSCIOFNC = 1  // disable secondary osc
#pragma config FPBDIV = 0b00 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM =  0b11 // do not enable clock switch
#pragma config WDTPS = 0b10100 // use slowest wdt
#pragma config WINDIS = 1 // wdt no window mode
#pragma config FWDTEN = 0 // wdt disabled
#pragma config FWDTWINSZ = 0b11 // wdt window at 25% //0b11?

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = 0xb001 // divide input clock to be in range 4-5MHz; resonater 8MHZ so divide by 2
#pragma config FPLLMUL = 0b111 // multiply clock after FPLLIDIV; used highest mult
#pragma config FPLLODIV = 0b001 // divide clock after FPLLMUL to get 48MHz
#pragma config UPLLIDIV = 0b001  // divider for the 8MHz input clock, then multiplied by 12 to get 48MHz for USB
#pragma config UPLLEN = 0 // USB clock on (enabled)

// DEVCFG3
#pragma config USERID = 0xFFFF // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = 0 // allow multiple reconfigurations
#pragma config IOL1WAY = 0 // allow multiple reconfigurations
#pragma config FUSBIDIO = 1 // USB pins controlled by USB module
#pragma config FVBUSONIO = 1 // USB BUSON controlled by USB module

#define dataLen 14

typedef struct {
    unsigned short x, y, z;
} Vector;

int main() {
    char message[100];
    Vector gyro, accel;
    unsigned short temperature;
    float temp_x, temp_y;
    unsigned char data[dataLen];    
    int t; 
    
    __builtin_disable_interrupts();

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    SPI1_init();
    LCD_init();
    i2c_init();
    i2c_expander_init();
    
    __builtin_enable_interrupts();
    
    LCD_clearScreen(BLACK);
    
    sprintf(message, "WHOAMI:%d", i2c_expander_get(WHO_AM_I));
    LCD_string(message, 10, 80, WHITE, BLACK);
    
    while(1)
    {
        t = _CP0_GET_COUNT() + 4800000;
        i2c_receive_multiple(OUT_TEMP_L, data, dataLen);
        temperature = (data[ 1] << 8) | data[ 0];
        gyro.x      = (data[ 3] << 8) | data[ 2];
        gyro.y      = (data[ 5] << 8) | data[ 4];
        gyro.z      = (data[ 7] << 8) | data[ 6];
        accel.x     = (data[ 9] << 8) | data[ 8];
        accel.y     = (data[11] << 8) | data[10];
        accel.z     = (data[13] << 8) | data[12];      
        
        temp_x = accel.x*.0061;
        temp_y = accel.y*.0061;
        sprintf(message, "AX: %.2f   ", temp_x);
        LCD_string(message, 10, 100, WHITE, BLACK);
        
        sprintf(message, "AY: %.2f   ", temp_y);
        LCD_string(message, 10, 90, WHITE, BLACK);
        
        LCD_bar(64,64,-(temp_x),4,50,  RED,BLACK,0);
        LCD_bar(64,64,-(temp_y),4,50,GREEN,BLACK,1);
        
        while(_CP0_GET_COUNT() < t){;}
    }
    
    
}