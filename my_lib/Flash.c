#include "Flash.H"
#include "main.H"




static IAP p_IAP = (IAP)IAP_LOCATION;					//	��������� IAP ��������� �� ����� 
														//	�������	������� IAP.

//	��������� ������ �������� � ������ �� ������ ������� �� ������������ FLASH 
//		(��� �������, ��� �� ��������� ��� ����-������ �� �������)
//			so it isn't used by the linker for something else

static unsigned char SectorState[SECTOR_SIZE] __attribute__((at(SECTOR_STARTADDR)));



//**************************************************************************
//		������� �������.
//**************************************************************************
int EraseSector  (	unsigned short CLK  )
{
  unsigned int Command[5], Result[5];

  // ������� ������
  Command[0] = 50;
  Command[1] = SECTOR_NUM;
  Command[2] = SECTOR_NUM;
  DISABLEIRQ;
  p_IAP(Command, Result);
  ENABLEIRQ;
  if (Result[0] != CMD_SUCCESS) return FALSE; 

  // ������� ������
  Command[0] = 52;
  Command[1] = SECTOR_NUM;
  Command[2] = SECTOR_NUM;
  Command[3] = CLK;
  DISABLEIRQ;
  p_IAP(Command, Result);
  ENABLEIRQ;
  if (Result[0] != CMD_SUCCESS) return FALSE;
  
  return TRUE; 
}
//**************************************************************************
//		���������� �������� ����� ����������� ���������.
//**************************************************************************
int GetState  (	void )
{
  return (unsigned int)(SectorState[0]);
}

//**************************************************************************
//    ��������� �������� ��������� � ������ SECTOR_NUM FLASH
//	����������: TRUE ��� ������, FALSE ��� ������
//**************************************************************************
int SetState  (	unsigned char _State, unsigned short CLK  )
{
  char Buffer[256];
  unsigned int Command[5], Result[5];

	Buffer[0]=_State;

  // prepare sector
  Command[0] = 50;
  Command[1] = SECTOR_NUM;
  Command[2] = SECTOR_NUM;
  DISABLEIRQ;
  p_IAP(Command, Result);
  ENABLEIRQ;
  if (Result[0] != CMD_SUCCESS) return FALSE; 

  // write to sector
  Command[0] = 51;
  Command[1] = (unsigned int)(SectorState);
  Command[2] = (unsigned int)Buffer;
  Command[3] = 256;
  Command[4] = CLK;
  DISABLEIRQ;
  p_IAP(Command, Result);
  ENABLEIRQ;
  if (Result[0] != CMD_SUCCESS) return FALSE;

  // verify
  Command[0] = 56;
  Command[1] = (unsigned int)(SectorState);
  Command[2] = (unsigned int)(Buffer);
  Command[3] = sizeof(_State);
  DISABLEIRQ;
  p_IAP(Command, Result);
  ENABLEIRQ;
  if (Result[0] != CMD_SUCCESS) return FALSE;

  return TRUE;

}

