#include <ansi_c.h>
#include "includes.h"  

/*
//TAXIIǰ-------------------------------------------------------------------------------------
typedef struct _stcTAX2Bef_     //������TAX2��󲿷ֵ����ݽṹ (32)
{
    unsigned char   TAX2BefAdrCod;      //�����ַ(38H)     1   ָͨѶ��¼��Ԫ�ĵ�ַ����Ϊ38H
    unsigned char   CharacterCod;       //������            1   ��,����0
    unsigned char  	NewFlg;      		//��־     			1   ��Э��:67H
    unsigned char   SoftVerNum;      	//����汾��      	1   
 	
	unsigned char   Reserve0;      		//Ԥ��0       			1   
    unsigned char   E_StationNum;       //��վ����չ�ֽ�    1   ��վ����չ�ֽ�
    unsigned char   TrainTyp[4];        //��������          4   ��������,���α����·���(����[2005]72���ļ�)
    														//						�ĳ��������ʶ��,������LKJ2000��ʽ��ͬ 
    unsigned char   E_MstDriverNum;     //˾������չ�ֽ�    1     
    unsigned char   E_SlvDriverNum;     //��˾������չ�ֽ�  1     
    
    unsigned char   Reserve1[2];      	//Ԥ��1 			2
    unsigned char   E_LocoTyp;          //�����ͺ���չ�ֽ�  1   �����ͺ������ֽ�,������LKJ2000��ʽ��ͬ
    unsigned char   RelRoadNum;         //ʵ�ʽ�·��        1   �����ʽ��LKJ2000��ͬ   
    
    unsigned char   Reserve2[11];      	//Ԥ��2 			11
    unsigned char   Car_Truck;          //��/������/��      1   D0��0/1 �� ��/��   D1��0/1 �� ��/��
    
    unsigned char   TrainNum[3];        //�������ֲ���      3   ���α����·���(����[2005]72����)�ĳ������ݲ���
    unsigned char   CheckSum;           //����            1   Ϊǰ�������ֽ��ۼӺ͵Ķ����Ʋ���
}stcTAX2Bef;

//TAXII��----------------------------------------------------------------------------------
typedef struct _stcTAX2Bak_     //������TAX2��󲿷ֵ����ݽṹ
{
    unsigned char   TAX2BakAdrCod;      //�����ַ(39H)     1   ָͨѶ��¼��Ԫ�ĵ�ַ����Ϊ39H
    unsigned char   CharacterCod;       //������            1   ��4λΪ3���ϴν��ճɹ���ΪC��
                                //                      �ϴν���ʧ�ܣ�����˵����������
                                //                      ͨѶ�������ܸ��ţ�Ӧ����Ч���ݴ���
                                //                      ����λ��Ϊ0
    unsigned char   DetectCellCod;      //��ⵥԪ����      1   ָ����ͨѶ��¼��ԪͨѶ�ļ�ⵥԪ���š�
                                //                      ��Ϊ01-�����⣬
                                //                          02-������⣬03-TMIS��04-DMIS��
                                //                          06-����¼����05��07-09Ԥ�������õ�Ԫ��
    unsigned char   Time[4];            //��/��/ʱ/��/��    4   D5~D0���룬D11~D6���֣�D16~D12��
                                //                          ʱD21~D17���գ�D25~D22����
    unsigned char   RelSpeed[3];        //ʵ��              3   D9~D0��ʵ�٣�D19~D10��Ԥ��
    unsigned char   LocoSign;           //�����ź�          1   B4 = 0/1  ����/���;   B3~B0 0-�޵�,1-��,2-��, 
    unsigned char   LocoWorkState;      //��������          1   b0-��λ,B1-���,B2-��ǰ,B3-�ƶ�,B4-ǣ��
    unsigned char   SignalNum[2];       //�źŻ����        2   
    unsigned char   SignalTyp;          //�źŻ�����        1   B2-B0 :2-��վ,3-��վ,4-ͨ��,5-Ԥ��,6-����
                                //                  3   -˫��,4-���,5-��,6-��,7-�̻�,8-��
    unsigned char   KmMark[3];   				//�����    3   ��λ����
    unsigned char   Weight[2];          //����              2
    unsigned char   PlanLen[2];         //�Ƴ�              2   ��λ��0.1
    unsigned char   VoitureCnt;         //����              1
    unsigned char   Car_Truck;          //��/������/��      1   D0��0/1 �� ��/��   D1��0/1 �� ��/��
    unsigned char   TrainNum[2];       	//����              2
    unsigned char   RoadNum;            //���κ�(��·��)    1
    unsigned char   StationNum;         //��վ��            1
    unsigned char   MstDriverNum[2];    //˾����            2
    unsigned char   SlvDriverNum[2];    //��˾����          2
    unsigned char   LocoNum[2];         //������            2
    unsigned char   LocoTyp;            //�����ͺ�          1   ��δ����
    unsigned char   LocoPipePress[2];   //�г���ѹ          2   B9-B0:��ѹ(��λ:1kPa)    
    unsigned char   LocoState;          //װ��״̬          1   B0 = 1/0 - ����/���; B2 = 1/0 - ����/�ǵ���
    unsigned char   Reserve1;           //Ԥ��0 			1
    unsigned char   CheckSum;           //����            1   Ϊǰ�������ֽ��ۼӺ͵Ķ����Ʋ���
}stcTAX2Bak;


typedef struct _stcTAX2Full_  	//������TAX2���ݽṹ
{
	stcTAX2Bef	sTAX2Bef;		//TAX2ǰ����			
	stcTAX2Bak	sTAX2Bak;		//TAX2�󲿷�
}stcTAX2Full;

*/
void     ReadTime(stcTime * sTime)
{
	struct tm *ptr;
    time_t lt; 
	
    lt=time(NULL);
    ptr=localtime((time_t*)&lt);
	
	
	sTime->Sec = ptr->tm_sec;
	sTime->Min = ptr->tm_min;
	sTime->Hour = ptr->tm_hour;	
	sTime->Date = ptr->tm_mday;	  	
	sTime->Month = ptr->tm_mon+1;
	sTime->Year = ptr->tm_year-100;	
//	printf("%d-%d-%d %d:%d:%d\r\n",sTime->Year ,sTime->Month,sTime->Date,sTime->Hour,sTime->Min,sTime->Sec);
}

