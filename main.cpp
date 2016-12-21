#include "my_lib\fastport.h"
#include "my_lib\pinsel.h"
#include "my_lib\vic.h"
#include "my_lib\uart.h"
#include "my_lib\flash.h"
#define $dir485(do)   P2[do]|=1<<2
#define $DBG_PORT(do) P0[do]|=1<<31
#define wait 3
#define Clk_CPU 76800
#define CONTACT_CHANNEL1 48
#define CONTACT_CHANNEL2 47
#define CONTACT_CHANNEL3 46
#define CONTACT_CHANNEL4 16
#define CONTACT_CHANNEL5 19
#define CONTACT_CHANNEL6 61
#define CONTACT_CHANNEL7 60
#define CONTACT_CHANNEL8 58
#define CONTACT_CHANNEL9 42
#define CONTACT_CHANNEL10 43
#define CONTACT_CHANNEL11 38
#define CONTACT_CHANNEL12 40
#define CONTACT_CHANNEL13 41
#define CONTACT_POROG 512
#define DELAY(num) {	  \
for (i=0;i<num;i++); \
}	   \

const unsigned char Crc8Table[256] = {
    0x00, 0x31, 0x62, 0x53, 0xC4, 0xF5, 0xA6, 0x97,
    0xB9, 0x88, 0xDB, 0xEA, 0x7D, 0x4C, 0x1F, 0x2E,
    0x43, 0x72, 0x21, 0x10, 0x87, 0xB6, 0xE5, 0xD4,
    0xFA, 0xCB, 0x98, 0xA9, 0x3E, 0x0F, 0x5C, 0x6D,
    0x86, 0xB7, 0xE4, 0xD5, 0x42, 0x73, 0x20, 0x11,
    0x3F, 0x0E, 0x5D, 0x6C, 0xFB, 0xCA, 0x99, 0xA8,
    0xC5, 0xF4, 0xA7, 0x96, 0x01, 0x30, 0x63, 0x52,
    0x7C, 0x4D, 0x1E, 0x2F, 0xB8, 0x89, 0xDA, 0xEB,
    0x3D, 0x0C, 0x5F, 0x6E, 0xF9, 0xC8, 0x9B, 0xAA,
    0x84, 0xB5, 0xE6, 0xD7, 0x40, 0x71, 0x22, 0x13,
    0x7E, 0x4F, 0x1C, 0x2D, 0xBA, 0x8B, 0xD8, 0xE9,
    0xC7, 0xF6, 0xA5, 0x94, 0x03, 0x32, 0x61, 0x50,
    0xBB, 0x8A, 0xD9, 0xE8, 0x7F, 0x4E, 0x1D, 0x2C,
    0x02, 0x33, 0x60, 0x51, 0xC6, 0xF7, 0xA4, 0x95,
    0xF8, 0xC9, 0x9A, 0xAB, 0x3C, 0x0D, 0x5E, 0x6F,
    0x41, 0x70, 0x23, 0x12, 0x85, 0xB4, 0xE7, 0xD6,
    0x7A, 0x4B, 0x18, 0x29, 0xBE, 0x8F, 0xDC, 0xED,
    0xC3, 0xF2, 0xA1, 0x90, 0x07, 0x36, 0x65, 0x54,
    0x39, 0x08, 0x5B, 0x6A, 0xFD, 0xCC, 0x9F, 0xAE,
    0x80, 0xB1, 0xE2, 0xD3, 0x44, 0x75, 0x26, 0x17,
    0xFC, 0xCD, 0x9E, 0xAF, 0x38, 0x09, 0x5A, 0x6B,
    0x45, 0x74, 0x27, 0x16, 0x81, 0xB0, 0xE3, 0xD2,
    0xBF, 0x8E, 0xDD, 0xEC, 0x7B, 0x4A, 0x19, 0x28,
    0x06, 0x37, 0x64, 0x55, 0xC2, 0xF3, 0xA0, 0x91,
    0x47, 0x76, 0x25, 0x14, 0x83, 0xB2, 0xE1, 0xD0,
    0xFE, 0xCF, 0x9C, 0xAD, 0x3A, 0x0B, 0x58, 0x69,
    0x04, 0x35, 0x66, 0x57, 0xC0, 0xF1, 0xA2, 0x93,
    0xBD, 0x8C, 0xDF, 0xEE, 0x79, 0x48, 0x1B, 0x2A,
    0xC1, 0xF0, 0xA3, 0x92, 0x05, 0x34, 0x67, 0x56,
    0x78, 0x49, 0x1A, 0x2B, 0xBC, 0x8D, 0xDE, 0xEF,
    0x82, 0xB3, 0xE0, 0xD1, 0x46, 0x77, 0x24, 0x15,
    0x3B, 0x0A, 0x59, 0x68, 0xFF, 0xCE, 0x9D, 0xAC
};			   

