# 关闭 C4996 警告(_CRT_SECURE_NO_DEPRECATE) - DoubleLi - 博客园






在使用VC 2005 的开发者会遇到这样的问题，在使用std命名空间库函数的时候，往往会出现类似于下面的警告：

warning C4996: strcpy was declared deprecated



出现这样的警告，是因为VC2005中认为CRT中的一组函数如果使用不当，可能会产生诸如内存泄露、缓冲区溢出、非法访问等安全问题。这些函数如：strcpy、strcat等。



对于这些问题，VC2005建议使用这些函数的更高级的安全版本，即在这些函数名后面加了一个_s的函数。这些安全版本函数使用起来更有效，也便于识别，如：strcpy_s,calloc_s等。



当然，如果执意使用老版本、非安全版本函数，可以使用_CRT_SECURE_NO_DEPRECATE标记来忽略这些警告问题。办法是在编译选项 C/C++ | Preprocessor | Preprocessor Definitions中，增加_CRT_SECURE_NO_DEPRECATE标记即可。



补充：



然而，本以为上面的说法是件漂亮的法子，不想用后不爽。遂用旧法：



#pragma warning(disable:4996) //全部关掉
#pragma warning(once:4996) //仅显示一个



项目属性---C/++ --->预处理定义下-》配置 -》下面添加_CRT_SECURE_NO_DEPRECATE和_SCL_SECURE_NO_DEPRECATE两个宏









