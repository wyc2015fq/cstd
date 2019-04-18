# HTML5实现浏览器播放全屏模式！ - weixin_33985507的博客 - CSDN博客
2016年11月29日 00:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9

先写点css样式：
*{margin:0;padding:0}
      .btn1{
                 position:absolute;right:0;bottom:0;
                  z-index:23;
       }
       .div1{
              position:relative;z-index:1;
              width:423px;
              height:230px;
         }
这是全屏的封装函数：
function   aaa(element){
         if(element.requestFullscreen){
                   element.requestFullscreen();
         }else if(element.mozRequestFullScreen){
                   element.mozRequestFullscreen();
         }else if(element.webkitRequestFullscreen){
                   element.webkitRequestFullscreen();
         }else if(element.msRequestFullscreen){
                  element.msRequestFullscreen();
         }
}
这是缩小的封装函数：
function   bbb(){
          if(document.exitFullscreen){
                document.exitFullscreen();
          }else if(document.mozExitFullscreen){
               document.mozExcitFullscreen();
           }else if(document.webkitExitFullscreen){
               document.webkitExitFullscreen();
          }
}
函数调用
window.onload=function(){
         varoV=document.getElementById('ov');
         varoBtn=document.getElementById('btn1')；
        oBtn.onclick=function(){
              aaa(document.getElementById('ov'));
         };
         oV.ondblclick=function(){
             bbb();
         };
};
