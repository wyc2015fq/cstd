# LowPloy风格的模型导入 - 逍遥剑客 - CSDN博客
2017年01月05日 12:44:15[xoyojank](https://me.csdn.net/xoyojank)阅读数：5205
LowPloy风格是一种小清新风格, 在美术设计上很有独特性. 典型的游戏作品有《纪念碑谷》《劳拉Go》等 
![这里写图片描述](https://lh3.googleusercontent.com/wF_AlvOzOnU0WQr8aDwaTjGrQwzeFnMpNbYosvdp0zW3ns1fFhb6FKYUqb2CaKWgJcg=h900-rw)
![这里写图片描述](https://madewith.unity.com/sites/default/files/styles/story_desktop/public/game-article/header-image-desktop/unity_article5.png?itok=yjeGUxL6)
![这里写图片描述](https://img-blog.csdn.net/20170105115754384?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2016上半年我们也进行过尝试, 从程序的角度来说, 这种风格是一种利好, 因为可以大大减少资源量, 对于性能的压力就小得多了, 所以在移动平台上会有比较好的性能表现.  
这种风格在实际制作过程中, 反而不如传统的风格简单.  
首先需要好的概念设计, 但是对于原画来说, 一般不会关注到3D模型的制作, 就会出现画出的风格与实际3D模型做出来不一样, 因为LowPloy风格是极大地依赖几何体三角形的拓扑结构的.  
其次对于3D美术来说, 虽然面少了, 但是细节也少了, 很多美术是倾向于”画”出细节而不是组合出细节, 如果完全不用贴图的话, 对于他们来说无疑是困绑住双手进行工作. 这一点在特效制作上也有体现, 很多特效美术也是习惯用贴图去画, 而不是用粒子去做运动模拟, 因为动力学和各种参数对他们来说不如直接画张图贴到Billboard上简单. 
这里我不讨论美术制作的可行性, 只简单提一下技术. 最理想的情况下, LowPloy可以不用一张贴图, 全部使用顶点色进行制作, 比如下面这个场景, 就没用一张贴图: 
![这里写图片描述](https://img-blog.csdn.net/20170105121814444?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
但是直接导入引擎会发现, 表现跟在3dsmax里不一样: 
![这里写图片描述](https://img-blog.csdn.net/20170105121026403?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
仔细观察可以得出结论, 即相邻面的顶点被合并了. 那怎么避免呢? 方法就是把相邻的不同颜色的面, Detach成不同的Element: 
![这里写图片描述](https://img-blog.csdn.net/20170105121302015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
再导入引擎就可以对比效果发现正确了: 
![这里写图片描述](https://img-blog.csdn.net/20170105121341547?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![这里写图片描述](https://img-blog.csdn.net/20170105121355969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
但是3D美术不干了, 这几千个面一个个Detach那不是累死? 而且美术做不到相同颜色的面拆到同一个Element里, 所以写了个max脚本自动拆面:
```
sel = selection as array
undo on
(
    fn ToByteColor c =
    (
        local result = color 0 0 0
        result.r = floor (c.x * 255 + 0.5)
        result.g = floor (c.y * 255 + 0.5)
        result.b = floor (c.z * 255 + 0.5)
        return result
    )
    if sel.count > 0 then
    (
        local polygon = sel[1]
        local numFaces = polyop.getNumMapFaces polygon 0
        local singleColorFaces = #()
        local singleColors = #()
        --search face colors
        local faceColors = #()
        for i = 1 to numFaces do
        (
            local isSingleColorFace = true
            local vertices = polyop.getMapFace polygon 0 i
            local faceColor = ToByteColor (polyop.getMapVert polygon 0 vertices[1])
            for j = 2 to vertices.count do
            (
                --exclude faces with multi colors
                local color = ToByteColor (polyop.getMapVert polygon 0 vertices[j])
                if (color != faceColor) then
                (
                    --format "%:%\n%:%\n\n" vertices[1] faceColor vertices[j] color
                    isSingleColorFace = false
                    exit
                )
                --format "%\n" color
            )
            if isSingleColorFace then
            (
                appendIfUnique faceColors faceColor
                append singleColorFaces i
                append singleColors faceColor
            )
        )
        --detach the same color faces to a element
        for c in faceColors do
        (
            format "%" c
            local sameColorFaces = #()
            for f = 1 to singleColorFaces.count do
            (
                if c == singleColors[f] then
                (
                    append sameColorFaces singleColorFaces[f]
                    format "% " singleColorFaces[f]
                )
            )
            polyop.detachFaces polygon sameColorFaces
            format "\n"
        )
    )
)
```
经过拆面后, 面数没变, 但是顶点数从8000+涨到了12000+, 导入引擎后有14000+. 嗯, 完全不用贴图的情况下, 只能靠增加面数来提升细节了. 
下面是导入引擎的效果, 不过有点不对劲, 颜色的饱和度降了不少: 
![这里写图片描述](https://img-blog.csdn.net/20170105120900557?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
试试在材质里做个Gamma校正: 
![这里写图片描述](https://img-blog.csdn.net/20170105123717008?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
最终效果: 
![这里写图片描述](https://img-blog.csdn.net/20170105123901318?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170105124322683?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
