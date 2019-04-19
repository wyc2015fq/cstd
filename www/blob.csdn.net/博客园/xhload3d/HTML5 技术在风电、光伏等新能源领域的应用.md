# HTML5 技术在风电、光伏等新能源领域的应用 - xhload3d - 博客园
随着新一轮工业革命兴起，应对气候变化日益成为全球共识，能源技术正在成为引领能源产业变革、实现创新驱动发展的源动力。从全球到中国，以风能、光伏发电为代表的新能源产业发展迅速，可再生能源发电与现代电网的融合成为了世界能源可持续转型的核心，发电技术继续沿着大规模、高效率和低成本方向持续进步，中国在风能和光伏发电领域的累计装机量和新增装机量已经跃居世界首位。
相比传统能源稳定、可控的生产方式，风能、太阳能本身具有先天的不可预测性，很大程度上要“靠天吃饭”，因此构建一个集物联网、大数据、云计算于一体的新能源设备管理平台，提升设备“能量可利用率”与“时间可利用率”，成为企业自身乃至整个新能源产业带来运营管理方式升级与转型的关键，其中必不可少的就是前端大数据可视化的重要环节。
以下是[致创能源](http://www.fcreate.com.cn/solution-yd-gf/)公司多年前基于 [HT for Web](http://www.hightopo.com/) 技术实现的 Web SCADA 光伏发电站智能管理解决方案系统截图：
![201407021746152A9B7013785E8BC4](http://www.hightopo.com/blog/wp-content/uploads/2017/02/201407021746152A9B7013785E8BC4.jpg)
![20140702174631E62E7016D29D607C](http://www.hightopo.com/blog/wp-content/uploads/2017/02/20140702174631E62E7016D29D607C.jpg)
基于 [HT for Web](http://www.hightopo.com/) 的 HTML5 技术除了在传统电信、电力和工业控制领域的应用外（参见《[基于HT for Web的Web SCADA工控移动应用](http://www.hightopo.com/blog/402.html)》），如今也已广泛应用于风电、光伏等新能源领域，基于 Web 技术的  [HT for Web](http://www.hightopo.com/) 天然就是如今物联网监控的 PaaS 和 SaaS 云平台服务首选方案，这里我们将基于一家国内首批采用微软智能云上 Azure IoT 套件，整合图扑软件  [HT for Web](http://www.hightopo.com/)  前端图形界面组件中间件，成功打造的 Power+ 物联网监控云端大数据平台的一个光伏监控界面为例，为大家介绍这个已陆续应用于全国各地 1,000 余台风力发电机组和 5,000 余台光伏逆变器和汇流箱的系统前端技术。
我们将具体介绍的页面最终呈现效果如下，[HT for Web](http://www.hightopo.com/) 官网[例子中心](http://www.hightopo.com/demos/index.html)的 [http://www.hightopo.com/demo/pv/index.html](http://www.hightopo.com/demo/pv/index.html)例子进行体验。
![Screen Shot 2017-02-10 at 8.23.17 PM](http://www.hightopo.com/blog/wp-content/uploads/2017/02/Screen-Shot-2017-02-10-at-8.23.17-PM.png)
最初得到设计师的设计稿后，很容易看得出整体界面由两部分构成，上头部分的汇总统计信息，以及下半部分的具体汇流箱详细信息展示。上半部分除了显示实时汇总信息外，还具备点击可过滤下部分汇流箱功能，而下半部分由于数据量众多，会有几百甚至上千个汇流箱，因此需要可缩放、平移和鹰眼等操作功能。
有上面的分析很容易得出下半部分肯定由 [HT for Web](http://www.hightopo.com/) 的 [GraphView](www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html) 拓扑图组件来实现合适，并且客户提出每个汇流箱需要能显示出所有重要指标的详细信息，并且显示需求可能存在多变性，因此采用简单的 Node 节点，通过图片、文字、冒泡、告警染色这些基本功能来显示肯定是不够的，这种需求就非常合适采用 [HT for Web](http://www.hightopo.com/) 的矢量 [www.hightopo.com/guide/guide/core/vector/ht-vector-guide.html](www.hightopo.com/guide/guide/core/vector/ht-vector-guide.html)解决方案，《[HT图形组件设计之道](http://www.hightopo.com/blog/189.html)》系列中已详细阐述了基本原理这里就不再展开了。
![Screen Shot 2017-02-10 at 8.51.37 PM](http://www.hightopo.com/blog/wp-content/uploads/2017/02/Screen-Shot-2017-02-10-at-8.51.37-PM.png)
对于汇流箱我们需要设计一个如上图所示的矢量图标，这并不难，采用 HT 矢量编辑器器三下五除二就搞定，然后设置好图形中颜色、文字、大小等图形参数与业务参数的数据绑定，具体数据绑定格式可参见《[HT for Web 数据绑定手册](http://www.hightopo.com/guide/guide/core/databinding/ht-databinding-guide.html)》的介绍的 JSON 格式，然后运行时只需要将后台得到的实时数据设置到图元的相关绑定属性即可。
虽然在《[数百个 HTML5 例子学习 HT 图形组件 – 拓扑图篇](http://www.hightopo.com/blog/461.html)》已经解释了  [HT for Web](http://www.hightopo.com/) 采用的 MVP/MVVM 数据模型设计架构，但还是有不少刚入门 Web 应用开发的同学询问 HT 如何与后台通讯实时采集数据，正好借此例子稍微展开说明下，在  [http://www.hightopo.com/demo/pv/index.html](http://www.hightopo.com/demo/pv/index.html) 例子中，大家会注意到以下 basicData 和 realTimeData 两个数据：
![Screen Shot 2017-02-10 at 5.25.51 PM](http://www.hightopo.com/blog/wp-content/uploads/2017/02/Screen-Shot-2017-02-10-at-5.25.51-PM.png)
这两个数据值在 demo 中写死值的，而实际运行中是通过后台传输数据而来，这些年越来越多基于 HTML5 的实时监控系统都采用了 WebSocket 的方案，已达到获取数据的实时性，本案例的客户也不例外，前面提到了他们采用了微软智能云的 Azure IoT 套件，因此采用基于 .NET  的 [ASP.NET SignalR](https://github.com/SignalR/SignalR) （[https://github.com/SignalR/SignalR](https://github.com/SignalR/SignalR)）自然是他们采用的 WebSocket 框架方案。
```
function createDatas() {
    json = JSON.parse(basicData);
    json.result.forEach(function(data) {
        var node = new ht.Node();
        node.setImage('symbols/enjoy/pv/pv-box.json');
        node.s({
            'select.color': 'white',
            '2d.selectable': false
        });
        node.a({
            deviceName: data.deviceName,
            capacity: data.capacity + 'KW',
            raw_capacity: data.capacity
        });
        node.setTag(data.deviceCode);
        graphView.getDataModel().add(node);
    });                        
}
```
页面打打开是，系统先在 CreateData 函数中根据 basicData 的信息，构建出几百个汇流箱图元，并且通过 node.setImage('symbols/enjoy/pv/pv-box.json'); 将图元设置上我们构建好的汇流箱矢量，然后通过例子中简单扩展的布局算法，将几百个汇流箱设备进行矩阵排版，当然遇到复杂的网络拓扑图你可以采用 HT 的自动布局来排布：[http://www.hightopo.com/guide/guide/plugin/autolayout/ht-autolayout-guide.html](http://www.hightopo.com/guide/guide/plugin/autolayout/ht-autolayout-guide.html)。
这里还需注意的一点我们在构建图元时通过 node.setTag(data.deviceCode) 设置了每个图元的 Tag 标签，这点很重要，这是用来后续可以快速查找到相应图元进行数据更新做准备。
```
function fillDatas() {
    var hlx_state_0 = hlx_state_1 = hlx_state_2 = hlx_state_3 = 0;
    var zc_state_1 = zc_state_2 = zc_state_3 = 0;
    json = JSON.parse(realTimeData);                        
    json.result.wtrtDatas.forEach(function(data) {       
        var comboxRTDto = data.comboxRTDto;                     
        var node = graphView.getDataModel().getDataByTag(comboxRTDto.deviceCode);
        if (node) { 
            var hlxState = comboxRTDto.pvDeviceStCode;                               
            node.a({
                hlxState: hlxStateMap[hlxState],
                discreteRate: comboxRTDto.discreteRate + '%',
                outputPower: comboxRTDto.outputPower + 'KW',
                percentage:  comboxRTDto.outputPower / node.a('raw_capacity')
            });                                                                
        }
    });  
}
```
以上工作完成后界面已经能显示所有光伏汇流箱设备了，但每个设备显示的参数信息都是我们构建矢量图标时设置的初始化，并不是真正的实时运行值，因此我们需要根据后台实时推送过来的数据，对图元进行数据更新，在以上 fillData 函数我们解析了 realTimeData 数据，然后遍历每个汇流箱数据，通过 dataModel.getDataByTag(deviceCode) 找到相应图元，设置上相应的 attr 属性，而这些属性由于在矢量设计时已经绑定好相应的图形参数，因此当设置上所有数据后，拓扑图上相应的每个汇流箱就能自动显示出真正的实时数据了。
这个例子我们只更新了一次实时数据，但正常的系统会通过 AJAX 间几秒轮询，或者采用 WebSocket 由后台在有变化的情况下实时推送到前端，然后多次调用 fillDatas 函数进行数据更新，另外这个界面场景内容是通过第一次的 basicData 查询数据后动态构建，如果你已经有拓扑图序列化的 JSON 数据了，你只需要通过反序列化即可构建拓扑图场景，序列化反序列化可参考：[http://www.hightopo.com/guide/guide/core/serialization/ht-serialization-guide.html](http://www.hightopo.com/guide/guide/core/serialization/ht-serialization-guide.html)
做完以上我们还仅是完成了一半的工作，记得我们还有个上半部分的汇总和过滤面板：
![Screen Shot 2017-02-10 at 10.32.37 PM](http://www.hightopo.com/blog/wp-content/uploads/2017/02/Screen-Shot-2017-02-10-at-10.32.37-PM.png)
刚开始看这个设计稿，很自然想到面板和操作按钮的东西采用  [HT for Web](http://www.hightopo.com/) 的面板组件即可搞定：[http://www.hightopo.com/guide/guide/plugin/form/ht-form-guide.html](http://www.hightopo.com/guide/guide/plugin/form/ht-form-guide.html) ，不过后来想想通用组件的风格比较固定，哪怕定制出效果也很难应对多变的需求，突发奇想其实我们照样可以采用 [HT for Web](http://www.hightopo.com/) 的 GraphView 拓扑图组件来干这事，上面的文字无非就是 ht.Text 图元，进度条也就是 shape 类型为 rect 的 Node 节点，而过滤控制按钮其实也只一堆 Node 图元，只不过我们设置了不同背景色，将 label 标签居中显示，同时将图元选中效果由边框改为 shadow 的阴影选中效果即可大功告成，而且由于是拓扑图的布局，因此不管布局位置或者界面风格的需求变化，用户都可以很容易妥妥拽拽，设置下新风格参数即可搞定多变的业务展示需求
```
function createHeader() {
    header = new ht.graph.GraphView();                   
    ht.Default.xhrLoad('displays/enjoy/pv/pv-header.json', function(json) {
        header.getDataModel().deserialize(json);                    
        header.getDataModel().setBackground(undefined);
        createDatas();
        fillDatas();  
        layoutDatas();                      
    });
    header.setInteractors(null);
    var handleClick = function(e) {
        if (!graphView.getView().contains(e.target)) {
            var data = header.getDataAt(e);
            header.sm().ss(data);                        
        }
    };
    document.body.addEventListener('mousedown', handleClick, false);
    document.body.addEventListener('touchstart', handleClick, false);               
}
```
汇总部分就是有以上 createHeader 函数搞定，注意这里我们通过 ht.Default.xhrLoad('displays/enjoy/pv/pv-header.json 直接加载已经序列化好的拓扑图信息，然后由于该汇总面板唯一需要的交互就是点击选中分类按钮进行过滤，于是我们通过 header.setInteractors(null); 直接关闭了所有  [HT for Web](http://www.hightopo.com/) 的默认交互，然后通过添加 mousedown 和 touchstart 的原生 HTML 监听事件自定义交互逻辑，这里只需要通过 header.getDataAt(event) 传入不管是 touch 还是 mouse 事件，HT 自动回返回当前操作点下的图元，后续过滤已经动画的逻辑比较简单，这里就不展开说明了，有兴趣的可以改造成更带感的过滤动画布局效果，可参考《[透过WebGL 3D看动画Easing函数本质](http://www.hightopo.com/blog/153.html)》一文了解   [HT for Web](http://www.hightopo.com/)  的各种预制的动画功能。
这里我们仅演示了光伏的一个页面效果，风电的风机也可以采用类似的方式呈现，例如 [http://www.hightopo.com/demo/fan/index.html](http://www.hightopo.com/demo/fan/index.html) 这个上万个矢量风机实时转动的 HTML5 性能效果，也可以结合例如百度地图、OpenLayers 或 GoogleMap 等地图方案呈现风机或光伏监控画面：
![Screen Shot 2017-02-10 at 10.54.14 PM](http://www.hightopo.com/blog/wp-content/uploads/2017/02/Screen-Shot-2017-02-10-at-10.54.14-PM.png)![Screen Shot 2017-02-10 at 10.54.48 PM](http://www.hightopo.com/blog/wp-content/uploads/2017/02/Screen-Shot-2017-02-10-at-10.54.48-PM.png)
对于看腻了 2D 矢量风机的，你也可以来个 3D 的风机感受下效果：[http://www.hightopo.com/demo/windTurbines/index.html](http://www.hightopo.com/demo/windTurbines/index.html) ，这个采用  [HT for Web](http://www.hightopo.com/)  实现的 3D 可旋转风机 HTML5 的代码也就寥寥几十行，今天篇幅有限先不展开介绍了，大家就先玩玩 demo 。
![Screen Shot 2017-02-10 at 10.56.57 PM](http://www.hightopo.com/blog/wp-content/uploads/2017/02/Screen-Shot-2017-02-10-at-10.56.57-PM.png)
写到此刻正好是 2017 年元宵，图扑软件团队给祝各位读者拜个晚年好，除了 [HT for Web](http://www.hightopo.com/)  外，我们还开发了开源免费的 HTML5 游戏引擎工具 QICI Engine： [https://github.com/qiciengine/qiciengine](https://github.com/qiciengine/qiciengine) ，对前端图形、游戏技术感兴趣的同学，不烦来认识一下：[http://www.hightopo.com/joinus.html](http://www.hightopo.com/joinus.html) 欢迎手机号加我微信，或给我邮件，非常感谢！
![overview](http://www.hightopo.com/blog/wp-content/uploads/2017/02/overview.png)
