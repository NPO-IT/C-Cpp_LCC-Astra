#ifndef __VIC_H__
#define __VIC_H__
#include "type.h"
	//����� ����������� ��������� ����������
	class VIC{
		private:
			static ulong *base_addr;	//(ulong*)0xFFFFF000;
			static ulong *vect_addr;	//(ulong*)0xFFFFF100;
			static ulong *priority;	 	//(ulong*)0xFFFFF200;
			static const uint VIC_SIZE;
			enum{
			 	IRQStatus=0,
				FIQStatus=1,
				RawIntr=2,
				IntSelect=3,
				IntEnable=4,
				IntEnClr=5,
				SoftInt=6,
				SoftIntClr=7,
				Protection=8,
				SWPriorityMask=9,
				Addr=960
			};
			
			VIC();
			~VIC();
		public:
			
			//��������� ����������
			enum INT_SRC{
			  	WDT_INT= 0		  	//Watchdoc
				,SWI_INT= 1			//����������
				,ARM_CORE0_INT= 2	//����
				,ARM_CORE1_INT=3	//����
				,TIMER0_INT= 4 		//������0
				,TIMER1_INT= 5		//������1
				,UART0_INT= 6		//����0
				,UART1_INT= 7 		//����1
				,PWM1_INT= 8		//PWM
				,I2C0_INT= 9
				,SPI0_INT= 10/* SPI and SSP0 share VIC slot */
				,SSP0_INT= 10
				,SSP1_INT= 11
				,PLL_INT= 12		//����
				,RTC_INT= 13		//���� ��������� �������
				,EINT0_INT= 14		//�������
				,EINT1_INT= 15
				,EINT2_INT= 16
				,EINT3_INT= 17
				,ADC0_INT= 18		//���
				,I2C1_INT= 19
				,BOD_INT= 20		  //????
				,EMAC_INT= 21
				,USB_INT= 22
				,CAN_INT= 23
				,MCI_INT= 24
				,GPDMA_INT= 25		//DMA
				,TIMER2_INT= 26		//
				,TIMER3_INT= 27
				,UART2_INT= 28
				,UART3_INT= 29
				,I2C2_INT= 30
				,I2S_INT= 31
			};
			//�������� ���������� ��������� 0..15(0 ����� �������)
			static bool InstallIRQ(VIC::INT_SRC int_num,void* Handler,uint _prio);
			static void Init();
			static void IntClear();
			
	};

#endif