void  ReadSimsTaxData(stcTAX2Full *sTax)
{
    unsigned char   	CheckSum;        		//����            1   Ϊǰ�������ֽ��ۼӺ͵Ķ����Ʋ���  
	stcTime		sTime;
	unsigned int		RealSpeed;
	unsigned int		Time;
	unsigned int		KmMark;
	unsigned short		LocoNum;
	unsigned short		MstDriverNum;
	
	sTax->sTAX2Bef.TAX2BefAdrCod 	= 0x38;
	sTax->sTAX2Bef.NewFlg			= 0x67;
	sTax->sTAX2Bef.CheckSum = GetCheckSum((unsigned char *)&sTax->sTAX2Bef,sizeof(sTax->sTAX2Bef) -1);

    sTax->sTAX2Bak.TAX2BakAdrCod = 0x39;

    ReadTime((stcTime *)&sTime);											// ʱ�� 
	
	Time	=  sTime.Sec + sTime.Min * (1 << 6) + sTime.Hour * (1 << 12) 
				+ sTime.Date * (1 << 17) + sTime.Month * (1 << 22) + sTime.Year * (1 << 26);
	memcpy(sTax->sTAX2Bak.Time,(unsigned char *)&Time,sizeof(Time));
	
//	printf("sTime  %d  %d\r\n",sTax->sTAX2Bak.Time,Time);  
	
	RealSpeed = 45;
//    RealSpeed	= GetSetTaxSpeed();		// ȡTAX����Ϣ]
	
	memcpy((unsigned char *)&sTax->sTAX2Bak.RelSpeed,(unsigned char *)&RealSpeed,sizeof(sTax->sTAX2Bak.RelSpeed));

	//KmMark	= RealSpeed * ((GetSysTime()/10)/36);  	//((GetSysTime()/100)/3600) ��λ��m
	KmMark	= 0xffffffff;  	//((GetSysTime()/100)/3600) ��λ��m  
	memcpy((unsigned char *)&sTax->sTAX2Bak.KmMark,(unsigned char *)&KmMark,sizeof(sTax->sTAX2Bak.KmMark));

    //sTax->sTAX2Bak.LocoTyp						=  GetSetTaxLocoTyp();		// ȡTAX������ͺ�    
    sTax->sTAX2Bak.LocoTyp						=  141;		// ȡTAX������ͺ�  
      
	//LocoNum	=  GetSetTaxLocoNum();					// ȡTAX��������
	LocoNum	=  314;					// ȡTAX��������
	memcpy((unsigned char *)&sTax->sTAX2Bak.LocoNum,(unsigned char *)&LocoNum,sizeof(sTax->sTAX2Bak.LocoNum));

//	MstDriverNum	=  GetSetTaxDriverNum();		// ȡTAX��˾����
	MstDriverNum = 251;
	memcpy((unsigned char *)&sTax->sTAX2Bak.MstDriverNum,(unsigned char *)&MstDriverNum,sizeof(sTax->sTAX2Bak.MstDriverNum));
													// �����

	sTax->sTAX2Bak.CheckSum = GetCheckSum((unsigned char *)&sTax->sTAX2Bak,sizeof(sTax->sTAX2Bak) -1);
}


void	SendSimsTaxData(stcTAX2Full * sTax)			//����TAX����
{
	SendCom3((uint8 *)sTax,sizeof(stcTAX2Full));	//����TAX��Ϣ
}

//--------------------------------------------------------------------------------------------------
void  SimulateTax(void)
{
	stcTAX2Full		sTax;
	static	uint32	Time;
	time_t t;  
	
	if(GetOpenCom3Flg())	
	{  
		ClearRecCom3EndFlg(); 
		
		if(GetSysTime() - Time > 10)				//20ms
		{
			ReadSimsTaxData((stcTAX2Full *)&sTax);		//��TAX��ģ��������

			SendSimsTaxData((stcTAX2Full *)&sTax);		//����TAX����
		
			Time = GetSysTime();
		}
	}
}
