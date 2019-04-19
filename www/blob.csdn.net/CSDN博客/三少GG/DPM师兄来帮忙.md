# DPM师兄来帮忙~~~ - 三少GG - CSDN博客
2013年05月16日 17:51:13[三少GG](https://me.csdn.net/scut1135)阅读数：3259
训练DPM的人体model，一个小时搞定~
前人种树，后人copy。总结下步骤：
在首先调通检测demo的基础上（参考我另外文章：[http://blog.csdn.net/scut1135/article/details/8195985](http://blog.csdn.net/scut1135/article/details/8195985)，[http://blog.csdn.net/scut1135/article/details/8012007](http://blog.csdn.net/scut1135/article/details/8012007)）
如下操作：
1。下载VOCdevkit和voc2011 dataset (http://pascallin.ecs.soton.ac.uk/challenges/VOC/voc2011/)
2。修改global.m和pascal_init.m中的路径设置。
下面【5】说的比较具体。VOCyear ，cachedir ，tmpdir ，VOCdevkit 
注意重新指定pascal_init中的路径设置。
 如果数据不在VOCdevkit的目录下还要改VOCinit.m中的路径，源文件中注释很清楚。
3。修改train.m和rewritedat.m  (还有以pascal开头的几个脚本以及
 global.m里的路径检查)
./learn--->learn
unix()--->system() 
cp--->copy
rm--->del
mv--->move
unix(['rm ' oldfile]) ---> delete(oldfile)
unix(['mv ' datfile ' ' oldfile])---> movefile(datfile, oldfile)
4。procid.m文件中的“/”修改为“\"
5。编译learn.cc
直接拷贝 Json_Nie【2】改好的code，建vs工程生成Release版本learn.exe。
6。pozen【1】说的“rewritedat.m中还会出现下标越界的情况”：
把这段代码：
```
I = sort(I);
pos = 1;
for i = 1:length(I)
  cnt = I(i)-pos;
  while cnt > 0
    % + 2 to include the num non-zero blocks and example length
    info = fread(fin, labelsize+2, 'int32');
    dim = info(end);
    fseek(fin, dim*4, 0);
    cnt = cnt - 1;
  end
  y = fread(fin, labelsize+2, 'int32');
  dim = y(end);
  x = fread(fin, dim, 'single');
  fwrite(fout, y, 'int32');
  fwrite(fout, x, 'single');
  pos = I(i)+1;
end
```
替换成：
```
I = sort(I);
pos = 1;
for i = 1:length(I)
  cnt = I(i)-pos;
  while cnt > 0
    % + 2 to include the num non-zero blocks and example length
    info = fread(fin, labelsize+2, 'int32');
    if length(info) == 0
        dim = 0;
    else
        dim = info(end);
    end
    %dim = info(end);
    fseek(fin, dim*4, 0);
    cnt = cnt - 1;
  end
  y = fread(fin, labelsize+2, 'int32');
  %//!
  if length(y) == 0
      dim = 0;
  else
      dim = y(end);
  end
  x = fread(fin, dim, 'single');
  fwrite(fout, y, 'int32');
  fwrite(fout, x, 'single');
  pos = I(i)+1;
end
```
类似的越界错误在检测模块中，检测完成提取框时，如果检测个数为0的话也可能出现，不过影响不大。
**执行：**
pascal('person', 3);  
训练开始啦~
**原作者ReadMe**
Using the learning code
=======================
1. Download and install the 2006/2007/2008 PASCAL VOC devkit and dataset.
**  (you should set VOCopts.testset='test' in VOCinit.m)**
2. Modify 'globals.m' according to your configuration.
3. Run 'make' to compile learn.cc, the LSVM gradient descent code.
   (Run from a shell, not Matlab.)
4. Start matlab.
5. Run the 'compile' script to compile the helper functions.
   (you may need to edit compile.m to use a different convolution 
    routine depending on your system)
6. Use the 'pascal' script to train and evaluate a model. 
example:
> pascal('person', 3);   % train and evaluate a 6 component person model
The learning code saves a number of intermediate files in a cache
directory defined in 'globals.m'.  You should delete these files before
training models on different datasets, or when training new models after
modifing the code.
The code also generates some very large temporary files during training
(the default configuration produces files up to about 3GB).  They are
placed in a temporary directory defined in 'globals.m'.  This directory
should be in a local filesystem.
---------------------------------------------------------------------------------------------------------------------
参考文章：
1. pozen （训练+异常）
[http://blog.csdn.net/pozen/article/details/7103412](http://blog.csdn.net/pozen/article/details/7103412)
2. Json_Nie  （训练）
[http://blog.csdn.net/dreamd1987/article/details/7399151](http://blog.csdn.net/dreamd1987/article/details/7399151)
3. Json_Nie  （检测 demo）
[http://blog.csdn.net/dreamd1987/article/details/7396620](http://blog.csdn.net/dreamd1987/article/details/7396620)
4.  pozen （检测 demo）
[http://blog.csdn.net/pozen/article/details/7023742](http://blog.csdn.net/pozen/article/details/7023742)
5. liuwucn （数据准备）
[http://hi.baidu.com/liuwucn/blog/item/77f3d2cf264bfae252664fb8.html](http://hi.baidu.com/liuwucn/blog/item/77f3d2cf264bfae252664fb8.html)
6. Json_Nie（star-cascade）
[http://blog.csdn.net/dreamd1987/article/details/7570430](http://blog.csdn.net/dreamd1987/article/details/7570430)
