# 理解js设计模式之代理模式 - ggabcda的博客 - CSDN博客





2017年08月21日 10:49:11[ggabcda](https://me.csdn.net/ggabcda)阅读数：101








代理模式：**想去做什么不直接去做，而是交给自己的代理去做**

比如需要一个图片懒加载的功能，那么在不是用代理模式的情况下，代码大体上应该是这样的：


```java
var realImg='http://new-img2.ol-img.com/985x695/79/172/lic9i4TykRiA.jpg';  //真实图片
var xiaojuhua='http://www.wallpaperama.com/post-images/forums/200903/07p-6606-loadingcircle.gif'; //小菊花图片

function loadImg(loaddingImg,realImg){
  var imgNode=document.createElement('img');   
  document.body.appendChild(imgNode);   
  
  imgNode.src=loaddingImg;
  
  var proxyImg=new Image();
  proxyImg.src=realImg;
  
  proxyImg.onload=function(){
    imgNode.src=this.src;
  }
}
loadImg(xiaojuhua,realImg)  //为了能看到小菊花，需要降低网络链接的数据 fast 3G ，slow 3G 都能看到
```


　　这样子的一个方法里面就融合了，创建节点，和创建代理图片对象和绑定事件，常说模块职责单一化，这个时候代理模式可以帮组解耦这个混杂的情况，**当你完成一个任务需要借助另外一个东西来帮你完成**

代码：


```java
var xiaojuhua='http://www.wallpaperama.com/post-images/forums/200903/07p-6606-loadingcircle.gif';
var realImg= 'http://new-img2.ol-img.com/985x695/79/172/lic9i4TykRiA.jpg';

 //这个方法只负责创建img标签，对外暴露设置img的src接口
function myImage(){
 var image=document.createElement('img');
  document.body.append(image);
  return {
    set:function(src){
      image.src=src;
    }
  }
}
//在body里面异步加载图片，传入图片地址参数，和本地loadding图片
var myImg=myImage();
var proxyImage=function(){
  var img=new Image();
  img.onload=function(){
    console.log(this);
    myImg.set(this.src);
  }
  return {
    set:function(loadimg,src){
        img.src=src;
        myImg.set(loadimg);
    }
  }
}

var proxy=proxyImage();
proxy.set(xiaojuhua,realImg);
```




**保护代理和虚拟代理：**

所谓的保护代理：顾名思义，会过滤掉一些对象的代理请求，比如我们图片代理要求图片地址的主机名不能是18禁，，，，咳咳，还有其他什么的；

那么虚拟代理是什么：将一些消耗比较大的放到代理中去，比如创建图片dom操作，可以等到判断完毕后再代理里面创建





