

#define		FALSE	0
#define		TRUE	1

#include	"FIFO.h"

//------------------------------------------------------------------------
// ��    �ƣ�   InitFIFO(stcFIFO *FIFO,unsigned char *Buf,unsigned short Size)
// ��    �ܣ�   ��ʼ��FIFO������
// ��ڲ�����	FIFO:Ҫ��ʼ����FIFO�������ṹ,Buf:��������ַ,SIZE:��������С
// ���ڲ�����
//------------------------------------------------------------------------
void   InitFIFO(stcFIFO *FIFO,unsigned char *Buf,unsigned short Size)
{
    FIFO->pWrite = 0;
    FIFO->pRead  = 0;
    FIFO->Size   = Size;
    FIFO->BufAddr= Buf;
}

//------------------------------------------------------------------------
// ��    �ƣ�   void   ClearFIFO(stcFIFO *FIFO)
// ��    �ܣ�   ��ʼ��FIFO������
// ��ڲ�����	FIFO:Ҫ��ʼ����FIFO�������ṹ,Buf:��������ַ,SIZE:��������С
// ���ڲ�����
//------------------------------------------------------------------------
void   ClearFIFO(stcFIFO *FIFO)
{
    FIFO->pWrite = 0;
    FIFO->pRead  = 0;	
}

//------------------------------------------------------------------------
// ��    �ƣ�   unsigned short  GetNoReadSizeFIFO(stcFIFO *FIFO)
// ��    �ܣ�   ��FIFO��������δ���ռ��С
// ��ڲ�����	FIFO:��FIFO�������ṹ
// ���ڲ�����	������δ�����ݴ�С
//------------------------------------------------------------------------
unsigned short  GetNoReadSizeFIFO(stcFIFO *FIFO)
{
    unsigned short  pWrite = FIFO->pWrite;
    unsigned short  pRead  = FIFO->pRead;
    unsigned short  pBufSize = FIFO->Size;

	
    return (pWrite + pBufSize - pRead) % pBufSize;
}

//------------------------------------------------------------------------
// ��    �ƣ�   unsigned short  GetNoWriteSizeFIFO(stcFIFO *FIFO)
// ��    �ܣ�   ��FIFO��������δд�ռ��С
// ��ڲ�����	FIFO:��FIFO�������ṹ
// ���ڲ�����	������δд���ݴ�С
//------------------------------------------------------------------------
unsigned short  GetNoWriteSizeFIFO(stcFIFO *FIFO)
{
    unsigned short  pBufSize = FIFO->Size;
    return  pBufSize - GetNoReadSizeFIFO(FIFO);
}

//------------------------------------------------------------------------
// ��    �ƣ�   unsigned char   WriteFIFO(stcFIFO *FIFO,unsigned char *DatBuf,unsigned short DatLen)
// ��    �ܣ�   дFIFO������
// ��ڲ�����	FIFO:��FIFO�������ṹ,DatBuf:д������,DatLen:���ݳ���
// ���ڲ�����	дFIFO�������ɹ�����1,���󷵻�0
//------------------------------------------------------------------------
unsigned char   WriteFIFO(stcFIFO *FIFO,unsigned char *DatBuf,unsigned short DatLen)
{
    unsigned short  i,j;
    unsigned short  NoWriteSize;
    unsigned short  pWrite = FIFO->pWrite;
    unsigned short  pBufSize = FIFO->Size;
    unsigned char   *FIFOBuf = FIFO->BufAddr;

    NoWriteSize =  GetNoWriteSizeFIFO(FIFO);   		//ȷ�ϻ�������û��δд�ռ��С

    if(DatLen >= NoWriteSize)
    {
        return  FALSE;                     	 		//���ݱȻ�����ʣ��ռ��,����д����
    }

    for(i = 0; i < DatLen; i++)             		//������д�뻺����
    {
        j =(pWrite + i) % pBufSize;         		//����д��ַ

        FIFOBuf[j] =  DatBuf[i];            		//д����
    }

    FIFO->pWrite = (pWrite + DatLen ) % pBufSize; 	//дָ��++

    return  TRUE;
}

//------------------------------------------------------------------------
// ��    �ƣ�   unsigned char   ReadFIFO(stcFIFO *FIFO,unsigned char *DatBuf,unsigned short DatLen)
// ��    �ܣ�   ��FIFO������
// ��ڲ�����	FIFO:��FIFO�������ṹ,DatBuf:�����ݻ���λ��,DatLen:Ҫ�����ݳ������ݳ���
// ���ڲ�����	��FIFO�������ɹ�1,����Ϊ0
//------------------------------------------------------------------------
unsigned char   ReadFIFO(stcFIFO *FIFO,unsigned char *DatBuf,unsigned short DatLen)
{
    unsigned short  i,j;
    unsigned short  NoReadSize;
    unsigned short  pRead = FIFO->pRead;
    unsigned short  pBufSize = FIFO->Size;
    unsigned char   *FIFOBuf = FIFO->BufAddr;

    NoReadSize = GetNoReadSizeFIFO(FIFO);      	//��δ���ռ��С


    if(DatLen > NoReadSize )                 	//���ռ�С��Ҫ������,���ض�ȡ����
    {
        return FALSE;
    }

    for(i = 0; i < DatLen; i++)
    {
        j = (pRead + i) % pBufSize;         	//�������ַ

        DatBuf[i] = FIFOBuf[j];             	//��ȡ����		
    }

    FIFO->pRead = (pRead + DatLen) % pBufSize;  //��ָ��++

    return  TRUE;
}


//------------------------------------------------------------------------
// ��    �ƣ�   unsigned char   ReadFIFO(stcFIFO *FIFO,unsigned char *DatBuf,unsigned short DatLen)
// ��    �ܣ�   ��FIFO������
// ��ڲ�����	FIFO:��FIFO�������ṹ,DatBuf:�����ݻ���λ��,DatLen:Ҫ�����ݳ������ݳ���
// ���ڲ�����	��FIFO�������ɹ�1,����Ϊ0
//------------------------------------------------------------------------
unsigned char   OnlyReadFIFO(stcFIFO *FIFO,unsigned char *DatBuf,unsigned short DatLen)
{
    unsigned short  i,j;
    unsigned short  NoReadSize;
    unsigned short  pRead = FIFO->pRead;
    unsigned short  pBufSize = FIFO->Size;
    unsigned char   *FIFOBuf = FIFO->BufAddr;

    NoReadSize = GetNoReadSizeFIFO(FIFO);      	//��δ���ռ��С

    if(DatLen > NoReadSize )                 	//���ռ�С��Ҫ������,���ض�ȡ����
    {
        return FALSE;
    }

    for(i = 0; i < DatLen; i++)
    {
        j = (pRead + i) % pBufSize;         	//�������ַ

        DatBuf[i] = FIFOBuf[j];             	//��ȡ����
    }

	//FIFO->pRead = (pRead + DatLen) % pBufSize;  //��ָ��++
	FIFO->pRead = (pRead ) % pBufSize;  //��ָ��++

    return  TRUE;
}
