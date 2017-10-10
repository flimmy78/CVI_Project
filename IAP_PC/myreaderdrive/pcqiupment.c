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

extern	uint8	l_befaddr;


unsigned char l_recFlg = 0;
unsigned char GetRecFlg(void)
{
	return l_recFlg;
}



//////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////


uint8	GetFramRecEndFlg(void)
{
	return GetRecCom0EndFlg();  
}

//�������ɱ�־  ����
void	ClearFramRecEndFlg(void)
{
	ClearRecCom0EndFlg();   
}

uint16	ReadRs485Data(uint8 *Buf)
{
		return	ReadCom0(Buf,256);  
}


unsigned int  l_eqiupmentcode = 0;

unsigned char *pmodel;
static stcModelCard  m_sModelCard;

unsigned char l_modelcardflg = 0;

unsigned char GetModelCardFlg(void)
{
	return	l_modelcardflg;
}

uint8	sentendflg = 0;
uint8	l_befaddr = 0;

void CarddataOperate(unsigned char *buf,unsigned int Len)		
{
	unsigned int   datalen = 0;
	unsigned int   modeldatanum = 0;
	
	if(Len < 4)
	{
		printfcom1("\r\n ���ݳ���С��4������");
		return	;
	}
	
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
			unsigned int  i;
			

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
							  //printfcom1("\r\n MODEL_CARD "); 
							  
	
							  memcpy((unsigned char *)&modeldatanum,&buf[datalen],sizeof(modeldatanum));
							  datalen += sizeof(modeldatanum);
							 
							  printfcom1("\r\n modeldatanum %d",modeldatanum);
							  
							  if(modeldatanum == 0)
							  {
								   pmodel = (unsigned char *)&m_sModelCard;
								   printfcom1("\r\n ָ����ֵ pmodel %x",pmodel);
								   l_modelcardflg = 1;
							  }
							  
							  
							  memcpy(pmodel,&buf[datalen],128<(Len-datalen)?128:(Len-datalen));
							  pmodel += 128<(Len-datalen)?128:(Len-datalen);

							  if(modeldatanum == 0xffffffff)
							  {
								l_modelcardflg = 0;
								
								printfcom1("\r\n ģ�ͽ��� sModel ��������� %d,%d",m_sModelCard.sModelSemple[1].PotNum,m_sModelCard.sModelSemple[1].ModelNum );
								for(i = 0; i< m_sModelCard.sModelSemple[1].PotNum;i++)
								{
									printfcom1("\r\n �߶ȣ����� %d,%d",m_sModelCard.sModelSemple[1].sModPot[i].HigVal,m_sModelCard.sModelSemple[1].sModPot[i].DipVal ); 
								}

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

unsigned char GetRecaddr()
{
	return	l_befaddr;
}

void  CSNR_RecDeal(void)     									//����
{
	unsigned char		RecBuf[200] = {0};						//���ջ�����
	unsigned char		DataBuf[200] = {0};						//
	static	unsigned int		Time = 0;
	unsigned int	i;

	unsigned int		RecLen;		
	unsigned int		DataLen = 16;		
			
			
	if( GetFramRecEndFlg() == 1 && sentendflg == 1)				//���µ�֡β���������ݴ���
	{
		sentendflg = 0;   										//����ָ��󣬵ȴ�����
		RecLen 	= ReadRs485Data(RecBuf);
		
		ClearFramRecEndFlg();									//���־��������ȡ��
		
		l_recFlg = 1;

		l_befaddr = CSNR_GetData(RecBuf,RecLen,DataBuf,(unsigned char*)&DataLen); 
		
		printfcom1("\r\n reclen! %d l_befaddr %x",RecLen,l_befaddr);

		switch(l_befaddr)
		{
			case 0xCA: 
					   printfcom1("\r\n pc�˴���");				//PC�˴���

					   CarddataOperate(DataBuf,DataLen); 			//�������Խ��յ���Ϣ���д��� 
					   
				break;
				
			case 0xC1: printfcom1("\r\n ���ߴ���");				//����ģ��
			
				break;
				
			case 0xC2: printfcom1("\r\n ���ݶ�ȡװ�ô���");		//���ݶ�ȡװ��
				
				break;
				
			case 0xC3: printfcom1("\r\n ���ܵ�������");			//���ܵ���
				
				break;	
				
			case 0x80: printfcom1("\r\n ������Ϣ��%d",DataLen);	//���ܵ���
			
				break;	
				
			default: ;
																	//������Ϣ
		}
	}
}


unsigned int GetHostBuf(unsigned char *Buf)
{
	memcpy(Buf,(unsigned char *)&g_cardsta,sizeof(g_cardsta));
	printfcom1("len %d,g_cardsta %d",sizeof(g_cardsta),g_cardsta); 
	return 	sizeof(g_cardsta);
}

void	HostStationSend_CSNR(void)
{
	unsigned char HostBuf[64];
	unsigned int  Len;
	unsigned char addr;
	
	Len = GetHostBuf(HostBuf);
	
	addr = 0xCF;
	
	DataSend_CSNR(0x80,addr,HostBuf,Len);	
}

void HostScan(void)
{
	static	unsigned char 	tmp = 0;
	static	unsigned int 	time = 0;
	//unsigned char 		 	ScanAddrList[9]={0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xCA};
	unsigned char 		 	ScanAddrList[9]={0xCA,0xCA,0xCA,0xCA,0xCA,0xCA,0xCA,0xCA,0xCA};
	unsigned char           addr;
	
	if(GetSysTime() - time > 100 || GetRecFlg())
	{
		if((tmp %10) == 9) 
		{
			HostStationSend_CSNR();
		}
		else
		{
			DataSend_CSNR(0x80,ScanAddrList[tmp%10],(unsigned char *)&l_befaddr,sizeof(l_befaddr));
		}

		tmp++;
		sentendflg = 1;
		l_recFlg = 0; 
		time 	 = GetSysTime(); 
	}
}




void  CSNR_Test(void)   
{
	if(GetOpenCom0Flg())
	 {
		 if(GetModelCardFlg() == 0) 
		 {
			 HostScan();   
		 }
		 else
		 {
			 sentendflg = 1;
		 }
		 
		 CSNR_RecDeal();
		 DelayX10ms(1);
	 }
}




