# ExtJs里表格自动显隐滚动条 - 左直拳的马桶_日用桶 - CSDN博客
2015年06月24日 10:48:45[左直拳](https://me.csdn.net/leftfist)阅读数：1863
ExtJs里面，layout:’border’这种布局应该很常用，但我用的时候，因为不熟，走了一些弯路。比如说，一个页面，大体布局是这样的：
上：查询输入框 
中+下：查询结果（表格，底部有分页控件）
代码如下，其中有些属性一定要设置才能达到理想的效果（参阅注释）：
```java
var formQuery = Ext.create('QueryFormTongji', {
        id: 'formPower1',region: 'north'//这个region很重要！如果没有它，此查询框将被下面的表格所屏蔽
    });//查询框
    var formTu = Ext.create('QueryFormTongjiTu', {
        id: 'formTu', heigth: 220, region: 'north'
    });//统计图
    var grid = Ext.create('my.view.test.List', {
        id: 'grid1',
        region: 'center',
        store: listStore,
        columns: [……]});//查询结果（表格）
        var vp = Ext.create('Ext.container.Viewport', {
        layout: { type: 'border' },
        items: [{
            xtype: 'tabpanel',
            region: 'center',
            id: 'topPanel',
            activeTab: 0,
            items: [
                            {
                                xtype: 'panel',
                                title: '查询',
                                id: 'panelPower',
                                layout: 'border',//有了这个属性，表格才会随分辨率而自动显隐垂直滚动条
                                items: [formQuery,grid]
                            },
                           {
                               xtype: 'panel',
                               title: '统计图',
                               layout: 'border',
                               id: 'tPanel',
                               items: [
                                    formTu,
                                    {
                                        autoScroll: true,
                                        xtype: 'panel',
                                        region: 'center',
                                        contentEl: 'DivMapWinInfo'
                                    }
                                ]
                           }
                    ],
            listeners: {
                tabchange: {
                    fn: this.onTopTabpanelTabChange,
                    scope: this
                }
            }
        }]
    });
```
