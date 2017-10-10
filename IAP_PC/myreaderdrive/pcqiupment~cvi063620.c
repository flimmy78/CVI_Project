#include <ansi_c.h>
#include "includes.h"

///////////////////////////////////////////////////////////

short 			g_density;
short 			g_hig; 
char			g_modelnum;
unsigned short 	g_loconum;
unsigned short 	g_locotype;


short  GetDensity(void)
{
	return  g_density;
}

short  GetHig(void)
{
	return  g_hig;
}

short  Getmodelnum(void)
{
	return  g_modelnum;
}

unsigned short  GetLocoType(void)
{
	return  g_locotype;
}

unsigned short  GetLocoNum(void)
{
	return  g_loconum;
}

stcModelCard	m_sModelCard;

static	uint32	l_StoreCnt  = 0;

void  GetRecData(stcFlshRec *msFlshRec)
{	
	uint32			FlshAddr;										//Flash�洢��ַ
	stcFlshRec		sFlshRec={0};									//Flsh��¼
	stcTAX2Full		sTAXTmp;										//TAXII����Ϣ����
	stcTime			sTime;											//ϵͳʱ��
	stcFixInfo 		sFixInfo;			
	static	uint32	l_StoreCnt  = 0;			//��ˮ��				
	uint8			l_RecTypCod = 0;       		//��¼����          	1   �������ͣ��������ϰ࿨���°࿨�����ݼ�¼��
	uint8			l_DriverUnitNum[2] ={0,0};	//˾����λ���      	2   //����Ψһȷ��˾��
	int32			l_MstLocoPower = 0;        	//����������          	2   ����豸����
	int32			l_SlvLocoPower = 0;        	//�ӻ�������          	2   ����豸����
	uint16 			l_MstDip = 0;           	//��������          	2   ���ͳ�����(���ӻ�����ʱΪ��������,����Ϊ����������)
	uint16 			l_SlvDip = 0;           	//�ӻ�����          	2   ���ͳ�����(���ӻ�����ʱΪ�ӻ�����,���������ֵΪ��)
	uint16			l_MstDip1Prs = 0;        	//ѹǿ            		2   �����ͳ�1ѹǿֵ
	uint16			l_MstDip2Prs = 0;        	//ѹǿ            		2   �����ͳ�2ѹǿֵ
	uint16			l_SlvDip1Prs = 0;       	//ѹǿ            		2   �ӻ��ͳ�1ѹǿֵ
	uint16 			l_SlvDip2Prs = 0;       	//ѹǿ            		2   �ӻ��ͳ�2ѹǿֵ
	uint16			l_MstDensity = 0;			//�ܶ�					2	����ȼ���ܶ�
	uint16			l_SlvDensity = 0;			//�ܶ�					2	�ӻ�ȼ���ܶ�
	int32			l_GpsLongitude = 0;			//����					4	������
	int32			l_GpsLatitude  = 0;			//γ��					4	����γ��
	int16			l_GpsHeight    = 0;			//�߶�					4	����߶�
	int16			l_MstEngRotSpd = 0;			//�������ͻ�ת��		2	�������ͻ�ת��
	int16			l_SlvEngRotSpd = 0;			//�ӻ����ͻ�ת��		2	�ӻ����ͻ�ת��
	uint16 			l_JcRealType = 0;			//�����ͺ�				2   �ⲿ���õĻ����ͺ�
	uint16 			l_JcRealNo   = 0;			//�������				2   �ⲿ���õĻ������
	uint8			l_ErrorCode  = 0;			//���ϴ���				1   0~255
	uint8 			l_DipValChgState = 0;  		//�����仯״̬   		1   �����仯����֣����ͳ�����/��С���仯ƽ�ȡ���΢���������Ҳ���
	uint8			l_MstDipTemp = 0;			//�¶�					1	�����¶�
	uint8			l_OilBoxModel= 0;			//����ģ�ʹ���			1	�ӻ��¶�				
	uint8			l_SoftVes	 = 0;			//����汾				1					
	uint8			l_Reserve1	 = 0;			//������Ϣ				1					
	
	
	//l_SoftVes 			= 0x01;				//����汾
//	l_SoftVes 			= 0x05;					//����汾
//	l_SoftVes 			= 0x06;					//����汾
	l_SoftVes 			= 0x07;					//����汾 20150302  ����
	

	l_StoreCnt++;
	l_MstDip1Prs		= 0x1111;
	l_MstDip2Prs		= 0x2222;	
//	l_MstDipTemp		= GetDipTmp();
	l_MstDipTemp 		= 1200;	
	l_DipValChgState	= 1300;
	l_MstDip			= 1500;
	
	l_GpsHeight			= 1600;
	
	l_MstDensity		= 1700;
	l_SlvDensity		= 1800;
//			
	l_ErrorCode			= 1;
	l_RecTypCod			= 2;
////	l_MstEngRotSpd		= GetDieselSpeed();	
	l_OilBoxModel		= 3;			
	
	sFlshRec.SoftVes		= l_SoftVes;						 
	sFlshRec.StoreCnt		= l_StoreCnt;						 
	sFlshRec.RecTypCod		= l_RecTypCod;						
   	sFlshRec.MstLocoPower	= l_MstLocoPower;						      
   	sFlshRec.SlvLocoPower	= l_SlvLocoPower;						      
   	sFlshRec.MstDip			= l_MstDip;							
	sFlshRec.SlvDip			= l_SlvDip;							
	sFlshRec.MstDip1Prs		= l_MstDip1Prs;						
	sFlshRec.MstDip2Prs		= l_MstDip2Prs;						
	sFlshRec.SlvDip1Prs		= l_SlvDip1Prs;						
	sFlshRec.SlvDip2Prs		= l_SlvDip2Prs;						
	sFlshRec.MstDensity		= l_MstDensity;						
	sFlshRec.SlvDensity		= l_SlvDensity;						
	sFlshRec.GpsLongitude	= l_GpsLongitude;					
	sFlshRec.GpsLatitude	= l_GpsLatitude;					
	sFlshRec.GpsHeight		= l_GpsHeight;						
	sFlshRec.MstEngRotSpd	= l_MstEngRotSpd;					
	sFlshRec.SlvEngRotSpd	= l_SlvEngRotSpd;					
	sFlshRec.JcRealType		= l_JcRealType;						
	sFlshRec.JcRealNo		= l_JcRealNo;						
	sFlshRec.ErrorCode		= l_ErrorCode;						
   	sFlshRec.MstDipTemp		= l_MstDipTemp;						
   	sFlshRec.OilBoxModel	= l_OilBoxModel;						
	sFlshRec.DipValChgState	= l_DipValChgState;					
	
	ReadTime((stcTime *)&sTime);											// ʱ��   
	
	sFlshRec.MyYear			= sTime.Year;			
	sFlshRec.MyMonth		= sTime.Month;			
	sFlshRec.MyData			= sTime.Date;			
	sFlshRec.MyHour			= sTime.Hour;			
	sFlshRec.MyMinute		= sTime.Min;			
	sFlshRec.MySecond		= sTime.Sec;			
	
	memcpy((unsigned char *)msFlshRec,(unsigned char *)&sFlshRec,sizeof(sFlshRec));
	
	sFlshRec.CrcCheck = GetCrc16Check((uint8 *)&sFlshRec,sizeof(sFlshRec) - 2);
}


