// GostDemo.cpp : Defines the entry point for the console application.
//
/*
功能： 实现Gost加密算法的加密解密功能    						 
																	 
说明：该程序是基于Gost算法的一个针对文本文件的加密解密工具        
		  以512字节为操作单位										 
      使用时用户需输入5个参数									 
		参数1. exe文件名(例如gost)									 
			2. 操作类型(加密或解密,1代表加密,2代表解密)				 
			3. 读入数据的txt文件名 (例如 1.txt)						 	
			4. 写入操作结果的文件名(例如 2.txt)						 
			5. 128位密钥(为确保安全,请输入128位长的密钥  )			 		
	例: 加密:gost 1 1.txt 2.txt 12345678123456781234567812345678     
     解密:gost 2 2.txt 3.txt 12345678123456781234567812345678     	
																	 
Copyright(c) 2002 by xi'an wuzhen(吴真)				             
																     
																     
版本号：1.0.0(6/5/2002)											 
																	 
																	 
AUTHOR:吴真(WUZHEN)												 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <pmalloc.h>

typedef  int INT32;
typedef  char INT8;
typedef  unsigned char  ULONG8;
typedef  unsigned short ULONG16;
//typedef  unsigned long  ULONG32;

#define SUCCESS 0 
#define FAIL -1

/*函数申明*/
/*加密接口函数*/
INT32 gost_enc(ULONG32 *data , ULONG32 *key);
/*解密接口函数*/
INT32 gost_dec(ULONG32 *data, ULONG32 *key);
INT32 encry_data( ULONG32 *Ldata,ULONG32 *Rdata,ULONG32 *key);
INT32 dencry_data( ULONG32 *Ldata,ULONG32 *Rdata,ULONG32 *key);
INT32 f(ULONG32 x);/*s-盒替换、循环左移11位操作*/
/*左右值交换*/
INT32 gost_swap( ULONG32 *Ldata,ULONG32 *Rdata);

/*Gost的s-盒*/
ULONG8 wz_sp[8][16] ={
	{ 0x4,0xa,0x9,0x2,0xd,0x8,0x0,0xe,0x6,0xb,0x1,0xc,0x7,0xf,0x5,0x3},
	{ 0xe,0xb,0x4,0xc,0x6,0xd,0xf,0xa,0x2,0x3,0x8,0x1,0x0,0x7,0x5,0x9},
	{ 0x5,0x8,0x1,0xd,0xa,0x3,0x4,0x2,0xe,0xf,0xc,0x7,0x6,0x0,0x9,0xb},
	{ 0x7,0xd,0xa,0x1,0x0,0x8,0x9,0xf,0xe,0x4,0x6,0xc,0xb,0x2,0x5,0x3},
	{ 0x6,0xc,0x7,0x1,0x5,0xf,0xd,0x8,0x4,0xa,0x9,0xe,0x0,0x3,0xb,0x2},
	{ 0x4,0xb,0xa,0x0,0x7,0x2,0x1,0xd,0x3,0x6,0x8,0x5,0x9,0xc,0xf,0xe},
	{ 0xd,0xb,0x4,0x1,0x3,0xf,0x5,0x9,0x0,0xa,0xe,0x7,0x6,0x8,0x2,0xc},
	{ 0x1,0xf,0xd,0x0,0x5,0x7,0xa,0x4,0x9,0x2,0x3,0xe,0x6,0xb,0x8,0xc}
};

/*加密密钥使用顺序表*/
ULONG32 wz_spkey[32] = {
	0,1,2,3,4,5,6,7,
	0,1,2,3,4,5,6,7,
	0,1,2,3,4,5,6,7,
	7,6,5,4,3,2,1,0
};

/*s-盒替换、循环左移11位操作*/
INT32 f (ULONG32 x) {
	x = wz_sp[7][(x>>28)&0xf]<<28 | wz_sp[6][(x>>24)&0xf]<<24
		| wz_sp[5][(x>>20)&0xf]<<20 | wz_sp[4][(x>>16)&0xf]<<16 
		| wz_sp[3][(x>>12)&0xf]<<12 | wz_sp[2][(x>>8)&0xf]<<8 
		| wz_sp[1][(x>>4)&0xf]<< 4   |  wz_sp[0][x&0xf];

	return x <<11 | x>>21;
}
/*左右值交换*/
INT32 gost_swap( ULONG32 *Ldata,ULONG32 *Rdata)
{
	ULONG32 tempbuf;
	tempbuf = *Rdata;
	*Rdata = *Ldata ;
	*Ldata = tempbuf;
	return SUCCESS;

}

/*32轮解密操作*/
INT32 dencry_data( ULONG32 *Ldata,ULONG32 *Rdata,ULONG32 *key)
{
	ULONG32 i = 0 ;
	ULONG32 tempbuf =  0 ;
	for (i = 0 ; i < 32;i++)
	{
		*Rdata ^=f(*Ldata + key[wz_spkey[31-i]]);
		gost_swap(Ldata,Rdata); /*左右值交换*/
	}
	gost_swap(Ldata,Rdata);	/*左右值交换*/
	return SUCCESS;
}

