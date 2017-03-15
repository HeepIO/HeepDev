#include "TCPIP Stack/TCPIP.h"


//return values
#define		TEST_OK			0
#define		TEST_GND		1
#define		TEST_VCC		2
#define		TEST_ZERO_A	3
#define		TEST_ZERO_B	4
#define		TEST_ZERO_C	5
#define		TEST_ZERO_D	6
#define		TEST_ZERO_E	7
#define		TEST_ZERO_F	8
#define		TEST_ZERO_G	9
#define		TEST_ZERO_H 10
#define		TEST_ZERO_J	11
#define		TEST_RTC		12
#define		TEST_ANALOG	13

#define DELAY_TIME 			300
#define DT 					150
#define	DELAY_TIME_LONG		2000

#define	BIT0	0x01
#define	BIT1	0x02
#define	BIT2	0x04
#define	BIT3	0x08
#define	BIT4	0x10
#define	BIT5	0x20
#define	BIT6	0x40
#define	BIT7	0x80


#define DELAY_TIME 			300
#define DT 					150
#define	DELAY_TIME_LONG		2000
	



volatile unsigned char mask_port_a 	= 0x38; 
volatile unsigned char mask_port_b 	= 0x2f;
volatile unsigned char mask_port_c 	= 0x04;	// possible problems with SPI memory?
volatile unsigned char mask_port_d 	= 0x07;
volatile unsigned char mask_port_e 	= 0x3F;
volatile unsigned char mask_port_f 	= 0x7e;
//volatile unsigned char mask_port_g 	= 0x10;

volatile unsigned char temp_porta 	= 0x0;
volatile unsigned char temp_portb 	= 0x0;
volatile unsigned char temp_portc 	= 0x0;
volatile unsigned char temp_portd 	= 0x0; 
volatile unsigned char temp_porte 	= 0x0;
volatile unsigned char temp_portf 	= 0x0;
//volatile unsigned char temp_portg 	= 0x0;

unsigned char 	i		= 0;


static int test_in_progress = 0;

void InitRTC(void)
{
	// Enable timer&external oscillator driver
	//T1CON = TMR1ON | TMR1CS | T1OSCEN;
	
	TRISC |= 0b11;
	
	T1CON = 0b00001011;
	
	// the first interrut will be in ~1 sec
	TMR1L = 0x00;
	TMR1H = 0x80;
	
	// enable timer1 interrupt on overflow
	PIE1bits.TMR1IE = 1;
	IPR1bits.TMR1IP = 1; //high priority
	
	INTCONbits.GIEH = 1;
	
}

#if 0
void
InterruptHandlerHigh();

#pragma code InterruptVectorHigh = 0x08
void
InterruptVectorHigh (void)
{
  _asm
    goto InterruptHandlerHigh //jump to interrupt routine
  _endasm
}

//----------------------------------------------------------------------------
// High priority interrupt routine

#pragma code
#pragma interrupt InterruptHandlerHigh

#endif /* TODO */

void
InterruptHandlerHigh ()
{
		// Toggle led 
	LED1_IO ^= 1;

	// Load TMRL and TMRH for one sec interrupt
	TMR1L = 0x00;
	TMR1H = 0x80;

	// clear
	PIR1bits.TMR1IF = 0;
}

// simple delay ~ 1 mS
void Delay(unsigned long a) { 
	 a *= 370;
	while (--a!=0); 
}

void InitPort(void) {

	// all as gpio
	ADCON1 	= 0x0F;

	// All as input
	TRISA 	= 0xFF;
	TRISB 	= 0xFF;
	TRISC 	= 0xFF;
	TRISD 	= 0xFF;
	TRISE 	= 0xFF;
	TRISF 	= 0xFF;
	//TRISG 	= 0xFF;

	//pull up - output
	TRISG &= ~BIT4;
	//pull up - high
	PORTG |= BIT4;

	Delay(10);

}

void GetPort_AND(void) {

	temp_porta 	= PORTA;
	temp_porta 	&= mask_port_a; 
	temp_portb 	= PORTB;
	temp_portb 	&= mask_port_b; 
	temp_portc 	= PORTC;
	temp_portc 	&= mask_port_c; 
	temp_portd 	= PORTD;
	temp_portd 	&= mask_port_d; 
	temp_porte 	= PORTE;
	temp_porte 	&= mask_port_e;
	temp_portf 	= PORTF;
	temp_portf 	&= mask_port_f;
	//temp_portg 	= PORTG;
	//temp_portg 	&= mask_port_g;
}

void GetPort_OR(void) {
	temp_porta 	= PORTA;
	temp_porta 	|= (~mask_port_a); 
	temp_portb 	= PORTB;
	temp_portb 	|= (~mask_port_b); 
	temp_portc 	= PORTC;
	temp_portc 	|= (~mask_port_c); 
	temp_portd 	= PORTD;
	temp_portd 	|= (~mask_port_d); 
	temp_porte 	= PORTE;
	temp_porte 	|= (~mask_port_e); 
	temp_portf 	= PORTF;
	temp_portf 	|= (~mask_port_f); 
	//temp_portg 	= PORTG;
	//temp_portg 	|= (~mask_port_g); 

}





