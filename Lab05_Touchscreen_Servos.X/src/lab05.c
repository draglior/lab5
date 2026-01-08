#include "lab05.h"

#include <xc.h>
//do not change the order of the following 2 definitions
#define FCY 12800000UL
#include <libpic30.h>
#include "math.h"
#include "types.h"
#include "lcd.h"
#include "led.h"

#define TCKPS_1   0x00
#define TCKPS_8   0x01
#define TCKPS_64  0x02
#define TCKPS_256 0x03

#define PWM_MIN_US 1000
#define PWM_MID_US 1500
#define PWM_MAX_US 2000
#define PWM_CYC_US 20000

void servo_initialize(void){
    //configure timer 2 for pwm (20ms period)
    CLEARBIT(T2CONbits.TON);
    CLEARBIT(T2CONbits.TCS);
    CLEARBIT(T2CONbits.TGATE);
    T2CONbits.TCKPS = TCKPS_64;
    TMR2 = 0;
    PR2 = 3999;
    //configure output compare 7 (servo y)and  8(servo X)
    CLEARBIT(TRISDbits.TRISD6);
    CLEARBIT(TRISDbits.TRISD7);
    //SET INITIAL DUTY CYCLE (SAFE MIDDLE POSITION)
    OC7R = 0;
    OC7RS = 0;
    OC8R = 0;
    OC8RS = 0;
    //SET PWM, no fault, timer2
    OC7CON= 0x0006;
    OC8CON= 0x0006;
    // SET TIMER 2
    SETBIT(T2CONbits.TON);
}
void touchscreen_initialize(void){
//CONFIGURATION CONTROL PINS (E1,E2,E3) as outputs
    CLEARBIT(TRISEbits.TRISE1);
    CLEARBIT(TRISEbits.TRISE2);
    CLEARBIT(TRISEbits.TRISE3);
    CLEARBIT(PORTEbits.RE1);
    SETBIT(PORTEbits.RE2);
    SETBIT(PORTEbits.RE3);
//configuration ADC1
    CLEARBIT(AD1CON1bits.ADON);
//Setup AN15 (X read)and An9 (Y read)as analog inputs
    CLEARBIT(AD1PCFGLbits.PCFG15); //AN15 analog
    CLEARBIT(AD1PCFGLbits.PCFG9);  //AN9 Analog
//Basic ADC CONFIG
    CLEARBIT(AD1CON1bits.AD12B);
    AD1CON1bits.FORM=0;
    AD1CON1bits.SSRC=7;
    AD1CON2=0;
    AD1CON3bits.ADRC= 0;
    AD1CON3bits.SAMC =0x1F;
    AD1CON3bits.ADCS =2;
    SETBIT(AD1CON1bits.ADON); //enable adc
}

void servo_setduty(uint8_t servo, uint16_t duty_us){
    if (duty_us < 1000) duty_us = 1000;
    if (duty_us > 2000) duty_us = 2000;
    
    uint16_t pulseTicks = duty_us / 5; //5us per tick (from the timer2)
    
    uint16_t invertedTicks = 4000 - pulseTicks; //20ms/5us = period ticks
    if (servo == 0) OC7RS = invertedTicks;
    else if (servo == 1) OC8RS = invertedTicks;
   
}


void main_loop()
{
    // print assignment information
    lcd_printf("Lab05: Touchscreen &\r\n");
    lcd_printf("       Servos");
    lcd_locate(0, 2);
    lcd_printf("Group: GroupName");
    
    servo_initialize();
    
    while(TRUE) {
    
        servo_setduty(0, 1000);
        servo_setduty(1, 1000);
        __delay_ms(3000);
        
        servo_setduty(0, 2000);
        servo_setduty(1, 1000);
        __delay_ms(3000);
        
        servo_setduty(0, 2000);
        servo_setduty(1, 2000);
        __delay_ms(3000);
        
        servo_setduty(0, 1000);
        servo_setduty(1, 2000);
        __delay_ms(3000);
   
    }
}
