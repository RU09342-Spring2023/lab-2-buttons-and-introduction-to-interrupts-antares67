/*
 *  Button Interrupt Example
 *
 *  Created on: Feb, 2023
 *      Author: Thomas Rasa
 */


#include <msp430.h>

char ToggleEnable = 0x01;                       // Global Variable to track if the LED should be on or off

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    // Configure GPIO
    P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;                          // Set P1.0 to output direction

    P6OUT |= BIT6;                         // Clear P6.6 output latch for a defined power-on state
    P6DIR |= BIT6;                          // Set P6.6 to output direction


    // @TODO You need to add in the configuration for the Green LED

    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
    P2REN |= BIT3;                          // P2.3 pull-up register enable
    P2IES &= ~BIT3;                         // P2.3 Low --> High edge
    P2IE |= BIT3;                           // P2.3 interrupt enabled

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    P2IFG &= ~BIT3;                         // P2.3 IFG cleared

    __bis_SR_register(GIE);                 // Enter LPM3 w/interrupt

    while(1)
    {
        // @TODO You will need to modify this code to change between blinking the Red LED or the Green LED
        if (ToggleEnable){
            P1OUT ^= BIT0;                  // P1.0 = toggle
            P6OUT &= ~BIT6;
           __delay_cycles(100000);
        } else {
            P6OUT ^= BIT6;
            P1OUT &= ~BIT0;
           __delay_cycles(100000);
        }
    }
}

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    // @TODO You might need to modify this based on your approach to the lab
    P2IFG &= ~BIT3;                         // Clear P1.3 IFG
    ToggleEnable ^= 0x01;                   // Enable if the toggle should be active
}

