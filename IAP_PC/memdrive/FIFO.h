#ifndef  _FIFO_h_
#define  _FIFO_h_


typedef	unsigned char  uint8;					/* �޷���8λ���ͱ���						*/
typedef	    	 char  int8;					/* �з���8λ���ͱ���						*/
//typedef	unsigned char  uint8;					/* �޷���8λ���ͱ���						*/
//typedef	signed	 char  int8;					/* �з���8λ���ͱ���						*/
typedef	unsigned short uint16;					/* �޷���16λ���ͱ���						*/
typedef	signed	 short int16;					/* �з���16λ���ͱ���						*/
typedef	unsigned int   uint32;					/* �޷���32λ���ͱ���						*/
typedef	signed	 int   int32;					/* �з���32λ���ͱ���						*/
typedef	float		   fp32;					/* �����ȸ�������32λ���ȣ�					*/
typedef	double		   fp64;					/* ˫���ȸ�������64λ���ȣ�					*/

typedef	unsigned char  UINT8;					/* �޷���8λ���ͱ���						*/
typedef	signed	 char  INT8;					/* �з���8λ���ͱ���						*/
typedef	unsigned short UINT16;					/* �޷���16λ���ͱ���						*/
typedef	signed	 short INT16;					/* �з���16λ���ͱ���						*/
typedef	unsigned int   UINT32;					/* �޷���32λ���ͱ���						*/
typedef	signed	 int   INT32;					/* �з���32λ���ͱ���						*/
typedef	float		   FP32;					/* �����ȸ�������32λ���ȣ�					*/
typedef	double		   FP64;					/* ˫���ȸ�������64λ���ȣ�					*/

typedef struct _stcFIFO                     //�����Ƚ��ȳ������������ݽṹ
{
    uint16  pWrite;                         //������дָ��
    uint16  pRead;                          //��������ָ��
    uint16  Size;                           //��������С
//	uint16	Rec;
    uint8   *BufAddr;                       //�������׵�ַ
}stcFIFO; 

//------------------------------------------------------------------------
// ��    �ƣ�   InitFIFO(stcFIFO *FIFO,uint8 *Buf,uint16 Size)
// ��    �ܣ�   ��ʼ��FIFO������
// ��ڲ�����	FIFO:Ҫ��ʼ����FIFO�������ṹ,Buf:��������ַ,SIZE:��������С
// ���ڲ�����
//------------------------------------------------------------------------
extern	void   InitFIFO(stcFIFO *FIFO,uint8 *Buf,uint16 Size);

//------------------------------------------------------------------------
// ��    �ƣ�   uint16  GetNoReadSizeFIFO(stcFIFO *FIFO)
// ��    �ܣ�   ��FIFO��������δ���ռ��С
// ��ڲ�����	FIFO:��FIFO�������ṹ
// ���ڲ�����	������δ�����ݴ�С
//------------------------------------------------------------------------
extern	uint16  GetNoReadSizeFIFO(stcFIFO *FIFO);

//------------------------------------------------------------------------
// ��    �ƣ�   uint16  GetNoWriteSizeFIFO(stcFIFO *FIFO)
// ��    �ܣ�   ��FIFO��������δд�ռ��С
// ��ڲ�����	FIFO:��FIFO�������ṹ
// ���ڲ�����	������δд���ݴ�С
//------------------------------------------------------------------------
extern	uint16  GetNoWriteSizeFIFO(stcFIFO *FIFO);

//------------------------------------------------------------------------
// ��    �ƣ�   uint8   WriteFIFO(stcFIFO *FIFO,uint8 *DatBuf,uint16 DatLen)
// ��    �ܣ�   дFIFO������
// ��ڲ�����	FIFO:��FIFO�������ṹ,DatBuf:д������,DatLen:���ݳ���
// ���ڲ�����	дFIFO�������ɹ�����1,���󷵻�0
//------------------------------------------------------------------------
extern	uint8   WriteFIFO(stcFIFO *FIFO,uint8 *DatBuf,uint16 DatLen);

//------------------------------------------------------------------------
// ��    �ƣ�   uint8   ReadFIFO(stcFIFO *FIFO,uint8 *DatBuf,uint16 DatLen)
// ��    �ܣ�   ��FIFO������
// ��ڲ�����	FIFO:��FIFO�������ṹ,DatBuf:�����ݻ���λ��,DatLen:Ҫ�����ݳ������ݳ���
// ���ڲ�����	��FIFO�������ɹ�1,����Ϊ0
//------------------------------------------------------------------------
extern	uint8   ReadFIFO(stcFIFO *FIFO,uint8 *DatBuf,uint16 DatLen);

//------------------------------------------------------------------------
// ��    �ƣ�   void   ClearFIFO(stcFIFO *FIFO)
// ��    �ܣ�   ��ʼ��FIFO������
// ��ڲ�����	FIFO:Ҫ��ʼ����FIFO�������ṹ,Buf:��������ַ,SIZE:��������С
// ���ڲ�����
//------------------------------------------------------------------------
extern	void   ClearFIFO(stcFIFO *FIFO);

extern	uint8   OnlyReadFIFO(stcFIFO *FIFO,uint8 *DatBuf,uint16 DatLen);
#endif                      
