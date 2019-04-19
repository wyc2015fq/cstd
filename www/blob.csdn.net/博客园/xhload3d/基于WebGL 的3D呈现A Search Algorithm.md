# 基于WebGL 的3D呈现A* Search Algorithm - xhload3d - 博客园
![IMG_1486](http://www.hightopo.com/blog/wp-content/uploads/2014/11/IMG_1486.png)
[http://www.hightopo.com/demo/astar/astar.html](http://www.hightopo.com/demo/astar/astar.html)
最近搞个游戏遇到最短路径的常规游戏问题，一时起兴基于[HT for Web](http://hightopo.com/)写了个A*算法的WebGL 3D呈现，算法基于开源 [https://github.com/bgrins/javascript-astar](https://github.com/bgrins/javascript-astar) 的javascript实现，其实作者也有个不错的2D例子实现 [http://www.briangrinstead.com/files/astar/](http://www.briangrinstead.com/files/astar/) ，只不过觉得所有A*算法的可视化实现都是平面的不够酷，另外还有不少参数需要调节控制，还是值得好好搞个全面的Demo，先上张2D和3D例子的对照图。
![Screen Shot 2014-11-24 at 5.36.33 PM](http://www.hightopo.com/blog/wp-content/uploads/2014/11/Screen-Shot-2014-11-24-at-5.36.33-PM.png)
实现代码比较容易一百多行，不过算法核心在astar.js了，界面核心在[ht.js](http://hightopo.com/)里面了，我只需要构建网格信息，只需监听用户点击，然后调用astar.js进行最短路径计算，将结果通过动画的方式呈现出走动的过程，所有代码如下：
```
function init() {                
    w = 40; m = 20; d = w * m / 2;            
    gridRows = [];                        
    dm = new ht.DataModel();             
    g3d = new ht.graph3d.Graph3dView(dm);                
    g3d.setGridVisible(true);
    g3d.setGridColor('#BBBBBB');
    g3d.setGridSize(m);
    g3d.setGridGap(w);            
    g3d.addToDOM();                                                                                                        
    g3d.sm().setSelectionMode('none');            
    anim = startBall = endBall = null;                        
    g3d.getView().addEventListener(ht.Default.isTouchable ? 'touchstart' : 'mousedown', function(e){                
        if(!anim){
            var p = g3d.getHitPosition(e);
            var x = Math.floor((p[0] + d)/ w);
            var y = Math.floor((p[2] + d)/ w);
            var endBall = dm.getDataByTag("cell_" + x + "_" + y);
            if(endBall && endBall.s('batch') !== 'wall'){                      
                if(startBall.a('x') === x && startBall.a('y') === y){
                    return;
                }                        
                var g = new Graph(gridRows, { 
                    diagonal: formPane.v('diagonal') 
                });
                var start = g.grid[startBall.a('x')][startBall.a('y')];
                var end = g.grid[x][y];
                var result = astar.search(g, start, end, {
                    closest: formPane.v('closest')                            
                });  
                if(!result.length){
                    return;
                }
                x = result[result.length-1].x;
                y = result[result.length-1].y;
                endBall = dm.getDataByTag("cell_" + x + "_" + y);
                endBall.s('3d.visible', true);
                startBall.s('3d.visible', false);
                formPane.setDisabled(true);
                anim = ht.Default.startAnim({
                    duration: 700,
                    finishFunc: function(){  
                        for(var i=0; i<result.length; i++){
                            var ball = dm.getDataByTag("cell_" + result[i].x + "_" + result[i].y);
                            ball.s({
                                '3d.visible': false,
                                'shape3d.opacity': 1,
                                'shape3d.transparent': false
                            }); 
                            startBall.p3(-d+w*x+w/2, w/2, -d+w*y+w/2);
                            startBall.a({x: x, y: y});
                            startBall.s('3d.visible', true);
                        }
                        anim = null;
                        formPane.setDisabled(false);
                    },
                    action: function(v){
                        var index = Math.round(v*result.length);
                        for(var i=0; i<index; i++){
                            var ball = dm.getDataByTag("cell_" + result[i].x + "_" + result[i].y);
                            ball.s({
                                '3d.visible': true,
                                'shape3d.opacity': i/index*0.3 + 0.7,
                                'shape3d.transparent': true
                            });                                    
                        }
                    }
                });                                                
            }
        }               
    }, false);                                    
    createFormPane();
    createGrid();                                
}                
function createGrid(){
    dm.clear();            
    var ball;
    gridRows.length = 0;
    for(var x = 0; x < m; x++) {
        var nodeRow = [];
        gridRows.push(nodeRow);
        for(var y = 0; y < m; y++) {                                
            var isWall = Math.floor(Math.random()*(1/formPane.v('frequency')));
            if(isWall === 0){
                nodeRow.push(0);
                createNode(x, y).s({
                    'batch': 'wall',
                    'all.color': '#9CA69D'
                });
            }else{
                nodeRow.push(1);
                ball = createNode(x, y).s({
                    'shape3d': 'sphere',  
                    'shape3d.color': '#FF703F',
                    '3d.visible': false
                });
            }            
        }       
    }
    if(!ball){
        createGrid();
        return;
    }            
    startBall = createNode(ball.a('x'), ball.a('y'), 'start').s({
        'shape3d': 'sphere',  
        'shape3d.color': '#FF703F'                    
    });  
    shape = new ht.Shape();
    shape.setPoints(new ht.List([
        {x: -d, y: d},
        {x: d, y: d},
        {x: d, y: -d},
        {x: -d, y: -d},
        {x: -d, y: d}
    ]));
    shape.setThickness(4);
    shape.setTall(w);
    shape.setElevation(w/2);
    shape.setClosePath(true);
    shape.s({
        'all.color': 'rgba(187, 187, 187, 0.8)', 
        'all.transparent': true, 
        'all.reverse.cull': true
    });
    dm.add(shape);                            
}
function createNode(x, y, tag){
    var node = new ht.Node();
    tag = tag || "cell_" + x + "_" + y;               
    node.setTag(tag);            
    node.a({ x: x,  y: y });
    node.s3(w*0.9, w*0.9, w*0.9);
    node.p3(-d+w*x+w/2, w/2, -d+w*y+w/2);
    node.s({
        'all.reverse.cull': true,
        'shape3d.reverse.cull': true
    });
    dm.add(node);
    return node;
}                       
function createFormPane() {           
    formPane = new ht.widget.FormPane();
    formPane.setWidth(230);
    formPane.setHeight(70);
    formPane.getView().className = 'formpane';
    document.body.appendChild(formPane.getView());            
    formPane.addRow(['Wall Frequency', {
        id: 'frequency',
        slider: {
            min: 0,
            max: 0.8,
            value: 0.1,                            
            onValueChanged: function(){
                createGrid();
            }
        }
    }], [100, 0.1]);                               
    formPane.addRow([
        {
            id: 'closest',
            checkBox: {
                label: 'Try Closest'
            }
        },
        {
            id: 'diagonal',
            checkBox: {
                label: 'Allow Diagonal'
            }        
        }
    ], [0.1, 0.1]);
}
```
自从iOS8支持WebGL后在移动终端上测试3D应用比当前的大部分Android平板舒服多了，以上的例子在iOS系统下呈现和算法都挺流畅，[http://v.youku.com/v_show/id_XODMzOTU1Njcy.html](http://v.youku.com/v_show/id_XODMzOTU1Njcy.html)，当然这个小例子数据量也不大，本质其实还是2D的最短路径算法，并非真正意义的3D空间最短路径，但还是足够解决很多实际应用问题了。[http://www.hightopo.com/demo/astar/astar.html](http://www.hightopo.com/demo/astar/astar.html)
http://player.youku.com/player.php/sid/XODMzOTU1Njcy/v.swf
![Screen Shot 2014-11-24 at 5.09.13 PM](http://www.hightopo.com/blog/wp-content/uploads/2014/11/Screen-Shot-2014-11-24-at-5.09.13-PM.png)
