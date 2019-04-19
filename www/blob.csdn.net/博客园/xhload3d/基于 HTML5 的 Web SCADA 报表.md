# 基于 HTML5 的 Web SCADA 报表 - xhload3d - 博客园
### 背景
最近在一个 SCADA 项目中遇到了在 Web 页面中展示设备报表的需求。一个完整的报表，一般包含了筛选操作区、表格、Chart、展板等多种元素，而其中的数据表格是最常用的控件。在以往的工业项目中，所有的表格看起来千篇一律，就是通过数字和简单的背景颜色变化来展示相关信息。但是现在通过各种移动 App 和 Web 应用的熏陶，人们的审美和要求都在不断提高，尤其是在 Web 项目中，还采用老式的数字表格确实也有点落伍了。 
如何选择一个合适的 HTML 前端表格控件？此处可以省略一万字。哈哈。jQuery、Angular、React 等阵营中的控件库中都有不少成熟案例，但是这些基于 DOM 的控件也有不足，一个是效率问题：如果在数据量很大表格的中采用自定义的单元格控件，对浏览器的负担实在太重，尤其是移动端。另一个问题是开发效率，上述的控件库中各自的封装程度、接口形式都有所不同，但整体上还是要求开发者对 CSS、JS 都有较深的了解。还有控件的复用、嵌入、发布、移植，也都是问题。 
基于上面的考虑，最后采用了基于 Canvas 的 [HT](http://www.hightopo.com)。通过 HT 表格控件的自定义渲染接口，以及 Web Worker 的多线程数据模拟，实现的表格控件效果如下： 
[http://www.hightopo.com/demo/pagetable/index.html](http://www.hightopo.com/demo/pagetable/index.html)
![](http://img.blog.csdn.net/20161215220232979?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2N0YmVhcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 开始
首先我们要做的就是结合业务逻辑，对表格中不同列的数据，进行不同的渲染。例如设备历史信息中的运行时间、停机时间等，比较适合用饼图来汇总展示，用户就可以很直观的从列表上对比出设备的历史状况。 我们来看看这一步是怎样实现的。 
[HT](http://www.hightopo.com) 有自己的 DataModel 数据模型，省略了我们对数据状态管理、时间派发、ui刷新的开发工作。DataModel 容器中的子元素 Data，即是 HT 中最基础的数据结构，可以映射到不同的ui控件上。在画布上，Data 可以展示成矢量、图片或者文字等，在树形控件上，Data 展示为树的一个节点。在表格当中每个 Data 对应着表格中的一行 Row。 也就是表格控件自身包含一个 DataModel，在绘制时，将这个 Model 中的每个 Data 都绘制成一行。 不同的列，展示的是该 Data 中的不同属性。例如我们可以把设备的停机时间，保存到 Data 的 stopping 属性。 在配置表格的列 Column 信息时，我们可以指定该列的表头描述“停机时间”，其数据单元格对应 Data 的 Stopping 属性，以及自定义绘制格式：
```
{
    name: 'stopping',       //对应的data属性
    accessType: 'attr',
    align: 'center',        
    color: '#E2E2E2',       //文字颜色
    displayName: '停机',    //表头描述
    drawCell: pageTable.getDrawLegend('stopping','#E2E2E2')
},
```
### 自定义渲染
在单元格的基本显示格式中，已经默认提供了文本、数组、颜色等类型，可以自动的对数据格式化，并展示为文字或背景颜色等，但是还未满足我们的个性需求，因此就要将 Column 中的 drawCell 重载为自定义的渲染函数。 drawCell 的参数：function (g, data, selected, column, x, y, w, h, view)，其中 g 是 Canvas 的环境信息，data 是该行的数据体，我们根据这些信息，再利用 HTML5 原生的 Canvas API 就可以画出想要的效果。 
懒得亲自直接用 HTML5 的原生接口？ [HT](http://www.hightopo.com) 提供了对 Canvas API 的封装接口，包括各种矢量类型以及一些简单的 Chart。利用该功能，可以轻松组合出复杂的效果，具体介绍可以参考我们的矢量手册（[http://www.hightopo.com/guide/guide/core/vector/ht-vector-guide.html](http://www.hightopo.com/guide/guide/core/vector/ht-vector-guide.html)）。
先创建一个对象，该 image 矢量对象负责包含对组合矢量的描述信息，然后将该 image 对象以及 drawCell 的上下文信息，作为参数传入 ht.Default.drawStretchImage 函数，即可实现自定义绘制。
```
//drawCell
function (g, data, selected, column, x, y, w, h, tableView) {
    var value = data.a(attr);
    var image = {
        width: 60,
        height: 30,
        comps: [
            {
                type: 'rect',
                rect: [11,11,8,8],
                borderWidth: 1,
                borderColor: '#34495E',
                background: legendColor,
                depth: 3
            },
            {
                type: 'text',
                text: value,
                rect:[30, 0, 30, 30],
                align: 'left',
                color: '#eee',
                font: 'bold 12px Arial'
            }
        ]};
    ht.Default.drawStretchImage(g, image, 'centerUniform', x, y, w, h);
}
```
因为有多个 Legend 图例显示的列，所以我们可以简单包装一下，用了一个 getDrawLegend 函数，参数是该列图例的颜色及 Data 属性名称，返回值是 drawCell 函数。
```
getDrawLegend: function(attr,legendColor){return drawCell}
```
至此，我们就完成了启停时间这几列的自定义绘制： 
![](http://img.blog.csdn.net/20161215220544186?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2N0YmVhcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
“统计”列的饼图，实际上更简单。还是利用 [HT](http://www.hightopo.com) 的矢量接口，把上述几项时间数据传入饼图矢量结构即可。
```
var values = [
    data.a('running'), 
    data.a('stopping'), 
    data.a('overhauling')
];
var image = {
    width: 200,
    height: 200,
    comps: [
        {
            type: 'pieChart',
            rect: [20,20, 150, 150],
            hollow: false,
            label: false,
            labelColor: 'white',
            shadow: true,
            shadowColor: 'rgba(0, 0, 0, 0.8)',
            values: values,
            startAngle: Math.PI,
            colors: pieColors
        }
    ]
};
```
其他列的渲染过程大同小异。在“风速”列中，我们可以根据风速大小计算一个颜色透明值，来实现同一色系的映射变换，比原来那种非红即绿的报警表，看起来更舒服一些。在“可用率”列，用 Rect 的不同长度变化，来模拟进度条的效果。在功率曲线中稍微有点不同，因为想实现曲线覆盖区域的颜色渐变，在 HT 的 lineChart 中没有找到相关接口，所以直接采用了 Canvas 绘制。 
![](http://img.blog.csdn.net/20161215220748166?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2N0YmVhcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
为了运行效率考虑，在表格的单元格中绘制 Chart，应该追求简洁大方，一目了然。这几个 Legend 图例小矩形，其实是应该画在表头的。我为了偷懒，就画在了单元格，导致画面显得有点乱。
### Web Worker
众所周知，浏览器的 JS 环境是基于单进程的，在页面元素较多，而且有很大运算需求的情况下，会导致无法兼顾渲染任务和计算任务，造成页面卡顿或失去响应。在这种情况，可以考虑使用 Web Worker 的多线程，来分担一些计算任务。 
Web Worker 是 HTML5 的多线程 API，和我们原来传统概念中的多线程开发有所不同。Web Worker 的线程之间，没有内存共享的概念，所有信息交互都采用 Message 的异步传递。这样多线程之间无法访问对方的上下文，也无法访问对方的成员变量及函数，也不存在互斥锁等概念。在消息中传递的数据，也是通过值传递，而不是地址传递。 
在 Demo 中，我们利用 Web Worker 作为模拟后端，产生虚拟数据。并采用前端分页的方式，从 worker 获取当前页显示条目的相关数据。 在主线程中，创建 Web Worker注册消息监听函数。
```
worker = new Worker("worker.js");    
worker.addEventListener('message', function(e) {  
    //收到worker的消息后，刷新表格
    pageTable.update(e.data);
});
pageTable.request = function(){
    //向worker发送分页数据请求
    worker.postMessage({
        pageIndex: pageTable.getPageIndex(),
        pageRowSize: pageTable.getPageRowSize()        
    });                 
}; 
pageTable.request();
```
本处的new Worker创建，对于主线程来说是异步的，等加载完 worker.js，并完成初始化后，该 worker 才是真正可用状态。我们不需要考虑 worker 的可用状态，可以在创建语句后直接发送消息。在完成初始化之前向其发送的请求，都会自动保存在主线程的临时消息队列中，等 worker 创建完成，这些信息会转移到 worker 的正式消息队列。 
在 worker 中，创造虚拟随机数据，监听主线程消息，并返回其指定的数据。
```
self.addEventListener('message', function(e) {
    var pageInfo = getPageInfo(e.data.pageIndex, e.data.pageRowSize);   
    self.postMessage(pageInfo);
}, false);
```
由于前面提到的无法内存共享，Web Worker 无法操作 Dom，也不适用于与主线程进行大数据量频繁的交互。那么在生产环境中，Web Worker 能发挥什么作用？在我们这种应用场景，Web Worker 适合在后台进行数据清洗，可以对从后端取到的设备历史数据进行插值计算、格式转换等操作，再配合上 [HT](http://www.hightopo.com) 的前端分页，就能实现大量数据的无压力展示。
### 分页
传统上有后端分页和前端分页，我们可以根据实际项目的数据量、网速、数据库等因素综合考虑。 
采用后端分页的话，可以简化前端架构。缺点是换页时会有延迟，用户体验不好。而且在高并发的情况下，频繁的历史数据查询会对后端数据库造成很大压力。 
采用前端分页，需要担心的是数据量。整表的数据量太大，会造成第一次获取时的加载太慢，前端资源占用过多。 
在本项目中，得益于给力的 GOLDEN 实时数据库，我们可以放心的采用前端分页。历史数据插值、统计等操作可以在数据库层完成，传递到前端的是初步精简后的数据。在数千台设备的历史查询中，得到的数据量完全可以一次发送，再由前端分页展示。 
在某些应用场景，我们会在表格中显示一些实时数据，这些数据是必须是动态获取的。类似在 Demo 中的趋势刷新效果，我们可以在创建表格时批量获取所有历史数据，然后再动态向数据库获取当前页所需的实时数据。如果网速实在不理想，也可以先只获取第一页的历史数据，随后在后台线程慢慢接收完整数据。 
这样的架构实现了海量数据的快速加载，换页操作毫无延迟，当前页面元素实时动态刷新的最终效果。 
还有一些传统客户，喜欢在一张完整的大表上进行数据筛选、排序等操作。 
我们可以把 Demo 中的数据总量改成一万条，单页数量也是一万条，进行测试： 
![](http://img.blog.csdn.net/20161215221218600?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2N0YmVhcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
出乎意料的是，HT 面对上万数据量的复杂表格，轻松经受住了考验。页面的滚动、点击等交互毫无影响，动态刷新没有延迟，表格加载、排序等操作时，会有小的卡顿，在可接受的程度之内。当然也跟客户端的机器配置有关。可以想象，几万个 Chart的展示以及动态刷新，对于基于dom的控件几乎是件无法完成的任务。关于 HT 的其他矢量和控件，同样有高性能特性：[http://www.hightopo.com/demo/fan/index.html](http://www.hightopo.com/demo/fan/index.html)
### 后记
如前文所述，我们基于 HT 的表格实现了海量数据的可定制展现，并取得了令人满意的效果。以下是一些还可以改进的地方。 
在 Demo 中，通过对 HT 表格控件的 drawCell 进行重载，实现了自定义渲染，然后把这些 drawCell 放到了 PageTable 的原型函数中，以供 Column 调用。实际上，更好的办法应该是把这些常见的 Chart、图例封装到 Column 的基本类型中，那样在配置表格 Column 列时，可以指定 type 为 pieChart 或 lineChart 即可，不需再自行绘制相关矢量。 
对于这些表格中的 Chart，也可以增加一些交互接口，例如可以增加单元格 Tooltip 的自定义渲染功能，在鼠标停留时浮出一个信息量更大的 Chart，可以对指定设备进行更深入的了解。 
界面美观优化。对 HT 的控件进行颜色定制，可以通过相关接口进行配置：
```
var tableHeader = pageTable.getTablePane().getTableHeader();    
tableHeader.getView().style.backgroundColor = 'rgba(51,51,51,1)';     
tableHeader.setColumnLineColor('#777');
var tableView = pageTable.getTablePane().getTableView();                 
tableView.setSelectBackground('#3D5D73');
tableView.setRowLineColor('#222941');
tableView.setColumnLineVisible(false);                
tableView.setRowHeight(30);
```
今后也可以对[htconfig](http://www.hightopo.com/guide/guide/core/theme/ht-theme-guide.html)进行全局配置，在单独文件中进行样式的整体管理，实现外观样式与功能的分离，有助于工程管理。
