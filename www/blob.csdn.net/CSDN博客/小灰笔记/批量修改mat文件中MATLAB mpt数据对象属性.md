# 批量修改mat文件中MATLAB mpt数据对象属性 - 小灰笔记 - CSDN博客





2017年03月01日 22:53:06[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2218








       终于还是开始着手好好学习一下MATLAB了！先前工作中这个东西经常用，但是很多时候很多功能的使用全都是凭借试探找到的解决方案。一旦找到了解决方案，基本上也就是在方案的基础上使用，也很少去做一下总结或者是深层次的考虑。或许正是这个原因，每次我处理mat文件的时候总是感觉到头痛，好多东西跟我预想的一直不一致。

       最近一段时间简单学习了Java，接触了面向对象的理念。在理解面向对象的模型时，借用了之前学习Python的思维模型，发现十分符合。其实，MATLAB中很多地方也是面向对象编程思想的应用，再次尝试，发现跟我想象的还真有点相似。

       创建了一堆数据，存储到test.mat(const不小心敲错了，错了就错了吧！不改了，就是一个简单的示意例子)。所有的数据目前的定义文件以及存储位置都是默认的，接下来我要把信号类的全都定义以及生命在*_MData.c以及相应的头文件中，参数类的数据全都定义到*_CData.c以及相应的头文件中。之所以取这么个名字，跟我第一个公司工作的惯性有关吧！

![](https://img-blog.csdn.net/20170301225243560?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







       写一个M函数如下;

function DataProcess(mat_file)

% useDataProcess('demo.mat') to process a model's mat file

%

if exist(mat_file)

        load(mat_file);

        signal_info = whos;

        header_file_name = strrep(mat_file,'.mat','_MData.h');

        defination_file_name = strrep(mat_file,'.mat','_MData.c');

for i = 1:length(signal_info)

            var_info = signal_info(i);

            var_class = var_info.class;

if strcmp(var_class,'mpt.Signal')

** var_name = var_info.name;**

**               var_object = eval(var_name);**

ifstrcmp(var_object.CoderInfo.CustomStorageClass,'Global')

                   var_object.CoderInfo.CustomAttributes.HeaderFile = header_file_name;

                   var_object.CoderInfo.CustomAttributes.DefinitionFile =defination_file_name;

end

end

end

        header_file_name = strrep(mat_file,'.mat','_CData.h');

        defination_file_name = strrep(mat_file,'.mat','_CData.c');

for i = 1:length(signal_info)

            var_info = signal_info(i);

            var_class = var_info.class;

if strcmp(var_class,'mpt.Parameter')

** var_name = var_info.name;**

**               var_object = eval(var_name);**

if strcmp(var_object.CoderInfo.CustomStorageClass,'Global')

                   var_object.CoderInfo.CustomAttributes.HeaderFile = header_file_name;

                   var_object.CoderInfo.CustomAttributes.DefinitionFile =defination_file_name;

end

end

end

else

        disp('Can''t find mat file! Please Check!')

end

    save(mat_file);

end



       加粗的部分，最初的时候我没有想到。直接用获取的数据对象的名称去修改属性，结果修改失败。这让我有点郁闷，因为我把名称字符串粘贴到命令窗中是能够输出各种属性参数的。这个问题，其实很多年前我就遇到了，不过那时候基本上也是各种尝试最终勉强找个可以实现的方法。比较着急的时候，我甚至尝试过把修改的命令一条条输出到M文件中然后最后运行M文件。还好，今天对这个问题的现象分析在我看来已经不是很难。或许前段时间通过学习Java了解到的一点面向对象的理念给了我一点提示。

       其实，获取到的对象名称只是一个字符串。而作为字符串对象，自然是没有mpt数据的那些属性的。这样修改肯定是会出问题的，修改出来的对象也很可能不再具有mpt原本的那些属性特征。

       想到了eval，这是之前的同事解决问题的时候常用的一个方法。不过，这个相关的文档描述我尝试看了好几次使用起来还是不得要领。不过，eval既然能够把字符串解析成命令，自然也能够把字符串解析成command窗口中能够解析的命令或者对象。通过这种方式，重新构造一个新的具有同样属性特征的对象。而根据我之前学习时候对面向对象的这种理解，我觉得这里构造的对象名称应该是无关紧要的。因为，这里的名称只是一个标签，而真正有实质作用被修改掉的是标签标识的内存数据。程序运行了一下，果真跟我猜想的一致。

       运行之后，加载数据看一下两种数据的属性。从截图的信息中能够看到，相应的属性信息已经成功修改掉。


![](https://img-blog.csdn.net/20170301225336623?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20170301225350920?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




