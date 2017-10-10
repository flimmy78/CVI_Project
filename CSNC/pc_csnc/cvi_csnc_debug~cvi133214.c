/*******************************************************************************
 *   Revised:        All copyrights reserved to redmorningcn.
 *   Revision:       v1.0
 *   Writer:	     redmorningcn.
 *   Date:			 20170927
 *   Description:
 *   Notes:
 *     				E-mail: redmorningcn@qq.com
 *
 *******************************************************************************/

/********************************************************************************************/
/* Include files																			*/
/********************************************************************************************/
#include <formatio.h>
#include "csnc_debug.h"
#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>

#include "cvi_csnc_debug.h"	

/********************************************************************************************/
/* Constants										    */
/********************************************************************************************/

/********************************************************************************************/
/* Globals										    	*/
/********************************************************************************************/
//������
stcmianpanelpara	smainpanelpara;
int 				gCsncDebugPanelHandle;										//װ�������

/********************************************************************************************/
//local function
/********************************************************************************************/
int		ReadMainPanelData_File(void);
char	WriteMainPanelData_File(void);

/********************************************************************************************/
//�ز���ʾ��塣	redmoringcn 20170927						            
/********************************************************************************************/
void LoadCsncDebugPanel(void)
{
    gCsncDebugPanelHandle = LoadPanel (0, UIR_CSNCDEBUG_NAME, CSNC_DEBUG);
	/* 	Display the panel and run the UI */
	DisplayPanel (gCsncDebugPanelHandle);
	
	ReadMainPanelData_File();
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
			
			WriteMainPanelData_File();													//��������
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
			
			WriteMainPanelData_File();													//��������
			QuitUserInterface(0);
			break;
		}
	return 0;
}


int CVICALLBACK SetSendFlgCallback (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	char	buf[512];
	switch (event)
	{
		case EVENT_COMMIT:
			
			WriteMainPanelData_File();													//��������(��ÿ���������)
			
			GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_COM1SENDTEXTBOX,buf); 			//ȡ������������

			if(smainpanelpara.sendpara.sendstyle == 1) 									//HEX
			{
				sCtrl.PC.ConnCtrl.sendlen = strtohex(buf,sCtrl.PC.wr);					//װ����HEX
			}
			else
			{
				strcpy(sCtrl.PC.wr,buf);												//string��ʽ
				sCtrl.PC.ConnCtrl.sendlen = strlen(buf);
			}
			
			sCtrl.PC.ConnCtrl.DestAddr 		= smainpanelpara.stccsnchead.denst;			//Դ��ַ
			sCtrl.PC.ConnCtrl.sourceaddr 	= smainpanelpara.stccsnchead.source;		//Ŀ���ַ
			sCtrl.PC.ConnCtrl.FrameCode		= smainpanelpara.stccsnchead.code;			
			sCtrl.PC.ConnCtrl.SendFramNum 	= smainpanelpara.stccsnchead.framenum;		
			
			sCtrl.PC.ConnCtrl.SendFlg		= 1;										//��������
			
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

			 ResetTextBox (panel, CSNC_DEBUG_INFOTEXTBOX, "");
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
	uint8	buf[512];

	if((times++ %200) == 1)
		WriteMainPanelData_File();				//����ˢ�����״̬
	
	if(sCtrl.PC.sCom->sConfig.open == 1)	  	//���ڴ�
	{				
		SetCtrlAttribute (panel, CSNC_DEBUG_COM1_SEND_DATA, ATTR_DIMMED, 0);
		SetCtrlAttribute (panel, CSNC_DEBUG_OPENCOM1, ATTR_LABEL_TEXT, "�رմ���");
	}

	if(sCtrl.PC.sCom->sConfig.open == 0)	  	//���ڹر�
	{
		SetCtrlAttribute (panel, CSNC_DEBUG_COM1_SEND_DATA, ATTR_DIMMED, 1);
		SetCtrlAttribute (panel, CSNC_DEBUG_OPENCOM1, ATTR_LABEL_TEXT, "�򿪴���");
	}
	
	if(sCtrl.PC.RxCtrl.recvflg)					//��Э�����ݽ��գ���ӡЭ����Ϣ
	{
		sprintf(buf,"Դ��ַ ��%02x \r\nĿ���ַ��%02x\r\n֡��ţ�%02d\r\n֡���%02x")
		printftext(panel,CSNC_DEBUG_INFOTEXTBOX,	
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

//////////////////////////////////////////////////////////////////������崦��
			CVI_COMPanelHandle(panel);

			break;
	}
	return 0;
}



/********************************************************************************************/
/* �������ļ�д������					                							    	*/
/********************************************************************************************/
char	WriteMainPanelData_File(void)
{
	char	dirstring[512];
	char	filestring[512];
	int		filehandle;

	GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_COM1SENDTEXTBOX,smainpanelpara.sentbuf);
	GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_SOURCEADDR,&smainpanelpara.stccsnchead.source);
	GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_DESTADDR,&smainpanelpara.stccsnchead.denst);
	GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_FRAMCODE,&smainpanelpara.stccsnchead.code);
	GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_FRAMNUM,&smainpanelpara.stccsnchead.framenum);

	GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_SENDFMT,&smainpanelpara.sendpara.sendstyle);  
	GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_RECVFMT,&smainpanelpara.sendpara.recvstyle);          
	GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_DELAYMS,&smainpanelpara.sendpara.autotime);          
	//GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_SENDFMT,&smainpanelpara.sendpara.sendstyle);        
	if(smainpanelpara.sendpara.autotime)		  				//�Զ����ͱ�ʶ
		smainpanelpara.sendpara.autosend = 1;
	else
		smainpanelpara.sendpara.autotime = 0;
	
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

	if(datalen)	
	{
		SetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_COM1SENDTEXTBOX,smainpanelpara.sentbuf);

		SetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_SOURCEADDR,smainpanelpara.stccsnchead.source);
		SetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_DESTADDR,smainpanelpara.stccsnchead.denst);
		SetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_FRAMCODE,smainpanelpara.stccsnchead.code);
		SetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_FRAMNUM,smainpanelpara.stccsnchead.framenum);
	}
	
	return datalen;
}

