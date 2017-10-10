#ifndef  _CardDataTypeDef_h_
#define  _CardDataTypeDef_h_

typedef struct _stcFixInfo    //�豸�����Ϣ 	        //����ʱ���� 28
{
    unsigned char   LocoTyp;            //�����ͺ�          1   
    unsigned char   E_LocoTyp;          //�����ͺ���չ�ֽ�  1   �����ͺ������ֽ�,������LKJ2000��ʽ��ͬ
    unsigned char   LocoNum[2];         //������            2
	unsigned char   FixDate[6];         //��װ����			6	"20090303"
	unsigned char   FixUint[4];         //��װ��λ			4	"GTZJ"	
	unsigned char   ReFixDate[6];       //��װ����			6	"20190303"   
	unsigned char   ReFixUint[4];      	//��װ��λ			4	"GTGJ"	
    unsigned short 	Reserve0;   		//Ԥ��				2
	unsigned short  CrcCheck;           //У���            2   
}stcFixInfo;

//ʱ��----------------------------------------
typedef		struct	_stcTime_
{
	unsigned char	Sec;
	unsigned char	Min;
	unsigned char	Hour;
	unsigned char	Date;
	unsigned char	Month;
	unsigned char	Year;
	unsigned short	CrcCheck;
}stcTime;

typedef struct _stcCardID_						//IC ��ID��	  32
{
	unsigned char	ProductNum[12];						//IC�����
	unsigned char	DriverNum[4];						//˾������
	unsigned char	DriverUnit[8];						//˾�����ڵ�λ���
	unsigned char	Reserve0[4];						//Ԥ��0
	unsigned short	Reserve1;	 						//Ԥ��1
	unsigned short	CrcCheck;							//CRC����
}stcCardID;

typedef struct _stcCardType_					//IC ������ 8
{
	unsigned short	CardType[3];						//IC������
	unsigned short	CrcCheck;							//CRC����
}stcCardType;

typedef struct _stcDensity_						//IC ���ܶ� 24
{
	unsigned char		LocoType[2];					//�����ͺ�
	unsigned char		LocoNum[2];						//�������
	//unsigned char		DriverNum[4];					//˾�����
	unsigned int		DenChageTimes;					//�ܶȱ仯��
	unsigned int		Density;						//�����ܶ�
	unsigned int		AddOilStationNum;				//���ͻ����
	unsigned int		AddOilerNum;					//������Ա���	
	unsigned short		AddVol;							//��������
	unsigned short		CrcCheck;						//CRC����
}stcDensity;

typedef struct _stcModPot_						//IC ��cd����
{
	unsigned short	HigVal;								//ѹ��ֵ
	unsigned short	DipVal;								//����ֵ
}stcModPot;

typedef struct _stcModel_						//����ģ�͸������ݽṹ
{
	stcModPot	sModPot[50];					//����ģ��
	unsigned short		PotNum;							//����ģ�͵�ĸ���	
	unsigned short		StartHight;						//��ʼ�߶�
	unsigned char		ModelNum;						//ģ�ͱ��
	unsigned char		Reserve0;						//Ԥ��    //100919   0x01��ʾʹ�ã�0x00��ʾδ��
	unsigned short		CrcCheck;						//CRCУ��
}stcModel;

typedef struct _stcModelSemple_					//����ģ�ͼ����ݽṹ
{
	stcModPot	sModPot[10];					//����ģ��
	unsigned short		PotNum;							//����ģ�͵�ĸ���	
	unsigned short		StartHight;						//��ʼ�߶�
	unsigned char		ModelNum;						//ģ�ͱ��
	unsigned char		Reserve0;						//Ԥ��	//100919	0x01��ʾʹ�ã�0x00��ʾδ��	
	unsigned short		CrcCheck;						//CRCУ��
}stcModelSemple;

typedef struct _stcModelComplex_				//����ģ�ͼ����ݽṹ
{
	stcModPot	sModPot[200];					//����ģ��
	unsigned short		PotNum;							//����ģ�͵�ĸ���	
	unsigned short		StartHight;						//��ʼ�߶�
	unsigned char		ModelNum;						//ģ�ͱ��
	unsigned char		Reserve0;						//Ԥ��
	unsigned short		CrcCheck;						//CRCУ��
}stcModelComplex;

typedef struct _stcModelCard_					//����ģ�͸������ݽṹ
{
	stcModel		sModel[18];					//��������ģ��
	stcModelSemple	sModelSemple[46];			//������ģ��
	unsigned short			Reserve0;					//Ԥ��
	unsigned short			CrcCheck;					//CRCУ��	
}stcModelCard;

typedef struct _stcReviseHight_					//IC �����߶�
{
	short		ReviseHight;					//�����߶���ʼ�߶�
	unsigned short		CrcCheck;						//CRCУ��
}stcReviseHight;

typedef struct _stcAddDipVal_					//IC ��������
{
	short		AddDipVal;						//��������
	unsigned short		CrcCheck;						//CRCУ��
}stcAddDipVal;

typedef struct _stcDebugCard_					//
{
	unsigned short		DebugCode;						//���Դ���
	unsigned short		BufLen;							//��Ч���ݳ���
	unsigned char		Buf[100];						//������
	unsigned short		Reserve0;						//Ԥ��
	unsigned short		CrcCheck;						//CRCУ��
}stcDebugCard;

//����ģ�Ϳ�����
typedef struct _stcCardPara_ 					//IC ��cd���� 352
{
	stcDensity		sDensity;					//�ܶ�
	unsigned int			SetVal;						//����ֵ
	stcTime			sTime;						//ʱ��
 	stcReviseHight	sReviseHight;				//������ʼ�߶�
	stcFixInfo      sFixInfo;              	 	//װ����
	stcDebugCard	sDebugCard;					//���Կ�
	unsigned short			Reserve0;					//Ԥ��
	unsigned short			CrcCheck;					//CRC����
}stcCardPara;

typedef struct _stcCardIndex_					//IC ���ļ����� 36
{
	unsigned char			LocoType[2];				//�����ͺ�
	unsigned char			LocoNum[2];					//�������
	unsigned char			ProNum[16];					//��Ʒ���
	unsigned int			StartAddr;					//��ʼ��ַ
	unsigned int			EndAddr;					//������ַ
	unsigned short			RecLen;						//��¼����
	unsigned short			RecNum;						//��¼����	
}stcCardIndex;

typedef struct _stcCardFlg_						//IC ����ʶ
{
	stcCardID		sCardID;					//IC�� ID�ṹ
	stcCardType		sCardType;					//IC�� IC�����ͣ����ݿ����ܶȿ�����
	stcCardPara		sCardPara;					//IC�� ������
	stcCardIndex	sCardIndex[100];			//IC�� �ļ�����
	unsigned short			WriteCardTimes;				//��д������
	unsigned short			CrcCheck;					//CRC����
}stcCardFlg;


#endif
