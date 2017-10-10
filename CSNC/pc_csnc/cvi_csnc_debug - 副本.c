#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include "pc_csnr.h"
#include "cvi_com_operate.h"
#include "csnr_package_deal.h"
#include "cvi_file.h"
#include "senddata.h"
#include "setpara.h"
#include "oilrecord.h"
#include "databasesample.h"
#include "csnr_package_deal.h"


/********************************************************************************************/
/* Constants										    */
/********************************************************************************************/
//uir�ļ���
#define		UIR_MAIN_NAME			"csnc_debug.uir"

//��ӡ������Ϣ
//#define		RECV_PRINTF	    1


/********************************************************************************************/
/* Globals										    */
/********************************************************************************************/
//������
int 		gPanelHandle;										//װ�������

static int		gthreadFunctionId1;									//�̱߳��
static int 		gExiting;											//�߳��˳�

int		gSendDataFlg = 0;

//���ڽṹ�弰ͨѶ
extern	stcUartConfig   	gsCom1Config;								//����1����
extern	stcUartContrl		gsCom1Contrl;								//����1ͨѶ
extern	stcCsnrProtocolPara	gsCsnrProtocolPara;							//Э��˿�

typedef 	struct	_stcmainpara_
{
	int		sendfmt;
	int		recvfmt;
}stcmainpara;

stcmainpara		gsmainpara;

/********************************************************************************************/
/* local Prototypes																				*/
/********************************************************************************************/
int		GetMainPanelHandle(void);										//�����
void 	printftext(int	panel,int id,const char *format, ...);
void 	CloseCom1(void);
char	WriteMainPanelData_File(void) ;
int		ReadMainPanelData_File(void) ;
static int 	CVICALLBACK ThreadFunction1 (void *functionData);
extern	void	SendTextBox(void);
/********************************************************************************************/
/* Application entry point.																	*/
/********************************************************************************************/
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)					/* Initialize CVI libraries */
		return -1;	/* out of memory */

    gPanelHandle 		= LoadPanel (0, UIR_MAIN_NAME, PANEL);

	gExiting = 0;									//�����߳�
	/* Schedule two thread functions */				//���̣߳��������̺߳���
	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE,
								   ThreadFunction1, NULL,
								   &gthreadFunctionId1);
	/* 	Display the panel and run the UI */
	DisplayPanel (gPanelHandle);

	RunUserInterface ();

	CloseCom1();
	WriteMainPanelData_File();

	gExiting = 1;									//�߳��˳�
	/* Wait for the thread functions to finish executing */
	CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,
											gthreadFunctionId1,
											OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
	/* Release thread functions */
	CmtReleaseThreadPoolFunctionID (DEFAULT_THREAD_POOL_HANDLE, gthreadFunctionId1);

	/*	Discard the loaded panels from memory */
	DiscardPanel (gPanelHandle);

	return 0;
}





#define		COMM_DATA_BUF_SZIE	512
#define		MASTE_ADDR_HOST		0xCA


struct stcRecvCtrl
{
	unsigned char		DestAddr;			//���յ�ַ		slave  =0xA1\A2
	unsigned char		SourceAddr; 		//Դ��ַ		master	 =0x80
	unsigned char		FramNum;			//֡���
	unsigned char		Len;				//������Ч���ݳ���
	unsigned char		FrameCode;			//֡������
	unsigned char		recvflg;
	unsigned char		Tmp[2];
	unsigned int		DataCode;			//���տ�����
} ;

struct stcConnCtrl
{
	unsigned char		ConnFlg;		//���ӿ���, 1���������ӣ�0������������
	unsigned char		RecvEndFlg; 	//���ձ�ʾ��1�����ݽ�����ɣ�0�������ݽ��ա�
	unsigned char		TimeOut;		//��ʱʱ�䣬��λ1s
	unsigned char		ErrFlg; 		//�����ʾ������������0�����Ӵ���1
	unsigned char		DestAddr;		//���յ�ַ		   slave  =0xCA
	unsigned char		sourceaddr; 	//Դ��ַ		   master	=0x80
	unsigned char		SendFramNum;	//֡���
	unsigned char		SendFlg;		//���ͱ�ʾ�� �����ݷ��ͣ�1�������ݷ��ͣ�0
	unsigned char		FrameCode;			//֡������
	int					sendlen;		//�������ݳ���
} ;

