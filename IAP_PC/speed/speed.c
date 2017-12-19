#include <ansi_c.h>
#include "includes.h" 

#define		BROAD_ADDR			0xAF

#define		GET_DIP_PRS			0xc0
#define		SET_DIP_MOD			0xD7
#define		SET_PRS_LIN			0xD6

#define		TEST_COM			0xD4
#define		BROAD_INFO			0x66

#define		INF_HEAD_LEN			5
#define		INF_START_POS			4


typedef	struct	_stcCalDist_
{
	uint32	ModDPR;					//��������
	uint32	Dist;					//���
}stcCalDist;

typedef struct _stcDist_						//�ͳ�
{
	stcCalDist	A_sDist;						//�����
	stcCalDist	C_sDist;						//�ֶ����
	stcCalDist	E_sDist;						//�Զ������
	uint32		Rec32;							//Ԥ��4�ֽ�
	uint16		Speed;							//�ٶ�
	uint16		Crc16;							//CRC����
}stcDist;

stcDist	 	g_sDist;		

//------------------------------------------------------------------------
//  ��  �� ��������ģ��
// ��ڲ�������
// ���ڲ�������ǰ��ˮ��
//------------------------------------------------------------------------
uint8	InitDistData(void)
{
	uint16	CrcCheck;
	uint8	Flg;

	g_sDist.A_sDist.Dist	= 12340;
	g_sDist.Speed 			= 68;   	
	
	CrcCheck = GetCrc16Check((uint8 *)&g_sDist,sizeof(stcDist) - 2);
	
	g_sDist.Crc16 = CrcCheck;

	return	1;
}


//------------------------------------------------------------------------
// ��    �ܣ�   �����װ�ù㲥��Ϣ
// ��ڲ�����	Buf:���ݻ����� BufLen:���ݻ��������ݳ��� ��Typ:��������
// ���ڲ�����	��
//------------------------------------------------------------------------
void SendBroadInfo(uint8 *Buf,uint32	BufLen,uint8 Typ)		
{
	uint8	SourceAddr,TargetAddr,DatTyp,StartCode,DataLen;
	uint8	BufTmp[50];
	uint8	i = 0;
	uint16	CrcCheck;
	
	DatTyp		=	Typ;
	StartCode 	= 	COM_START_CODE;
	TargetAddr  = 	BROAD_ADDR;	
	SourceAddr 	=	HOST_ADDR;
	DataLen		= 	BufLen + sizeof(DatTyp);
	
	BufTmp[i++] =	StartCode;
	BufTmp[i++] =	TargetAddr;
	BufTmp[i++] =	SourceAddr;
	BufTmp[i++] =	DataLen;
	BufTmp[i++] =	DatTyp;
	
	memcpy(&BufTmp[i],Buf,BufLen);
	
	CrcCheck 	=	GetCrc16Check(BufTmp,BufLen + INF_HEAD_LEN);				
	
	memcpy(&BufTmp[BufLen + i],(uint8 *)&CrcCheck,sizeof(CrcCheck));	

	SendCom4(BufTmp,i + BufLen + sizeof(CrcCheck));					
}


uint8	l_SpeedDataConFlg =0;    


extern int			l_programCom4Num ; 
extern  unsigned	int	l_recCom4Time ;  
extern  char		l_recCom4EndFlg ;
extern  char		l_Com4StartFlg ;
//------------------------------------------------------------------------
// ��    �ƣ�   void    BroadsDist(void)	
// ��    �ܣ�   �㲥�����Ϣ
// ��ڲ�����	��
// ���ڲ�����	��
//------------------------------------------------------------------------
void    BroadsDist(void)
{
	static	uint32	Time  = 0;
	static	uint32	Times = 0;
	uint32	len;
	uint8	Buf[64];
	
	if(GetOpenCom4Flg())
	{
		if( GetSysTime() - Time > 100)		//0.25s�㲥һ��
		{
			InitDistData();
	
			SendBroadInfo((uint8 *)&g_sDist,sizeof(stcDist),BROAD_INFO);	
	
			Time = GetSysTime();											//����ǰֵ
			
			Times++;
			// 
		}
		
		len = ReadCom4(Buf,sizeof(Buf)); 
		if(len)	
		{
			l_SpeedDataConFlg = 1;
			ClearRecCom4EndFlg();    
			Times = 0;
		}
		else
		{
			if(Times > 3)
			{
				 l_SpeedDataConFlg = 0;
			}
		}
	}
}

uint8	GetSpeedDataConFlg(void)
{
	return	  l_SpeedDataConFlg;
}
