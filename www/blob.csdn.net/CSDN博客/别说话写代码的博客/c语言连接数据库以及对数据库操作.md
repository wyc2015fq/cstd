# c语言连接数据库以及对数据库操作 - 别说话写代码的博客 - CSDN博客





2017年07月13日 15:03:19[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：2228








查询数据库接口代码

原数据库效果：

所有的表

![](https://img-blog.csdn.net/20170713150728140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE5OTc2MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

表中原始数据为空，表属性为

![](https://img-blog.csdn.net/20170713150647047?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE5OTc2MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170713151051516?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE5OTc2MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




```cpp
#include <mysql/mysql.h>
#include <stdio.h>

int main()
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";//本地连接
	char* user="root";//
	char* password="525215980";//mysql密码
	char* database="student";//数据库名
	char* query="select * from class";//需要查询的语句
	int t,r;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		printf("Query made...\n");
		res=mysql_use_result(conn);
		if(res)
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{	
				//printf("num=%d\n",mysql_num_fields(res));//列数
				for(t=0;t<mysql_num_fields(res);t++)
					printf("%8s ",row[t]);
				printf("\n");
			}
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return 0;
}
```



执行完上述代码数据库中数据为

![](https://img-blog.csdn.net/20170713151051516?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE5OTc2MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


插入



```cpp
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
int main(int argc,char* argv[])
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="525215980";
	char* database="student";
	char query[200];
	int t,r;
	FILE *fp;
	if(argc!=3)
	{
		printf("error args\n");
		return -1;
	}
	fp=fopen(argv[1],"r");
	if(NULL==fp)
	{
		perror("fopen");
		return -1;
	}
	char name[30];
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		printf("Connected...\n");
	}
	char people[20];
	strcpy(people,argv[2]);
	while(memset(name,0,sizeof(name)),fgets(name,sizeof(name),fp)!=NULL)
	{
		name[strlen(name)-1]='\0';
		memset(query,0,sizeof(query));
		sprintf(query,"%s%s%s%s%s","insert into class(code,name) values('",name,"','",people,"')");//插入语句，根据自己情况改变
		puts(query);
		t=mysql_query(conn,query);
		if(t)
		{
			printf("Error making query:%s\n",mysql_error(conn));
			break;
		}else{
			printf("insert success\n");
		}
	}			
	fclose(fp);
	mysql_close(conn);
	return 0;
}
```



执行完上述代码数据库中数据为


![](https://img-blog.csdn.net/20170713150812500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE5OTc2MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
删除


```cpp
#include <mysql/mysql.h>
#include <stdio.h>

int main()
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="525215980";
	char* database="student";
	char* query="delete from class where name='yuting'";//只需要改变这一句
	int t,r;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		printf("delete success\n");
	}
	mysql_close(conn);
	return 0;
}
```



执行完上述代码数据库结果为
![](https://img-blog.csdn.net/20170713151051516?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE5OTc2MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



