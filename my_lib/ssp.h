#ifndef __SSP_H_
#define __SSP_H_

#include "peripheral.h"


//����� SSP
enum SSPName {
    SSP_0 = (uint)SSP0_BASE_ADDR
    , SSP_1 = (uint)SSP1_BASE_ADDR
};

#define SSP_SIZE 2

struct SSPOption{
	byte dss;		//����� ��� 4..16
	byte frf;		//����� �������� SPI|TI|microwire
	byte scr;			//��������� ��������
	byte div;		//��������	 2..254
	byte spo;			//���������� 
	byte sph;			//����
	byte ms;		    //������/�����������
	byte lbm;			//����� ����������� ��������
	byte int_mode;		//����� ����������
	byte dma;			//��� 1 -RECEIVE DMA 2 - TRANSMIT DMA
};

class SSP:public Peripheral{
	private:
		enum{
			CR0=0,CR1=1,DR=2,SR=3,CPSR=4,IMSC=5,RIS=6,MIS=7,ICR=8,DMACR=9	
		};
		static SSP* DEV[SSP_SIZE];
	public:
		//��������� ����������
		enum{
			 
			TFE=1,TNF=2,RNE=4,RFF=8,BSY=16
		};

		//����������
		enum{
			//������������
			// ������ ������ �� ���� - �������
			/// ������ ������ �� �������� �����/
			// ������ �������� �� �������� ���� 
			ROR=1,RT=2,RX=4,TX=8	
		};
		SSP(SSPName name);
		~SSP();
		void Init(SSPOption* opt);					//���������
		const uint GetIntStatus();					//���������
		const uint GetStatus();						//���������
		bool Read(byte* buf,const uint& size);		//!������ ���������
		bool Send(const byte* buf,const uint& size);		//������ ���������
		void ClearRxFIFO();							//������ ���������
		void ClearTxFIFO();							//������ ���������
		void Write2Byte(ushort data);
		void GetAllData(byte* buf,uint& size);
		//��������� �������
		static SSP* Get(uint num);
		static void Register(uint num,SSP* obj);

};


#define PREPARE_SSP \
SSP ssp_dev0(SSP_0),ssp_dev1(SSP_1);\
SSP::Register(0,&ssp_dev0);\
SSP::Register(1,&ssp_dev1);
#endif