unsigned char TestExt(void) 
{
	unsigned char rtctemp;	
	
	unsigned int adctemp;


	// TEST EXTENSION PORT

	// Check for GND ======================================
	InitPort();

	GetPort_OR();

	if( ((temp_porta) != (0xFF)) ||  
		((temp_portb) != (0xFF)) ||  
		((temp_portc) != (0xFF)) ||  
		((temp_portd) != (0xFF)) ||  
		((temp_porte) != (0xFF)) ||  
		((temp_portf) != (0xFF)) )  return TEST_GND;


	// Check for VCC =============================
	// all as input
	InitPort();

	//pull up - output
	TRISG &= ~BIT4;
	//pull up - low
	PORTG &= ~BIT4;

	Delay(10);

	GetPort_AND();
 
	if( ((temp_porta) != (0x0)) ||
		((temp_portb) != (0x0)) ||
		((temp_portc) != (0x0)) ||
		((temp_portd) != (0x0)) ||
		((temp_porte) != (0x0)) ||
		((temp_portf) != (0x0))  ) return TEST_VCC;


	// Running zero =========================================
	
	// Port A
	// all as input
	InitPort();

	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_a)&(1<<i))) continue;
		
		// TRISA = ~((1<<i)|BIT2);
		// PORTA = ~(1<<i);

		TRISA = ~(1<<i);
		PORTA = ~(1<<i);

		Delay(10);

		GetPort_OR();

		// check for other zero at PortA
		if((temp_porta) != (0xFF&(~(1<<i)))) 	{ return TEST_ZERO_A; } 
		// check for other zero at PortB
		if((temp_portb) != (0xFF)) 				{ return TEST_ZERO_A; } 
		// check for other zero at PortC
		if((temp_portc) != (0xFF)) 				{ return TEST_ZERO_A; } 
		// check for other zero at PortD
		if((temp_portd) != (0xFF)) 				{ return TEST_ZERO_A; } 
		// check for other zero at PortE
		if((temp_porte) != (0xFF)) 				{ return TEST_ZERO_A; } 
		// check for other zero at PortF
		if((temp_portf) != (0xFF)) 				{ return TEST_ZERO_A; } 		
	}


	// Port B
	// all as input
	InitPort();

	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_b)&(1<<i))) continue;
		
		TRISB = ~(1<<i);
		PORTB = ~(1<<i);

		Delay(10);

		GetPort_OR();

		// check for other zero at PortA
		if((temp_porta) != (0xFF)) 				{ return TEST_ZERO_B; } 
		// check for other zero at PortB
		if((temp_portb) != (0xFF&(~(1<<i)))) 	{ return TEST_ZERO_B; } 
		// check for other zero at PortC
		if((temp_portc) != (0xFF)) 				{ return TEST_ZERO_B; } 
		// check for other zero at PortD
		if((temp_portd) != (0xFF)) 				{ return TEST_ZERO_B; } 
		// check for other zero at PortE
		if((temp_porte) != (0xFF)) 				{ return TEST_ZERO_B; } 
		// check for other zero at PortF
		if((temp_portf) != (0xFF)) 				{ return TEST_ZERO_B; } 
	}


	// PortC
	// all as input
	InitPort();

	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_c)&(1<<i))) continue;
	
		TRISC = ~(1<<i);
		PORTC = ~(1<<i);
		
		Delay(10);

		GetPort_OR();	
		
		// check for other zero at PortA
		if((temp_porta) != (0xFF)) 				{ return TEST_ZERO_C; } 
		// check for other zero at PortB
		if((temp_portb) != (0xFF)) 				{ return TEST_ZERO_C; } 
		// check for other zero at PortC
		if((temp_portc) != (0xFF&(~(1<<i))))	{ return TEST_ZERO_C; } 
		// check for other zero at PortD
		if((temp_portd) != (0xFF)) 				{ return TEST_ZERO_C; } 
		// check for other zero at PortE
		if((temp_porte) != (0xFF)) 				{ return TEST_ZERO_C; } 
		// check for other zero at PortF
		if((temp_portf) != (0xFF)) 				{ return TEST_ZERO_C; } 
	}


	// PortD
	// all as input
	InitPort();
	
	//loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_d)&(1<<i))) continue;
	
		TRISD = ~(1<<i);
		PORTD = ~(1<<i);
		
		Delay(10);

		GetPort_OR();
	
		// check for other zero at PortA
		if((temp_porta) != (0xFF)) 				{ return TEST_ZERO_D; } 
		// check for other zero at PortB
		if((temp_portb) != (0xFF)) 				{ return TEST_ZERO_D; } 
		// check for other zero at PortC
		if((temp_portc) != (0xFF))				{ return TEST_ZERO_D; } 
		// check for other zero at PortD
		if((temp_portd) != (0xFF&(~(1<<i)))) 	{ return TEST_ZERO_D; } 
		// check for other zero at PortE
		if((temp_porte) != (0xFF)) 				{ return TEST_ZERO_D; } 
		// check for other zero at PortF
		if((temp_portf) != (0xFF)) 				{ return TEST_ZERO_D; } 
	}


	// PortE
	// all as input
	InitPort();

	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_e)&(1<<i))) continue;
	
		TRISE = ~((1<<i)|BIT5);
		PORTE = ~(1<<i);
		
		Delay(10);
	
		GetPort_OR();	

		// check for other zero at PortA
		if((temp_porta) != (0xFF)) 				{ return TEST_ZERO_E; } 
		// check for other zero at PortB
		if((temp_portb) != (0xFF)) 				{ return TEST_ZERO_E; } 
		// check for other zero at PortC
		if((temp_portc) != (0xFF))				{ return TEST_ZERO_E; } 
		// check for other zero at PortD
		if((temp_portd) != (0xFF)) 				{ return TEST_ZERO_E; } 
		// check for other zero at PortE
		if((temp_porte) != (0xFF&(~(1<<i))))	{ return TEST_ZERO_E; } 
		// check for other zero at PortF
		if((temp_portf) != (0xFF)) 				{ return TEST_ZERO_E; } 
	}

	// PortF
	// all as input
	InitPort();

	// loop
	for(i=0; i<8; i++) {
	
		// this port is not tested
		if(!((mask_port_f)&(1<<i))) continue;
	
		TRISF = ~(1<<i);
		PORTF = ~(1<<i);
		
		Delay(10);
	
		GetPort_OR();	

		// check for other zero at PortA
		if((temp_porta) != (0xFF)) 				{ return TEST_ZERO_F; } 
		// check for other zero at PortB
		if((temp_portb) != (0xFF)) 				{ return TEST_ZERO_F; } 
		// check for other zero at PortC
		if((temp_portc) != (0xFF))				{ return TEST_ZERO_F; } 
		// check for other zero at PortD
		if((temp_portd) != (0xFF)) 				{ return TEST_ZERO_F; } 
		// check for other zero at PortE
		if((temp_porte) != (0xFF))				{ return TEST_ZERO_F; } 
		// check for other zero at PortF
		if((temp_portf) != (0xFF&(~(1<<i)))) 	{ return TEST_ZERO_F; } 
	}
	
	InitRTC();
	Delay(100);
	Delay(100);
	rtctemp = TMR1L;
	Delay(100);
	Delay(100);
	if (TMR1L == rtctemp) return TEST_RTC;
	
	TRISA &= 0b11110111;  // RA3 as output
	TRISA |= 0b00000100;  // RA2 as input
	PORTA |= 0b00001000;	// high
	

	ADCON0bits.CHS0 = 0; // select channel 2
	ADCON0bits.CHS1 = 1;
	ADCON0bits.CHS2 = 0;
	ADCON0bits.CHS3 = 0;
	
	ADCON1 = 0x0c; // AD0-2 as analog
	Delay(10);
	
	ADCON0bits.ADON = 1;
	
	Delay(10);
	ADCON0bits.GO = 1;
	

    // Wait until A/D conversion is done
    while(ADCON0bits.GO);
    
    adctemp = *((WORD*)(&ADRESL));
    
    
    

	return TEST_OK;
}