/*解密接口函数*/
INT32 gost_dec(ULONG32 *data ,/*待解密数据首地址,内容需保证是64位长*/
  ULONG32 *key/*用户输入密钥首地址,内容需保证是256位长*/
)
{
	ULONG32 *Ldata  ;
	ULONG32 *Rdata ;
	Ldata = data ;
	Rdata = &Ldata[1];/*分成左右两个部分,每部分32字节*/
	dencry_data( Ldata, Rdata,key ) ;
	/*明文可用data读出*/	
	return SUCCESS;
}

INT32 gost_enc(ULONG32 *data , /*待加密数据首地址,内容需保证是64位长*/
 ULONG32 *key/*用户输入密钥首地址,内容需保证是256位长*/
)
{
	ULONG32 *Ldata  ;
	ULONG32 *Rdata ;
	Ldata = data ;
	Rdata = &Ldata[1];/*分成左右两个部分,每部分32字节*/
	encry_data( Ldata, Rdata,key ) ;
	/*密文可用data读出*/	
	return SUCCESS;
}
/*32轮加密操作*/
INT32 encry_data( ULONG32 *Ldata,ULONG32 *Rdata,ULONG32 *key)
{
	ULONG32 i = 0 ;
	ULONG32 tempbuf  =  0;
	for (i = 0 ; i < 32;i++)
	{
		*Rdata ^=f(*Ldata + key[wz_spkey[i]]); 
		gost_swap(Ldata,Rdata); /*左右值交换*/
	}
	gost_swap(Ldata,Rdata);	/*左右值交换*/
	return SUCCESS;
}


#define WZ_COMMEND_NUM 4  
#define WZUSEHELPNUM 7
#define READFILESIZE 512 /*一次从文件中读取多少字节,可以根据内存的大小调节*/

INT32 encodehex(ULONG8 *tobuf,ULONG8 *frombuf,ULONG32 len)
{
	ULONG8 *readfirst = frombuf ;
	ULONG8 *readend = &frombuf[1] ;	
	INT8 *s;
	ULONG8 y[2] ;		    
	ULONG32 i;
	for ( i = 0 ; i < len ; i++)
	{
		y[0] = *readfirst ;
		y[1] = *readend ;
		readfirst += 2 ;
		readend += 2 ;
		tobuf[i] = (ULONG8)strtol((INT8*)y, &s, 16);			
	}	
	return SUCCESS;
}

#if 0

INT32 hextofile( ULONG8 *buf ,FILE *writefile, ULONG32 length)
{
	ULONG32 writelen = 0 ;
		/*以16进制形式写入文件*/
    while( writelen < length)
	{
		if(buf[writelen] == 0)
		{
			fprintf( writefile, "%x", 0 );
			fprintf( writefile, "%x", 0 );
		}
		else if (buf[writelen] < 0x10)
		{
			fprintf( writefile, "%x", 0 );
			fprintf( writefile, "%x", buf[writelen] );				
		}
		else
		{
			fprintf( writefile, "%x", buf[writelen] );
			
		}			
		writelen++;
		
	}
	return SUCCESS;
}

INT32 file_enc( FILE *readfile, FILE *writefile,ULONG8 *key)
{
	INT32 filelen = 0,readlen = 0,writelen = 0;
	ULONG32 totalfilelen = 0 ;/*统计实际的文件的长度*/
	INT32 i;
	ULONG8 readbuf[READFILESIZE] = { 0 };		
	filelen = fread( readbuf, sizeof( INT8 ), READFILESIZE, readfile );
	while( filelen == READFILESIZE )
	{	
		totalfilelen += READFILESIZE;
		for ( i = 0 ; i < READFILESIZE ; i += 8)
		{
			gost_enc( (ULONG32*)&readbuf[i] ,(ULONG32*) key);/*加密*/			
		}				
		hextofile( readbuf, writefile, READFILESIZE );/*以16进制形式写入文件*/
		memset(readbuf,0,READFILESIZE);	
		filelen = fread( readbuf, sizeof( INT8 ), READFILESIZE, readfile );
	}	
	/*这是从文件中读出的最后一批数据,长度可能会等于0,所以要先判断*/

	if ( filelen > 0 )
	{
		/*如果从文件中读出的长度不等于0,那么肯定有8个字节以上的空间
		文件长度存在最后8个字节中*/
		totalfilelen += filelen;
		memcpy( &readbuf[READFILESIZE-8], (ULONG8*)&totalfilelen,4);	
		for ( i = 0 ; i < READFILESIZE ; i += 8)
		{
			gost_enc( (ULONG32*)&readbuf[i] ,(ULONG32*) key);/*加密*/
		}		
		hextofile( readbuf, writefile,READFILESIZE );/*以16进制形式写入文件*/		
		memset(readbuf,0 ,READFILESIZE);
	}
	else /*filelen == 0*/
	{
		memcpy( &readbuf[0], (ULONG8*)&totalfilelen,4);			
		gost_enc( (ULONG32*)&readbuf[0] ,(ULONG32*) key);/*加密*/		
		hextofile( readbuf, writefile, 8);/*以16进制形式写入文件*/		
	}
	return SUCCESS;
}