void	 EquipmentReadData(unsigned int startnum,unsigned int endnum)
{
	unsigned int  recnum;
	stcFlshRec		sFlshRec={0};								//Flsh��¼   
	
	recnum = endnum - startnum;
	
	if(recnum >1000)
	{
		recnum = 100;
	}
	while(recnum--)
	{
		GetRecData((stcFlshRec *)&sFlshRec);

		DataSend_CSNR(0x80,0xca,(unsigned char *)&sFlshRec,sizeof(sFlshRec));
		printfcom1("\r\n �������� %d,%d",recnum,sizeof(sFlshRec));
	}
}

//////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////



unsigned int  l_eqiupmentcode = 0;

unsigned char *pmodel;
stcModelCard  m_sModelCard;

void PCOperate(unsigned char *buf,unsigned int Len)		
{
//	unsigned char  buf[256];
	unsigned int   datalen = 0;
	unsigned int   modeldatanum = 0;
	
	if(Len < 4)
	{
		printfcom1("\r\n ���ݳ���С��4������");
		return	;
	}
	
//	l_eqiupmentcode = 0;
	datalen = 0;
	memcpy((unsigned char *)&l_eqiupmentcode,&buf[datalen],sizeof(l_eqiupmentcode));
	datalen += sizeof(l_eqiupmentcode);
	
	printfcom1("���յ��������� %x",l_eqiupmentcode);
	
	if(l_eqiupmentcode != 0)
	{
		switch (l_eqiupmentcode)
		{
			unsigned int startnum=0,endnum=0xffffffff;
			short  density; 
			stcTime		sTime;   
			short   hig; 
			char	modelnum;
			unsigned short locotype,loconum;  			
			

			case DATA_CARD:  
							  printfcom1("\r\n DATA_CARD ׼���������� ");
							  memcpy((unsigned char *)&startnum,&buf[datalen],sizeof(startnum));
							  datalen += sizeof(startnum);
							  memcpy((unsigned char *)&endnum,&buf[datalen],sizeof(endnum));  
							  
							  EquipmentReadData(startnum,endnum);								//
								
							  break;    
			case DENSITY_CARD: 
				              printfcom1("\r\n DENSITY_CARD ");  
							  memcpy((unsigned char *)&density,&buf[datalen],sizeof(density));
							  
							  printfcom1("\r\n density %d  ���ݳ��� %d datalen %d  buf %x buf++ %x",density,sizeof(density),datalen,buf[datalen],buf[datalen+1]);  
							  break;    
			case MODEL_CARD:  
							  printfcom1("\r\n MODEL_CARD "); 
							  
	
							  memcpy((unsigned char *)&modeldatanum,&buf[datalen],sizeof(modeldatanum));
							  datalen += sizeof(modeldatanum);
							 
							  printfcom1("\r\n modeldatanum %d",modeldatanum);
							  
							  if(modeldatanum == 0)
							  {
								   pmodel = (unsigned char *)&m_sModelCard;
								   printfcom1("\r\n ָ����ֵ pmodel %x",pmodel);
							  }
							  
							  
							  memcpy(pmodel,&buf[datalen],128<(Len-datalen)?128:(Len-datalen));
							  pmodel += 128<(Len-datalen)?128:(Len-datalen);
							  printfcom1("\r\n 128<(Len-datalen)?128:(Len-datalen) %d  ,Len %d",128<(Len-datalen)?128:(Len-datalen),Len);

							  if(modeldatanum == 0xffffffff)
							  {
							  	printfcom1("\r\n ģ�ͽ��� CrcCheck %x",m_sModelCard.CrcCheck);
								
								printfcom1("\r\n ģ�ͽ��� sModel ��������� %x,%x",m_sModelCard.sModel[1].PotNum,m_sModelCard.sModel[1].ModelNum );
							  }
							  
							  
							  break;   
			case TIME_CARD:   
							  printfcom1("\r\n TIME_CARD ");
							  
							  memcpy((unsigned char *)&sTime,&buf[datalen],sizeof(sTime));
							  
							  printfcom1("\r\n ʱ�䣺%d-%d-%d %d-%d-%d",sTime.Year,sTime.Month,sTime.Date,sTime.Hour,sTime.Min,sTime.Sec);
							  
							  break; 
			case HIGHT_CARD:  
							  printfcom1("\r\n HIGHT_CARD ");
							  
							  memcpy((unsigned char *)&hig,&buf[datalen],sizeof(hig));

							  printfcom1("\r\n �߶ȣ�%d",hig);
							  
							  break; 
			case COPY_CARD:  	
							  printfcom1("\r\n COPY_CARD ");
							  
							  
						      break; 
			case MODEL_SELECT_CARD:
							  printfcom1("\r\n MODEL_SELECT_CARD "); 
							  
							  memcpy((unsigned char *)&modelnum,&buf[datalen],sizeof(modelnum));

							  printfcom1("\r\n ѡģ��%d",modelnum);

							  break; 
			case FIX_CARD:  
							  printfcom1("\r\n FIX_CARD ");

							  memcpy((unsigned char *)&locotype,&buf[datalen],sizeof(locotype));
							  datalen += sizeof(locotype);
							 
							  memcpy((unsigned char *)&loconum,&buf[datalen],sizeof(loconum));
							  
							  printfcom1("\r\n �������ͣ�%d��������ţ�%d",locotype,loconum);
							  
				     		  break; 
			case COPY_FRAM_CARD:  
							  printfcom1("\r\n COPY_FRAM_CARD "); 
							  break; 
			case EXPAND_CARD:  	
							  printfcom1("\r\n EXPAND_CARD "); 
							  break; 
			default:
				   printfcom1("\r\n default "); 
		}
		
		l_eqiupmentcode = 0;
	}
}

void DipOperate(unsigned char *buf,unsigned int Len)		
{
	short			hig;
	unsigned short 	den;
	unsigned char	sta;
	char			tmp;
	
	unsigned int   datalen = 0;
	
	memcpy((unsigned char *)&hig,&buf[datalen],sizeof(hig));
	datalen += sizeof(hig);
	memcpy((unsigned char *)&den,&buf[datalen],sizeof(den));
	datalen += sizeof(den);
	memcpy((unsigned char *)&sta,&buf[datalen],sizeof(sta));
	datalen += sizeof(sta);
    memcpy((unsigned char *)&tmp,&buf[datalen],sizeof(tmp));
	datalen += sizeof(tmp);		
	
	if(GetRecaddr()==0xA1)
	{
		printfcom1("\r\n �߶� %d, %d, %d, %d",hig,den,sta,tmp);
	}
	else if(GetRecaddr()==0xA2)
	{
		printfcom1("\r\n �߶� %d, %d, %d, %d",hig,den,sta,tmp);
	}
	
}







