# MySQL 之动态字符串处理 - 文章 - 伯乐在线
原文出处： [zxszcaijin](http://blog.chinaunix.net/uid-20708886-id-5570581.html)
MySQL中，常常会看到一些关于动态字符串的处理，列如：DYNAMIC_STRING。
为了记录动态字符串的实际长度，缓冲区的最大长度，以及每次字符串需要调整时，及时分配新的内存，以及调整长度。MySQL使用了DYNAMIC_STRING来保存动态字符串相关的信息:


```
typedef struct st_dynamic_string
{
	char	*str;
	size_t	length, max_length, alloc_increment;
} DYNAMIC_STRING;
```
在这个结构体中，str存储实际字符串的首地址，length记录字符串的实际长度，max_length记录字符串缓冲区最多可以存放多少字符，alloc_increment表示当字符串需要分配内存时，每次分配多少内存。
下面看看这个结构体的初始化过程:


```
my_bool init_dynamic_string( DYNAMIC_STRING *str, const char *init_str, size_t init_alloc, size_t alloc_increment )
{
	size_t length;
	DBUG_ENTER( "init_dynamic_string" );
	if ( !alloc_increment )
		alloc_increment = 128;
	length = 1;
	if ( init_str && (length = strlen( init_str ) + 1) < init_alloc )
		init_alloc = ( (length + alloc_increment - 1) / alloc_increment) * alloc_increment;
	if ( !init_alloc )
		init_alloc = alloc_increment;
	if ( !(str->str = (char *) my_malloc( init_alloc, MYF( MY_WME ) ) ) )
		DBUG_RETURN( TRUE );
	str->length = length - 1;
	if ( init_str )
		memcpy( str->str, init_str, length );
	str->max_length		= init_alloc;
	str->alloc_increment	= alloc_increment;
	DBUG_RETURN( FALSE );
}
```
从上述函数可以看到，初始化时，初始分配的字符串缓冲区大小init_alloc会根据需要初始的字符串来做判断。在分配好该DYNAMIC_STRING空间之后，我们会根据缓冲区的大小，字符串的实际长度，以及alloc_increment来初始化：
length:字符串的实际长度
max_length:缓冲区的最大长度
alloc_increment:空间不够时，下次分配内存的单元大小.
初始化这些内容之后，如果下次需要在该缓冲区添加更多字符，就可以根据这些值来判断是否需要对该缓冲区扩容：


```
my_bool dynstr_append_mem( DYNAMIC_STRING *str, const char *append, size_t length )
{
	char *new_ptr;
	if ( str->length + length >= str->max_length ) /* 如果新增字符串后，总长度超过缓冲区大小 */
	{
/* 需要分配多少个alloc_increment 大小的内存，才能存下新增后的字符串 */
		size_t new_length = (str->length + length + str->alloc_increment) /
				    str->alloc_increment;
		new_length *= str->alloc_increment;
		if ( !(new_ptr = (char *) my_realloc( str->str, new_length, MYF( MY_WME ) ) ) )
			return(TRUE);
		str->str	= new_ptr;
		str->max_length = new_length;
	}
/* 将新分配的内容，append到str之后 */
	memcpy( str->str + str->length, append, length );
	str->length		+= length;                              /* 扩容之后str新的长度 */
	str->str[str->length]	= 0; /* Safety for C programs */        /* 字符串最后一个字符为’\0' */
	return(FALSE);
}
```
从上述代码可以看到，在字符串初始化化好之后，之后如果需要给该字符串增加新的内容，只需要根据之前存储的信息来动态的realloc就好了。由于该结构体记录了字符串相关的完整内容，所以动态的扩容会非常方便处理。
当然，除了这些，还有比如字符串截断，字符串初始设置，转义OS的引号等等：
将字符串偏移大于N之后的截断。


```
my_bool dynstr_trunc( DYNAMIC_STRING *str, size_t n )
{
	str->length -= n;
	str->str[str->length] = '\0';
	return(FALSE);
}
```
返回字符串中第一次出现某个字符的地址。若没有，则返回字符串结尾的地址(指向’’)


```
char *strcend( register const char *s, register pchar c )
{
	for (;; )
	{
		if ( *s == (char) c )
			return( (char *) s);
		if ( !*s++ )
			return( (char *) s - 1);
	}
}
```
字符串内容扩容:


```
my_bool dynstr_realloc( DYNAMIC_STRING *str, size_t additional_size )
{
	DBUG_ENTER( "dynstr_realloc" );
	if ( !additional_size )
		DBUG_RETURN( FALSE );
	if ( str->length + additional_size > str->max_length ) /* 如果新的字符串内容超过缓冲区的最大长度 */
	{
		str->max_length = ( (str->length + additional_size + str->alloc_increment - 1) /
				    str->alloc_increment) * str->alloc_increment;
		if ( !(str->str = (char *) my_realloc( str->str, str->max_length, MYF( MY_WME ) ) ) )
			DBUG_RETURN( TRUE );
	}
	DBUG_RETURN( FALSE );
}
```
对字符串用引号括起来，对其中的单引号进行转义,主要用于执行一些系统命令(system(cmd))。
比如:ls -al 会变成 ‘ls -al’
比如:ls -a’l会变成’ls -a\’l’


```
/*
 * Concatenates any number of strings, escapes any OS quote in the result then
 * surround the whole affair in another set of quotes which is finally appended
 * to specified DYNAMIC_STRING. This function is especially useful when
 * building strings to be executed with the system() function.
 *
 * @param str Dynamic String which will have addtional strings appended.
 * @param append String to be appended.
 * @param ... Optional. Additional string(s) to be appended.
 *
 * @ note The final argument in the list must be NullS even if no additional
 * options are passed.
 *
 * @return True = Success.
 */
my_bool dynstr_append_os_quoted( DYNAMIC_STRING *str, const char *append, ... )
{
	const char	*quote_str	= "\'";
	const uint	quote_len	= 1;
	my_bool		ret		= TRUE;
	va_list		dirty_text;
	ret &= dynstr_append_mem( str, quote_str, quote_len ); /* Leading quote */
	va_start( dirty_text, append );
	while ( append != NullS )
	{
		const char	*cur_pos	= append;
		const char	*next_pos	= cur_pos;
/* Search for quote in each string and replace with escaped quote */
		while ( *(next_pos = strcend( cur_pos, quote_str[0] ) ) != '\0' )
		{
			ret	&= dynstr_append_mem( str, cur_pos, (uint) (next_pos - cur_pos) );
			ret	&= dynstr_append_mem( str, "\\", 1 );
			ret	&= dynstr_append_mem( str, quote_str, quote_len );
			cur_pos = next_pos + 1;
		}
		ret	&= dynstr_append_mem( str, cur_pos, (uint) (next_pos - cur_pos) );
		append	= va_arg( dirty_text, char * );
	}
	va_end( dirty_text );
	ret &= dynstr_append_mem( str, quote_str, quote_len ); /* Trailing quote */
	return(ret);
}
```
通过定义动态字符串的结构体信息，每次分次进行字符串添加更多字符，都会根据字符串的当前的长度动态的扩容。而且每次扩容后，该结构体都记录的当前字符串的实际信息(当前字符串的长度，缓冲器可容纳字符串的长度，进行扩容的单元长度)。这样，动态字符串的处理操作就变得非常方便了。
