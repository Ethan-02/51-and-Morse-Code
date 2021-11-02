



#include <reg52.h>
#define LCD1602_DB P0

sbit LCD1602_RS = P1^0;
sbit LCD1602_RW = P1^1;
sbit LCD1602_E  = P1^5;

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

sbit KEY_IN_1  = P2^4;
sbit KEY_IN_2  = P2^5;
sbit KEY_IN_3  = P2^6;
sbit KEY_IN_4  = P2^7;
sbit KEY_OUT_1 = P2^3;
sbit KEY_OUT_2 = P2^2;
sbit KEY_OUT_3 = P2^1;
sbit KEY_OUT_4 = P2^0;
sbit BUZZ = P1^6;

unsigned int TouchTime = 0;
unsigned int StopTime = 0;
unsigned char C0 = 0xFF;
unsigned char C1 = 0xFF;
unsigned char C2 = 0xFF;
unsigned char C3 = 0xFF;
unsigned char C4 = 0xFF;
unsigned char C5 = 0xFF;
unsigned char C6 = 0xFF;
unsigned char x = 0;
unsigned char y = 0;
unsigned char ij = 0;
unsigned char j = 0;
unsigned char CG = 0;
unsigned char cc = 0;
unsigned int cnt = 0;
unsigned int cnt2 = 0;
unsigned int ENT = 0;
unsigned char morse1;



char morse_code_J[27][6] = 
{
	{1,1,1,1,1,1,},
	{1,-1,0,0,0,0,},{-1,1,1,1,0,0,},{-1,1,-1,1,0,0,},{-1,1,1,0,0,0,},{1,0,0,0,0,0,},
	{1,1,-1,1,0,0,},{-1,-1,1,0,0,0,},{1,1,1,1,0,0,},{1,1,0,0,0,0,},{1,-1,-1,-1,0,0,},
	{-1,1,-1,0,0,0,},{1,-1,1,1,0,0,},{-1,-1,0,0,0,0,},{-1,1,0,0,0,0,},{-1,-1,-1,0,0,0,},
	{1,-1,-1,1,0,0,},{-1,-1,1,-1,0,0,},{1,-1,1,0,0,0,},{1,1,1,0,0,0,},{-1,0,0,0,0,0,},
	{1,1,-1,0,0,0,},{1,1,1,-1,0,0,},{1,-1,-1,0,0,0,},{-1,1,1,-1,0,0,},{-1,1,-1,-1,0,0,},
	{-1,-1,1,1,0,0,},
};
unsigned char morse_code_Z[] = " abcdefghijklmnopqrstuvwxyz";
char morse[6] = {0,0,0,0,0,0,};
unsigned char morse_code[16] = "                ";


void LedDriver(unsigned char a, unsigned char b);
void LcdDriver(unsigned char *str);
void CleanAll();

void main()
{
	unsigned char i = 0;
	unsigned char Wei = 0;

	
	
	KEY_OUT_1 = 0;
	KEY_OUT_4 = 0;
	//ENLED = 0;    
  //ADDR3 = 0;    
	
	
	EA = 1;   
	TMOD = 0x01;  
	TH0  = 0xFC;  
	TL0  = 0x67;
	ET0  = 1;     
	TR0  = 1;     
  
	while(1)
	{
		if(ENT == 1)
		{
			LcdDriver(morse_code);
		}
		else
		{	if(StopTime >= 500)
			{
				for(i = 0; i < 27; i++)
				{
					ij = 0;
					for(j = 0; j < 6; j++)
					{
						if(morse[j] == morse_code_J[i][j])
						{
							ij++;
						}
						/*if(i == 27 && j == 6 && ij != 6)//´íÎó´úÂë
						{
							StopTime = 0;
							CG = 0;
							cnt2 = 0;
							for(i = 0; i <= 6; i++)
							{
								morse[i]=0;
							}
						}*/
						if(ij == 6)
						{	
							morse_code[Wei++] = morse_code_Z[i];
							//morse_code[0] = morse_code_Z[i];
							//LcdDriver(morse_code);//lcdÇý¶¯
							StopTime = 0;
							CG = 0;
							cnt2 = 0;
							cc = 0;
							P0 = 0xFF;
							C1 = 0xFF;
							C2 = 0xFF;
							C3 = 0xFF;
							C4 = 0xFF;
							C5 = 0xFF;
							C6 = 0xFF;
							morse1 = 0xFF;
							/*if(Wei == 16)
							{
								unsigned char morse_code[16] = "";
							}*/
							for(i = 0; i < 6; i++)
							{
								morse[i]=0;
							}
						}
					}
				}
			}
			
			if(TouchTime > 0 && TouchTime <= 200)
			{
				morse[CG]= 1;
				TouchTime = 0;
				cnt = 0;
				LedDriver(1, CG);
				CG++;
				cnt2 = 0;
				StopTime = 0;

			}
			else if(TouchTime > 200)
			{
				morse[CG] = -1;
				TouchTime = 0;
				cnt = 0;
				LedDriver(-1, CG);
				CG++;
				cnt2 = 0;
				StopTime = 0;
			}
		}
	}
	
	
}

