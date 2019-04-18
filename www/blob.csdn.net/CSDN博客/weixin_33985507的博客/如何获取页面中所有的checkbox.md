# 如何获取页面中所有的checkbox - weixin_33985507的博客 - CSDN博客
2009年02月24日 15:43:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
用getElementsByTagName就可以了，如下：   
  <div   id="tt">   
  <form   method="POST"   action="">   
  在这个DIV里共有<span   id="t"></span>个checkbox   
  <input   type="checkbox"   name="C1"   value="ON"   checked>   
  <input   type="checkbox"   name="C2"   value="ON"   checked>   
  <input   type="checkbox"   name="C3"   value="ON"   checked>   
  <input   type="checkbox"   name="C4"   value="ON"   checked>   
  <input   type="checkbox"   name="C5"   value="ON"   checked>   
  <input   type="checkbox"   name="C6"   value="ON"   checked>   
  <input   type="checkbox"   name="C7"   value="ON"   checked>   
  <input   type="checkbox"   name="C8"   value="ON"   checked>   
  </form>   
  <script   language="javascript">   
  var   getCK=document.getElementsByTagName('input');   
  var   j=0;   
  for(var   i=0;i<getCK.length;i++)   
  {   
      whichObj=getCK[i];   
      if(whichObj.type=="checkbox")   
      {   
          whichObj.checked=false;   
          j++;   
      }   
  }   
  document.getElementById('t').innerText=j;   
  </script>   
  </div>   
[Top](http://topic.csdn.net/t/20060407/10/4669511.html#)
### **3 楼brothercat（猫猫 ^_^）**回复于 2006-04-07 20:23:00 得分 *5*
要加按钮的话就这样写：   
  <div   id="tt">   
  <form   method="POST"   action="">   
  如何获取页面中所有的checkbox在这个DIV里共有<span   id="t">几</span>个checkbox   
  <input   type="checkbox"   name="C1"   value="ON"   checked>   
  <input   type="checkbox"   name="C2"   value="ON"   checked>   
  <input   type="checkbox"   name="C3"   value="ON"   checked>   
  <input   type="checkbox"   name="C4"   value="ON"   checked>   
  <input   type="checkbox"   name="C5"   value="ON"   checked>   
  <input   type="checkbox"   name="C6"   value="ON"   checked>   
  <input   type="checkbox"   name="C7"   value="ON"   checked>   
  <input   type="checkbox"   name="C8"   value="ON"   checked>   
  <input   type="button"   value="reset"   onclick="changeStates()">   
  </form>   
  <script   language="javascript">   
  function   changeStates()   
  {   
      var   getCK=document.getElementsByTagName('input');   
      var   j=0;   
      for(var   i=0;i<getCK.length;i++)   
      {   
          whichObj=getCK[i];   
          if(whichObj.type=="checkbox")   
          {   
              whichObj.checked=false;   
              j++;   
          }   
      }   
      document.getElementById('t').innerText=j;   
  }   
  </script>   
  </div>   
[Top](http://topic.csdn.net/t/20060407/10/4669511.html#)
### **4 楼chouchy（城市刀客）**回复于 2006-04-07 20:36:01 得分 *15*
用getElementsByTagName("INPUT")获得div里面的所有checkbox,循环设置checked=false就可以了。   
  楼上的方法没有判断该checkbox是否在div中。   
  <div   id="myDiv"   style="border:1px   solid   #F00">   
  <input   type="checkbox"   name="c1">   
  <input   type="checkbox"   name="c2">   
  <input   type="checkbox"   name="c3">   
  <input   type="checkbox"   name="c4">   
  </div>   
  <input   type="checkbox"   name="c5"   checked="checked">这个在div外   
  <input   type="button"   value="取消选择"   onclick="unCheck()">   
  <script   language="javascript">   
  function   unCheck()   
  {   
  var   div=document.getElementById("myDiv");   
  var   ckb=document.getElementsByTagName("INPUT");   
  for(i=0;i<ckb.length;i++)   
  {   
  if(ckb[i].type=="checkbox"   &&   ckb[i].parentNode==div)   
  ckb[i].checked=false;   
  }   
  }   
  </script>   
[Top](http://topic.csdn.net/t/20060407/10/4669511.html#)
### **5 楼wizfly（）**回复于 2006-04-10 11:53:16 得分 0 
谢谢两位的回复。很精典。多谢指点。