typedef struct _stcThreadCommCtrl_
{
	char		wr[COMM_DATA_BUF_SZIE];			//������
	char		rd[COMM_DATA_BUF_SZIE];			//������

	stcRecvCtrl	RxCtrl;							//���տ���
	stcConnCtrl	ConnCtrl;						//���Ϳ���
} stcThreadCommCtrl;


//----------------------------------------------------------------------------
// ��    �ܣ� ��ʼ���շ��߳�
// ��    ����
// ��    �ߣ� redmornigcn 20170927
//----------------------------------------------------------------------------
int	cvi_InitBspComm(void)
{
	if(gthreadFunctionId1 == 0)					//��ʼ���շ������߳�
	{
		CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,
												gthreadFunctionId1,
												OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
	}
	return	gthreadFunctionId1;
}


//----------------------------------------------------------------------------
// ��    �ܣ� ���ڵ��ա�������.ͨ����ʶλ���̣߳����Ƴ����շ�Э��
// ��    ����
// ��    �ߣ� redmornigcn 20170927
//----------------------------------------------------------------------------
static int CVICALLBACK Thread_CommRecvAndSendCtrl (void *functionData)
{
	char recbuf[COMM_DATA_BUF_SZIE];
	char sendbuf[COMM_DATA_BUF_SZIE];
	int	 reclen;
	stcCsnrProtocolPara sCsncPara;		//csns�Y��

	while (!gComThreadExiting)									//�߳��˳�
	{
////////////////////////////////////////////////////////////////����
		if(cvi_GetComRecEndFlg(&sCtrl.sCom1))					//�������ݣ�����ʾ
		{
			cvi_ClearComRecEndFlg(&sCtrl.sCom1);				//��ս��ձ�ʶ
			reclen = cvi_ReadCom(&sCtrl,recbuf,sizeof(recbuf));	//ȡ��������

			sCsncPara.rxtxbuf 	= (uint8 *)recbuf;				//ԭʼ����
			sCsncPara.rxtxlen 	= reclen;						//ԭʼ���ݳ���
			sCsncPara.databuf	= sCtrl.sThreadCom1.rd;			//����������

			char	flg = DataUnpack_CSNC(&sCsncPara);			//�������

		    if ( MASTE_ADDR_HOST != sCsncPara.destaddr || flg == 0 ) //�������󡢵�ַ�����˳���
		    {
				sCtrl.PC.RxCtrl.recvflg = 0;					//����ʧ��
	        	return 0;
	        }
	        else
	        {
				sCtrl.PC.RxCtrl.SourceAddr 	= sCsncPara.sourceaddr; 			//Դ��ַ
				sCtrl.PC.RxCtrl.DestAddr	= sCsncPara.destaddr;				//Ŀ���ַ
				sCtrl.PC.RxCtrl.FramNum		= sCsncPara.framnum;				//���յ���֡��
				sCtrl.PC.RxCtrl.FrameCode	= (char)sCsncPara.framcode & (0x0f);//ָ���� ������װ��δ����
				sCtrl.PC.RxCtrl.Len			= sCsncPara.datalen;				//����������

				sCtrl.PC.RxCtrl.recvflg 	= 1;								//���ճɹ�( ��Ӧ�ò�ӿ� )
	        }
		}

////////////////////////////////////////////////////////////////���ݷ���
		if(
					sCtrl.sCom1.sConfig.open 	== 1			//�����Ѵ�
				&&  sCtrl.PC.ConnCtrl.SendFlg 	!= 0			//���ݷ��������ݷ���
		  )
		{
			sCsncPara.databuf	= sCtrl.PC.wr;					//����������
			sCsncPara.datalen	= sCtrl.PC.ConnCtrl.sendlen;	//��������������
			sCsncPara.rxtxbuf 	= (uint8 *)sendbuf;				//

			sCsncPara.sourceaddr= sCtrl.PC.ConnCtrl.sourceaddr;	//��ַ
			sCsncPara.destaddr  = sCtrl.PC.ConnCtrl.DestAddr;
			sCsncPara.framnum   = sCtrl.PC.ConnCtrl.SendFramNum;
			sCsncPara.framcode	= sCtrl.PC.ConnCtrl.FrameCode;

			DataPackage_CSNC(&sCsncPara);						//��csncЭ����

			cvi_SendCom(&sCtrl.sCom, sCsncPara.rxtxbuf, sCsncPara.rxtxlen)//ͨ�����ڷ�������

			sCtrl.PC.ConnCtrl.SendFlg 	= 0;					//�÷���λ��Ч.���η��ͽ���()
		}
	}

	return 0;
}


