# Direct3D学习（一）：3D Sierpinski镂垫绘制 - 逍遥剑客 - CSDN博客
2007年03月29日 01:07:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2544
![](http://home.qlsc.sdu.edu.cn/images/smilies/yumen.gif) 自己几何也太差劲了，时间都花在计算坐标位置上了![](http://home.qlsc.sdu.edu.cn/images/smilies/dead.gif)
![](http://home.qlsc.sdu.edu.cn/images/attachicons/image.gif)[图片附件](http://home.qlsc.sdu.edu.cn/member.php?action=credits&view=getattach): [Sierpinski.JPG](http://home.qlsc.sdu.edu.cn/attachment.php?aid=363004) (2007-3-29 00:56, 39.68 K)
![](http://images.blogcn.com/2007/3/29/1/xoyojank,2007032911352.jpg)
![](http://home.qlsc.sdu.edu.cn/images/attachicons/binary.gif)[附件](http://home.qlsc.sdu.edu.cn/member.php?action=credits&view=getattach): [D3DStudy.exe](http://home.qlsc.sdu.edu.cn/attachment.php?aid=363005) (2007-3-29 00:56, 64 K)
主要算法，就是个递归：
/**//***************************************
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)*三角形绘制
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)*vertex[3]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)***************************************/
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)inline void DrawTriangle(CustomVertex vertex[])
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//拷贝到顶点缓存
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    memcpy(pVertices,vertex,3*sizeof(CustomVertex));
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,1);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
/**//****************************************
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)*Sierpinski镂垫绘制
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)*deepth:递归深度
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)****************************************/
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)void DrawSierpinski(CustomVertex vertex[],int deepth)
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if(deepth ==0)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        DrawTriangle(vertex);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }else
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//计算中点
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        CustomVertex midVertex[] =
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{(vertex[1].x + vertex[2].x)/2,(vertex[1].y + vertex[2].y)/2,(vertex[1].z + vertex[2].z)/2,vertex[0].color},
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{(vertex[0].x + vertex[2].x)/2,(vertex[0].y + vertex[2].y)/2,(vertex[0].z + vertex[2].z)/2,vertex[1].color},
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{(vertex[1].x + vertex[0].x)/2,(vertex[1].y + vertex[0].y)/2,(vertex[1].z + vertex[0].z)/2,vertex[2].color}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)        };
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        CustomVertex tempVertex[3];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//分三部分进行递归绘制
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        tempVertex[0] = vertex[0];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        tempVertex[1] = midVertex[2];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        tempVertex[2] = midVertex[1];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        DrawSierpinski(tempVertex,deepth -1);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        tempVertex[0] = midVertex[2];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        tempVertex[1] = vertex[1];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        tempVertex[2] = midVertex[0];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        DrawSierpinski(tempVertex,deepth -1);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        tempVertex[0] = midVertex[1];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        tempVertex[1] = midVertex[0];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        tempVertex[2] = vertex[2];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        DrawSierpinski(tempVertex,deepth -1);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
