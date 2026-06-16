#include<LPC21XX.H>
#define lcd_d 0xff<<2
#define e 1<<10
#define rs 1<<11
#define LED0 1<<17
#define MOTOR_PINS ((1<<12)|(1<<13)|(1<<14)|(1<<15))

volatile unsigned int start = 0;

void LCD_INIT(void);
void LCD_COMMAND(unsigned char );
void LCD_DATA(unsigned char );
void delay_milliseconds(unsigned int );
void LCD_STR(unsigned char *);
void UART0_INIT(void);
void UART0_TX(unsigned char );
unsigned char UART0_RX(void);
void UART0_STR(unsigned char *);
void MOTOR_INIT(void);
void FORWARD(void);
void BACKWARD(void);
void RIGHT(void);
void LEFT(void);



void LCD_INIT(void)
{
	IODIR0|=lcd_d|rs|e;
	LCD_COMMAND(0X01);//TO CLEAR THE DISPLAY
	LCD_COMMAND(0X02);//SET THE CURSOR TO HOME POSITION
	LCD_COMMAND(0X0C);//DISPLAY ON, CURSOR OFF
	LCD_COMMAND(0X38);//8 BIT INTERFACE MODE WITH BOTH LINES
}
void LCD_COMMAND(unsigned char cmd)
{
	IOCLR0=lcd_d;
	IOSET0=cmd<<2;
	IOCLR0=rs;
	IOSET0=e;
	delay_milliseconds(2);
	IOCLR0=e;
}
void LCD_DATA(unsigned char d)
{
	IOCLR0=lcd_d;
	IOSET0=d<<2;
	IOSET0=rs;
	IOSET0=e;
	delay_milliseconds(2);
	IOCLR0=e;
}

void delay_milliseconds(unsigned int n)
{
//	T0TCR=0X02;
	T0PR=15000-1;
	T0TCR=0X01;
	while(T0TC<n);
	T0TCR=0X03;
	T0TCR=0X00;
}

void LCD_STR(unsigned char *p)
{
	while(*p)
	{
		LCD_DATA(*p++);
	}
}


void UART0_INIT(void)
{
	PINSEL0|=5;
	U0LCR=0X83;//DLAB IS ENABLED TO SET THE BAUD RATE AND CHOOSE 8NI FORMAT
	U0DLL=97;//BAUD RATE IS SET AS 9600
	U0LCR=0X03;//DLAB IS DISABLED AND BUFFER IS ENABLED
}

void UART0_TX(unsigned char ch)
{
	while(((U0LSR>>5)&1)==0);//IF THRE IS 0, THEN IT IS HOLDING DATA
	U0THR=ch;
}

unsigned char UART0_RX(void)
{
	while((U0LSR&1)==0);
		return U0RBR;
}

void UART0_STR(unsigned char *s)
{
	while(*s)
		UART0_TX(*s++);
}

void MOTOR_INIT()
{
	IODIR0|=MOTOR_PINS;
	IOCLR0  = MOTOR_PINS;
}

void FORWARD(void)
{
	IOCLR0=MOTOR_PINS;
	IOSET0 |= 1<<12|1<<14;
	delay_milliseconds(500);
	IOCLR0=MOTOR_PINS;
}

void BACKWARD(void)
{
	IOCLR0=MOTOR_PINS;
	IOSET0 |= 1<<13|1<<15;
	delay_milliseconds(500);
	IOCLR0=MOTOR_PINS;
}

void RIGHT(void)
{
	IOCLR0=MOTOR_PINS;
	IOSET0 |= 1<<12|1<<15;
	delay_milliseconds(500);
	IOCLR0=MOTOR_PINS;
}

void LEFT(void)
{
	IOCLR0=MOTOR_PINS;
	IOSET0 |= 1<<13|1<<14;
	delay_milliseconds(500);
	IOCLR0=MOTOR_PINS;
}

void ext_int0_isr(void) __irq
{
    EXTINT = 0x01;      

    start ^= 1; 
	  if(start)
        IOSET0 = LED0;
    else
        IOCLR0 = LED0;

    VICVectAddr = 0;    
}


int main()

{
	unsigned char a[2],i;
	PINSEL1 = 0X1;//P0.16 as EINT0 pin
  IODIR0 |= LED0;
  VICIntSelect = 0;//All as IRQs
  /*Allocate slot-0 to EINT0 interrupt source*/
	VICVectCntl0 = (1<<5)|14;
  VICVectAddr0 = (unsigned long)ext_int0_isr;
  /*******************************************/

  /*select the triggering mode for EINT0*/
	 EXTMODE  = 0X00;
	 EXTPOLAR = 0X00;
  /**************************************/

	VICIntEnable = 1<<14;//Enable EINT0 interrupt

	MOTOR_INIT();
	UART0_INIT();
	LCD_INIT();
	LCD_STR("          SMARTBOT by KIRRAN VIGNESH");
	for(i=0;i<36;i++)
	{
		LCD_COMMAND(0X18);
		delay_milliseconds(150);
	}

	while(1)
	{
	unsigned int i=0;
	while(1)
	{
		
		a[i] = UART0_RX();
		//UART0_TX(a[i]);
		if(a[i]=='S')
		{
			a[i]='\0';
			break;
		}
		i++;
		
		
	}
	
	if(a[0]=='F')
	{
		LCD_COMMAND(0X01);
		LCD_STR("FORWARD");
		FORWARD();
	}
	else if(a[0]=='B')
	{
		LCD_COMMAND(0X01);
		LCD_STR("BACKWARD");
		BACKWARD();
	}
	else if(a[0]=='R')
	{
		LCD_COMMAND(0X01);
		LCD_STR("RIGHT");
		RIGHT();
	}
	else if(a[0]=='L')
	{
		LCD_COMMAND(0X01);
		LCD_STR("LEFT");
		LEFT();
	}
	else
	{
		LCD_COMMAND(0X01);
		LCD_STR("HELLO");
	}

}


}