void InterruptTimer0() interrupt 1
{
	static unsigned char i = 0;
	static unsigned char keybuf[3] = {0xFF, 0xFF, 0xFF, };
	
	TH0 = 0xFC; 
	TL0 = 0x67;
	
	keybuf[0] = (keybuf[0] << 1) | KEY_IN_4;
	keybuf[1] = (keybuf[1] << 1) | KEY_IN_3;
	if ((keybuf[0] & 0x0F) == 0x00)
	{  
		cnt++;
		cc = 1;
		
		//BUZZ = ~BUZZ;
	}
	else if ((keybuf[0] & 0x0F) == 0x0F && cc != 0)
	{
		TouchTime = cnt;
		cnt2++;
		StopTime = cnt2;
		
	}
	if ((keybuf[1] & 0x0F) == 0x00)
		{  
			//KeySta[i] = 0;
			ENT = 1;
		}
		else if ((keybuf[1] & 0x0F) == 0x0F)
		{   
			//KeySta[i] = 1;
			ENT = 0;
		}
	
	
	P0 = 0xFF;   
	switch (i)
	{
			case 0: ADDR2=0; ADDR1=0; ADDR0=0; i++; P0=C0; break;
			case 1: ADDR2=0; ADDR1=0; ADDR0=1; i++; P0=C1; break;
			case 2: ADDR2=0; ADDR1=1; ADDR0=0; i++; P0=C2; break;
			case 3: ADDR2=0; ADDR1=1; ADDR0=1; i++; P0=C3; break;
			case 4: ADDR2=1; ADDR1=0; ADDR0=0; i++; P0=C4; break;
			case 5: ADDR2=1; ADDR1=0; ADDR0=1; i++; P0=C5; break;
			case 6: ADDR2=1; ADDR1=1; ADDR0=0; i++; P0=0xFF; break;
			case 7: ADDR2=1; ADDR1=1; ADDR0=1; i=0; P0=0xFF; break;
			default: break;
	}
		
}


void LedDriver( char a, unsigned char b)
{
	//unsigned char morse1;
	ENLED = 0;
	ADDR3 = 0;
	if(a == 1)
	{
		morse1 = 0xfe;
	}
	else if(a == -1)
	{
		morse1 = 0xf8;
	}
	switch(b)
	{
		case 0: C0 = morse1;break;
		case 1: C1 = morse1;break;
		case 2: C2 = morse1;break;
		case 3: C3 = morse1;break;
		case 4: C4 = morse1;break;
		case 5:	C5 = morse1;break;
		default: break;
	}
}




void LcdWaitReady()
{
    unsigned char sta;
    
    LCD1602_DB = 0xFF;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do {
        LCD1602_E = 1;
        sta = LCD1602_DB; 
        LCD1602_E = 0;
    } while (sta & 0x80); 
} 
void LcdWriteDat(unsigned char dat)
{
    LcdWaitReady();
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    LCD1602_DB = dat;
    LCD1602_E  = 1;
    LCD1602_E  = 0;
}
void LcdWriteCmd(unsigned char cmd)
{
    LcdWaitReady();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DB = cmd;
    LCD1602_E  = 1;
    LCD1602_E  = 0;
}

void LcdSetCursor(unsigned char x, unsigned char y)
{
    unsigned char addr;
    
    if (y == 0)  
        addr = 0x00 + x;  
    else
        addr = 0x40 + x;  
    LcdWriteCmd(addr | 0x80); 
}
void LcdShowStr(unsigned char *str)
{
		/*
		if(str == "\n")
		{
			y++;
		}
		*/
    //LcdSetCursor(x, y);   
		LcdSetCursor(0, 0);
		while(*str != '\0')  
    {
      LcdWriteDat(*str++);  
    }
		/*if(str != "\n")
		{
			x++;
		}*/
}
void InitLcd1602()
{
    LcdWriteCmd(0x38);  
    LcdWriteCmd(0x0C);  
    LcdWriteCmd(0x06);  
    LcdWriteCmd(0x01);  
}

void LcdDriver(unsigned char *str)
{
	ENLED = 1;
	ADDR3 = 1;
	InitLcd1602();
	LcdShowStr(str);
}





/*void CleanAll()
{
	C1 = 0xFF;
	C2 = 0xFF;
	C3 = 0xFF;
	C4 = 0xFF;
	C5 = 0xFF;
	TouchTime = 0;
	StopTime = 0;
	CengGao = 0;
	keybuf[3] = {0xFF, 0xFF, 0xFF, };
	cnt = 0;
	cnt2 = 0;
	x = 0;
	y = 0;
	ij = 0;
}*/
