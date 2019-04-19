# 基于 HTML5 Canvas 的 3D 压力器反序列化 - xhload3d - 博客园
在实际应用中，我觉得能够通过操作 JSON 文件来操作 3D 上的场景变化是非常方便的一件事，尤其是在做编辑器进行拖拽图元并且在图元上产生的一系列变化的时候，都能将数据很直观地反应给我们，这边我们简单地做了个基础的例子，给大家参考看看。
本例地址： [http://www.hightopo.com/guide/guide/core/serialization/examples/example_exportimport.html](http://www.hightopo.com/guide/guide/core/serialization/examples/example_exportimport.html)
实践场景再现：
![](https://images2017.cnblogs.com/blog/591709/201712/591709-20171217215355749-1731031449.gif)
首先我们搭建一下这个例子的场景，熟悉的朋友可能已经看出来了，这个场景分为三个部分：左、右上以及右下。HT 通过 [ht.widget.SplitView](http://www.hightopo.com/guide/guide/core/splitview/ht-splitview-guide.html) 可以很轻松地将场景分割，实现良好的页面布局，最后将这个分割组件添加进 html 的 body 体中：
```
//场景搭建
dataModel = new ht.DataModel();//数据容器
                
g3d = new ht.graph3d.Graph3dView(dataModel);//3D 组件
propertyView = new ht.widget.PropertyView(dataModel);// 属性组件
formPane = new ht.widget.FormPane();//表单组件
rightSplit = new ht.widget.SplitView(propertyView, formPane, 'v', 100);//分割组件
                        
new ht.widget.SplitView(g3d, rightSplit, 'h', 0.65).addToDOM();
```
接下来就是向场景中添加图元，并把图元添加到 3D 场景中，这时我们可以向图元中添加各种属性和样式以及标签作为标记，本例中用到的图元是 3D 模型，利用 [ht.Default.parseObj](http://www.hightopo.com/guide/guide/plugin/obj/ht-obj-guide.html#ref_parseobj) 函数对 obj 和 mtl 文件进行解析：
```
//添加模型
var params = {center: true};//JSON格式控制参数 传入 ht.Default.parseObj 函数中
var modelMap = ht.Default.parseObj(meter_obj, meter_mtl, params);//解析obj和mtl文件， 解析后返回的map结构json对象中，每个材质名对应一个模型信息
```
当然，前提是要已经声明了 meter_obj 以及 meter_mtl 两个文件，这里我们是将这两个部分分别放到 js 文件中，并在头部调用。
从上面的动图中我们可以看到，这个例子中需要变化的模型部分只有“指针”以及下面的“开关”两个部分，所以我们通过遍历的方式获取这两个 obj 模型的部分，并注册 3D 模型：
```
var array = [];
for(var name in modelMap){
    var model = modelMap[name];//modelMap 中的模型
    array.push(model);
                    
    if(name === 'pointer'){//obj 文件中的一个模型 名称为 pointer
        model.mat = {//矩阵变化参数，可对模型进行矩阵变化后导入 
            func: function(data){
                var start = Math.PI * 0.736,
                range = Math.PI * 1.49,   
                angle = start - range * data.a('meter.value') / 4;//动态获取了 meter.value 的值
                return ht.Default.createMatrix([//将一组JSON描述的缩放、移动和旋转等操作转换成对应的变化矩阵
                    { t3: [0, -82.5, 0] },
                    { r3: [0, 0, angle] },
                    { t3: [0, 82.5, 0]  }
                ]);
            }
        };                         
    }
    if(name === 'switch'){//obj 文件中的一个模型 名称为 switch
        model.mat = {
            func: function(data){
                return ht.Default.createMatrix([
                    { t3: [0, 48.5, 0] },
                    { r3: [0, 0, data.a('meter.angle')] },
                    { t3: [0, -48.5, 0]  }
                ]);
            }
        }; 
        model.color = {
            func: function(data){
                if(data.a('meter.angle')){
                    return 'rgb(186, 0, 0)';
                }else{
                    return 'black';
                }
            }
        };
    }
}
ht.Default.setShape3dModel('meter', array);//注册3D模型，请参考modeling建模手册 第一参数为模型名称，第二参数为 JSON 类型对象
```
之后用户可以在需要用到的地方直接设置属性 shape3d 为这边注册过的 3D 模型名称，我们下面就创建 3 个节点，并将节点设置为此 3D 模型：
```
for(var i=0; i<3; i++){//创建3个节点 meter
    var node = new ht.Node();
    node.setTag(i);//设置 tag 标签
    node.setName('Meter - 00' + (i+1));//设置图元名称一般显示在图元的下方
    node.s({
        'label.color': 'white',
        'label.background': '#5271B8',
        'label.face': 'center',
        'label.position': 23,
        'label.scale': 2,
        'label.reverse.flip': true, 
                        
        'note.scale': 1.5,//设置字体大小，这种方式不会碰到浏览器最小字体的问题
        'note.t3': [-30, -5, -90], 
                        
        'note2.scale': 1.2,
        'note2.position': 17,
        'note2.t3': [0, -20, -30],
        'note2.color': 'black',
        'note2.background': 'yellow', 
                        
        'shape3d': 'meter',//设置为前面注册的 meter 3D 模型
        'shape3d.scaleable': false,
        'wf.visible': 'selected',//选中图元时显示线框
        'select.brightness': 1
    });
    node.a({//自定义属性 下面会利用这些自定义属性进行数据绑定
        'meter.value': i+1,
        'meter.angle': i * Math.PI / 3
    });
    node.p3(i*200-200, params.rawS3[1]/2, i===1?100:-100);                    
    node.r3(0, -Math.PI/6*(i-1), 0);
    node.s3(params.rawS3);//设置图元的大小为 rawS3 模型的原始尺寸
    dataModel.add(node); //向数据模型中添加节点           
}
dataModel.sm().ss(dataModel.getDataByTag(1));//设置默认选中 tag 标签为1的图元
```
我们在这边为节点添加两个标注，作为文字提示，可以通过重载 getNote/getNote2（HT 中一个节点支持双标注，所以提供了 note2 第二个标注） 函数重载 note 的命名方法，当然 HT 中其他类似的文字提示也可以通过这种途径来改变文字的显示信息，这里我们通过数据绑定获取 meter.value 以及 meter.angle 两个属性的动态数据：
```
g3d.getNote = function(data){//重载 getNote 方法
    return 'Value:' + data.a('meter.value').toFixed(2);
};
g3d.getNote2 = function(data){
    var value = Math.round(data.a('meter.angle') / Math.PI * 180);//获取了 meter.angle 属性，数据实时变化                 
    return value ? 'Angle:' + value : 'Switch is off';
};
```
我们还在场景的显示部分使了一点小心机~通过改变实现 eye 和 center 的值来实现视线由远及近的效果：
```
var oldEye = g3d.getEye().slice(0),
oldCenter = g3d.getCenter().slice(0),
newEye = [200, 300, 650],
newCenter = [0, params.rawS3[1]/2, 0];
ht.Default.startAnim({//动画              
    duration: 1000,//持续时间
    easing: function(t){ //动画缓动函数，默认采用 ht.Default.animEasing
        return (t *= 2) < 1 ? 0.5 * t * t : 0.5 * (1 - (--t) * (t - 2));                      
    },
    action: function(k){//action 函数必须提供，实现动画过程中的属性变化 参数 k 代表通过 `easing(t) `函数运算后的值        g3d.setEye(
            oldEye[0] + (newEye[0] - oldEye[0]) * k,
            oldEye[1] + (newEye[1] - oldEye[1]) * k,
            oldEye[2] + (newEye[2] - oldEye[2]) * k
        );
        g3d.setCenter(
            oldCenter[0] + (newCenter[0] - oldCenter[0]) * k,
            oldCenter[1] + (newCenter[1] - oldCenter[1]) * k,
            oldCenter[2] + (newCenter[2] - oldCenter[2]) * k
        );    
    }                  
});
```
整个左边实现完成~接着该实现右上部分，属性值的显示以及控制，我们总共添加了四个属性：名称、meter.value、meter.angle以及旋转 rotation，通过数据绑定操作属性栏中的值来改变 3D 模型中的显示状态，数据绑定我们通过获取 accessType 以及 name 中的值来配合调用到这个属性：
```
propertyView.addProperties([//用 json 的数组参数方式批量添加属性信息
    {
        name: 'name',//属性名 这里不用设置 accessType，因为 accessType 默认的值为 setName/getName 这种格式 
        editable: true//设置为可编辑状态
    },
    {
        name: 'meter.value',//用于存取name属性，该属性结合accessType属性最终实现对Data属性的存取
        accessType: 'attr',//通过 getAttr/setAttr 获取或设置属性值
        editable: true,
        slider: {
            min: 0,
            max: 4
        }
    },
    {
        name: 'meter.angle',
        accessType: 'attr',
        editable: true,
        formatValue: function(value){//一般用于将数字转换更易读的文本格式
            return Math.round(value / Math.PI * 180);
        },
        slider: {
            min: 0,
            max: Math.PI,
            step: Math.PI/180*5,//每移动一下滑动的步进
            getToolTip: function(){//设置鼠标放在图元上的文字提示
                return Math.round(this.getValue() / Math.PI * 180);
            }
        }
    },
    {
        name: 'rotation',
        editable: true,
        formatValue: function(value){
            return Math.round(value / Math.PI * 180);
        },
        slider: {
            min: -Math.PI,
            max: Math.PI,
            step: Math.PI/180*5,
            getToolTip: function(){
                return Math.round(this.getValue() / Math.PI * 180);
            }
        }
    }                    
]);
```
最后进行右下部分 formPane 表单面板的解析，formPane 通过 addRow 函数向表单中添加行，这个表单中总共两行，其中第一行有两个部分：
```
formPane.addRow([//向表单组件中添加行
    {
        id: 'export',
        button: {//按钮                              
            label: 'Export JSON',
            onClicked: function(){//点击时触发的函数
                var json = dataModel.serialize();
                formPane.v('textArea', json);
            }
        }
    },
    {
        button: {                            
            label: 'Import JSON',
            onClicked: function(){
                dataModel.clear();//清空数据模型
                dataModel.deserialize(formPane.v('textArea'));//将获取到的 textArea 中的数据反序列化，是下面一行的 id 值
            }
        }
    }
],
[0.1, 0.1]);  //最后的参数是这行的宽度分配比例 小于1的值为比例，大于1为实际值                             
formPane.addRow([
    {
        id: 'textArea',
        textArea: {
        }
    }
],
[0.1], 0.1);
```
这样，我们就可以根据修改属性栏中或者 JSON 文件，直接看到 3D 中我们修改的效果啦~怎么样？是不是很酷很快？
