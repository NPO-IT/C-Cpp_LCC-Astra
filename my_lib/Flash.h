//=================================================================================================
//
//	5.08.2011			(�������� ��������� FLASH)
//
//=================================================================================================
//

#define CMD_SUCCESS 0
#define TRUE 1
#define FALSE 0

// ��������� ����� ������� ��� �������� �������

#define SECTOR_STARTADDR 0x00007000

//	����� �������.										   
#define SECTOR_NUM 7


//	������ �������

#define SECTOR_SIZE 0x1000
extern "C" {
/**************************************************************************
DOES:    Sets the value of a variable
RETURNS: TRUE for success, FALSE for error
**************************************************************************/
int SetState  (unsigned char*, unsigned short,int  );

/**************************************************************************
DOES:    Gets the value of a variable
RETURNS: TRUE for success, FALSE for error
**************************************************************************/
int GetState  (int  );
}

//**************************************************************************
//		������� �������.
//*************************************************************************

int EraseSector  (	unsigned short  );

// CPU clock in kHz
//#define CPU_CLK 48000		� ���� ��� ���� (Clk_CPU)


//	�������� � �������� ����������

#define DISABLEIRQ __disable_irq();
#define ENABLEIRQ  __enable_irq();


//=================================================================================================

#define IAP_LOCATION 0x7FFFFFF1							//	����� ������� ������� IAP

typedef void (*IAP)(unsigned int [], unsigned int []);	//	���������� ��� ���������
														//	�� �������.

