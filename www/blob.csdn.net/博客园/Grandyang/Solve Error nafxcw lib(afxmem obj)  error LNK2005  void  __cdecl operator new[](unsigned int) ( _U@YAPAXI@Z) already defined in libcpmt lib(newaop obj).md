# Solve Error: nafxcw.lib(afxmem.obj) : error LNK2005: "void * __cdecl operator new[](unsigned int)" (??_U@YAPAXI@Z) already defined in libcpmt.lib(newaop.obj) - Grandyang - 博客园







# [Solve Error: nafxcw.lib(afxmem.obj) : error LNK2005: "void * __cdecl operator new[](unsigned int)" (??_U@YAPAXI@Z) already defined in libcpmt.lib(newaop.obj)](https://www.cnblogs.com/grandyang/p/7679763.html)







Error:

nafxcw.lib(afxmem.obj) : error LNK2005: "void * __cdecl operator new[](unsigned int)" (??_U@YAPAXI@Z) already defined in libcpmt.lib(newaop.obj)



Solution:

Go to **Property** -> **Linker** -> **Input** -> **Additional Dependencies**

Add **nafxcw.lib**

Go to **Ignore Specific Default Libraries**

Add **nafxcw.lib**












