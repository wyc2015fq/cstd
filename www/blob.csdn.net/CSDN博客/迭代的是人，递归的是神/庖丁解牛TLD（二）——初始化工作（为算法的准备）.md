# 庖丁解牛TLD（二）——初始化工作（为算法的准备） - 迭代的是人，递归的是神 - CSDN博客





2011年11月10日 20:11:54[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：15304








我说的初始化，还不是算法的初始化工作，而是读入图像，响应键盘鼠标之类的工作。作者提供的代码中的工作包含了从摄像头读取和从文件中读取两种输入方案。这里介绍一下从文件输入的办法。因为OpenCV从视频读取图像序列的办法有很好的demo，我这里就不介绍摄像头的办法了。TLD下载后有一个文件夹是_input，里面存放着一组图片组，图片文件的名字为00001.png、00002.png....。我读取图片组的关键代码如下，这段代码具有普遍意义，可以移植到以后想读入图片组的任意程序中：



```cpp
stringstream fileNameStream;
string sourceImageFileName;
for(int nFrame=0; nFrame<NUM; nFrame++)
{
    nFrame++;
    fileNameStream << "_input\\" << setw(5) << setfill('0') << nFrameNum << ".png";
    sourceImageFileName = fileNameStream.str();
    fileNameStream.clear();
    fileNameStream.str("");

    // 读取图像
    g_src = imread(sourceImageFileName);
}
```


这就实现了图片的读入工作，再参考camshiftdemo的办法实现了鼠标和键盘的响应。鼠标的响应就是得到目标区域的范围，用鼠标选中boundingbox。文件读进来了，目标区域boundingbox也得到了，接下来就是需要对算法进行研究了。





先介绍几个我研究过的心得，bbox文件夹下面的代码主要都是对boundingbox的处理。tld文件夹下面的存放的是主干的算法，从run_TLD入手，感觉就是对起始帧进行初始化工作，然后逐帧读入图片序列，进行算法处理。还是先分析初始化工作，作者的tldInitSource函数实现的就是基本的初始化，给一些变量赋值，开辟矩阵大小，这个没什么好讲的。tldInitFirstFrame文件完成的工作就是选中boundingbox，这个功能我已经通过鼠标的响应得到了boundingbox，也可以略过不细分析。重点的初始化工作是在tldInit里实现的，这个函数也是我接下来研究的重点，本人Matlab较差，真希望有高人指点啊，一起研究啊