/********************************************************************************************/
/* PanelCB ():  Quits this application.														*/
/********************************************************************************************/
int CVICALLBACK PanelCB (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
		}
	return 0;
}

/********************************************************************************************/
/* PanelCB ():  Quits this application.														*/
/********************************************************************************************/
int CVICALLBACK Quit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			QuitUserInterface(0);
			break;
		}
	return 0;
}


int CVICALLBACK SetSendFlgCallback (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			gSendDataFlg = 1;
			break;
	}
	return 0;
}


int CVICALLBACK ClearTextBoxCallback (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			 ResetTextBox (panel, PANEL_INFOTEXTBOX, "");
			break;
	}
	return 0;
}


int   g_systime = 0;


/********************************************************************************************/
/*CVI_COM_Tick_Handl���ڴ�����,��com��弰�������¼�							            */
/********************************************************************************************/
void CVI_COMPanelHandle(int panel)
{
	static	int	times = 0;

	if(times == 0)
	{
		times++;
		if(ReadCom1ConfigFromFile(&gsCom1Config))	//�紮�������ļ���Ч��ֱ�Ӵ�
		{
			Com1Config();							//���ô���
		}
		ReadMainPanelData_File();
	}

	if(gsCom1Config.open == 1)	  //���ڴ�
	{
		SetCtrlAttribute (panel, PANEL_OPEN_BIN_FILE, ATTR_DIMMED, 0);
		SetCtrlAttribute (panel, PANEL_COM1_SEND_DATA, ATTR_DIMMED, 0);
		SetCtrlAttribute (panel, OPEN_COM1_PANEL, ATTR_LABEL_TEXT, "�رմ���");
	}

	if(gsCom1Config.open == 0)	  //���ڹر�
	{
		SetCtrlAttribute (panel, PANEL_OPEN_BIN_FILE, ATTR_DIMMED, 1);
		SetCtrlAttribute (panel, PANEL_COM1_SEND_DATA, ATTR_DIMMED, 1);
		SetCtrlAttribute (panel, OPEN_COM1_PANEL, ATTR_LABEL_TEXT, "�򿪴���");
	}
}

/********************************************************************************************/
/*CVI_Set_Data_Handl���ڴ�����,����������ؾ��								            */
/********************************************************************************************/
void CVI_SentDataHandle(int panel)
{
	static	int	autotime = 0;
	double	autoflg = 0;
	int		i= 10;


	GetCtrlVal (panel, PANEL_TIMER, &autoflg);	//ȡ�Զ����ͱ�ʾ

	GetCtrlAttribute (panel, PANEL_TIMER, ATTR_INTERVAL,&autoflg );

	if( autoflg < 0.01)				//�Զ�����
	{
		if(autotime < 10 )		//10ms
		{
			GetCtrlVal (panel, PANEL_DELAYMS, &autotime);
			gSendDataFlg = 1;	//���ñ�ʾ��1
		}
		else
		{
			i = 10;
			while(i--)
				autotime--;
		}
	}else
	{
		 autotime = 0;
	}

////////////////////////////////////////////////////////////ȡЭ���ڵ�ֵ
	GetCtrlVal(panel,PANEL_SOURCEADDR,&gsCsnrProtocolPara.sourceaddr);
	GetCtrlVal(panel,PANEL_DESTADDR,&gsCsnrProtocolPara.destaddr);
	GetCtrlVal(panel,PANEL_FRAMNUM,&gsCsnrProtocolPara.framnum);
	GetCtrlVal(panel,PANEL_FRAMCODE,&gsCsnrProtocolPara.framcode);

////////////////////////////////////////////////////////////������
	GetCtrlVal(panel,PANEL_SENDFMT,&gsmainpara.sendfmt);
	GetCtrlVal(panel,PANEL_RECVFMT,&gsmainpara.recvfmt);
}

void	DisplayTimeOnSetPanel(void);
/********************************************************************************************/
/*CVI_SetPara���ڲ���������ؾ��												            */
/********************************************************************************************/
void CVI_SetParaHandle(int panel)
{
	DisplayTimeOnSetPanel();
	Com_SetParaTask();									//���ڷ��Ͳ�������ָ��
}

