#include "mian.h"

#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>

#include "csnc_debug.h"   

#include "cvi_com1.h"  
#include "cvi_csnc_debug.h"  
#include "cvi_config.h"   

/********************************************************************************************/
/* Constants										    */
/********************************************************************************************/
//uir�ļ���
#define		UIR_MAIN_NAME			"csnc_debug.uir"

/********************************************************************************************/
/* Globals										    */
/********************************************************************************************/
//������
stcSysCtrl	sCtrl;		
int			gmainPanel  = 0;						//ȫ��������

/********************************************************************************************/
/* Application entry point.																	*/
/********************************************************************************************/
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)				/* Initialize CVI libraries */
		return -1;	/* out of memory */
	
	gmainPanel = LoadPanel (0, "mian.uir", MAINPANEL); //װ�������

	DisplayPanel (gmainPanel);

	RunUserInterface ();
				
	/*	Discard the loaded panels from memory */
	DiscardPanel (gmainPanel);

	return 0;
}


int CVICALLBACK OpenDebugPanelCallback (int panel, int control, int event,
										void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			LoadCsncDebugPanel();
			
			/* Schedule two thread functions */				//���̣߳��������̺߳���
			cvi_InitBspComm(&sCtrl.PC);
			
			break;
	}
	return 0;
}


int CVICALLBACK mainPanelCB (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			cvi_CloseBspComm(&sCtrl.PC);					//�߳��˳� 			
			break;
	}
	return 0;
}
