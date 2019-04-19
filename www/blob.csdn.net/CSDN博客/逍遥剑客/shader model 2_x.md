# shader model 2_x - 逍遥剑客 - CSDN博客
2008年11月03日 15:49:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3668
在2002年DirectX 9.0发布的时候，当时新增的shader model包括了shader model 2.0（vertex shader 2.0+pixel shader 2.0）、shader model 2.0a（也被称作shader model 2.0+或者shader model 2_x，即vertex shader 2.0a+pixel shader2.0a）以及shader model 3.0。DirectX9.0中的shader model 3.0只提供了参考渲染器，缺乏对硬件层的支持，直到DirectX 9.0b问世的时候依然只是供开发人员试验性的规格。shader model 2.0和shader model 2.0a分别对应的是R3X0/RV3X0体系和NV3X体系，后者在前者的基础上增加了更长的指令数。
![hlhl3.jpg](http://img.article.pchome.net//00/02/61/59/hlhl3.jpg)
DirectX 9.0c提供了shader model 3.0的硬件支持和新增了ATi R420对应的shader model 2.0b规格，其中Pixel Shader 2.0b由Veretex Shader 2.0 + Pixel Shader 2.0b组成。在以往，版本号高的shader model向下兼容版本号较低的shader model，例如shader 3.0的硬件必定支持shader 2.0的硬件，但是到了DX9.0c的时候，情况有了些微妙的变化，原因在于shader model 2.0b的规格其实并没有超出shader model 2.0a，这样一来，具备shader model 2.a支持的NV3X在理论上是能够跑相当部分的shader model 2.0b程序的，不过由于NV3X本身缺乏MRT（Multi Render Target）的支持，因此又有一些2.0的shader是跑不了的。
具备shader model 2.0b支持的RADEON X800能够支持512条Pixel Shader指令，但是由于RADEON X800的每条流水线具备了1个独立纹理单元、1个3D向量ALU和1个1D矢量ALU，因此在理论上的最大shader程序长度极限是512条贴图指令+512条算法指令+512条算法指令，共计1500条左右，相比之下，R3X0/RV3X0只能做到32条贴图指令+64条算法指令。
Pixel shader 2.0b是shader model 2.0b相对shader model 2.0的唯一改进之处，但是和pixel shader 2.0相比，指令长度的增加并非唯一的改进，在每个像素ALU的临时寄存器数量方面，Pixel Shader 2.0b从Pixel Shader 2.0的12个增加到了32个，比Pixel Shader 2.0a（NV3X）的22个相比多了10个，同时这也已经达到Pixel Shader 3.0的规格。
