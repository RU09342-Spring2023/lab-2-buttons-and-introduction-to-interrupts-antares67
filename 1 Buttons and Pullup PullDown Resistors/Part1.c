/*
 *  Pull-Up Resistor Configuration
 *
 *  Created on: Jan 31, 2023
 *      Author: Thomas Rasa
 *      Version: 2.0
 *
 *      This example will blink LED's while the button is held down
 */

#include <msp430.h>

void gpioInit();

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    gpioInit();                 // Initialize all GPIO Pins for the project


    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings


    while(1)
    {
        if (~P2IN & BIT3)           // If S2 (P2.3) is unpressed
            P6OUT ^= BIT6;          // Toggle P6.6
        else
            P6OUT &= ~BIT6;         //Prevent LED from sticking high by setting to Low
        if (~P4IN & BIT1)           // If S1 (P4.1) is unpressed
            P1OUT ^= BIT0;          // Toggle P1.0
        else
            P1OUT &= ~BIT0;     //Prevent LED from sticking high by setting to Low
        __delay_cycles(100000);             // Delay for 100000*(1/MCLK)=0.1s
    }


    return 0;
}

void gpioInit()
{
   // Setting Directions of Pins

       P1DIR |= BIT0;              // Configure P1.0 to an Output
       P6DIR |= BIT6;              // Configure P6.6 to an Output
       P2DIR &= ~BIT3;             // Configure P2.3 to an Input
       P4DIR &= ~BIT1;             // Configure P4.1 to an Input

   // Configuring Pullup Resistors per MSP430FR2355 Family User Guide
   /*
    *   PXDIR | PXREN | PXOUT | I/O Configuration
    *     0       0       X     Input
    *     0       1       0     Input with Pull Down Resistor
    *     0       1       1     Input With Pull Up Resistor
    *     1       X       X     Output
    */

       P2REN |= BIT3;               // Enable Resistor on P2.3
       P2OUT |= BIT3;               // Configure Resistor on P2.3 to be Pullup

       P4REN |= BIT1;               // Enable Resistor on P4.1
       P4OUT |= BIT1;               // Configure Resistor on P4.1 to be Pullup
}
