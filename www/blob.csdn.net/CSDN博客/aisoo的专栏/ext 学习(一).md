# ext 学习(一) - aisoo的专栏 - CSDN博客
2009年12月30日 22:11:00[aisoo](https://me.csdn.net/aisoo)阅读数：495标签：[ext																[tools																[tree																[border																[html																[function](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=border&t=blog)](https://so.csdn.net/so/search/s.do?q=tree&t=blog)](https://so.csdn.net/so/search/s.do?q=tools&t=blog)](https://so.csdn.net/so/search/s.do?q=ext&t=blog)
// [http://www.faceye.com/blogs/category/ext?startIndex=40](http://www.faceye.com/blogs/category/ext?startIndex=40)
Ext.onReady(function() {
 // layout
 Ext.BLANK_IMAGE_URL = 'ext/resources/images/default/s.gif';
 Ext.state.Manager.setProvider(new Ext.state.CookieProvider());
 // create some portlet tools using built in Ext tool ids
 // var tools = [
 // // {
 // // id:'gear',
 // // handler: function(){
 // // Ext.Msg.alert('Message', 'The Settings tool was clicked.');
 // // }
 // // },
 // {
 // id:'close',
 // handler: function(e, target, panel){
 // panel.ownerCt.remove(panel, true);
 // }
 // }];
 var viewport = new Ext.Viewport({
  layout : 'border',
  items : [new Ext.BoxComponent({
       region : 'north',
       el : 'north',
       height : 50
      }), new Ext.BoxComponent({
       region : 'south',
       el : 'south',
       height : 25
      }), {
     region : 'west',
     id : 'west-panel',
     // autoHeight : true,
     split : true,
     width : 200,
     minSize : 175,
     maxSize : 200,
     margins : '10 0 0 0',
     layout : 'accordion',
     title : '导航',
     collapsible : true,
     layoutConfig : {
      animate : true
     },
     items : [{
      title : '公告信息',
      border : false,
      html : '<div id="tree-div" style="overflow:auto;width:100%;height:100px"></div>'
       // iconCls:'nav'
     }, {
      title : '项目管理',
      border : false,
      // iconCls:'settings',
      html : '<div id="tree" style="overflow:auto;width:100%;height:100%"></div>'
     }, {
      title : '考勤管理',
      border : false,
      // iconCls:'settings',
      html : '<div id="tree3" style="overflow:auto;width:100%;height:100%"></div>'
     }, {
      title : '设备管理',
      border : false,
      // iconCls:'settings',
      html : '<div id="tree" style="overflow:auto;width:100%;height:100%"></div>'
     }, {
      title : '资料借阅',
      border : false,
      // iconCls:'settings',
      html : '<div id="tree" style="overflow:auto;width:100%;height:100%"></div>'
     }, {
      title : '座位管理',
      border : false,
      // iconCls:'settings',
      html : '<div id="tree" style="overflow:auto;width:100%;height:100%"></div>'
     }, {
      title : '门区管理',
      border : false,
      // iconCls:'settings',
      html : '<div id="tree" style="overflow:auto;width:100%;height:100%"></div>'
     }, {
      title : '帐号管理',
      border : false,
      // iconCls:'settings',
      html : '<div id="tree" style="overflow:auto;width:100%;height:100%"></div>'
     }, {
      title : '维基百科',
      border : false,
      // iconCls:'settings',
      html : '<div id="tree" style="overflow:auto;width:100%;height:100%"></div>'
     }, {
      title : '公司规定',
      border : false,
      // iconCls:'settings',
      html : '<div id="tree" style="overflow:auto;width:100%;height:100%"></div>'
     }]
    }, {
     xtype : 'portal',
     region : 'center',
     margins : '10 5 5 0',
     items : [{
      columnWidth : .68,
      style : 'padding:10px 0 10px 10px',
      items : [{
         height : 200,
         title : '新闻',
         // tools: tools,
         html : "<p>jkjkhjkhnjgddgdgddghdghdhdhdghd</p>"
        }, {
         height : 200,
         title : '代办事宜',
         // tools: tools,
         html : "<p>jkjkhjkhnjgddgdgddghdghdhdhdghd</p>"
        }, {
         height : 200,
         title : '投票',
         // tools: tools,
         html : "<p>jkjkhjkhnjgddgdgddghdghdhdhdghd</p>"
        }, {
         height : 200,
         title : '今日日程',
         // tools: tools,
         html : "<p>jkjkhjkhnjgddgdgddghdghdhdhdghd</p>"
        }, {
         height : 200,
         title : '内部讨论区',
         // tools: tools,
         html : "<p>jkjkhjkhnjgddgdgddghdghdhdhdghd</p>"
        }]
     }, {
      columnWidth : .32,
      style : 'padding:10px',
      items : [{
         height : 200,
         title : '公告通知',
         // tools: tools,
         html : "<p>jkjkhjkhnjgddgdgddghdghdhdhdghd</p>"
        }, {
         height : 200,
         title : '内部邮件',
         // tools: tools,
         html : "<p>jkjkhjkhnjgddgdgddghdghdhdhdghd</p><p>jkjkhjkhnjgddgdgddghdghdhdhdghd</p>"
        }, {
         height : 200,
         title : '近日日程(10日内)',
         // tools: tools,
         html : "<p>jkjkhjkhnjgddgdgddghdghdhdhdghd</p><p>jkjkhjkhnjgddgdgddghdghdhdhdghd</p>"
        }, {
         height : 200,
         title : '外出人员',
         // tools: tools,
         html : "<p>jkjkhjkhnjgddgdgddghdghdhdhdghd</p><p>jkjkhjkhnjgddgdgddghdghdhdhdghd</p>"
        }, {
         height : 200,
         title : '本月日程',
         // tools: tools,
         html : "<p>jkjkhjkhnjgddgdgddghdghdhdhdghd</p><p>jkjkhjkhnjgddgdgddghdghdhdhdghd</p>"
        }]
     }]
    }]
 });
 /**
  * ---------------------------------------------------------- Start Tree
  * Define Define tree Struct Tree Define By Bai Lian Dong
  * ----------------------------------------------------------
  */
 var Tree = Ext.tree;
 var treepanel = new Tree.TreePanel({
    id : 'default-manager-tree',
    el : 'tree-div', // 填充区域
    rootVisible : false, // 隐藏根节点
    border : false, // 边框
    animate : true, // 动画效果
    autoScroll : true, // 自动滚动
    enableDD : false, // 拖拽节点
    containerScroll : true,
    lines : false,
    loader : new Tree.TreeLoader({
      // dataUrl : BP + 'treeAction.do?method=tree'
      })
   });
 // 异步加载根节点
 var rootnode = new Tree.AsyncTreeNode({
    id : 'source',
    draggable : false,
    text : '目录树根节点'
   });
 // 设置根节点
 treepanel.setRootNode(rootnode);
 treepanel.on('click', function(node) {
  if (node.isLeaf()) {
   var centerContainer = Ext.getCmp('center-container');
   var container = Ext.getCmp('manager-center-container');
   if (container) {
    container = new Ext.Panel({
     autoScroll : false,
     id : 'manager-center-container',
     border : false,
     html : '<iframe id="content-iframe" frameborder="no" style="width:100%;height:99.9%"></iframe>'
    });
    centerContainer.add(container);
    centerContainer.doLayout();
   }
   if (node.attributes.url.indexOf('.js') != -1) {
    var url = node.attributes.url;
    Ext.Ajax.request({
       url : url,
       params : {
        id : node.id,
        name : node.text
       },
       success : function(response, options) {
        var source = response.responseText;
        var headerDom = document.getElementsByTagName('head').item(0);
        var jsDom = document.createElement('script');
        jsDom.type = 'text/javascript';
        jsDom.language = 'javascript';
        jsDom.defer = true;
        jsDom.text = source;
        headerDom.appendChild(jsDom);
        var portlet;
        portlet = com.easou.portal.portlet.SinglePortlet.init(
        options.params.id, options.params.name);
        if (portlet.draggable) {
         portlet.draggable = false;
        }
        if (portlet.tools) {
         portlet.tools = null;
        }
        if (portlet.collapsible) {
         portlet.collapsible = false;
        }
        var vH = document.body.clientHeight;
        portlet.setHeight(vH - 30);
        // portlet.doLayout();
        container.add(portlet);
        centerContainer.doLayout();
       }
      });
   } else {
    Ext.get('content-iframe').dom.src = node.attributes.link
      + '&node=' + node.id;
   }
   return true;
  } else {
   // open node by single click,not double click.
   node.toggle();
  }
 });
 // render the tree
 treepanel.render();
 rootnode.expand();
 // tree2
 var root2 = new Ext.tree.TreeNode({
    id : "root2",
    text : "树的根"
   });
 var c1 = new Ext.tree.TreeNode({
    id : 'c1',
    icon : 'images/im2.gif',
    text : '信息征集'
   });
 var c2 = new Ext.tree.TreeNode({
    id : 'c2',
    icon : 'images/im2.gif',
    href : '[http://www.163.com/](http://www.163.com/)',
    target : '_blank',
    text : 'bug征集'
   });
 var c22 = new Ext.tree.TreeNode({
    id : 'c22',
    icon : 'images/im2.gif',
    href : '[http://www.163.com/](http://www.163.com/)',
    target : '_blank',
    text : '实例征集'
   });
 var c3 = new Ext.tree.TreeNode({
    id : 'c3',
    icon : 'images/im2.gif',
    href : '[http://www.163.com/](http://www.163.com/)',
    target : '_blank',
    text : '给我留言'
   });
 var c4 = new Ext.tree.TreeNode({
    id : 'c4',
    icon : 'images/im2.gif',
    href : '[http://www.163.com/](http://www.163.com/)',
    target : '_blank',
    text : '项目主页'
   });
 c1.appendChild(c2);// 为c1增加子结点
 c1.appendChild(c22);
 root2.appendChild(c1);// 为根节点增加子结点c1
 root2.appendChild(c3);
 root2.appendChild(c4);
 // 生成树形面板
 var tree2 = new Ext.tree.TreePanel({
    renderTo : "tree",
    root : root2,
    animate : true,
    enableDD : false,
    border : false,
    rootVisible : false,
    containerScroll : true
   });
 // tree3
 var root3 = new Ext.tree.TreeNode({
    id : "root3",
    text : "树的根"
   });
 var c1 = new Ext.tree.TreeNode({
    id : 'c1',
    icon : 'images/im2.gif',
    text : '信息征集'
   });
 var c2 = new Ext.tree.TreeNode({
    id : 'c2',
    icon : 'images/im2.gif',
    href : '[http://www.163.com/](http://www.163.com/)',
    target : '_blank',
    text : 'bug征集'
   });
 var c3 = new Ext.tree.TreeNode({
    id : 'c3',
    icon : 'images/im2.gif',
    href : '[http://www.163.com/](http://www.163.com/)',
    target : '_blank',
    text : '给我留言'
   });
 c1.appendChild(c2);// 为c1增加子结点
 root3.appendChild(c1);// 为根节点增加子结点c1
 root3.appendChild(c3);
 // 生成树形面板
 var tree2 = new Ext.tree.TreePanel({
    renderTo : "tree3",
    root : root3,
    animate : true,
    enableDD : false,
    border : false,
    rootVisible : false,
    containerScroll : true
   });
 // end loding
 setTimeout(function() {
    Ext.get('loading').remove();
    Ext.get('loading-mask').fadeOut({
       remove : true
      });
   }, 250);
});
