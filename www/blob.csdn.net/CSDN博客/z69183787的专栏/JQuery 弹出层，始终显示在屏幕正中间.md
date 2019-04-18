# JQuery 弹出层，始终显示在屏幕正中间 - z69183787的专栏 - CSDN博客
2012年10月24日 20:36:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：20486
1.让层始终显示在屏幕正中间：
      样式代码：
Html代码  ![收藏代码](http://xutao5641745.iteye.com/images/icon_star.png)
- .model{  
-             position: absolute; z-index: 1003;   
-             width:320px; height:320px; text-align:center;  
-             background-color:#0066FF; display: none;  
-         }  
      jquery代码：
Js代码  ![收藏代码](http://xutao5641745.iteye.com/images/icon_star.png)
- //让指定的DIV始终显示在屏幕正中间
- function letDivCenter(divName){   
- var top = ($(window).height() - $(divName).height())/2;   
- var left = ($(window).width() - $(divName).width())/2;   
- var scrollTop = $(document).scrollTop();   
- var scrollLeft = $(document).scrollLeft();   
-         $(divName).css( { position : 'absolute', 'top' : top + scrollTop, left : left + scrollLeft } ).show();  
-     }  
    html代码：
Html代码  ![收藏代码](http://xutao5641745.iteye.com/images/icon_star.png)
- <ahref="javascript:;"onclick="letDivCenter('#model')">点我让DIV始终显示在屏幕中间</a><br/>
- <div>
- <divid="model"class="model">
-         这是内容（不过没有垂直居中显示）希望各位高手，能够补充。小弟在此谢过了。  
- </div>
- </div>
运行一下看看效果吧。
接下来总结一下，将它们整合成一个。即，当弹出ｄｉｖ层的时候，同时也要弹出遮罩层，好，废话不多说，看代码：
　　　１。ＣＳＳ样式：
Html代码  ![收藏代码](http://xutao5641745.iteye.com/images/icon_star.png)
- <styletype="text/css">
-     .mask {    
-             position: absolute; top: 0px; filter: alpha(opacity=60); background-color: #777;  
-             z-index: 1002; left: 0px;  
-             opacity:0.5; -moz-opacity:0.5;  
-         }  
-     .model{  
-             position: absolute; z-index: 1003;   
-             width:320px; height:320px; text-align:center;  
-             background-color:#0066FF; display: none;  
-         }  
- 
- </style>
　　　２。Ｊquery代码：
Html代码  ![收藏代码](http://xutao5641745.iteye.com/images/icon_star.png)
- <scripttype="text/javascript">
-     //兼容火狐、IE8  
-     function showMask(){  
-         $("#mask").css("height",$(document).height());  
-         $("#mask").css("width",$(document).width());  
-         $("#mask").show();  
-     }  
-     //让指定的DIV始终显示在屏幕正中间  
-     function letDivCenter(divName){   
-         var top = ($(window).height() - $(divName).height())/2;   
-         var left = ($(window).width() - $(divName).width())/2;   
-         var scrollTop = $(document).scrollTop();   
-         var scrollLeft = $(document).scrollLeft();   
-         $(divName).css( { position : 'absolute', 'top' : top + scrollTop, left : left + scrollLeft } ).show();  
-     }  
-     function showAll(divName){  
-         showMask();  
-         letDivCenter(divName);  
-     }  
- </script>
      3.HTML代码：
Html代码  ![收藏代码](http://xutao5641745.iteye.com/images/icon_star.png)
- <divid="mask"class="mask"></div>
- <ahref="javascript:;"onclick="showMask()">点我显示遮罩层</a><br/>
- <ahref="javascript:;"onclick="letDivCenter('#model')">点我让DIV始终显示在屏幕中间</a><br/>
- <ahref="javascript:;"onclick="showAll('#model')">点我显示所有</a><br/>
- <div>
- <divid="model"class="model">
-         这是内容（不过没有垂直居中显示）希望各位高手，能够补充。小弟在此谢过了。  
- </div>
- </div>
