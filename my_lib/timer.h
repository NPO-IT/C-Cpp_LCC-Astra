#ifndef __TIMER_H_
#define __TIMER_H_
#include "peripheral.h"
#include "type.h"
#include <LPC23xx.h>
//����� �������
struct TimerMatchOption{
	uint presc;					//������������
	uint mode;					//����� ������� ����������
	uint mr0;					//�������� ��� ������� �������
	uint mr1;
	uint mr2;
	uint mr3;	
}; 

//������ ��������
enum TimerName{
	TIMER_0=(uint)TMR0_BASE_ADDR,
	TIMER_1=(uint)TMR1_BASE_ADDR,
	TIMER_2=(uint)TMR2_BASE_ADDR,
	TIMER_3=(uint)TMR3_BASE_ADDR
};

//����������� ��������
#define TIMER_SIZE 4

//����� ��������
class Timer:public Peripheral{
			
	
	public:
	Timer(TimerName name);
	~Timer();

	//void Init(TimerMatchOption* opt);
	void Init(const TimerMatchOption* opt);
	//�����
	void Clear();
	//���������� �����
	//void SetMode();
	void Start();
	void Stop();
	uint GetCount();
	//������!
	//�������� ����������
	void ClearInt();
	const uint GetIntStatus() const;
	static void Register(uint num,Timer* obj);
	static Timer* Get(uint num);
	
	protected:
	enum{
		IR=0,TCR=1,TC=2,PR=3,PC=4,MCR=5,MR0=6,MR1=7,MR2=8,
		MR3=9,CCR=10,CR0=11,CR1=12,CR2=13,CR3=14,EMR=15,CTCR=28	
	};
	private: 
	//�������!
	static Timer* DEV[TIMER_SIZE];
}; 

#define PREPARE_TIMER Timer t0_dev(TIMER_0),t1_dev(TIMER_1),t2_dev(TIMER_2),t3_dev(TIMER_3);\
Timer::Register(0,&t0_dev);\
Timer::Register(1,&t1_dev);\
Timer::Register(2,&t2_dev);\
Timer::Register(3,&t3_dev);
#endif

