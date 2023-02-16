

/*
 * OccupancyDetector.c
 *
 *  Created on: Jan 30, 2023
 *      Author: Thomas Rasa
 */



#include <msp430.h>


char currentState;                       // Global Variable to track if the LED should be on or off
const char armedState = 0x01;
const char warningState = 0x02;
const char alarmState = 0x03;
char sensor = 0x00;
char button = 0x00;
char counter = 0x00;
char systemReset = 0x00;

char currentState = armedState; //Initialize system to armed state

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    // Configure GPIO
    P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;                          // Set P1.0 to output direction

    P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
    P6DIR |= BIT6;                          // Set P6.6 to output direction


    // @TODO You need to add in the configuration for the Green LED

    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
    P2REN |= BIT3;                          // P2.3 pull-down register enable
    P2IES |= BIT3;                         // P2.3 High --> Low edge
    P2IE |= BIT3;                           // P2.3 interrupt enabled


    P4OUT |= BIT1;                          // Configure P4.1 as pulled-up
    P4REN |= BIT1;                          // P4.1 pull-down register enable
    P4IES |= BIT1;                         // P4.1 High --> Low edge
    P4IE |= BIT1;                           // P4.1 interrupt enabled

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    P2IFG &= ~BIT3;                         // P2.3 IFG cleared
    P4IFG &= ~BIT1;                         // P2.3 IFG cleared

    __bis_SR_register(GIE);                 // Enter LPM3 w/interrupt



    while(1)
    {

        switch(currentState)
        {

        case armedState:
            sensor = 0x00;
            counter = 0x00;
            systemReset = 0x00;
            P1OUT &= ~BIT0; //reset red LED to low from alarm state
            P6OUT ^= BIT6;
            __delay_cycles(100000);
            P6OUT ^= BIT6;
            __delay_cycles(300000);        //Red LED blinks, 3secs off and 1000ms on

            if(sensor == 0x01){
                currentState = warningState;
            }
            break;

        case warningState:

            counter = 0x00;

            while(sensor && (counter < 10)){

            P1OUT ^= BIT0;
            __delay_cycles(500000); //Red LED blinks, 500ms on and 500ms off
            P1OUT ^= BIT0;
            __delay_cycles(500000);

            counter++;

            }

            if(!sensor && (counter < 10)){ //if the sensor no longer detects anyone and the timer has not reached 10 seconds
                currentState = armedState;

            } else if(counter >= 10){ // if timer is still high and counter has reached 10 seconds
                currentState = alarmState;
            }


            break;
        case alarmState:
            P1OUT |= BIT0; //set red LED solid
            __delay_cycles(1000000); //cycle once per second


            if (!sensor && (counter < 15)){
                currentState = armedState;
            } else if(!sensor && (counter > 15)){
                P1OUT |= BIT0;
            }
            else if(systemReset){
                currentState = armedState;
            }


            break;

        }



    }
}

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{

    P2IFG &= ~BIT3;
    sensor ^= BIT0;
    P2IES ^= BIT3;                         // P2.3 High --> Low edge



}


#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{

        P4IFG &= ~BIT1;                         // Clear P1.3 IFG
        systemReset ^= BIT0;               //reset back to armed state with port 4.1 button
   }