const unsigned int MX_P1_CACSH[256] = 
{
 0X04408110,0X04488110,0X04508110,0X04588110,0X04608110,0X04688110,0X04708110,0X04788110,
 0X04808010,0X04888010,0X04908010,0X04988010,0X04A08010,0X04A88010,0X04B08010,0X04B88010,									 
 0X24008100,0X24088100,0X24108100,0X24188100,0X24208100,0X24288100,0X24308100,0X24388100,
 0X04010001,0X04090001,0X04110001,0X04190001,0X04210001,0X04290001,0X06008012,0X06088012,
 0X06108012,0X06188012,0X06208012,0X06288012,0X06308012,0X06388012,0X05008002,0x05088002,
 0x05108002,0x05188002,0x05208002,0x05288002,0x05308002,0x05388002,0x04208400,0x04288400,
 0x04308400,0x04388400,0x04048112,0x040C8112,0x04148112,0x041C8112,0x04248112,0x042C8112,
 0x04348112,0x043C8112,0x0400C102,0x0408C102,0x0410C102,0x0418C102,0x0428C102,0x0430C102,
 0x44010100,0x44090100,0x44110100,0x44190100,0x44210100,0x44290100,0x44310100,0x44390100,
 0x84010102,0x84090102,0x84110102,0x84190102,0x84210102,0x84290102,0x84310102,0x84390102,
 0x04010110,0x04010012,0x04010010,0x04000000,0X04390001,0X04310001,0x04088400,0x04108400,
 0x04188400,0x0438C102,0x04310202,0x04390202,0x04290202,0x04210202,0x04008400,0x0420C102,
 0x04190202,0x04110202,0x04090202,0x04010202
};	
 
