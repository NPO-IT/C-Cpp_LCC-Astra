#include "ssp.h"

SSP::SSP(SSPName name):Peripheral((ulong*)name){

}
SSP::~SSP(){

}
void SSP::Init(SSPOption* opt){
	/*
	if(!IsValidOption(opt)){
		return false;
	}
	*/
	
	base_addr[CR1]&=~2;			//���������
	//��� ����� ������ ����� ���������� ����� ��������� ��������
	base_addr[CR0]=opt->dss|opt->frf<<4|opt->spo<<6|opt->sph<<7|opt->scr<<8;
	//����������� �������� ������ ��� �����������
	base_addr[CR1]=opt->lbm|opt->ms<<2;
	base_addr[CPSR]=opt->div;			//��������
	base_addr[IMSC]=opt->int_mode; 		//����������
	base_addr[DMACR]=opt->dma;
	base_addr[CR1]|=2;					//��������	
	//return true;
}

const uint SSP::GetIntStatus() {
	return base_addr[MIS];
}
const uint SSP::GetStatus() {
	return base_addr[SR];
}
/*
  ��� ����� ������� �������� � ����������� ����� ��� 8 ��� ������
  ���� ��������� ���������� ������
*/
bool SSP::Read(byte* buf,const uint& size){
	for(uint i=0;i<size;i++){
		while(!base_addr[SR]&RNE);
		buf[i]=base_addr[DR];
	}
	return true;
}
bool SSP::Send(const byte* buf,const uint& size){
	for(uint i=0;i<size;i++){
		while(!base_addr[SR]&TNF);
		base_addr[DR]=buf[i];
		while(!base_addr[SR]&BSY);
	}
	return true;
}
void SSP::ClearRxFIFO(){
	ushort data;
	while(!(base_addr[SR]&RNE)){
		data=base_addr[DR];
	}
}
//� ���� ��� ����� ��� ��� ��������
void SSP::ClearTxFIFO(){
	//while(base_addr[SR]&)
}
		
SSP* SSP::Get(uint num){
	if(num>=SSP_SIZE) return (SSP*)0;
	return DEV[num];
}

void SSP::Register(uint num,SSP* obj){
	if(num>=SSP_SIZE) return;
	DEV[num]=obj;
}

SSP* SSP::DEV[SSP_SIZE];

/*������� ������� ��������� ����� ��� ���� */
void SSP::Write2Byte(ushort data){
	while(!base_addr[SR]&TNF);
	base_addr[DR]=data;
	while(!base_addr[SR]&BSY);	
}

void SSP::GetAllData(byte* buf,uint& size){
	size=0;
	//byte val;
	while(base_addr[SR]&RNE){
		buf[size++]=base_addr[DR];	
	}
}
