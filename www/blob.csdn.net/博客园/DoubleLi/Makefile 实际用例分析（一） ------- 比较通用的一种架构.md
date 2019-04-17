# Makefile 实际用例分析（一） ------- 比较通用的一种架构 - DoubleLi - 博客园






这里不再说Makefile的基本知识，如果需要学习，那么请参考： 下载：makefile 中文手册

或者


[点击打开链接](http://blog.csdn.net/shanshanpt/article/details/7365630)

或者

[跟我一起写Makefile( 陈皓 )](http://blog.csdn.net/haoel/article/details/2886)



这里说的是一般的实际的一个工程应该怎么去写。

环境：ubuntu 10.04



先看看我的文件的分布情况：

顶层：

![](http://img.blog.csdn.net/20131208111821921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbnNoYW5wdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

然后src中：是所有的源程序以及头文件（ 我这里是使用自己的IR树的代码作为实验 ）

![](http://img.blog.csdn.net/20131208112030468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbnNoYW5wdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



而build文件夹是为了编译使用的！下面有：

![](http://img.blog.csdn.net/20131208112148140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbnNoYW5wdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

obj文件夹里面放的是编译过程中的.o和.d文件，还有一个subdir.mk的子文件，

用于指示怎么生成.o

obj中：

![](http://img.blog.csdn.net/20131208112416828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbnNoYW5wdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



下面我们从顶层开始慢慢分析：



*******温馨提示：下面的注释是为了方便处理，写在每一条语句后面，其实这样的风格是不好的，所以，如果

       你使用了这个makefile，请将注释换行...或者去掉，否则可能编译异常！谢谢记住！

*******



最外层的makefile：





**[plain]**[view plain](http://blog.csdn.net/shanshanpt/article/details/17198413#)[copy](http://blog.csdn.net/shanshanpt/article/details/17198413#)

[print](http://blog.csdn.net/shanshanpt/article/details/17198413#)[?](http://blog.csdn.net/shanshanpt/article/details/17198413#)

- SHELL = /bin/sh             # 这个地方是指示使用的shell是sh  
- EXEC = ir_tree              # 最终生成的binary的名称  
- BUILD_DIR = build           # 这个子文件夹，此处也就是我们build文件夹  
- 
- all:                        # all在此处是终极目标，这个你应该知道的。一般我们make的时候，第一个目标作为终极目标  
-     @( cd ${BUILD_DIR}; make )  # 这句是进去build文件夹去执行那个makefile  
- 
- clean:                      # clean就不说了  
-     @echo 'start clean...'  
-     @($(RM) $(EXEC))  
-     @(cd ${BUILD_DIR}; make clean)  
-     @echo 'Finished!'  
-     @echo ''  







现在进入build文件夹，看这个文件夹下面的makefile





**[plain]**[view plain](http://blog.csdn.net/shanshanpt/article/details/17198413#)[copy](http://blog.csdn.net/shanshanpt/article/details/17198413#)

[print](http://blog.csdn.net/shanshanpt/article/details/17198413#)[?](http://blog.csdn.net/shanshanpt/article/details/17198413#)

- SHELL = /bin/sh            # 同上  
- 
- INCLUDE_DIR :=             # include文件夹，一般我们在引用库的时候，需要将其头文件放在一个include中，然后自己的程序                           # 编译的时候需要包含这个include，例如-I$(<span style="font-family: SimHei;">INCLUDE_DIR</span><span style="font-family: SimHei;">)</span>  
- LIB_DIR := -lm             # 引入的库  
- EXEC = ../ir_tree          # 这是一个最终binary名称，这里是将这个可执行放在了上层文件夹中  
- 
- -include obj/subdir.mk     # 这个地方是include了一个子文件  
-                            # 这里子文件作用是，为了生成所有的.o文件(当然附带生成.d文件！)，生成.o之后，才能回到这一                           # 层的makefile进行链接成最终的可执行的操作！具体操作我们稍后再看  
- 
- all:${EXEC}                # 好！这里是这个makefile的第一个目标。即终极目标，所有需要找<span style="font-family: SimHei;">${EXEC}的生成规则！</span>  
- 
- ${EXEC}: ${OBJS}           # <span style="font-family: SimHei;">${EXEC}的生成规则，注意这里我们没有看到$(OBJS)，那是因为在</span><span style="font-family: SimHei;">obj/subdir.mk中！</span><span style="font-family: SimHei;">  
- </span> @echo ' Building target: $@ '  
-     gcc -o $@ $(OBJS) $(LIB_DIR)   # 这一句就是为了将所有的.o文件 + 引用的库 链接起来，生成最后的$@，也就是$(EX                                       # EC)，也就是最后的binary！  
-     @echo 'Finished building target: $@'  
-     @echo ''  
- 
- clean:  
-     @echo 'start rm objs and deps ...'  
-     $(RM) $(OBJS) \  
-     $(C_DEPS)  
-     @echo 'Finish rm objs and deps ...'  
- 
- .PHONY: all clean                      # 伪目标  
- .SECONDARY:  



下面需要看看obj中的subdir.mk的内容了!这个是为了生成所有的.o文件。



同时！请注意：当我们的一个.c或者.h被修改之后，需要重新编译！这一点非常重要！

特别是.h被修改的时候，不能忘记重新编译( 当然，有些时候.h修改，我们不需要编译，这个先暂时不说，后面在讨论！其实，你使用一个make --touch就可以~ )





**[plain]**[view plain](http://blog.csdn.net/shanshanpt/article/details/17198413#)[copy](http://blog.csdn.net/shanshanpt/article/details/17198413#)

[print](http://blog.csdn.net/shanshanpt/article/details/17198413#)[?](http://blog.csdn.net/shanshanpt/article/details/17198413#)

- C_SRCS += \            # 所有的.c文件，当然你喜欢使用wildcard也是可的！  
- ../src/card.c \        # $(<span style="font-family: SimHei;">wildcard ../src/*.c</span><span style="font-family: SimHei;">)</span>  
- ../src/index.c \  
- ../src/node.c \  
- ../src/rect.c \  
- ../src/split_l.c \  
- ../src/test.c  
- 
- OBJS += \             <span style="font-family: SimHei;"># 所有的.c文件，当然你喜欢使用wildcard也是可的！</span>  
- ./obj/card.o \        # OBJS = $(patsubst %.c,%.o,$(wildcard ../src/*.c))  
- ./obj/index.o \       # 但是你要将src文件目录改成obj的 <span style="font-family: SimHei;">OBJS := $(addprefix "./obj/",$(notdir $(OBJS)))</span>  
- ./obj/node.o \  
- ./obj/rect.o \  
- ./obj/split_l.o \  
- ./obj/test.o  
- 
- C_DEPS += \          # deps  
- ./obj/card.d \  
- ./obj/index.d \  
- ./obj/node.d \  
- ./obj/rect.d \  
- ./obj/split_l.d \  
- ./obj/test.d  
- 
- all: $(OBJS)        # 注意在这个subdir中，这个是终极目标，也就是所有的objs  
- 
- obj/%.o: ../src/%.c ./obj/%.d    #这里是o文件的依赖规则：注意是.c和.d同时成为依赖，.d文件中是一个目标的所有的依赖文                                 # 件，包括.c和.h文件，所有一旦.h被修改，这个地方也是可以识别的！  
-     @echo 'start building $< ...'  
-     gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" \  
- -MT"$(@:%.o=%.d)" -o "$@" "$<"  
-     @echo 'Finished building: $< '  
-     @echo ''  
- 
- -include $(C_DEPS) # 注意：这里将所有的.d文件引入！注意，第一次引入时候，没有.d文件，会寻找.d的生成规则，也就是下面                   # 的，这个.d又是依赖.c文件的，所有，一旦一个.c文件中多了一个头文件之类，又可以跟新.d，从而，执行                   # 上面的.o生成时候，就能够实时跟新  
- 
- ./obj/%.d: ../src/%.c   # 注意：这里为了生成.d  
-     @echo 'start building $(notdir $@)...'  
-     $(CC) $< $(INCLUDE) -MM -MD -o $@  



好了，上面所有的都分析完了，然后可以make一下，.、ir_tree 看看效果吧~





如果你要下载这个工程，我已经上传了，你可以免费下载：[ir_tree](http://download.csdn.net/detail/shanshanpt/6678235)

from:http://blog.csdn.net/shanshanpt/article/details/17198413









