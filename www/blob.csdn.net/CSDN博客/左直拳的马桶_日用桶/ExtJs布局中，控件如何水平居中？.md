# ExtJs布局中，控件如何水平居中？ - 左直拳的马桶_日用桶 - CSDN博客
2015年06月26日 18:40:39[左直拳](https://me.csdn.net/leftfist)阅读数：11820
如此即可，有图有代码有j8:
![这里写图片描述](https://img-blog.csdn.net/20150626183950781)
```java
var formGridHead = Ext.create('Ext.form.Panel', {
        id: 'MyGridHead',
        region: 'north',
        bodyPadding: 10,
        bodyStyle: 'background:#fff',
        split: false,
        header: false,
        collapsible: true,
        items: [
                { xtype: 'panel'
                , border: false
                , layout: {
                    type: 'vbox',
                    align: 'center'
                }, width: '100%', items: [
                {
                    xtype: 'fieldcontainer'
                    , layout: 'column'
                    , items: [
{
    xtype: 'button',
    text: '<span class="hydg_btn">标准查询</span>',
    scale: 'medium',
    id: 'btnSearchOption',
    listeners: {
        click: {
    }
}
}, { html: '', border: false, width: 30 }
, {
    xtype: 'button',
    text: '<span class="hydg_btn">高级查询</span>',
    scale: 'medium',
    id: 'btnSearchOptionPower',
    listeners: {
        click: {
    }
}
}, { html: '', border: false, width: 30 }
                        , {
                            xtype: 'button',
                            text: '<span class="hydg_btn">浏览统计图</span>',
                            scale: 'medium',
                            id: 'btnViewTongji',
                            listeners: {
                                click: {
                            }
                        }
                    }
                        ]
                }
                    ]
                }
            ]
    });
```
其中重点在于：
```java
, layout: {
                    type: 'vbox',
                    align: 'center'
                }, width: '100%',
```
