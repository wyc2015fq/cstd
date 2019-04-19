# CSS笔记 - 左直拳的马桶_日用桶 - CSDN博客
2010年04月12日 11:39:00[左直拳](https://me.csdn.net/leftfist)阅读数：1175标签：[css																[class																[div](https://so.csdn.net/so/search/s.do?q=div&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=css&t=blog)
一、星号*
应该是个通配符。比如
<style type="text/css">
.roundBorder *
{
 background: white;
 display: block;
 height: 1px;
 overflow: hidden;
}
</style>
……
 <b class="roundBorder">
 <b class="roundBorderLayer3"></b>
 <b class="roundBorderLayer2"></b>
 <b class="roundBorderLayer1"></b>
</b>
那么这个roundBorderLayer1、2、3都会受这个.roundBorder *的定义影响。
二、继承
CSS可以有多重继承。如
        <div id="divTenet">
            <div style="font-size:16px;width:240px;">鸟0</div>
            <div><img src="img/k.gif" width="24" height="20" hspace="3" alt="鸟1"/>鸟1</div>
            <div><img src="img/jc.gif" width="21" height="23" hspace="3" alt="鸟2"/>鸟2</div>
            <div><img src="img/fx.gif" width="22" height="20" hspace="3" alt="鸟3"/>鸟3</div>
        </div>
CSS里可以这样定义里面的<IMG>：#divTenet div img{vertical-align:middle;}
