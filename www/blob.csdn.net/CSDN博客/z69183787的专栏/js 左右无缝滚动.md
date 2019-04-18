# js 左右无缝滚动 - z69183787的专栏 - CSDN博客
2015年01月09日 14:31:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1462
```
<!DOCTYPE html>
<html lang="en">
<body><div id="gpic" style="overflow:hidden; width:600px; height:200px;">
<table border="0" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <td id="gpic1" valign="top" align="center"><table width="974" border="0" align='center' cellpadding="0" cellspacing="0">
        <tr>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/278_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/279_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/280_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/281_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/361_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/614_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/615_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/616_12.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/338_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/339_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/340_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/341_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/671_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/673_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/674_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/672_12.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/865_26.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1097_12.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/637_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/637_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/636_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/311_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/312_12.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/837_26.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/838_26.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/839_26.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/840_26.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/863_26.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/969_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/970_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/971_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/972_12.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1108_12.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/599_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/628_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/650_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/655_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/862_26.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1575_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1576_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1625_12.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/627_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/656_12.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1113_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1109_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1110_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1111_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1112_12.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1505_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1506_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1507_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1508_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1565_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/101501-11.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/102043-11.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1124_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1125_12.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1305_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1306_12.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1303_12.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1304_12.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/1217_12.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/101875-11.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/102012-11.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/102010-11.gif"></td>
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/102011-11.gif"></td>
          
          <td height="120" align="center"><img src="http://www.wayisa.com/pet/101616-11.gif"></td>
        </tr>
        <tr>
          <td align="center"><font color=green><b>利则诺顿</b></font></td>
          <td align="center"><font color=green><b>扬奇洛斯</b></font></td>
          <td align="center"><font color=green><b>邦浦洛斯</b></font></td>
          <td align="center"><font color=green><b>邦奇诺</b></font></td>
          <td align="center"><font color=green><b>布鲁顿</b></font></td>
          <td align="center"><font color=green><b>邦诺斯娜</b></font></td>
          <td align="center"><font color=green><b>杨格斯</b></font></td>
          <td align="center"><font color=green><b>迪基格斯</b></font></td>
          
          <td align="center"><font color=green><b>奥卡洛斯</b></font></td>
          <td align="center"><font color=green><b>左迪洛斯</b></font></td>
          <td align="center"><font color=green><b>巴朵兰恩</b></font></td>
          <td align="center"><font color=green><b>帖拉所伊朵</b></font></td>
          <td align="center"><font color=green><b>朵巴奈特</b></font></td>
          <td align="center"><font color=green><b>阿米朵</b></font></td>
          <td align="center"><font color=green><b>邦司凉朵</b></font></td>
          <td align="center"><font color=red><b>斑尼迪克</b></font></td>
          
          <td align="center"><font color=red><b>凯恩</b></font></td>
          
          <td align="center"><font color=green><b>蒙奇</b></font></td>
          
          <td align="center"><font color=green><b>佩露寅夏</b></font></td>
          <td align="center"><font color=red><b>佩露夏</b></font></td>
          <td align="center"><font color=green><b>普鲁夏</b></font></td>
          <td align="center"><font color=green><b>普露卡</b></font></td>
          <td align="center"><font color=green><b>普露伊卡</b></font></td>
          
          <td align="center"><font color=red><b>葛雷基欧</b></font></td>
          <td align="center"><font color=red><b>葛雷洛比</b></font></td>
          <td align="center"><font color=red><b>葛雷吉鲁</b></font></td>
          <td align="center"><font color=red><b>葛雷马恩</b></font></td>
          
          <td align="center"><font color=green><b>吉比</b></font></td>
          
          <td align="center"><font color=green><b>卡恩格登</b></font></td>
          <td align="center"><font color=green><b>卡恩米兹</b></font></td>
          <td align="center"><font color=green><b>卡恩法尔</b></font></td>
          <td align="center"><font color=green><b>卡恩修比</b></font></td>
          
          <td align="center"><font color=green><b>艾迪希洛</b></font></td>
          
          <td align="center"><font color=green><b>贝洛金</b></font></td>
          <td align="center"><font color=green><b>戈登尔顿</b></font></td>
          <td align="center"><font color=green><b>奇卡洛斯</b></font></td>
          <td align="center"><font color=green><b>沙瓦列</b></font></td>
          <td align="center"><font color=green><b>鲁比</b></font></td>
          <td align="center"><font color=green><b>卡芙丽</b></font></td>
          <td align="center"><font color=green><b>卡萝丽</b></font></td>
          <td align="center"><font color=green><b>卡普莉</b></font></td>
          
          <td align="center"><font color=green><b>梅尔顿</b></font></td>
          <td align="center"><font color=red><b>嘎吱拉</b></font></td>
          
          <td align="center"><font color=red><b>狼人系列</b></font></td>
          <td align="center"><font color=red><b>格林巴肯</b></font></td>
          <td align="center"><font color=red><b>雷恩巴肯</b></font></td>
          <td align="center"><font color=red><b>法亚巴肯</b></font></td>
          <td align="center"><font color=red><b>奇欧巴肯</b></font></td>
          
          <td align="center"><font color=red><b>利则诺顿</b></font></td>
          <td align="center"><font color=red><b>扬奇洛斯</b></font></td>
          <td align="center"><font color=red><b>邦奇诺</b></font></td>
          <td align="center"><font color=red><b>布鲁顿</b></font></td>
          <td align="center"><font color=red><b>邦浦洛斯</b></font></td>
          
          <td align="center"><font color=green><b>雷帝斯洛</b></font></td>
          
          <td align="center"><font color=green><b>朵巴翼特</b></font></td>
          
          <td align="center"><font color=green><b>米鲁多</b></font></td>
          <td align="center"><font color=green><b>多鲁多</b></font></td>
          
          <td align="center"><font color=green><b>毕可斯</b></font></td>
          <td align="center"><font color=green><b>毕卡斯</b></font></td>
          
          <td align="center"><font color=blue><b>克雷多</b></font></td>
          <td align="center"><font color=blue><b>培罗多</b></font></td>
          
          <td align="center"><font color=green><b>史卡鲁</b></font></td>
          
          <td align="center"><font color=green><b>怀沃夫</b></font></td>
          <td align="center"><font color=green><b>怀肯沃</b></font></td>
          <td align="center"><font color=green><b>阔米尔</b></font><img src="http://www.wayisa.com/images/new.jpg"></td>
          <td align="center"><font color=green><b>阔尔达</b></font><img src="http://www.wayisa.com/images/new.jpg"></td>
          
          <td align="center"><font color=red><b>菲奇亚</b></font><img src="http://www.wayisa.com/images/new.jpg"></td>
        </tr>
      </table></td>
    <td id="gpic2" valign="top"></td>
  </tr>
</table>
</div>
</body>
<script>
var speed=15
gpic2.innerHTML=gpic1.innerHTML
function Marquee(){
if(gpic2.offsetWidth-gpic.scrollLeft<=0)
gpic.scrollLeft-=gpic1.offsetWidth
else{
gpic.scrollLeft++
}
}
var MyMar=setInterval(Marquee,speed)
gpic.onmouseover=function() {clearInterval(MyMar)}
gpic.onmouseout=function() {MyMar=setInterval(Marquee,speed)}
</script>
</html>
```
