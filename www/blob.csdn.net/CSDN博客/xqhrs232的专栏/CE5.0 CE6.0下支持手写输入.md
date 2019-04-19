# CE5.0/CE6.0下支持手写输入 - xqhrs232的专栏 - CSDN博客
2010年02月26日 10:50:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1402标签：[input																[输入法																[wince																[library																[output																[dll](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=output&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)](https://so.csdn.net/so/search/s.do?q=输入法&t=blog)](https://so.csdn.net/so/search/s.do?q=input&t=blog)
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
CSDN相关文章：：[http://topic.csdn.net/u/20090531/10/d3a80026-06a1-4139-85dc-b7b0282224df.html](http://topic.csdn.net/u/20090531/10/d3a80026-06a1-4139-85dc-b7b0282224df.html)
在环境变量里面设置SYSGEN_TRANSCRIBER环境变量就可以了。相关的关键字为Transcriber Handwriting Recognizer。
不知道Handwriting Recognizer Engine跟Transcriber Handwriting Recognizer是什么关系，有什么区别。
对应CE的组件为international/locale specific support.但只看到ENGLISH/FRENCH/GERMAN组件下对应有那个支持手写输入的AP组件存在，
简体中文/繁体中文下没看到这个AP组件存在。CE下在目前的情况下不支持简体中文/繁体中文的手写输入？？？只能用拼音输入法？？？
还是得自己去开发（像什么汉王输入法什么的）？
XXX_touch。DLL的两个分支------引用C_WINCE500_PUBLIC_COMMON_CESYSGEN/MAKEFILE文件
tchmdd::
        @set TARGETTYPE=LIBRARY
        @set TARGETNAME=$@
        @set RELEASETYPE=OAK
        @set TARGETLIBS=
!IFDEF SYSGEN_TRANSCRIBER
        @set SOURCELIBS=$(SG_INPUT_LIB)/tchmain.lib $(SG_INPUT_LIB)/tch_trns.lib
        echo touch includes transcriber hooks
!ELSE
        @set SOURCELIBS=$(SG_INPUT_LIB)/tchmain.lib $(SG_INPUT_LIB)/tchbasic.lib
        echo touch is minimal
!ENDIF
        $(MAKECMD) /NOLOGO $(SG_OUTPUT_OAKLIB)/$@.lib
