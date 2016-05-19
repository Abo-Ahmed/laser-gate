

#include <avr/io.h>
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>


//function to display data on 7seg
unsigned char _7seg (char c )
{
	switch (c)
	{
	case 0:{c=0b00111111;}
	break;
	case 1:{c=0b00000110;}
	break;
	case 2:{c=0b01011011;}   
	break;
	case 3:{c=0b01001111;}  
	break;
	case 4:{c=0b01100110;}   
	break;
	case 5:{c=0b01101101;}   
	break;
	case 6:{c=0b01111101;}  
	break;
	case 7:{c=0b00000111;}   
	break;
	case 8:{c=0b01111111;}
	break;
	case 9:{c=0b01101111;}
	break;
		
	}
	
	return ~c; //we have used not bitwise operator because we are using common anode 7segment
	
}	

// this function is used with port b and port d
void _double7seg (int num )
{
	PORTD = _7seg((int) num / 10 );
	PORTB = _7seg(num - (num / 10) * 10);
	
}	

void _adjust (int num  )
{
	if(num < 0 ){
		num = 0 ;
		return;}		
	else if (num > 99){
		num = 99 ;
		return;}
		
				
	_double7seg(num);
	
	
	if (num  == 0 )
	PORTC = 0b00000010 ;
	else 
	PORTC = 0b00000101 ;
}	
int main(void)
{

	DDRA=0;
	PORTA=0xff;
	DDRB=0xFF;
	PORTB = _7seg(0);
	DDRC=0xFF;
	PORTC = 0b00000010 ;
	DDRD=0xFF;
	PORTD = _7seg(0);
	

	int num = 0 ;

    while(1)
    {

		// to check the first LDR		
		if((PINA&0b00000001)!=0)
		{
			_delay_ms(20);
			if((PINA&0b00000001)!=0)  //check again after delay
				{
				_delay_ms(100);
				if(((PINA&0b00000001)!=0)&&((PINA&0b00000010)!=0))
				{
					_delay_ms(20);
					if(((PINA&0b00000001)!=0)&&((PINA&0b00000010)!=0))  //check again after delay
					{
						_adjust(++num);
						while(((PINA&0b00000001)!=0)&&((PINA&0b00000010)!=0))
						{
							
						}
					}					
				}			
				}			
		
		}

	
	// to check the second LDR
	if((PINA&0b00000010)!=0)
	{
		_delay_ms(20);
		if((PINA&0b00000010)!=0)  //check again after delay
		{
			_delay_ms(100);
			if(((PINA&0b00000001)!=0)&&((PINA&0b00000010)!=0))
			{
				_delay_ms(20);
				if(((PINA&0b00000001)!=0)&&((PINA&0b00000010)!=0))  //check again after delay
				{
					_adjust(--num);			
					while(((PINA&0b00000001)!=0)&&((PINA&0b00000010)!=0))
					{
						
					}
				}
			}
			
		}
	}



	// to check the reset button 
	if((PINA&0b00000100)==0)
	{
	_delay_ms(20);
	if((PINA&0b00000100)==0)  //check again after delay
	{
		num = 0 ;
		_adjust(num);
	}
	
	}


	// to check the increase button
	if((PINA&0b00001000)==0)
	{
	_delay_ms(20);
	if((PINA&0b00001000)==0)  //check again after delay
	_adjust(++num);
	}


	// to check the decrease button
	if((PINA&0b00010000)==0)
	{
		_delay_ms(20);
		if((PINA&0b00010000)==0)  //check again after delay
	_adjust(--num);
	}



    }
}