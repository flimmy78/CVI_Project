/*******************************************************************************
 *   Filename:       cvi_senddata.c
 *   Revised:        All copyrights reserved to redmorningcn.
 *   Revision:       v1.0
 *   Writer:	     redmorningcn.
 *
 *   Description:    ����csncЭ���ʽ�������ݡ�
////////////////////////////////////////////////////////////////////////////
 *   Notes:
 *     				E-mail: redmorningcn@qq.com
 *
 *******************************************************************************/


/********************************************************************************************/
/* Include files																			*/
/********************************************************************************************/
#include <rs232.h>
#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include <formatio.h>
#include "cvi_file.h"



/********************************************************************************************/
/* Constants																				*/
/********************************************************************************************/
stcCsnrProtocolPara	gsCsnrProtocolPara;

unsigned char	senddatabuf[256];
unsigned char 	txdatabuf[256];

extern	int		gPanelHandle;
extern	int		gBinFilePanelHandle;
void 	printftext(int	panel,int id,const char *format, ...) ;

#define		MY_ADDR				0xca


/*******************************************************************************************
���ݷ��ͣ� SendCsnrDataDeal
�����ݰ���Э���ʽ���͡�
��ʽ��ֵ  	ϵͳ����ʱ�Զ����ġ�
*******************************************************************************************/
void	SendCsnrDataDeal(char *buf,int len)
{
	 int i;

	 gsCsnrProtocolPara.databuf =  senddatabuf;		//����ָ�븳ֵ
	 gsCsnrProtocolPara.rxtxbuf =  txdatabuf;

	 memcpy(gsCsnrProtocolPara.databuf,buf,len);	//���ݷ�ֵ
	 gsCsnrProtocolPara.datalen = (unsigned char)len;

	 DataPackage_CSNC(&gsCsnrProtocolPara);			//��Э����

	 SendCom1(gsCsnrProtocolPara.rxtxbuf,gsCsnrProtocolPara.rxtxlen);  //�Ӵ���������


#ifdef	SEND_PRINTF
	for (i = 0 ;i < gsCsnrProtocolPara.rxtxlen; i++)
	{
		printftext(gBinFilePanelHandle,IAP_PANEL_INFOTEXTBOX,"%02X",gsCsnrProtocolPara.rxtxbuf[i]);
	}
	printftext(gBinFilePanelHandle,IAP_PANEL_INFOTEXTBOX,"\r\n");
#endif
}


/*******************************************************************************************
���ݷ��ͣ� ȡSENDTEXTBOX�����ݣ���stringת��Ϊhex
*******************************************************************************************/
int	GetSendTextBoxData(uint8 buf[])
{
	uint8	string[512];
	uint32	len,i,j;
	uint8	charbuf[4]="0";

	GetCtrlVal(gPanelHandle,PANEL_COM1SENDTEXTBOX,string);  	//ȡ�ؼ��е�����

	len = 0;

	for(i = 0,j = 0;i<strlen(string);i++)							//����ת��
	{
		if(string[i] == ' ')								//ȥ�ո�
		{
			continue;
		}else{

			//�жϷǷ��ַ�
			charbuf[j%2] = string[i];
			if(j %2 == 1)
			{
			    charbuf[2] 	= 	'\0';
				buf[len++]  = 	(unsigned char)strtol(charbuf,NULL,16);
			}
			j++;
		}
	}

	return	len;
}

/*******************************************************************************************
���ݷ��ͣ� SendTextBox()
����SENDTEXTBOX������
*******************************************************************************************/
void	SendTextBox(void)
{
	char	buf[256];
	int		len;

	len = GetSendTextBoxData(buf);		//ȡ������ֵ

	SendCsnrDataDeal(buf,len);
}