int paramIndex = 0;
void UART1Handler() __irq;
int index=0,i=0,j=0; 
unsigned int Y=0,Y2=0,Y_,Y2_,X2=0,count2=0,state=0;
bool SYNC=true;
bool OUT=true;
bool Mode_32Khz=false;
byte QUERY[40],Z=0,Z2=0,Contact=0,Contact_old=0,Contact2=0,X3_save=0,X4_save=0,buf=0,sc=0;
uint int_cnt=0;
short X=0;
FastPort P0;
FastPort P2;
UART* uart1,*uart0;
int FAST_PARAMETRS_BSUFTK[96],FAST_PARAMETRS_ULK[96];
int NUMBER_FAST_PARAMETRS;
int MODE=255;
unsigned char FastNumbers[22];
unsigned char fast_nums[100];
unsigned char CH_VAL = 0;
byte Query2[14],buff[30];
int main(){
	int count = 0;
	SCS|=1;					  //Razreshaem fast porti
	PREPARE_UART
	VIC::Init();
	
	P0=GetFastPort(PORT_0);
	P2=GetFastPort(PORT_2);

	P0[DIR]|=1<<12;			  //konfiguriruem miso kak vihod
	P2[DIR]|=1<<2;			  //konfiguriruem dir485 kak vihod processora

	$dir485(CLR);
	//-------UART---------//
	const UARTOption u_opt={0,0,1,3,7 | 3<<6};
	uart1=UART::Get(1);       //uart1
	uart1->Init(&u_opt);
	
	uart0 = UART::Get(0);

	const UARTOption u_opt0={42,0,1,3,7 | 1<<6};		  
	uart0->Init(&u_opt0);
	PINSEL::Set(0,2,func_01);	

	byte data[80],test[4],X_save=0,X2_save=0;
	for(uint i=0;i<80;i++){
		data[i]=0;
	};
	PINSEL::Set(2,0,func_10);		  //TX485
	PINSEL::Set(2,1,func_10);		  //RX485 

	FIO0DIR |= 1<<31;				  //Nogi dlya otladki
	FIO4DIR |= 1<<4;
	
	FIO1DIR = 0xF7FDC713;				  //Konfiguriruem kak vihodi processora

	SCS=(1<<5) | 1;						  //Vibiraem oscillator i razreshaem fast porti
	FIO2DIR |= (1<<10);					  //BSL kak vihod

	PCONP |= (1 << 12);				//Podaem sinhrosignal na acp
	AD0CR |= (1<<21)|(1);			//Vkluchaem acp	 10 bit		 
	AD0CR|=(1<<16)|(1<<19);			//�������� ���������� ����� ������
	AD0CR |= (0x10<<8);				//Delitel chastoti dlya ACP

  	PINSEL1 |= 1<<14;			   // Konfiguriruem AN kak vhod s ACP

	FIO0DIR |= (1<<14) | (1<<7) | (1<<13);			  //SSEL, SCK i MOSI kak vihodi			  
	PINSEL0 |= (1<<25) | (1<<15);	  

	VIC::InstallIRQ(VIC::UART1_INT,(void*)UART1Handler,0);
	FIO1PIN = MX_P1_CACSH[0];		   

	Contact=0;
	Y=0;Y_=0;

	FIO1PIN = 0;
	FIO1PIN |= 1<<26;
	
	FAST_PARAMETRS_BSUFTK[0]=MX_P1_CACSH[84]; 			//IN72
	FAST_PARAMETRS_BSUFTK[1]=MX_P1_CACSH[99];			//IN96
	FAST_PARAMETRS_BSUFTK[2]=MX_P1_CACSH[51];			//IN75
	FAST_PARAMETRS_BSUFTK[3]=MX_P1_CACSH[96];			//IN93
	FAST_PARAMETRS_BSUFTK[4]=MX_P1_CACSH[93];			//IN90
	FAST_PARAMETRS_BSUFTK[5]=MX_P1_CACSH[90];			//IN88
	FAST_PARAMETRS_BSUFTK[6]=MX_P1_CACSH[97];			//IN94
	FAST_PARAMETRS_BSUFTK[7]=MX_P1_CACSH[94];			//IN91
	FAST_PARAMETRS_BSUFTK[8]=MX_P1_CACSH[98];			//IN95
	FAST_PARAMETRS_BSUFTK[9]=MX_P1_CACSH[95];			//IN92
	FAST_PARAMETRS_BSUFTK[10]=MX_P1_CACSH[89];			//IN44
	FAST_PARAMETRS_BSUFTK[11]=MX_P1_CACSH[88];			//IN29
	FAST_PARAMETRS_BSUFTK[12]=MX_P1_CACSH[87];			//IN30
	FAST_PARAMETRS_BSUFTK[13]=MX_P1_CACSH[86];			//was IN78	now IN31 = 8-2_4
	FAST_PARAMETRS_BSUFTK[14]=MX_P1_CACSH[91];			//was IN79	now IN71 = 8-2_5
	FAST_PARAMETRS_BSUFTK[15]=MX_P1_CACSH[85];			//was IN80	now IN73 = 8-2_8
	
	NUMBER_FAST_PARAMETRS=3;
	FAST_PARAMETRS_ULK[0]=MX_P1_CACSH[89]; 			
	FAST_PARAMETRS_ULK[1]=MX_P1_CACSH[88];			
	FAST_PARAMETRS_ULK[2]=MX_P1_CACSH[87];	

	WDCLKSEL=1;                     //������������ WatchDog �� �������� �����������
	WDTC=6000;                     	//����� ����� 312,5 �����������
	WDMOD=3;                       	//����� ���������� ��� ���������� �������� 0
	WDFEED = 0xAA;					
    WDFEED = 0x55;

//******************************************************************************************
//						  Rabota v kachestve yacheiki YALK								   
//******************************************************************************************
	while (1)
	{
	if (MODE==1)									   
	{	   
		while(SYNC)
		{
			if (Mode_32Khz)
			{
				FIO1PIN = MX_P1_CACSH[QUERY[1]];
				AD0CR=0x00211001;						//������������� 10-������ ����� ���
				Mode_32Khz=false;		
				$dir485(SET);
				uart1->Send(data,1);
				DELAY(15);
				while (!(AD0DR0&(1<<31))) ;				//Ozhidaem okonchaniya preobrazovaniya	
  				data[0]=(AD0DR0>>8);
				for (i=0;i<5;i++) ;
				$dir485(CLR);
				WDFEED = 0xAA;					                //����� WatchDog �������
				WDFEED = 0x55;
			}
		}
	
/*************************** Opros medlennix parametrov *************************************/ 									 			
		while (!(AD0DR0&(1<<31))) ;				//Ozhidaem okonchaniya preobrazovaniya
		X2=(AD0DR0>>7)&0x3FF;
						
/*************************** Opros bistrih parametrov *************************************/ 	 	 							   		
		FIO1PIN = FAST_PARAMETRS_BSUFTK[0];
		AD0CR=0x00291001;					//������������� 6-������ ����� ���
		$dir485(SET);		  				//������������� ���� DIR RS-485
		SYNC=true;
		U1THR=data[0];						
		DELAY(15);		
		U1THR=data[1];					
		while (!(AD0DR0&(1<<31))) ;			//Ozhidaem okonchaniya preobrazovaniya
		data[0]=(AD0DR0>>10);		

		FIO1PIN = FAST_PARAMETRS_BSUFTK[1];
		U1THR=data[2];	
		DELAY(15);		
		U1THR=data[3];			
		while (!(AD0DR0&(1<<31))) ;			//Ozhidaem okonchaniya preobrazovaniya	
		data[1]=(AD0DR0>>10);

		FIO1PIN = FAST_PARAMETRS_BSUFTK[2];
		U1THR=data[4];
		DELAY(15);			
		U1THR=data[5];				
		while (!(AD0DR0&(1<<31))) ;			//Ozhidaem okonchaniya preobrazovaniya	
		data[2]=(AD0DR0>>10);

		FIO1PIN = FAST_PARAMETRS_BSUFTK[3];
		U1THR=data[6];	
		DELAY(15);						
		U1THR=data[7];
		while (!(AD0DR0&(1<<31))) ;			//Ozhidaem okonchaniya preobrazovaniya	
		data[3]=(AD0DR0>>10);	

		FIO1PIN = FAST_PARAMETRS_BSUFTK[4];
		U1THR=data[8];	
		DELAY(15);		
		U1THR=data[9];			
		while (!(AD0DR0&(1<<31))) ;			//Ozhidaem okonchaniya preobrazovaniya	
		data[4]=(AD0DR0>>10);

		FIO1PIN = FAST_PARAMETRS_BSUFTK[5];
		U1THR=data[10];
		DELAY(15);		
		U1THR=data[11];				
		while (!(AD0DR0&(1<<31))) ;			//Ozhidaem okonchaniya preobrazovaniya	
		data[5]=(AD0DR0>>10);

		FIO1PIN = FAST_PARAMETRS_BSUFTK[6];
		U1THR=data[12];	
		DELAY(15);		
		U1THR=data[13];					
		while (!(AD0DR0&(1<<31))) ;			//Ozhidaem okonchaniya preobrazovaniya	
		data[6]=(AD0DR0>>10);	

		FIO1PIN = FAST_PARAMETRS_BSUFTK[7];
		DELAY(15);			
		$dir485(CLR);			
		while (!(AD0DR0&(1<<31))) ;			//Ozhidaem okonchaniya preobrazovaniya	
		data[7]=(AD0DR0>>10);

		FIO1PIN = FAST_PARAMETRS_BSUFTK[8];	
		DELAY(15);
		i=0;								//�������������� ��������
		while (!(AD0DR0&(1<<31))) ;			//Ozhidaem okonchaniya preobrazovaniya	
		data[8]=(AD0DR0>>10);

		FIO1PIN = FAST_PARAMETRS_BSUFTK[9];	
		DELAY(15);
		i=0;								//�������������� ��������						
		while (!(AD0DR0&(1<<31))) ;			//Ozhidaem okonchaniya preobrazovaniya	
		data[9]=(AD0DR0>>10);

		FIO1PIN = FAST_PARAMETRS_BSUFTK[10];	
		DELAY(17);				
		while (!(AD0DR0&(1<<31))) ;			//Ozhidaem okonchaniya preobrazovaniya
		data[10]=(AD0DR0>>10);	

		FIO1PIN = FAST_PARAMETRS_BSUFTK[11];					
		DELAY(15);	
		i=0;								//�������������� ��������					
		while (!(AD0DR0&(1<<31))) ;			//Ozhidaem okonchaniya preobrazovaniya		
		data[11]=(AD0DR0>>10); 	 

		FIO1PIN = FAST_PARAMETRS_BSUFTK[12];
		DELAY(17);	
		while (!(AD0DR0&(1<<31))) ;			//Ozhidaem okonchaniya preobrazovaniya	
		data[0]|=(AD0DR0>>4)&0xC0;
		data[1]|=(AD0DR0>>6)&0xC0;
		data[2]|=(AD0DR0>>8)&0xC0;

		FIO1PIN = FAST_PARAMETRS_BSUFTK[13];
		DELAY(15);	
		i=0;								//�������������� ��������
		while (!(AD0DR0&(1<<31))) ;			//Ozhidaem okonchaniya preobrazovaniya	
		data[3]|=(AD0DR0>>4)&0xC0;
		data[4]|=(AD0DR0>>6)&0xC0;
		data[5]|=(AD0DR0>>8)&0xC0;
											   
		FIO1PIN = FAST_PARAMETRS_BSUFTK[14];
		DELAY(17);	
		while (!(AD0DR0&(1<<31))) ;			//Ozhidaem okonchaniya preobrazovaniya	
		data[6]|=(AD0DR0>>4)&0xC0;
		data[7]|=(AD0DR0>>6)&0xC0;
		data[8]|=(AD0DR0>>8)&0xC0;

		FIO1PIN = FAST_PARAMETRS_BSUFTK[15];
		DELAY(17);	
		while (!(AD0DR0&(1<<31))) ;			//Ozhidaem okonchaniya preobrazovaniya	
		data[9]|=(AD0DR0>>4)&0xC0;
		data[10]|=(AD0DR0>>6)&0xC0;
		data[11]|=(AD0DR0>>8)&0xC0; 		   	   

 /*************************** Opros kontaktnih parametrov *********************************/
   
		FIO1PIN = MX_P1_CACSH[QUERY[2]];
		AD0CR=0x00211001;						//������������� 10-������ ����� ���
		for(i=0;i<3;i++) ;
		FIO1SET |= 1<<28;
		for(i=0;i<10;i++) ;	
		if ((FIO3PIN & 4)!=0) Contact=1; else Contact=0;	
		FIO1CLR |= 1<<28;		 
					
/*********************��������� ������ ��� ��������� ����������****************************/	
//if ((QUERY[1]!=78)&&(QUERY[1]!=79)&&(QUERY[1]!=72)&&(QUERY[1]!=73)) FIO1CLR |= 1<<26;
		for(i=0;i<6;i++);
		FIO1SET |= 1<<26;
		FIO1PIN = MX_P1_CACSH[QUERY[1]];
		
		if (Contact_old==1) X2|=0x200;
		Contact_old=Contact;
		data[12] = X2;
	   	data[13] = (X2 >> 8);

		WDFEED = 0xAA;					                //����� WatchDog �������
		WDFEED = 0x55;
			
		FIO1CLR |= 1<<26;
		FIO1SET |= 1<<26;	  														 			  									   				  																   		
	}

//*****************************************************************************************
// 						   Rabota v kachestve yacheiki ULK
//*****************************************************************************************

	else if (MODE==0)
	{
		while(SYNC)
		{
			if (Mode_32Khz)
			{
				FIO1PIN = MX_P1_CACSH[QUERY[1]];
				AD0CR=0x00211001;						//������������� 10-������ ����� ���
				Mode_32Khz=false;		
				$dir485(SET);
				uart1->Send(data,1);
				DELAY(15);
				while (!(AD0DR0&(1<<31))) ;				//Ozhidaem okonchaniya preobrazovaniya	
  				data[0]=(AD0DR0>>8);
				for (i=0;i<5;i++) ;
				$dir485(CLR);
				WDFEED = 0xAA;					                //����� WatchDog �������
				WDFEED = 0x55;
			}
		}						  
		
		SYNC=true;		
		if (OUT)
		{
			$dir485(SET);
			uart1->Send(data,14);
		}		  				 					
	
/*************************** Opros medlennix parametrov *************************************/							 			
		while (!(AD0DR0&(1<<31))) ;				//Ozhidaem okonchaniya preobrazovaniya
		X2=(AD0DR0>>7)&0x3FF;		
				
/*************************** Opros bistrih parametrov *************************************/
		j=0;
		AD0CR=0x00291001;					//������������� 6-������ ����� ���
		for (index=0;((index<NUMBER_FAST_PARAMETRS)&&(index<6));index++)
		{	 	 							   
			FIO1PIN = FAST_PARAMETRS_ULK[index];
			DELAY(25);
			while (!(AD0DR0&(1<<31))) ;				//Ozhidaem okonchaniya preobrazovaniya	
  			if (OUT) data[j]=(AD0DR0>>10);
				else data[j+1]=(AD0DR0>>10);   
			j=j+2;			 							  
		} 

		$dir485(CLR);
		
		if (OUT)
		{ 		 	  	
 /*************************** Opros kontaktnih parametrov *********************************/
   
			if ((QUERY[2]==CONTACT_CHANNEL1) || (QUERY[2]==CONTACT_CHANNEL2) || (QUERY[2]==CONTACT_CHANNEL3) || (QUERY[2]==CONTACT_CHANNEL4) || (QUERY[2]==CONTACT_CHANNEL5) || (QUERY[2]==CONTACT_CHANNEL6) || (QUERY[2]==CONTACT_CHANNEL7) 
			|| (QUERY[2]==CONTACT_CHANNEL8) || (QUERY[2]==CONTACT_CHANNEL9) || (QUERY[2]==CONTACT_CHANNEL10) || (QUERY[2]==CONTACT_CHANNEL11) || (QUERY[2]==CONTACT_CHANNEL12) || (QUERY[2]==CONTACT_CHANNEL13))
			{
				AD0CR=0x00211001;						//������������� 10-������ ����� ���
				FIO1CLR |= 1<<26;
				for(i=0;i<6;i++) ;
				FIO1SET |= 1<<26;
				FIO1PIN = MX_P1_CACSH[QUERY[2]];
				for(i=0;i<3;i++) ;	
				FIO1CLR |= 1<<26;
				FIO1SET |= 1<<26;	
				for(i=0;i<40;i++) ;
				while (!(AD0DR0&(1<<31))) ;				//Ozhidaem okonchaniya preobrazovaniya	
				i=((AD0DR0>>6)&0x3FF);
				if (i>=CONTACT_POROG) Contact=1;
					else Contact=0;	
			}
            else                                                                                               
			{
				FIO1PIN = MX_P1_CACSH[QUERY[2]];	
				AD0CR=0x00211001;						//������������� 10-������ ����� ���
				for(i=0;i<3;i++) ;
				FIO1SET |= 1<<28;
				for(i=0;i<10;i++) ;	  	 
				if ((FIO3PIN & 4)!=0) Contact=1; else Contact=0;	
				FIO1CLR |= 1<<28;
			}
					
/*********************��������� ������ ��� ��������� ����������****************************/	
			FIO1CLR |= 1<<26;
			for(i=0;i<6;i++) ;
			FIO1SET |= 1<<26;
			FIO1PIN = MX_P1_CACSH[QUERY[1]];
			for(i=0;i<3;i++) ;	
			FIO1CLR |= 1<<26;
			FIO1SET |= 1<<26;	  														 
		}

		if (Contact==1) X2|=0x200;
		data[12] = X2;
	   	data[13] = (X2 >> 8);  

		WDFEED = 0xAA;					                //����� WatchDog �������
		WDFEED = 0x55;																																										
	}
	}															
	return 0;
}

