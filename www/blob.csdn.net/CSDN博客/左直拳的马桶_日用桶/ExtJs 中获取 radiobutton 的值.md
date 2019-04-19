# ExtJs 中获取 radiobutton 的值 - 左直拳的马桶_日用桶 - CSDN博客
2015年08月27日 19:33:36[左直拳](https://me.csdn.net/leftfist)阅读数：1629
ExtJs中使用radiobutton，想在ExtJs本身代码中（注意，不是在后台喔）获取 radioButton 的值，居然发现很难，无从下手的感觉。后来在网上寻寻觅觅，痛苦摸索之后，真是大跌眼镜。
代码如下：
```java
{
    xtype: 'fieldcontainer', layout: { type: 'column' }
    , items: [
            {
                xtype: 'radiogroup', fieldLabel: '统计方式', id: 'rdTjType', name: 'TjType'
                , columnWidth: 0.5
                , items: [
                { boxLabel: '按行政区域', name: 'TjType', inputValue: 'qhid', checked: true }
                , { boxLabel: '按年份', name: 'TjType', inputValue: 'year' }
            ]
                , listeners: {
                    'change': function (group, newV, oldV, e) {
//看到没，获得radiobuttongroup的选中值，居然是属性“TjType”！这个属性并不是ExtJs固有的，而是根据我们设置的radiobutton的name来相应设置的。并不是什么newV.inputValue!真是匪夷所思
                        var type = newV.TjType;
                        setTongJiParam(type);
                    }
                }
            }
    ]
}
```
ExtJs一般给人的感觉就是很笨重，界面好像也很难修改；它大包大揽，除了语法还是javascript + json，好多东西都需要开发者猛查资料才明白，更遑论这些匪夷所思的小地方了。疯狂。
