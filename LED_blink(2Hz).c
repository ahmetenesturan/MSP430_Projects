#include <msp430.h> 

// Blinks the LED in P1.0 every 0.5 seconds

//warning #10420-D: For FRAM devices, at start up, 
//the GPIO power-on default high-impedance mode needs 
//to be disabled to activate previously configured port settings. 
//This can be done by clearing the LOCKLPM5 bit in PM5CTL0 register.

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;       // Disable the GPIO power-on default high-impedance mode
                                // to activate previously configured port settings

	P1DIR |= 0x01; // set P1.0 as output
	P1OUT &= ~0x01; // toggle off LED

	TB0CTL |= TBCLR; // clear the timer
	TB0CTL |= TBSSEL__ACLK; // select ACLK(f = 32768 Hz)
	TB0CTL |= MC__UP; // select UP mode
	TB0CCR0 = 16384; // set compare value (16384 / 32768 = 0.5s)

	// interrupt enable

	TB0CCTL0 |= CCIE; // compare interrupt enable
	__enable_interrupt(); // global interrupt enable
	TB0CCTL0 &= ~CCIFG; // clear flag


	while(1){} // main loop
}


#pragma vector = TIMER0_B0_VECTOR //set interrupt vector

__interrupt void ISR_timer_compare(void)
{
    TB0CCTL0 &= ~CCIFG; // clear flag
    P1OUT ^= 0x01; // toggle LED
}
