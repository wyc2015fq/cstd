// 力导向算法简单实现
//
// 力导向算法 核心
//
// 1. 随机分布初始节点位置；
// 2. 计算每次迭代局部区域内两两节点间的斥力所产生的单位位移（一般为正值）；
// 3. 计算每次迭代每条边的引力对两端节点所产生的单位位移（一般为负值）；
// 4. 步骤 2、3 中的斥力和引力系数直接影响到最终态的理想效果，它与节点间的距离、节点在系统所在区域的平均单位区域均有关，需要开发人员在实践中不断调整；
// 5. 累加经过步骤 2、3 计算得到的所有节点的单位位移；
// 6. 迭代 n 次，直至达到理想效果。
#include <math.h>
typedef struct {
  void* ptr;
  double x, y;
} gnode_t;
typedef struct {
  int id1;
  int id2;
  void* ptr;
} gedge_t;
int springLayout(gnode_t* nodes, int nodes_size, gedge_t* edges, int edges_size)
{
  //2计算每次迭代局部区域内两两节点间的斥力所产生的单位位移（一般为正值）
  int area = 1024 * 768;
  double k = sqrt(area / (double)nodes_size);
  double diffx, diffy, diff;
  double ejectfactor = 0.6;
  //DPOINT* map = NULL;
  DPOINT map[3] = {0};
  int v, u, e;
  MYREALLOC(map, nodes_size);
  for (v = 0; v < nodes_size; v++) {
    map[v].x = 0.0;
    map[v].y = 0.0;
  }
  for (v = 0; v < nodes_size; v++) {
    for (u = 0; u < nodes_size; u++) {
      if (u != v) {
        diffx = nodes[v].x - nodes[u].x;
        diffy = nodes[v].x - nodes[u].y;
        diff = sqrt(diffx * diffx + diffy * diffy);
        if (diff < 30) {
          ejectfactor = 5;
        }
        map[v].x += (diffx * k * ejectfactor) / (diff * diff);
        map[v].y += (diffy * k * ejectfactor) / (diff * diff);
      }
    }
  }
  //3. 计算每次迭代每条边的引力对两端节点所产生的单位位移（一般为负值）
  double condensefactor = 5 / k;
  for (e = 0; e < edges_size; e++) {
    int eIDS = edges[e].id1;
    int eIDE = edges[e].id2;
    gnode_t* visnodeS = nodes + edges[e].id1;
    gnode_t* visnodeE = nodes + edges[e].id2;
    diffx = visnodeS->x - visnodeE->x;
    diffy = visnodeS->y - visnodeE->y;
    diff = sqrt(diffx * diffx + diffy * diffy);
    ASSERT(eIDS < nodes_size);
    ASSERT(eIDE < nodes_size);
    double dx, dy;
    dx = diffx * condensefactor;
    dy = diffy * condensefactor;
    map[eIDS].x -= dx;
    map[eIDS].y -= dy;
    map[eIDE].x += dx;
    map[eIDE].y += dy;
  }
  //set x,y
  double maxtx = 2, maxty = 2;
  for (v = 0; v < nodes_size; v++) {
    double dx = map[v].x;
    double dy = map[v].y;
    double t = fabs(dx) + fabs(dy);
    if (t > 0) {
      double disppx = BOUND(dx, -maxtx, maxtx);
      double disppy = BOUND(dy, -maxty, maxty);
      nodes[v].x += disppx;
      nodes[v].y += disppy;
    }
  }
  double sumx = 0, sumy = 0;
  for (v = 0; v < nodes_size; v++) {
    sumx += nodes[v].x;
    sumy += nodes[v].y;
  }
  sumx /= nodes_size;
  sumy /= nodes_size;
  for (v = 0; v < nodes_size; v++) {
    nodes[v].x += 300 - sumx;
    nodes[v].y += 300 - sumy;
  }
  //FREE(map);
  return 0;
}
//前端用CanvasXpress，因为它也支持设置 x y坐标方式布局。
// 效果显示（和参考资料中预期的一样）：
int edge_add(gedge_t* edge, int edge_size, int id1, int id2)
{
  int i;
  if (id1 > id2) {
    CV_SWAP(id1, id2, i);
  }
  else if (id1 == id2) {
    return edge_size;
  }
  for (i = 0; i < edge_size; ++i) {
    if (edge[i].id1 == id1 && edge[i].id2 == id2) {
      return edge_size;
    }
  }
  edge[edge_size].id1 = id1;
  edge[edge_size].id2 = id2;
  ++edge_size;
  return edge_size;
}
int test_springLayout()
{
  //1.set Node(x,y) , Random 随机分布初始节点位置
  //canvas size 1024*768
  static gnode_t node[3];
  static gedge_t edge[1000];
  static int i, n = countof(node), ne = 0;
  if (0 == ne) {
    double start_x, start_y, initSize = 40.0;
    uint seed = 324;
    for (i = 0; i < n; ++i) {
      start_x = 0 + 1024 * .5;
      start_y = 0 + 768 * .5;
      node[i].x = start_x + initSize * (myrandreal(&seed) - .5);
      node[i].y = start_y + initSize * (myrandreal(&seed) - .5);
    }
    for (i = 0; i < n * 3; ++i) {
      unsigned int t = myrand32(&seed);
      ne = edge_add(edge, ne, i % n, t % n);
    }
  }
  springLayout(node, n, edge, ne);
  {
    GETGUIIO2();
    float blendPositions[] = {0, 1};
    COLOR presetColors[] = {Color_Red, Color_Blue};
    gcSolidCircle(ctx, 100, 100, 50, 1, 0, 0);
    for (i = 0; i < n; ++i) {
      int a = edge[i].id1;
      int b = edge[i].id2;
      gcSolidLine(ctx, node[a].x, node[a].y, node[b].x, node[b].y, _rgb(1, 0, 0), 1);
    }
    for (i = 0; i < n; ++i) {
      char buf[256];
      _snprintf(buf, 256, "%d", i);
      gcSolidCircle(ctx, node[i].x, node[i].y, 5, _rgb(1, 0, 0), 0, 0);
      gcSolidText(ctx, iRECT2(node[i].x - 10, node[i].y - 10, 40, 20), buf, NULL, 0, TF_VCENTER | TF_CENTER, Color_Black);
    }
  }
  return 0;
}