INT32 file_dec( FILE *readfile, FILE *writefile,ULONG8 *key)
{
	INT32 filelen = 0,readlen = 0,writelen = 0;
	ULONG32 totalfilelen = 0 ;/*统计实际的文件的长度*/
	INT32 i,num;
	ULONG8 readbuf[READFILESIZE] = { 0 };
	ULONG8 sendbuf[READFILESIZE*2] = { 0 };	
	
	fseek(readfile,-16,SEEK_END);/*最后16个字节的表示文件长度的空间*/			
	filelen = fread( sendbuf, sizeof( INT8 ), 16, readfile );	
	encodehex( readbuf,sendbuf,8);	
	gost_dec( (ULONG32*)&readbuf[0] ,(ULONG32*) key);/*解密*/
	memcpy((ULONG8*)&totalfilelen, &readbuf[0],4);/*得到文件总长*/
	memset(readbuf,0 ,8);
	memset(sendbuf,0 ,16);			
    
	num = totalfilelen/READFILESIZE;/*有几个READFILESIZE组*/
	totalfilelen %= READFILESIZE;

	fseek(readfile,0,SEEK_SET);/*跳到文件头*/	
	while(num--)
	{
		filelen = fread( sendbuf, sizeof( INT8 ), READFILESIZE*2, readfile );			
		encodehex( readbuf,sendbuf,READFILESIZE);
		for ( i = 0 ; i < READFILESIZE ; i += 8)
		{
			gost_dec( (ULONG32*)&readbuf[i] ,(ULONG32*) key);/*解密*/
		}				
		writelen = fwrite(readbuf, sizeof( INT8 ), READFILESIZE, writefile);				  		
		memset(readbuf,0 ,READFILESIZE);
		memset(sendbuf,0 ,READFILESIZE*2);			
	}
	if ( totalfilelen > 0 )/*最后一块有多余的元素*/
	{
		filelen = fread( sendbuf, sizeof( INT8 ), READFILESIZE*2, readfile );	
		encodehex( readbuf,sendbuf,READFILESIZE);
		for ( i = 0 ; i < READFILESIZE ; i += 8)
		{
			gost_dec( (ULONG32*)&readbuf[i] ,(ULONG32*) key);/*解密*/
		}		
		writelen = fwrite(readbuf, sizeof( INT8 ), totalfilelen, writefile);	
		memset(readbuf,0 ,READFILESIZE);
		memset(sendbuf,0 ,READFILESIZE*2);						
		
	}	
	return SUCCESS;
}

void wz_printhelp()
{
	INT32 i ;
	printf("\t");
	for (  i = 0 ; i < 22 ; i++)
	{
		printf("%c ",5);
	}
	printf("\n");
	for( i = 0 ; i < WZ_COMMEND_NUM ; i++)
	{
		printf("\t%c\t%s %c\n",5,WZ_Commend_Help[i],5);	
	}
	printf("\t");
	for (  i = 0 ; i < 22 ; i++)
	{
		printf("%c ",5);
	}	
	printf("\n");
	for( i = 0 ; i < WZUSEHELPNUM ; i++)
	{
		printf("\t%s\n",WZ_USE_HELP[i]);
	}
	return ;
}


void main(INT32 argc,INT8 *argv[])
{
INT8 *WZ_Commend_Help[] =
{
	"基于GOST的加密解密工具v1.0	 ",/*0*/	
	"追求卓越,勇于创新		 ",	
	"----著者 :  吴真---		 ",
	"			  	 "	
};

INT8 *WZ_USE_HELP[]={
	"输入5个参数:",
	"\t1.可执行文件名 *.exe",
	"\t2.操作类型 1:加密;2:解密;",
	"\t3.读出数据的文件名*.txt",
	"\t4.写入数据的文件名*.txt",	
	"\t5.密钥(32字节)",	
	"******************************"
};


	INT8 *FILENAME1,*FILENAME2;    
	FILE *fp, *fp2;		
	ULONG8 key[33] = { 0 };	/*密钥容器*/		
	if ( argc != 5 )	
	{
		wz_printhelp();
	}
	FILENAME1 = argv[2];
	FILENAME2 = argv[3];
	if ((fp= fopen(FILENAME1,"rb")) == NULL	|| (fp2 = fopen(FILENAME2,"wb"))==NULL)
	{			
		printf("Can't open file\n");		
		return ;
	}  	
    memcpy( key, argv[4] , strlen( argv[4]) );/*取得密钥*/	
	switch( atoi(argv[1] ))
	{
	case 1:/*加密操作*/			
		file_enc(fp,fp2,key);				
		printf("\n \tGost 加密完毕,密文存于%s文件\n",FILENAME2);
		break;
	case 2:
		/*解密*/
		file_dec(fp,fp2,key);	
		printf("\n\t Gost解密完毕,明文存于%s文件\n",FILENAME2);
		break;
	default:	
		printf("请选择是加密|解密 plese choose encrypt|deencrypt\n");		
		break;
	}
	
	fclose(fp);
	fclose(fp2);
}

#endif

/*算法实现代码结束*/

