
//节点和连接数据
int layout_force(ctrl_t* c)	{
  SYSIO;
  //节点
  const char* nodes[] = {
    "A",    "B",    "C",    "D",
      "E",    "F",    "G",    "H",
      "I",    "G",    "K",    "L",
      "M",    "N",    "O",    "P",
      "Q",    "R",    "S",    "T",
      "U",    "V",    "W",    "X",
      "Y",    "Z"
  };
  links_t links[] = {	//连接
    {1,0},    {2,0},
    {3,0},    {3,2},
    {4,0},    {5,0},
    {6,0},    {7,0},
    {8,0},    {9,0},
    {11,10},    {11,3},
    {11,2},    {11,0},
    {12,11},    {13,11},
    {14,11},    {15,11},
    {17,16},    {18,16},
    {18,17},    {19,16},
    {19,17},    {19,18},
    {20,16},    {20,17},
    {20,18},    {20,19},
    {21,16},    {21,17},
    {21,18},    {21,19},
    {21,20},    {22,16},
    {22,17},    {22,18},
    {22,19},    {22,20},
    {22,21},    {23,16},
    {23,17},    {23,18},
    {23,19},    {23,20},
    {23,21},    {23,22},
    {23,12},    {23,11},
    {24,23},    {24,11},
    {25,24},    {25,23},
    {25,11}
  };
  
  //定义颜色比例尺
  const COLOR* color = d3_category20c;
  static force_t force[1] = {0};
  static int inited = 0;
  float linkDistance = 100;
  int i;
  if (!inited) {
    inited = 1;
    force_init(force);
    //加载数据，启动力布局
    force_start(force, 500, 500, countof(links), links, &linkDistance, NULL);
  }
  
  if (io->tick) {
    force_tick(force);
  }
  gcSolidPen(g, 0xffcccccc);
  gcSolidBrush(g, ColorRed);
  //绘制连接线
  for (i=0; i<countof(links); ++i) {
    forcenode_t* s = force->nodes + force->links[i].source;
    forcenode_t* t = force->nodes + force->links[i].target;
    gcDrawLine(g, s->x, s->y, t->x, t->y);
  }
  //绘制节点
  for (i=0; i<force->nodes_length; ++i) {
    forcenode_t* s = force->nodes + i;
    gcCircle(g, GcOptFill, s->x, s->y, 5);
  }
#if 0
  var node = svg.selectAll(".node")
    .data(data.nodes)
    .enter().append("circle")
    .attr("fill", function(d,i){ return color(i);})
    .attr("r", 5)
    .attr("stroke","black")
    .attr("stroke-width",1);
  
  //按照力布局的节拍移动线和点的位置，直到收敛
  force.on("tick", function() {
    link.attr("x1", function(d) { return d.source.x; })
      .attr("y1", function(d) { return d.source.y; })
      .attr("x2", function(d) { return d.target.x; })
      .attr("y2", function(d) { return d.target.y; });
    
    node.attr("cx", function(d) { return d.x; })
      .attr("cy", function(d) { return d.y; });
  });
  
  
#endif
  //force_free(force);
  return 0;
}