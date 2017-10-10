
#include <userint.h>
#include "ndpcom.h"  
#include "includes.h"



#define		Led0	0							
#define		Led1	0							
#define		Led2	0							
#define		Led3	0							
#define		Led4	0
#define		Led5	0
#define		Led6	0
#define		Led7	0





#define		LED_MAX	8

typedef struct _stcLed_
                {
                    volatile 	uint32  WorkingFlag;            //ָʾ�ƿ�����־
                    volatile 	uint32  times;            		//ָʾ����˸�Ĵ���
                    volatile 	uint32  WorkXms;              	//ָʾ������ʱ��
                    volatile 	uint32  unWorkXms;             	//ָʾ�Ʋ�����ʱ��
                    volatile 	uint32	TotalTime;
                    volatile 	uint32  StarTimer;
                }stcLed;

stcLed	sLed[8] =
			{
				{0,0,0,0,1,0},
				{0,0,0,0,1,0},
				{0,0,0,0,1,0},
				{0,0,0,0,1,0},
				{0,0,0,0,1,0},
				{0,0,0,0,1,0},
				{0,0,0,0,1,0},
				{0,0,0,0,1,0},
			};
			
uint8  	HLAB_i;
uint32  CurentTime;

//------------------------------------------------------------------------
//	��	�� ��void LedUnLight(void)
//	��	�� ��ָʾ�Ƴ�����
// ��ڲ�����
// ���ڲ�����
//------------------------------------------------------------------------
void UnLightLed(uint8 num)
{
	uint8 buf[]={Led0,Led1,Led2,Led3,Led4,Led5,Led6,Led7};
	SetCtrlVal(panelHandle,buf[num],0);	
}
//------------------------------------------------------------------------
//	��	�� ��void LedLight(void)
//	��	�� ��ָʾ�Ƴ�����
// ��ڲ�����
// ���ڲ�����
//------------------------------------------------------------------------
void LightLed(uint8 num)
{
	uint8 buf[]={Led0,Led1,Led2,Led3,Led4,Led5,Led6,Led7};
	SetCtrlVal(panelHandle,buf[num],1);	
}

//------------------------------------------------------------------------
//	��	�� ��void OnLed(uint8 num,uint32 times,uint16 LightTime,uint16 unLightTime)
//	��	�� ������ָʾ��
// ��ڲ�����uint8 Num			���ƵĶ�Ӧ���	 ���� Driver.h �й�����
//			 uint16	times			��������Ĵ���
//			 uint16	LightTime		���ʱ��  �� 0.1sΪ��λ
//			 uint16	unLightTime		�����ʱ�� �� 0.1s Ϊ��λ
// ���ڲ����������ɹ����� TRUE	����ʧ�ܷ��� FAULSE
//------------------------------------------------------------------------
void OnLed(uint8 num,uint32 times,uint16	LightTime,uint16 unLightTime)
{
	
	if(num >= LED_MAX || times == 0 || sLed[num].WorkingFlag)
	{
	    return;
	}
	if( LightTime ==0 || times == 0 )
	{
		return;
	}
	
	sLed[num].times 		= times;
	sLed[num].WorkXms		= LightTime * 10;
	sLed[num].unWorkXms 	= unLightTime * 10;
	sLed[num].WorkXms		= LightTime;
	sLed[num].unWorkXms 	= unLightTime;	
	sLed[num].TotalTime 	= sLed[num].WorkXms + sLed[num].unWorkXms ;
	sLed[num].StarTimer		= GetSysTime();
	sLed[num].WorkingFlag	= 1;
	LightLed(num);	
}

//------------------------------------------------------------------------
//	��	�� ��void OnAllLed(void)
//	��	�� ��ȫ��
// ��ڲ�����
// ���ڲ�����
//------------------------------------------------------------------------
void OnAllLed(void)
{
	uint8 num;
	for(num = 0; num < LED_MAX; num++)
	{
		OnLed(num,100,10000,0);						//����
    }
}

//------------------------------------------------------------------------
//	��	�� ��void LedOff(uint8 LedNum)
//	��	�� ���رն�Ӧ���ָʾ��
// ��ڲ������� 
// ���ڲ�����
//------------------------------------------------------------------------
void OffLed(uint8 num)
{
	sLed[num].WorkingFlag	= 0;            //ָʾ�ƿ�����־
    sLed[num].times			= 0;            //ָʾ����˸�Ĵ���
    sLed[num].WorkXms		= 0;            //ָʾ������ʱ��
    sLed[num].unWorkXms		= 0;          	//ָʾ�Ʋ�����ʱ��
    sLed[num].TotalTime		= 1;
    sLed[num].StarTimer		= 0;
    UnLightLed(num);
}

//------------------------------------------------------------------------
//	��	�� ��void OffAllLed(void)
//	��	�� ����ȫ��
// ��ڲ�������
// ���ڲ�����
//------------------------------------------------------------------------
void OffAllLed(void)
{
	uint8 num;
	for(num = 0; num < LED_MAX; num++)
	{
		OffLed(num);								//�ص�
    }
}

//------------------------------------------------------------------------
//	��	�� ��void HoldLed(void)
//	��	�� ��ָʾ��ά��
// ��ڲ�����
// ���ڲ�����
//------------------------------------------------------------------------
void HoldLed(void)
{
	CurentTime = GetSysTime();
	for(HLAB_i =	0; HLAB_i < LED_MAX; HLAB_i++)
	{
		if(sLed[HLAB_i].WorkingFlag)
		{
			if( ( (CurentTime - sLed[HLAB_i].StarTimer) % sLed[HLAB_i].TotalTime) 
					< sLed[HLAB_i].WorkXms)
				LightLed(HLAB_i);
			else
			{
				UnLightLed(HLAB_i);
			}
			if( ( (CurentTime	- sLed[HLAB_i].StarTimer) / sLed[HLAB_i].TotalTime) 
					>= (sLed[HLAB_i].times))
		   	{
		   	 	OffLed(HLAB_i);
		   	}
		}
	}
}

//------------------------------------------------------------------------
//	��	�� ��void InitLed( void )
//	��	�� ��ָʾ�Ƴ�ʼ��
// ��ڲ�����
// ���ڲ�����
//------------------------------------------------------------------------
void InitLed( void )
{
	uint8	i;	
//	PINSEL2 |= 0x00000010;					//��ʼ����������ܶ˿�
//	
//	IO2DIR  |=  (0x0000001<<Led0)
//	          +(0x0000001<<Led1)
//	          +(0x0000001<<Led2)
//             +(0x0000001<<Led3)
//             +(0x0000001<<Led4)
//              +(0x0000001<<Led5)
//              +(0x0000001<<Led6)
//              +(0x0000001<<Led7);					
    
	for(i = 0;i < 2; i++)					
	{
		OnAllLed();								
	
	    DelayX10ms(25);

		OffAllLed();						
		
		DelayX10ms(25);
	}
}
