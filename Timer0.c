#include <avr/interrupt.h>
#include <inttypes.h> 

static volatile uint8_t SecFlag; 
static volatile uint32_t mSecCnt = 0;


void Timer0_Ini(void)
{
	TCNT0= 0; //Valor inicial Timer0
	TCCR0A= (2<< WGM00);  //Inicializa Timer0 en CTC
	TCCR0B= (2<<CS00);  //Prescalador 8 0.5 us por tick
	OCR0A = 199;  //0.1ms 100us (0.1ms/0.5us -1)  tiempo/(1/(fHz/preescalador)) - 1
	TIMSK0= (1<<OCIE0A); //Interrupcion Timer0 cuando el timer llega a ocra
    sei();   		/* habilita interrupciones (global) 	*/
}

uint8_t Timer0_SecFlag ( void )
{  
   if( SecFlag )
   {
      SecFlag=0;
      return 1;
   }
   else
   {
      return 0;
   }
}

uint32_t uS100(void)
{
	return mSecCnt; 
}

ISR(TIMER0_COMPA_vect) /* TimerCounter0 Compare Match A */
{ /* TIMER0_OVF_vect */
	TCNT0-=200; //Reinicializar Timer0 sin perder conteo
	mSecCnt++; //Incrementa contador
}
       
ISR (TIMER0_OVF_vect)	/* Timer/Couner0 Overflow */
{
   static uint16_t  mSecCnt;
   TCNT0+=0x06;    		/* reinicializar Timer0 sin perder conteo	*/    
   mSecCnt++;  		/* Incrementa contador de milisegundos		*/
   if( mSecCnt==1000 )
   {
      mSecCnt=0;   
      SecFlag=1;  		/* Bandera de Segundos 				*/
   } 
}