unsigned char Crc8(unsigned char *pcBlock, unsigned char len)
{
    unsigned char crc = 0xFF;
 
    while (len--)
        crc = Crc8Table[crc ^ *pcBlock++];
 
    return crc;
}

void UART1Handler() __irq{					  //Obrabotchik prerivaniya UART

	if ((U1IIR & 0xE) == 0xC) 				  //prerivanie po taim-autu
	{				
		QUERY[0]=U1RBR;						  //������ ������ �� UART
		QUERY[1]=U1RBR;
		QUERY[2]=U1RBR;
		QUERY[3]=U1RBR;
		switch (QUERY[0])
		{
			case 42: 						  //����� ��� � ������� ��� ���
				if (Crc8(QUERY,3)==QUERY[3])  
				{
					SYNC=false;
					OUT=true;
					MODE=1;
				}
				break;
			case 43: 						  //����� ��� � ������� ���
				if (Crc8(QUERY,3)==QUERY[3])  
				{
					SYNC=false;
					OUT=true;
					MODE=0;
				}
				break;
			case 45:					  	  	//��������� ��������� ����� 1 ������, ����� �������� ����� � ��������� ������
				if (Crc8(QUERY,3)==QUERY[3]) Mode_32Khz=true;
				MODE=0;
				break;
			default:
				if (Crc8(QUERY,3)==QUERY[3]) 
				{
					OUT=false;	 
				 	SYNC=false;
				}
				break;
		}
	}
		else uart1->Read(Query2,14);			//Prerivanie po prinyatiyu 14 bait		
		 
	VICVectAddr=0;
	}