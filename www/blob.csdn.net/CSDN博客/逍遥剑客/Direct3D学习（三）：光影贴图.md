# Direct3D学习（三）：光影贴图 - 逍遥剑客 - CSDN博客
2007年04月18日 23:58:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2626
刚看完了DXUT，拿来练练手
这个框架却实方便啊，终于不用写那些令人恶心的API函数了
光影贴图，说白了就是在纹理上表现出来光影效果，并不是实时计算的，所以适用于固定的场影平面，如地面，墙壁等
原理嘛，就是多层纹理的叠加，除了模型本身的纹理外，还需要光纹理
效果如图
![](http://images.blogcn.com/2007/4/18/12/xoyojank,20070418232942.jpg)
关键代码如下：
                pd3dDevice->SetTexture( 0, pTexture0 );        /**//// 在0号纹理层固定纹理（壁面）
        pd3dDevice->SetTexture( 1, pTexture1 );        /**//// 在1号纹理层固定纹理（光影贴图）
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
        pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );    /**//// 0号纹理：使用0号纹理索引
        pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );    /**//// 1号纹理：使用0号纹理索引
        pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );    /**//// 0号纹理层的放大滤镜
        pd3dDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );    /**//// 1号纹理层的放大滤镜
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
        pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );    /**//// 运用MODULATE运算进行颜色混合
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        pd3dDevice->SetTextureStageState( 2, D3DTSS_COLOROP,   D3DTOP_DISABLE );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        pd3dDevice->SetTextureStageState( 2, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