extern void InitializeBoard(void);

void HTTPPrint_olimex_ext_test(void)
{
	ROM void* str;
	unsigned char stat;

	test_in_progress = 1;

	// Disable pull up on PortB
	INTCON2bits.RBPU = 1;

	// UART bit as gpio
	RCSTA = 0x0;

	// ADC off
	ADCON0 = 0x0;
	ADCON1 = 0x0F;    
	stat = TestExt();
	InitializeBoard();	
	MPFSInit();	

	switch(stat) {
		case TEST_OK:		str = (ROM void*)"OK"; break;
		case TEST_GND:		str = (ROM void*)"ERROR: GND"; break;
		case TEST_VCC:		str = (ROM void*)"ERROR: VCC"; break;
		case TEST_ZERO_A:	str = (ROM void*)"ERROR: ZERO A"; break;
		case TEST_ZERO_B:	str = (ROM void*)"ERROR: ZERO B"; break;
		case TEST_ZERO_C:	str = (ROM void*)"ERROR: ZERO C"; break;
		case TEST_ZERO_D:	str = (ROM void*)"ERROR: ZERO D"; break;
		case TEST_ZERO_E:	str = (ROM void*)"ERROR: ZERO E"; break;
		case TEST_ZERO_F:	str = (ROM void*)"ERROR: ZERO F"; break;
		case TEST_ZERO_G:	str = (ROM void*)"ERROR: ZERO G"; break;
		case TEST_ZERO_H:	str = (ROM void*)"ERROR: ZERO H"; break;
		case TEST_ZERO_J:	str = (ROM void*)"ERROR: ZERO J"; break;
		case TEST_RTC:		str = (ROM void*)"ERROR: RTC"; break;
		case TEST_ANALOG:	str = (ROM void*)"ERROR: ANALOG"; break;
		default:		str = (ROM void*)"ERROR: unknown error code"; break;
	}

	TCPPutROMString(sktHTTP, (ROM void*)str);
	test_in_progress = 0;
}

