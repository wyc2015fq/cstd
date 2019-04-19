# Zip 压缩和解压技术在 HTML5 中的应用 - xhload3d - 博客园
[JSZip](https://stuk.github.io/jszip/) 是一款可以创建、读取、修改 .zip 文件的 javaScript 工具。在 web 应用中，免不了需要从 web 服务器中获取资源，如果可以将所有的资源都合并到一个 .zip 文件中，这时候只需要做一次请求，这样既减少了服务器的压力，同时也可以加快 web 应用的呈现速度。
今天就来探讨下 JSZip 如何与 [HT](http://www.hightopo.com/)应用结合。先来看看这期 Demo 的效果图：
![](https://images0.cnblogs.com/blog2015/591709/201504/122354555409811.png)
第一步、需要将应用对相关资源打包成 .zip 文件，
![](https://images0.cnblogs.com/blog2015/591709/201504/122355094934416.png)
这是我要压缩的文件列表，把响应的资源文件存放到对应的文件夹下，然后在 loadorder 文件中标明资源加载的顺序，loadorder 文件内容如下：
```javascript;gutter
'js/ht.js', 
'js/ht-obj.js',
 'js/ht-modeling.js',
 'obj/equipment.mtl',
 'obj/equipment.obj', 
'image/equipment.jpg'
```
在资源加载顺序中，要标明响应资源的相对于 .zip 文件的路径，这样方便在读取 .zip 文件时快速找到相应的资源文件。
第二步、在 html 文件中引入 [JSZip](https://stuk.github.io/jszip/) 和 JSZipUtils 库，接下来就是请求 .zip 文件，并对 .zip 文件做解析处理。
```javascript;gutter
JSZipUtils.getBinaryContent('res/ImportObj.zip', function(err, data) {
    if(err) {
        throw err; // or handle err
    }
    var zip = new JSZip(data);
    var loadorderStr = zip.file('loadorder').asText(),
            order;
    eval('order = [' + loadorderStr + ']');
    var len = order.length,
            image = {},
            mtlStr = '',
            objStr = '';
    for(var i = 0; i < len; i++) {
        var fileName = order[i];
        if(fileName.indexOf('js/') >= 0) {
            var js = document.createElement('script');
            js.innerHTML = zip.file(fileName).asText();
            document.getElementsByTagName('head')[0].appendChild(js);
        } else if(fileName.indexOf('image/') >= 0) {
            var buffer = zip.file(fileName).asArrayBuffer(),
                    str = _arrayBufferToBase64(buffer),
                    pIndex = fileName.indexOf('.'),
                    type = fileName.substr(pIndex + 1),
                    re = 'data:image/' + type + ';base64,';
            image[fileName] = re + str;
        } else if(fileName.indexOf('obj/') >= 0) {
            var str = zip.file(fileName).asText();
            if(fileName.indexOf('.mtl') > 0) {
                mtlStr = str;
            } else if(fileName.indexOf('.obj') > 0) {
                objStr = str;
            }
        }
    }
    init(objStr, mtlStr, image);
});
```
首先通过 [JSZipUtils](https://stuk.github.io/jszip/) 获取 .zip 文件，将获取到的文件内容通过 new JSZip(data) 方法加载到 zip 变量中，通过 zip.file(fileName) 读取 loadorder 文件内容，试用 eval 命令动态执行脚本，将文本内容转换为 js 变量 order，最后通过遍历 order 变量，将 js 资源动态引入到页面中。
在 .zip 文件中有包含图片文件，JSZip 只能获取到图片文件的 ArrayBuffer 数据，这时需要将 ArrayBuffer 转换为 Base64 才能够为浏览器所识别，所以这里定义了一个转换函数：_arrayBufferToBase64 
```javascript;gutter
function _arrayBufferToBase64( buffer ) {
    var binary = '';
    var bytes = new Uint8Array( buffer );
    var len = bytes.byteLength;
    for (var i = 0; i < len; i++) {
        binary += String.fromCharCode( bytes[ i ] );
    }
    return window.btoa( binary );
}
```
这次的案例中，有涉及到 3D 模型数据与 [HT 3D 拓扑](http://www.hightopo.com/)应用的结合，在 .zip 文件中的 obj 目录就是存放 3D 模型数据，在文件读取中，将 3D 模型数据以文本对形势读取出来存放到变量中，再将数据传递到 init 函数中，通过 ht.Default.parseObj() 方法将 3D 模型数据加载到 [HT](http://www.hightopo.com/) 中。
```javascript;gutter
function init(objStr, mtlStr, image) {
    dataModel = new ht.DataModel();
    g3d = new ht.graph3d.Graph3dView(dataModel);
    view = g3d.getView();
    view.className = 'main';
    document.body.appendChild(view);
    window.addEventListener('resize', function (e) {
        g3d.invalidate();
    }, false);
    g3d.setEye([0, 500, 1000]);
    g3d.setCenter([0, 200, 0]);
    g3d.setGridVisible(true);
    g3d.setGridColor('#74AADA');
    var param = {
        shape3d: 'E1',
        center: true,
        cube: true
    };
    var modelMap = ht.Default.parseObj(objStr, mtlStr, param);
    for(var model in modelMap) {
        var map = modelMap[model],
                i = map.image,
                index = i.lastIndexOf('/'),
                fileName = i.substr(index + 1),
                rawS3 = map.rawS3;
        for(var imgName in image) {
            if(imgName.indexOf(fileName) >= 0) {
                ht.Default.setImage(i, 256, 256, image[imgName]);
            }
        }
    }
    var node = new ht.Node();
    node.s({
        'shape3d': 'E1',
        'wf.visible': 'selected',
        'wf.width': 3,
        'wf.color': '#F7F691'
    });
    node.s3(param.rawS3);
    node.p3(0, param.rawS3[1]/2, 0);
    dataModel.add(node);
}
```
上述是生成 [3D 拓扑](http://www.hightopo.com/)、3D 模型引入和引用 3D 模型创建拓扑节点的代码。其中那段 setImage 的代码需要特别注意，为什么我要大费周张的去判断 image 的文件名呢，那是因为在 mtl 3D 模型描述文件中有一个设置贴图的属性，该属性可以指定文件的绝对路径，也可以指定文件的相对路径，因为采用 JSZip 无法将 .zip 中的文件内容写回到本地目录中，所以只能将贴图属性对应的属性名称作为 [HT](http://www.hightopo.com/) 中的 image 名称设置到 HT 中，以便 HT 模型加载的时候能够获取得到模型所需要的图片资源。HT 3D 拓扑的应用可以参考《[3D拓扑自动布局之Node.js篇](http://www.hightopo.com/blog/84.html)》。
[JSZip](https://stuk.github.io/jszip/) 在压缩或解压数据的时候，如果出现速度较慢的情况，可以考虑使用 Web Worker，Web Worker的具体应用可以参考《[3D拓扑自动布局之Web Workers篇](http://www.hightopo.com/blog/70.html)》。
