# 通过HTML5的Drag and Drop生成拓扑图片Base64信息 - xhload3d - 博客园
HTML5 原生的 Drag and Drop是很不错的功能，网上使用例子较多如 [http://html5demos.com/drag](http://html5demos.com/drag) ，但这些例子大部分没实际用途，本文将搞个有点使用价值的例子，通过Drag and Drop生成图片的[Base64](http://en.wikipedia.org/wiki/Base64)的字符串信息。
使用[Base64](http://en.wikipedia.org/wiki/Base64)方式的图片有诸多好处，可将多个图片信息整合到单个js文件避免多次http请求，可以避免WebGL例子跨域访问的安全限制无法本地文件运行等好处，当然弊端也不少例如不能有效利用浏览器图片缓存机制等。使[用HT for Web](http://www.hightopo.com/)的朋友会发现[HT](http://www.hightopo.com/)的例子很多注册图片都采用Base64的方式，这主要是为了方便用户直接本地文件打开[HT](http://www.hightopo.com/)的手册即可操作浏览，无需构建web服务器发布进行访问，用户常问然后将图片转出Base64信息，我们使用的就是本文介绍的小工具。
![Screen Shot 2014-12-18 at 11.49.18 PM](http://www.hightopo.com/blog/wp-content/uploads/2014/12/Screen-Shot-2014-12-18-at-11.49.18-PM.png)
该工具由一个列表、一个拓扑图和一个文本框三部分组成，用户任意拖拽本地多图片文件到任意页面部分，HT自动将图片信息生成对应的DataModel数据模型，列表显示图片效果、名称和宽高信息，拓扑显示图片、修改时间和文件大小等信息，文本框生成对应注册到htDefault.setImage函数的代码片段，用户直接可以将文本框内的代码拷贝到自己的工程的js文件进行使用。
```
function init(){                                                                 
    dataModel = new ht.DataModel();                                                                             
    listView = new ht.widget.ListView(dataModel); 
    graphView = new ht.graph.GraphView(dataModel);
    splitView = new ht.widget.SplitView(listView, graphView);
    textArea = new ht.widget.TextArea(); 
    textArea.getElement().style.wordWrap = 'normal';
    textArea.getElement().style.color = '#777';
    textArea.setEditable(false);
    new ht.widget.SplitView(splitView, textArea, 'v').addToDOM();    
    new ht.layout.ForceLayout(graphView).start();                            
    listView.setRowHeight(50);   
    listView.drawRowBackground = function(g, data, selected, x, y, width, height){
        if(this.isSelected(data)){
            g.fillStyle = '#87A6CB';
        }
        else if(this.getRowIndex(data) % 2 === 0){
            g.fillStyle = '#F1F4F7';
        }
        else{
            g.fillStyle = '#FAFAFA';
        }
        g.beginPath();
        g.rect(x, y, width, height);
        g.fill();
    };
    ht.Default.setImage('icon', {
        width: 50,
        height: 50,
        clip: function(g, width, height) {   
            g.beginPath();
            g.arc(width/2, height/2, Math.min(width, height)/2-3, 0, Math.PI * 2, true);
            g.clip();                        
        },
        comps: [
            {
                type: 'image',
                stretch: 'uniform',
                rect: [0, 0, 50, 50],
                name: {func: 'getImage'}
            }
        ]
    });
    listView.setIndent(60);  
    listView.setVisibleFunc(function(data){
        return data instanceof ht.Node;
    });
    listView.getIcon = function(data){
        return 'icon';
    };                                  
    listView.getLabel = function(data){
        var name = data.getName(name);
        var image = ht.Default.getImage(name);
        if(image){
            name += ' ( ' + image.width + ' X ' + image.height + ' )';
        }
        return name;
    };
    window.addEventListener("dragenter", dragEnter, false);
    window.addEventListener("dragexit", dragExit, false);
    window.addEventListener("dragover", dragOver, false);
    window.addEventListener("drop", drop, false);                                             
}
function dragEnter(evt) {
    evt.stopPropagation();
    evt.preventDefault();
}
function dragExit(evt) {
    evt.stopPropagation();
    evt.preventDefault();
}
function dragOver(evt) {
    evt.stopPropagation();
    evt.preventDefault();
}
function drop(evt) {
    evt.stopPropagation();
    evt.preventDefault();
    dataModel.clear();
    textArea.setText("");
    lastNode = null;                
    var files = evt.dataTransfer.files;
    var count = files.length;                
    for (var i = 0; i < count; i++) {
        var file = files[i];
        var reader = new FileReader();
        reader.onloadend = handleReaderLoadEnd;
        reader.file = file;
        reader.readAsDataURL(file);
    }
}
function handleReaderLoadEnd(evt) {
    var reader = evt.target;  
    var file = reader.file;
    var name = file.name;
    name = name.substr(0, name.length - 4);
    var text = "ht.Default.setImage('" + name + "', '" + reader.result + "');\n";
    textArea.setText(textArea.getText() + text);                 
    ht.Default.setImage(name, reader.result); 
    var note = 'Date: ' + file.lastModifiedDate.toLocaleString() + '\n'
            + 'Name: ' + file.name + '\n'
            + 'Size: ' + file.size + '\n'
            + 'Type: ' + file.type;
    var node = new ht.Node();
    node.setName(name);
    node.setImage(name);
    node.s({
        'note': note,
        'note.position': 3
    });
    dataModel.add(node);
    if(lastNode){
        var edge = new ht.Edge(lastNode, node);
        dataModel.add(edge);                    
    }
    lastNode = node;                
}
```
该代码主要对window添加了dragenter、dragexit、dragover和drop的拖拽处理，大部分都是通过e.stopPropagation();和evt.preventDefault();阻止默认行为，我们仅需在最后的drop事件中通过e.dataTransfer.files得到所有当前拖拽文件信息，构建FileReader进行加载，然后对加载的信息构建对应DataModel中的ht.Node对象和属性就完事了。
最后代码中还有几处使[用HT for Web](http://www.hightopo.com/)的技术细节值得提到，左侧list列表通过自定义矢量图标，并且在定义矢量时采用了clip的功能，这样列表的图标就会显示成clip裁剪后的圆形效果。重载了listView.drawRowBackground函数，实现隔行变色的列表效果。重载了listView.getLabel显示了更多的动态文本信息。通过listView.setVisibleFunc过滤不显示连线信息在列表中。
以下为该Base64转换工具的操作效果视频和抓图供参考：[http://v.youku.com/v_show/id_XODUxNzY3OTA4.html](http://v.youku.com/v_show/id_XODUxNzY3OTA4.html)
http://player.youku.com/player.php/sid/XODUxNzY3OTA4/v.swf
![Screen Shot 2014-12-18 at 11.43.41 PM](http://www.hightopo.com/blog/wp-content/uploads/2014/12/Screen-Shot-2014-12-18-at-11.43.41-PM.png)