int  InsertoilRecToDBTable (void);
/********************************************************************************************/
/*CVI_SetPara���ڲ���������ؾ��												            */
/********************************************************************************************/
void CVI_DataBaseHandle(int panel)
{
	if(gsRecvOilRecordCtrl.storeflg == 1 && db_panelHandle)
	{
		gsRecvOilRecordCtrl.storeflg = 0;

		InsertoilRecToDBTable();
	}

	if(db_panelHandle && gsRecvOilRecordCtrl.ICflg == 1)
	{
		SetCtrlVal(db_panelHandle,SETM_PANEL_CUR_NUM,gsRecvOilRecordCtrl.currecnum);
		SetCtrlVal(db_panelHandle,SETM_PANEL_ICREAD_NUM,gsRecvOilRecordCtrl.ICreadnum);
	}
}

/********************************************************************************************/
/*��嶨ʱ������																            */
/********************************************************************************************/
int CVICALLBACK SysTickCallback (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{

	switch (event)
	{
		case EVENT_TIMER_TICK:

			g_systime++;									   	//ϵͳ��ʱ

///////////////////////////////////////////////////////////////////������崦��
			CVI_COMPanelHandle(panel);

//////////////////////////////////////////////////////////////////�������ݷ��������ش���
			CVI_SentDataHandle(panel);

//////////////////////////////////////////////////////////////////�������ݷ��������ش���
			CVI_SetParaHandle(panel);

//////////////////////////////////////////////////////////////////�������ݷ��������ش���
			CVI_DataBaseHandle(panel);

			break;
	}
	return 0;
}


//�����������
#define	MNAI_PANEL_FILE	"cvi_main_myinfo.ini"
typedef	struct	_stcmianpanelpara{
	struct	_csnchead_
	{
		char	source;
		char	denst;
		char 	code;
		char	framenum;
	}stccsnchead ;

	char	sentbuf[256];
}stcmianpanelpara;

//������
stcmianpanelpara	smainpanelpara;

/********************************************************************************************/
/* �������ļ�д������					                							    	*/
/********************************************************************************************/
char	WriteMainPanelData_File(void)
{
	char	dirstring[512];
	char	filestring[512];
	int		filehandle;

	GetCtrlVal(gPanelHandle,PANEL_COM1SENDTEXTBOX,smainpanelpara.sentbuf);
	GetCtrlVal(gPanelHandle,PANEL_SOURCEADDR,&smainpanelpara.stccsnchead.source);
	GetCtrlVal(gPanelHandle,PANEL_DESTADDR,&smainpanelpara.stccsnchead.denst);
	GetCtrlVal(gPanelHandle,PANEL_FRAMCODE,&smainpanelpara.stccsnchead.code);
	GetCtrlVal(gPanelHandle,PANEL_FRAMNUM,&smainpanelpara.stccsnchead.framenum);

	GetProjectDir (dirstring);
	MakePathname (dirstring, MNAI_PANEL_FILE, filestring);

	filehandle = OpenFile (filestring, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_BINARY);
	SetFilePtr (filehandle, 0, 0);

	WriteFile(filehandle,(char *)&smainpanelpara,sizeof(smainpanelpara));

	CloseFile(filehandle);

	return 1;
}

/********************************************************************************************/
/* �������ļ��ж�ȡ����					                								*/
/********************************************************************************************/
int	ReadMainPanelData_File(void)
{
	char	dirstring[512];
	char	filestring[512];
	int		filehandle;
	int		datalen = 0;

	GetProjectDir (dirstring);
	MakePathname (dirstring, MNAI_PANEL_FILE, filestring);

	filehandle = OpenFile (filestring, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_BINARY);
	SetFilePtr (filehandle, 0, 0);
	datalen = ReadFile(filehandle,(char *)&smainpanelpara,sizeof(smainpanelpara));

	CloseFile(filehandle);

	SetCtrlVal(gPanelHandle,PANEL_COM1SENDTEXTBOX,smainpanelpara.sentbuf);

	SetCtrlVal(gPanelHandle,PANEL_SOURCEADDR,smainpanelpara.stccsnchead.source);
	SetCtrlVal(gPanelHandle,PANEL_DESTADDR,smainpanelpara.stccsnchead.denst);
	SetCtrlVal(gPanelHandle,PANEL_FRAMCODE,smainpanelpara.stccsnchead.code);
	SetCtrlVal(gPanelHandle,PANEL_FRAMNUM,smainpanelpara.stccsnchead.framenum);

	return datalen;
}

