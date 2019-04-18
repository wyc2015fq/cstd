# ProE常用曲线方程：Python Matplotlib 版本代码（玫瑰曲线） - wishchinYang的专栏 - CSDN博客
2017年12月27日 17:31:31[wishchin](https://me.csdn.net/wishchin)阅读数：727
## Pyplot教程：[https://matplotlib.org/gallery/index.html#pyplots-examples](https://matplotlib.org/gallery/index.html#pyplots-examples)
## 玫瑰曲线
### 文字描述
平面内,围绕某一中心点平均分布整数个正弦花瓣的曲线.
### 数学描述
在极坐标下可表示为ρ=a*sin(nθ),a为定长,n为整数.
### 图形描述
在极坐标系中，以下方程表示的曲线称为玫瑰曲线：
r = sin ( k θ )　或　r = cos ( k θ )
当 k 是奇数时，玫瑰曲线有 k 个花瓣；当 k 是偶数时，玫瑰曲线有 2k 个花瓣。执行效果如下图：
![](https://img-blog.csdn.net/20171227173211378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Python代码：
```python
def drawFlowers():
    theta=np.arange(0,2*np.pi,0.02); 
    plt.subplot(111,polar=True);
    
    plt.plot(theta,np.cos(6*theta),'-',lw=2);#6瓣花瓣
    plt.plot(theta,np.cos(5*theta),'--',lw=2);#5瓣花瓣
    plt.plot(theta,2*np.cos(4*theta),lw=2);#4瓣花瓣
      
    plt.rgrids(np.arange(0.5,2,0.5),angle=45);  
    plt.thetagrids([0,45,90]);
    
    plt.show();
```
玫瑰曲线：
![](https://img-blog.csdn.net/20171227173349869?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

