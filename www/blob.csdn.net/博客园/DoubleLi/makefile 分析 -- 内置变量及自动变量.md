# makefile 分析 -- 内置变量及自动变量 - DoubleLi - 博客园






makefile 分析1 

-p 选项，可以打印出make过程中的数据库， 下面研究一下内置的变量和规则。 
-n 选项， 只运行，不执行， 
-d 选项，相当于--debug=a,  b(basic),v(verbose),i(implicity),j(innvocation of command),m(remake files) 

这里着重解释一下 -p 
make -p -f /dev/null 可以打印出内置变量和内置规则 
变量可以分为3类， 
第一类: 环境变量， 比较重要的是PATH, PWD 就不一一列举了。 
第二类: 内置变量， 比较重要的是cc, CXX, .INCLUDE_DIRS, .DEFAULT_GOAL等 
例如: 
cc 是 /usr/bin/cc -> /usr/bin/gcc 
CXX 是 g++ 

查找makefile 维护的目标 
.DEFAULT_GOAL  默认的维护的目标（命令行未指定目标）
MAKECMDGOALS  命令行指定的维护目标。 
为完整起见，贴出我机器上的内置变量。大可不必死记硬背，掌握重要的，领会其含义即可。 
[hjj@hjj ~]$ cat 2.txt 
.FEATURES := target-specific order-only second-expansion else-if archives jobserver check-symlink 
.INCLUDE_DIRS = /usr/include /usr/local/include /usr/include 
.LIBPATTERNS = lib%.so lib%.a 
.VARIABLES :=  
AR = ar 
ARFLAGS = rv 
AS = as 
CC = cc 
CHECKOUT,v = +$(if $(wildcard $@),,$(CO) $(COFLAGS) $< $@) 
CO = co 
COFLAGS =  
COMPILE.C = $(COMPILE.cc) 
COMPILE.F = $(FC) $(FFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c 
COMPILE.S = $(CC) $(ASFLAGS) $(CPPFLAGS) $(TARGET_MACH) -c 
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c 
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c 
COMPILE.cpp = $(COMPILE.cc) 
COMPILE.def = $(M2C) $(M2FLAGS) $(DEFFLAGS) $(TARGET_ARCH) 
COMPILE.f = $(FC) $(FFLAGS) $(TARGET_ARCH) -c 
COMPILE.mod = $(M2C) $(M2FLAGS) $(MODFLAGS) $(TARGET_ARCH) 
COMPILE.p = $(PC) $(PFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c 
COMPILE.r = $(FC) $(FFLAGS) $(RFLAGS) $(TARGET_ARCH) -c 
COMPILE.s = $(AS) $(ASFLAGS) $(TARGET_MACH) 
CPP = $(CC) -E 
CTANGLE = ctangle 
CWEAVE = cweave 
CXX = g++ 
F77 = $(FC) 
F77FLAGS = $(FFLAGS) 
FC = f77 
GET = get 
LD = ld 
LEX = lex 
LEX.l = $(LEX) $(LFLAGS) -t 
LINK.C = $(LINK.cc) 
LINK.F = $(FC) $(FFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH) 
LINK.S = $(CC) $(ASFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_MACH) 
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH) 
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH) 
LINK.cpp = $(LINK.cc) 
LINK.f = $(FC) $(FFLAGS) $(LDFLAGS) $(TARGET_ARCH) 
LINK.o = $(CC) $(LDFLAGS) $(TARGET_ARCH) 
LINK.p = $(PC) $(PFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH) 
LINK.r = $(FC) $(FFLAGS) $(RFLAGS) $(LDFLAGS) $(TARGET_ARCH) 
LINK.s = $(CC) $(ASFLAGS) $(LDFLAGS) $(TARGET_MACH) 
LINT = lint 
LINT.c = $(LINT) $(LINTFLAGS) $(CPPFLAGS) $(TARGET_ARCH) 
M2C = m2c 
MAKE = $(MAKE_COMMAND) 
MAKEFILES :=  
MAKEINFO = makeinfo 
MAKE_COMMAND := make 
MAKE_VERSION := 3.81 
OUTPUT_OPTION = -o $@ 
PC = pc 
PREPROCESS.F = $(FC) $(FFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -F 
PREPROCESS.S = $(CC) -E $(CPPFLAGS) 
PREPROCESS.r = $(FC) $(FFLAGS) $(RFLAGS) $(TARGET_ARCH) -F 
RM = rm -f 
SUFFIXES := .out .a .ln .o .c .cc .C .cpp .p .f .F .r .y .l .s .S .mod .sym .def .h .info .dvi .tex .texinfo .texi .txinfo .w .ch .web .sh .elc .el 
TANGLE = tangle 
TEX = tex 
TEXI2DVI = texi2dvi 
WEAVE = weave 
YACC = yacc 
YACC.y = $(YACC) $(YFLAGS) 

第三类：自动变量。我的机器是这样的。 
[hjj@hjj ~]$ cat 3.txt 
%D = $(patsubst %/,%,$(dir $%)) 
*D = $(patsubst %/,%,$(dir $*)) 
+D = $(patsubst %/,%,$(dir $+)) 
?D = $(patsubst %/,%,$(dir $?)) 
@D = $(patsubst %/,%,$(dir $@)) 
^D = $(patsubst %/,%,$(dir $^)) 
%F = $(notdir $%) 
*F = $(notdir $*) 
+F = $(notdir $+) 
<F = $(notdir $<) 
?F = $(notdir $?) 
@F = $(notdir $@) 
^F = $(notdir $^) 

代表文件（4个） 
$@--目标文件， 
$<--第一个依赖文件。 
$*--代表"茎",例如：文件“dir/a.foo.b”，当目标的模式为“a.%.b ”时,“$* ”的值为“dir/a.foo ” 
$%--当规则的目标文件是一个静态库文件时，代表静态库的一个成员名 
代表文件列表(3个） 
$^--所有的依赖文件， 
$?--所有比目标文件更新的依赖文件列表 
$+--类似“$^”，但是它保留了依赖文件中重复出现的文件 

$(@D) -- 目标的目录部分，文件名部分 
$(@F)  
$(*D) -- 代表"茎"的目录部分,文件名部分 
$(*F)  
$(<D) -- 第一个依赖文件目录部分，文件名部分 
$(<F)  
$(?D) -- 被更新的依赖文件目录部分，文件名部分  
$(?F)  
$(^D) -- 所有依赖文件目录部分，文件名部分 
$(^F)  
$(%D) -- 库文件成员目录部分，文件名部分 
$(%F)  
$(+D) -- 所有依赖的目录部分，文件名部分(可存在重复文件） 
$(+F)  









