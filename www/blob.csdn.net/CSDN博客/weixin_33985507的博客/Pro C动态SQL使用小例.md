# Pro*C动态SQL使用小例 - weixin_33985507的博客 - CSDN博客
2010年08月09日 23:47:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
环境：
WinXPSP2+VC6+Oracle10g
代码：
// 定义符号常数
#define USERNAME "SCOTT"
#define PASSWORD "x"
#include <stdio.h>
// 说明SQLCA和ORACA
EXEC SQL INCLUDE SQLCA;
EXEC SQL INCLUDE ORACA;
// 启用ORACLE通讯区：ORACA=YES，使它能被使用
EXEC ORACLE OPTION (ORACA=YES);
// 说明SQL变量
EXEC SQL BEGIN DECLARE SECTION;
	char* username=USERNAME;
	char* password=PASSWORD;
	VARCHAR sqlstmt[80];
	int emp_number;
	VARCHAR emp_name[15];
	VARCHAR job[50],job1[50],job2[50];
	float salary;
EXEC SQL END DECLARE SECTION;
main()
{
	EXEC SQL WHENEVER SQLERROR GOTO sqlerror;
	// 发生错误时，保存SQL语句至ORACA
	oraca.orastxtf=ORASTFERR;
	// 登录到ORACLE
	EXEC SQL CONNECT :username IDENTIFIED BY :password;
	printf("/nConnect to ORACLE./n");
	// 构造动态SQL语句
	sqlstmt.len=sprintf(sqlstmt.arr,"INSERT INTO EMP(EMPNO,ENAME,JOB,SAL)VALUES(:V1,:V2,:V3,:V4)");
	// 显示SQL语句
	puts(sqlstmt.arr);
	// 用PREPARE语句分析当前的动态INSERT语句，语句名是S
	EXEC SQL PREPARE S FROM :sqlstmt;
	// 循环插表
	for(;;)
	{
		printf("/nEnter employee number:");
		scanf("%d",&emp_number);
		if(emp_number==0)break;
		printf("/nEnter employee name:");
		scanf("%s",&emp_name.arr);
		emp_name.len=strlen(emp_name.arr);
		printf("/nEnter employee job:");
		scanf("%s",&job.arr);
		job.len=strlen(job.arr);
		salary = 0; // With VC6, Missing this line will cause C Run-Time Error R6002.
		printf("/nEnter salary:");
		scanf("%f",&salary);
		EXEC SQL EXECUTE S USING :emp_number,:emp_name,:job,:salary;
	}
	
	// 提交事务，退出ORACLE
	EXEC SQL COMMIT RELEASE;
	printf("/nHave a good day!/n");
	exit(0);
sqlerror:
	// 打印错误信息
	printf("/n%.*s/n",sqlca.sqlerrm.sqlerrml,sqlca.sqlerrm.sqlerrmc);
	// 打印出错SQL语句
	printf("/n/"%.*s.../"/n",oraca.orastxt.orastxtl,oraca.orastxt.orastxtc);
	// 打印出错SQL语句所在行号及所在文件名
	printf("on line %d of %.*s/n/n",oraca.oraslnr,
		oraca.orasfnm.orasfnml,oraca.orasfnm.orasfnmc);
	
	// 回滚事务，退出ORACLE
	EXEC SQL WHENEVER SQLERROR CONTINUE;
	EXEC SQL ROLLBACK RELEASE;
	exit(1);
}
