#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// BASE ADDRESSES
#define PMM_BASE    0x0120 // PMM base address
#define PM5CTL0     (*(volatile uint8_t *)(PMM_BASE + 0x10)) // PM5CTL0 Register from PMM with offset 0x10
#define P1_2_BASE   0x0200 // Port p1, p2 and  base address
#define P4_BASE     0x0220 // Port p4 and  base address
#define P6_BASE     0x0240 // Port p6 and  base address
// PORT1
#define P1_OUT      (*(volatile uint8_t *)(P1_2_BASE + 0x02))
#define P1_DIR      (*(volatile uint8_t *)(P1_2_BASE + 0x04))
#define P1_PEN      (*(volatile uint8_t *)(P1_2_BASE + 0x06))
// PORT2
#define P2_IN       (*(volatile uint8_t *)(P1_2_BASE + 0x01))
#define P2_DIR      (*(volatile uint8_t *)(P1_2_BASE + 0x05))
#define P2_PEN      (*(volatile uint8_t *)(P1_2_BASE + 0x07))
// PORT4
#define P4_IN       (*(volatile uint8_t *)(P4_BASE + 0x01))
#define P4_DIR      (*(volatile uint8_t *)(P4_BASE + 0x05))
#define P4_PEN      (*(volatile uint8_t *)(P4_BASE + 0x07))
// PORT6
#define P6_OUT      (*(volatile uint8_t *)(P6_BASE + 0x03))
#define P6_DIR      (*(volatile uint8_t *)(P6_BASE + 0x05))
#define P6_PEN      (*(volatile uint8_t *)(P6_BASE + 0x07))

void main(void)
{
    // stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // CLEAR LOCKLPM5
    PM5CTL0 &= ~(1 << 0);

    // PORT2 CONFIG (Button on P2.3)
    P2_DIR &= ~(1 << 3);   // Set P2.3 as input
    P2_PEN |= (1 << 3);    // Enable pull-up resistor for P2.3

    // PORT4 CONFIG (Button on P4.1)
    P4_DIR &= ~(1 << 1);   // Set P4.1 as input
    P4_PEN |= (1 << 1);    // Enable pull-up resistor for P4.1

    // PORT1 CONFIG (LEDs on P1.0, P1.2, P1.5)
    P1_DIR |= (1 << 0);    // Set P1.0 as output
    P1_DIR |= (1 << 2);    // Set P1.2 as output
    P1_DIR |= (1 << 5);    // Set P1.5 as output
    P1_DIR |= (1 << 6);    // Set P1.6 as output
    P1_PEN &= ~(1 << 0);   // Disable pull-up/down resistor for P1.0
    P1_PEN &= ~(1 << 2);   // Disable pull-up/down resistor for P1.2
    P1_PEN &= ~(1 << 5);   // Disable pull-up/down resistor for P1.5
    P1_PEN &= ~(1 << 6);   // Disable pull-up/down resistor for P1.6


    // PORT6 CONFIG (LED on P6.6)
    P6_DIR |= (1 << 6);    // Set P6.6 as output
    P6_PEN &= ~(1 << 6);   // Disable pull-up/down resistor for P6.6

    // LED CONFIG
    P1_OUT &= ~(1 << 0);
    P1_OUT &= ~(1 << 2);
    P1_OUT &= ~(1 << 5);
    P1_OUT &= ~(1 << 6);
    P6_OUT &= ~(1 << 6);

    // counter variable
    uint8_t current_led = 0;

    while(1) {
        // Check if button on P2.3 is pressed (active-low)
        if (!(P2_IN & (1 << 3))) {
            P6_OUT |= (1 << 6);   // (turn on LED) on p6.6
            current_led = (current_led + 1) % 3;
            if (current_led == 0) P1_OUT |= (1 << 2);
            else if (current_led == 1) P1_OUT |= (1 << 5);
            else if (current_led == 2) P1_OUT |= (1 << 6);
            __delay_cycles(500000);
        }else{
            P1_OUT &= ~(1 << 0);
            P1_OUT &= ~(1 << 2);
            P1_OUT &= ~(1 << 5);
            P1_OUT &= ~(1 << 6);
            P6_OUT &= ~(1 << 6);
        }
        // Check if button on P4.1 is pressed (active-low)
        if (!(P4_IN & (1 << 1))) {
            P1_OUT |= (1 << 0);   // Turn on LED on P1.0
            P1_OUT |= (1 << 2);
            P1_OUT |= (1 << 5);
            P1_OUT |= (1 << 6);
            __delay_cycles(500000);
        }else{
            P1_OUT &= ~(1 << 0);
            P1_OUT &= ~(1 << 2);
            P1_OUT &= ~(1 << 5);
            P1_OUT &= ~(1 << 6);
            P6_OUT &= ~(1 << 6);
        }
    }
}
