# flex-分页控件 - aisoo的专栏 - CSDN博客
2010年08月17日 17:13:00[aisoo](https://me.csdn.net/aisoo)阅读数：284
<mx:Script>
   <![CDATA[
    import mx.controls.Alert;
    private var enuName:EnuName = new EnuName();
    private var enuUsed:EnuUsed = new EnuUsed();
    private var enuValue:EnuValue = new EnuValue(); 
    private function init():void{    
     nav.addChild(enuName);
     nav.addChild(enuUsed);
     nav.addChild(enuValue); 
    }   
    private function test():void{
     if(nav.selectedIndex==1){
      enuUsed.init();
     }else if(nav.selectedIndex==2){
      enuValue.init();
     }
    }   
   ]]>
</mx:Script>
   <mx:TabNavigator id="nav" width="100%" height="100%" change="test()" styleName="tabStyle" > 
   </mx:TabNavigator>
效果图![](http://hiphotos.baidu.com/henvy/pic/item/b81560118069f74acb80c4f2.jpg)
