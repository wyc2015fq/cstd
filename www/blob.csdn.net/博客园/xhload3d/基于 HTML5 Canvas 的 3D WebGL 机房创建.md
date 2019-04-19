# 基于 HTML5 Canvas 的 3D WebGL 机房创建 - xhload3d - 博客园
对于 3D 机房来说，监控已经不是什么难事，不同的人有不同的做法，今天试着用 HT 写了一个基于 HTML5 的机房，发现果然 HT 简单好用。本例是将灯光、雾化以及 eye 的最大最小距离等等功能在 3D 机房中进行的一个综合性的例子。接下来我将对这个例子的实现进行解析，算是自己对这个例子的一个总结吧。整个例子因为没有设计师的参与，所以样式上可能比较简陋，但是在一些细节的地方，比如墙上的贴图、门框嵌入以及灭火器等等，都是尽可能地还原真实的场景，也是花了些心思做这个例子的！
本例地址：[http://www.hightopo.com/guide/guide/core/3d/examples/example_3droom.html](http://www.hightopo.com/guide/guide/core/3d/examples/example_3droom.html)
本例动态图：
![](https://images2017.cnblogs.com/blog/591709/201712/591709-20171224205621006-729271335.gif)
从最基础的开始，场景的布置，照 HTML 的思路，这个场景就是将整个页面放在一个 div 中，再向这个 div 中添加一个顶部的 div 以及一个中间部分的 div，说实在，如果用 HTML 实现这个步骤，我们要写的代码不多，但是如果要写几次这段代码，想必谁都会觉得烦吧。。。HT 将这种实现方法封装了起来，整个外部的 div 就是 HT 的基础组件，这里我们用到的是 ht.widget.BorderPane 组件，上部的 div 封装在 ht.widget.Toolbar 工具条类中，下部分是 3D 部分 ht.graph3d.Graph3dView 组件，将页面中的两个部分通过下面的方式添加进 BorderPane 组件中，并将 BorderPane 添加进 body 体中：
```
toolbar = new ht.widget.Toolbar(items);                                                                                                
dataModel = new ht.DataModel();
g3d = new ht.graph3d.Graph3dView(dataModel);    
g3d.getView().style.background = 'black';
g3d.setGridSize(100);
g3d.setGridGap(100); 
g3d.setFar(30000);
g3d.setOrthoWidth(5000);  
g3d.setFogNear(100);//默认为1，代表从该距离起物体开始受雾效果影响
g3d.setFogFar(8000);
g3d.reset = reset;
g3d.reset();
g3d.enableToolTip();                                 
borderPane = new ht.widget.BorderPane();
borderPane.setTopView(toolbar);
borderPane.setCenterView(g3d);       
view = borderPane.getView();  
view.className = 'main';
document.body.appendChild(view);
window.addEventListener('resize', function (e) {
    borderPane.invalidate();
}, false);
```
上面代码将 borderPane.getView() 添加进 body 体中，是因为 Graph3dView 的界面 DOM 结构是由最底层的 div 元素，以及渲染层 canvas 元素组合而成，通过 getView() 可得到最底层的 div 元素。
我们还注意到上面代码中没有提及的一个参数 items，这个 items 是 toolbar 工具条的元素，一个数组元素，下面展示一下代码，这里简单地解释了一下，详细信息请参考 [HT for Web 工具条手册](http://www.hightopo.com/guide/guide/core/toolbar/ht-toolbar-guide.html)：
```
items = [ //工具条中的元素数组
    {
        label: 'White',//工具条元素的标签文字
        groupId: 'headLightColor',//对工具条元素进行分组，同一个分组内的元素选中会自动出现互斥效果
        selected: true,//工具条元素是否被选中，值为true或false，对复选框、开关按钮和单选按钮有效
        action: function(){// 函数类型，工具条元素被点击时调用
            g3d.setHeadlightColor('white');
        }
    },        
    {
        label: 'Red',
        groupId: 'headLightColor',
        action: function(){                             
            g3d.setHeadlightColor('red');
        }
    }, 
    {
        label: 'Blue',
        groupId: 'headLightColor',
        action: function(){                             
            g3d.setHeadlightColor('blue');
        }
    },        
    {
        label: 'Yellow',
        groupId: 'headLightColor',        
        action: function(){                             
            g3d.setHeadlightColor('yellow');
        }
    },        
    {
        id: 'step',                        
        unfocusable: true,//工具条元素是否不可获取焦点，默认鼠标滑过时会显示一个矩形边框，可设置为true关闭此效果
        slider: {
            width: 70,
            step: 500,
            min: 0,
            max: 10000,
            value: 0,                            
            onValueChanged: function(){
                g3d.setHeadlightRange(this.getValue());
            }       
        }                
    },
    'separator', //表示分隔条
    {
        label: 'Fog',
        type: 'check',//工具条元素类型，check表示复选框，toggle表示开关按钮，radio表示单选按钮                        
        action: function(){
            g3d.setFogDisabled(!this.selected);
        }
    },
    {
        label: 'White',
        groupId: 'fogColor',
        selected: true,
        action: function(){                             
            g3d.setFogColor('white');
        }
    },                     
    {
        label: 'Red',
        groupId: 'fogColor',
        action: function(){                             
            g3d.setFogColor('red');
        }
    },        
    {
        label: 'Yellow',
        groupId: 'fogColor',        
        action: function(){                             
            g3d.setFogColor('yellow');
        }
    },
    {                       
        unfocusable: true,
        label: 'FogNear',
        slider: {
            width: 70,
            min: 10,
            max: 4000,
            value: 100,                            
            onValueChanged: function(){
                g3d.setFogNear(this.getValue());
            }       
        }                
    },                        
    {                       
        unfocusable: true,
        label: 'FogFar',
        slider: {
            width: 70,
            min: 5000,
            max: 15000,
            value: 8000,                            
            onValueChanged: function(){
                g3d.setFogFar(this.getValue());
            }       
        }                
    },                    
    'separator', 
    {
        id: 'movable',
        label: 'Movable',
        type: 'check',
        selected: false                
    }, 
    {
        label: 'Editable',
        type: 'check',
        selected: false,
        action: function(item){
            g3d.setEditable(item.selected);
        }
    },
    {
        label: 'Wireframe',
        type: 'check',
        selected: false,
        action: function(item){
            if(item.selected){
                dataModel.each(function(data){
                    data.s({
                        'wf.visible': 'selected',
                        'wf.color': 'red'                                        
                    });
                });                                
            }else{
                dataModel.each(function(data){
                    data.s({
                        'wf.visible': false                                       
                    });
                });                               
            }                            
        }
    },
    {
        type: 'toggle',
        label: 'Orthographic Projection',                        
        action: function(item){  
            g3d.setOrtho(item.selected);                           
        }                    
    },
    {
        type: 'toggle',
        selected: false,
        label: 'First Person Mode',
        action: function(item){
            g3d.setFirstPersonMode(item.selected);  
            g3d.reset();
        }
    },                                                       
    'separator',                      
    {
        type: 'check',
        label: 'Origin Axis',
        selected: false,
        action: function(item){
            g3d.setOriginAxisVisible(item.selected);                           
        }
    },   
    {
        type: 'check',
        label: 'Center Axis',
        selected: false,
        action: function(item){
            g3d.setCenterAxisVisible(item.selected);                           
        }
    },                             
    {
        type: 'check',
        label: 'Grid',
        selected: false,
        action: function(item){
            g3d.setGridVisible(item.selected);                           
        }
    },                    
    {
        label: 'Export Image',
        action: function(){                             
            var w = window.open();
            w.document.open();                            
            w.document.write("<img src='" + g3d.toDataURL(g3d.getView().style.background) + "'/>");
                            w.document.close();
        }
    }                    
];
```
接下来就是创建场景中的各个模型，首先是三个灯光，中间部分一个，左右后方各一个（我将光源标记出来了，看图~），颜色分别为绿、红、蓝，以及地板：
![](https://images2017.cnblogs.com/blog/591709/201712/591709-20171224221132568-1696877536.png)
```
redLight = new ht.Light();//灯光类
redLight.p3(-1600, 200, -2200);
redLight.s({
    'light.color': 'red',
    'light.range': 1000
});
dataModel.add(redLight);
                
blueLight = new ht.Light();
blueLight.p3(1600, 200, -2200);
blueLight.s({
    'light.color': 'blue',
    'light.range': 1000
});
dataModel.add(blueLight);                                   
                
greenLight = new ht.Light();
greenLight.p3(-800, 400, -200);
greenLight.s({
    'light.center': [-300, 0, -900],
    'light.type': 'spot',
    'light.color': 'green',
    'light.range': 4000,
    'light.exponent': 10
});
dataModel.add(greenLight);                
                
floor = createNode([0, -5, -1500], [5200, 10, 4200]);
floor.s({                             
    'shape3d': 'rect',
    'shape3d.top.color': '#F0F0F0'                    
});
```
接下来将场景中间部分的服务器、两边的服务器、灭火器，墙上的空调、墙背面的空调等等等等都添加进场景中：
```
for(i=0; i<3; i++){
    for(j=0; j<3; j++){
        createServer1(250+i*280, -1200-500*j, j === 2);
        createServer1(-250-i*280, -1200-500*j, j === 1);
    }
}
                     
//创建2排2的服务器 放在场景的两边
for(i=0; i<2; i++){
    for(j=0; j<2; j++){
        createServer2(1500+i*200, -700-500*j, 
                            (i === 1 ? (j === 1 ? '#00FFFF' : '#C800FF') : null));
        createServer2(-1500-i*200, -700-500*j, 
                            (j === 1 ? (i === 1 ? 'red' : 'yellow') : null));
    }
}                                          
          
// fire extinguisher
createFireExtinguisher(1300, -1800, [0.45, 0]); 
createFireExtinguisher(-1300, -1800); 
createFireExtinguisher(1100, -2450);   
createFireExtinguisher(-1100, -2450, [0.45, 0]);  
                
// air condition
createNode([1120, 170, -700], [80, 340, 170]).s({
    'all.color': '#EDEDED',
    'left.image': 'stand'
}).setToolTip('Air Conditioner'); 
createNode([-1120, 170, -700], [80, 340, 170]).s({
    'all.color': '#EDEDED',
    'right.image': 'stand'
}).setToolTip('Air Conditioner'); 
createNode([1680, 400, -1850], [370, 120, 60]).s({
    'all.color': '#767676',
    'front.image': 'air1'
}).setToolTip('Air Conditioner');             
createNode([-1680, 400, -1850], [370, 120, 60]).s({
   'all.color': '#767676',
   'front.image': 'air2'
}).setToolTip('Air Conditioner'); 
for(i=0; i<2; i++){
    createNode([300+i*580, 90, -2630], [260, 180, 60]).s({
        'all.color': '#EDEDED',
        'back.image': 'air3'
    }).setToolTip('Air Conditioner');  
    createNode([-300-i*580, 90, -2630], [260, 180, 60]).s({
        'all.color': '#EDEDED',
        'back.image': 'air3'
    }).setToolTip('Air Conditioner');                      
}
```
 其中 createServer1 方法是用来创建服务器的方法，由一个 ht.Node 作为机身以及 ht.Shape 作为机门组合而成，并在机柜的内部添加了随机数台设备：
```
function createServer1(x, z, disabled){//创建服务器 1（中间部分）
    var h = 360, w = 150, d = 170, k = 10,
         main = createNode([0, 0, 0], [w, h, d]),
         face = new ht.Shape(),
         s = {'all.visible': false, 'front.visible': true};  //设置node节点只有前面可见       
                  
    dataModel.add(face);
    face.addPoint({x: -w/2, y: d/2-1});//门上的三个点
    face.addPoint({x: w/2, y: d/2-1});
    face.addPoint({x: w+w/2, y: d/2-1});
    face.setSegments([1, 2, 1]);                
    face.setTall(h);
    face.setThickness(1);//设置厚度      
    face.s(s);                
    face.setHost(main);//吸附在main节点上
                
    main.s({
        'all.color': '#403F46',
        'front.visible': false
    });                
                
    if(!disabled){                
        face.s({
            'all.color': 'rgba(0, 40, 60, 0.7)',
            'all.reverse.flip': true,//反面是否显示正面的东西
            'all.transparent': true//设置为透明
       });
       face.face = true;//初始化face属性             
       face.open = false;
       face.angle = -Math.PI * 0.6;                       
       face.setToolTip('Double click to open the door');
                    
       var up = createNode([0, h/2-k/2, d/2], [w, k, 1]).s(s),
            down = createNode([0, -h/2+k/2, d/2], [w, k, 1]).s(s),
            right = createNode([w/2-k/2, 0, d/2], [k, h, 1]).s(s),
            left = createNode([-w/2+k/2, 0, d/2], [k, h, 1]).s(s);
                                
       up.setHost(face);
       down.setHost(face);
       left.setHost(face);
       right.setHost(face);
       //随机创建机柜中的设备数量 2个或者4个
       var num = Math.ceil(Math.random() * 2),
       start = 20 + 20 * Math.random();
       for(var i=0; i<num; i++){
           var node = createNode([0, start+45*i, 0], [w-6, 16, d-30]);
           node.setHost(main);
           node.s({
               'front.image': 'server',
               'all.color': '#E6DEEC',
               'all.reverse.cull': true
           });
           node.pop = false;//初始化设备“弹出”的属性
           node.setToolTip('Double click to pop the server');
           var node = createNode([0, -start-45*i, 0], [w-6, 16, d-30]);
           node.setHost(main);
           node.s({
               'front.image': 'server',
               'all.color': '#E6DEEC',
               'all.reverse.cull': true
           }); 
           node.pop = false;
           node.setToolTip('Double click to pop the server');
       }                    
   }
                
   main.p3(x, h/2+1, z);
}
```
这个服务器我们还制作了门的打开以及关闭的动作，还有服务器内部的设备的弹出以及恢复位置。首先，对 3d 组件添加了过滤函数，对双击事件的元素过滤：
![](https://images2017.cnblogs.com/blog/591709/201712/591709-20171224233444256-2107749922.png)
```
g3d.onDataDoubleClicked = function(data, e, dataInfo){//图元被双击时回调
    if(data.face){//face是定义门时候开启的属性 是在节点定义的时候添加的 下面的 pop 也是一样
        data.getHost().getAttaches().each(function(attach){//遍历吸附到自身的所有节点的ht.List类型数组
            if(attach.pop){//pop 是定义设备是否弹出的属性
                toggleData(attach);
            }
       });
    }
    toggleData(data, dataInfo.name);
};
```
 以下是对 toggleData 的定义：
```
function toggleData(data, name){//开关门 以及 设备弹出缩回的动作
    var angle = data.angle,
    pop = data.pop;
            
    if(angle != null){
        if(anim){
            anim.stop(true);//ht内置的函数 参数为true时终止动画
        }
        var oldAngle = data.window ? data.getRotationX() : data.getRotation();
        if(data.open){
            angle = -angle;
        }
        data.open = !data.open;
        anim = ht.Default.startAnim({
            action: function(v){
                if(data.window){
                    data.setRotationX(oldAngle + v * angle);    
               }else{
                   data.setRotation(oldAngle + v * angle);
               }                                
           }
       });
   }
   else if(pop != null){
        if(anim){
           anim.stop(true);
        }
        var p3 = data.p3(),
            s3 = data.s3(),
            dist = pop ? -s3[2] : s3[2];
        data.pop = !data.pop;                        
        if(data.pop){
            data.s({
                'note': 'Detail...',  
                'note.background': '#3498DB',
                'note.font': '26px Arial',
                'note.position': 6,
                'note.t3': [-30, -3, 30],
                'note.expanded': true,
                'note.toggleable': false,
                'note.autorotate': true                        
            });                                     
        }else{
             data.s('note', null);
        }                                                
        anim = ht.Default.startAnim({
             action: function(v){
                 data.p3(p3[0], p3[1], p3[2] + v * dist);                                                               
             }
        }); 
    }            
}
```
中间部分的服务器我就不赘述了，这里聊一下比较有趣的部分，灭火器模型的制作，并不是 Max3d 制作而成的，而是代码生成的：
![](https://images2017.cnblogs.com/blog/591709/201712/591709-20171224232542537-507672601.png)
从图中可以看出来，这个灭火器是由底部一个圆柱，中间一个半圆形以及顶部一个小圆柱并在上面贴图合成的：
```
function createFireExtinguisher(x, z, uvOffset){//创建灭火器
    var w = 80, 
          h = 200,
          body = createNode([0, 0, 0], [w, h, w]).s({//身体的圆柱
              'shape3d': 'cylinder',//圆柱
              'shape3d.image': 'fire',
              'shape3d.uv.offset': uvOffset,//决定3d图形整体贴图的uv偏移
              'shape3d.reverse.cull': true
          }),                    
          sphere = createNode([0, h/2, 0], [w, w, w]).s({
              'shape3d': 'sphere',//球体
              'shape3d.color': '#F20000',
              'shape3d.reverse.cull': true
          }),
          top = createNode([0, h/2+w/3, 0], [w/2, w/2, w/2]).s({//头部的圆柱
              'shape3d': 'cylinder',
              'shape3d.color': '#242424',
              'shape3d.reverse.cull': true
          });
                    
    sphere.setHost(body);//设置吸附 只要body移动sphere也会移动
    top.setHost(sphere);
    body.p3([x, h/2, z]);
    body.setToolTip('Fire Extinguisher');
    sphere.setToolTip('Fire Extinguisher');
    top.setToolTip('Fire Extinguisher');
}
```
 以上！这个 3d 机房的例子非常有代表性，性能也展示得很全面，觉得有必要拿出来讲一下，希望能对你们有一定的帮助~
