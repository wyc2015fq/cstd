# gdbm编程示例 - Orisun - 博客园







# [gdbm编程示例](https://www.cnblogs.com/zhangchaoyang/articles/2367804.html)





我在ubuntu是需要先安装GDBM的。

总结一下GDBM的特点：
- 按key-value存储数据，value是可变长的。它只对key进行索引，只能按key进行查询。
- 高效的查询，低效的插入，适合于存储比较静态的数据。
- GDBM可以很容易地编译进一个可发布的二进制文件中，不需要独立地安装数据库服务器。
- 不支持SQL，不支持表之间创建关系，更不用说存储过程、触发器什么的了。

操作GDBM就跟操作文件很相似，下面的代码用于创建一个数据库，并存入一条记录。

#include<gdbm.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/stat.h>
#include<string.h>

#define ISBN_MAX 13
#define AUTHOR_MAX 50
#define TITLE_MAX 50
#define DB_FILE_BLOCK "book_data"

typedef struct {		/*定义图书结构体 */
	char isbn[ISBN_MAX + 1];
	char author[AUTHOR_MAX + 1];
	char title[TITLE_MAX + 1];
	int numb;
} book_entry;

int main()
{
	book_entry newbook;	/*创建新的图书记录，并赋值 */
	memset(&newbook, '\0', sizeof(newbook));
	strncpy(newbook.isbn, "9787302184942", ISBN_MAX);
	strncpy(newbook.author, "Microsoft Research Asia", AUTHOR_MAX);
	strncpy(newbook.title, "Microsoft's Dream Works", TITLE_MAX);
	newbook.numb = 735;

	datum key, data;	/*datum结构体有两个成员：dptr指向存储的数据，dsize记录数据的大小 */
	key.dptr = (char *)newbook.isbn;	/*用ISBN作key */
	key.dsize = ISBN_MAX;
	data.dptr = (char *)&newbook;	/*用整条数据记录作value */
	data.dsize = sizeof(newbook);

	GDBM_FILE dbm_ptr;
	/*打开数据库（跟打开文件很相似），返回数据库句柄 */
	dbm_ptr = gdbm_open(DB_FILE_BLOCK,	/*文件名 */
			    0,	/*文件大小，设为0时GDBM将使用文件系统的统计块大小 */
			    GDBM_WRCREAT,	/*读写模式。WRCREAT读写，数据库文件不存在时创建；READER读；WRITER写 */
			    S_IRUSR | S_IWUSR,	/*权限标志位 */
			    NULL	/*出错时的空参数回调函数 */
	    );
	/*把记录存入数据库 */
	gdbm_store(dbm_ptr,	/*数据库句柄 */
		   key,		/*key值 */
		   data,	/*value值 */
		   GDBM_REPLACE	/*如果是GDBM_INSERT则插入重复记录会出错；GDBM——REPLACE则只是覆盖原先存在的相同记录 */
	    );

	char isbnarr[ISBN_MAX + 1] = { 0 };
	char *isbn = isbnarr;
	puts("请输入你要查询图书的ISBN号:");
	scanf("%s",isbn);
	key.dptr = (char *)isbn;
	key.dsize = ISBN_MAX;
	/*数据库查询 */
	data = gdbm_fetch(dbm_ptr, key);	/*参建：数据库句柄的key值 */
	if (data.dsize == 0)
		printf("查无结果\n");
	else {
		memset(&newbook, 0, sizeof(newbook));
		memcpy(&newbook, data.dptr, data.dsize);	/*将从数据库读到的记录赋给book_entry结构体 */
		printf("%s\t%s\t%s\t%d\n", newbook.isbn, newbook.author,
		       newbook.title, newbook.numb);
	}

	/*关闭数据库 */
	gdbm_close(dbm_ptr);
}


由于GDBM只支持按key查询，如果我们想持按data中的某个字段查询，就必须自己遍历数据库，逐一地和记录（这是个struct）的特定成员进行对比。

我们通过两个函数来遍历数据库：

gdbm_firstkey(dbm_ptr)　　　　获取数据库第一条记录的key值

gdbm_nextkey(dbm_ptr, key)　　获取下一条记录的key值

要想实现模糊查询也只能借助于strstr()函数，它用于判断子串是否在主串中出现过。当然调用strstr之前，你通常还需要统计转换为大写或者小写，tolower和touper可以对单个字符进行大小写转换。

下面的代码就是遍历数据库，进行模糊查询。

#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<gdbm.h>
#include<stdio.h>
#include<ctype.h>

#define DB_FILE_BLOCK "book_data"
#define ISBN_MAX 13
#define AUTHOR_MAX 50
#define TITLE_MAX 50
#define FILED_MAX 50		/*最宽的数据域 */

typedef struct {
	char isbn[ISBN_MAX + 1];
	char author[AUTHOR_MAX + 1];
	char title[TITLE_MAX + 1];
	int numb;
} book_entry;

char *str_con(char *input, int len)
{
	int count = 0;
	do {
		input[count] = tolower(input[count]);
		count++;
	} while (count <= len);
	return input;
}

int main()
{
	GDBM_FILE dbm_ptr;
	datum key, data;
	book_entry vbook, sbook;/*vbook存储数据库中的原始记录，sbook是转换成小写以后的*/ 
	char keyword[FILED_MAX];
	printf("请输入一个关键字开始模糊查询:");
	scanf("%s",keyword);
	dbm_ptr = gdbm_open(DB_FILE_BLOCK, 0, GDBM_READER, 0, NULL);
	for (key = gdbm_firstkey(dbm_ptr); key.dptr;
	     key = gdbm_nextkey(dbm_ptr, key)) {
		data = gdbm_fetch(dbm_ptr, key);
		memcpy(&sbook, data.dptr, data.dsize);
		memcpy(&vbook, data.dptr, data.dsize);
		strcpy(keyword, str_con(keyword, FILED_MAX));
		strcpy(sbook.isbn, str_con(sbook.isbn, ISBN_MAX));
		strcpy(sbook.title, str_con(sbook.title, TITLE_MAX));
		strcpy(sbook.author, str_con(sbook.author, AUTHOR_MAX));
		if ((strstr(sbook.isbn, keyword) || strstr(sbook.title, keyword)
		     || strstr(sbook.author, keyword))) {
			printf("%s\t%s\t%s\t%d\n", vbook.isbn, vbook.author,
			       vbook.title, vbook.numb);
		} else {
			printf("查无记录\n");
		}
		gdbm_close(dbm_ptr);

	}
	return 0;
}













