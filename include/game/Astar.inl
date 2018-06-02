typedef struct _ClickPt { //已检索过的点
  POINT point; //点本身
  int fp; //父节点在本结构中的位置
  BOOL ckclient; //TRUE为待查点，FALSE为已查点
  int pv; //本点与终点的步长
} ClickPt;
//查找最小PV值，返回结构记录
int SFMinPv(ClickPt* cpt, int n)
{
  int i, minpv = 100, minsn = 0;
  for (i = 0; i < n; i++) {
    if (cpt[i].ckclient) {
      if (cpt[i].pv < minpv) {
        minpv = cpt[i].pv;
        minsn = i;
      }
    }
  }
  return minsn;
}
//取得返回路径,函数返回步数
//path 路径数组
//lastn 终点结构索引号
int BackPath(POINT* backpoint, int n, ClickPt* cpt, POINT astarpoint)
{
  int i, cout = 0, lastn = n - 1;
  POINT* npoint;
  npoint = (POINT*)malloc(n * sizeof(POINT));
  for (i = 0; i < n; i++) {
    npoint[i] = cpt[lastn].point;
    lastn = cpt[lastn].fp;
    cout += 1;
    if (lastn == 0) {
      npoint[cout] = astarpoint;
      break;
    }
  }
  for (i = 0; i <= cout; i++) {
    backpoint[i] = npoint[cout - i];
  }
  free(npoint);
  return cout;
}
//比较两点是否相等
#define CTwoPt(pt1, pt2) (((pt1).x == (pt2).x) && ((pt1).y == (pt2).y))
//计算一个点的PV值
#define KclaterPv(pt) (abs(pt.x - aendpoint.x) + abs(pt.y - aendpoint.y))
//用一个点填充结构体
#define FSPoint(pt, bb) { cpt[n].point = pt;cpt[n].ckclient=bb;cpt[n].fp = bn;cpt[n].pv = KclaterPv(pt); ++n;}
// A*算法构造函数说明：
// starpoint参数： 人物起始点
// endpoint参数： A*寻路终点
// backpoint A*找到的最佳路径返回点数组,若地图不能到达返回0
// map 地图信息
// aendpoint 起点
// astarpoint 终点
// bn 基点记录
// n 结构体索引
//返回值为到达终点所需步数
int AstarGetWay(int h, int w, char* astarmap, int step, POINT astarpoint, POINT aendpoint, POINT* backpoint)
{
  int i, j, k, n, bn;
  ClickPt* cpt = (ClickPt*)malloc(h * w * sizeof(ClickPt));
  POINT bspt; //基点
  POINT cpoint;
  int minpv = 0; //最小pv值的点的索引号
  int err;
  n = 0;
  bn = 0;
  bspt = astarpoint;
  FSPoint(astarpoint, FALSE);
  //检查是否无点可加
  for (j = 0; j < h * w; ++j) {
    err = n;
    for (i = 0; i < 4; ++i) { //循环查找上、下、左、右四点
      switch (i) {
      case 0:
        //返回左点
        cpoint.x = bspt.x - 1;
        cpoint.y = bspt.y;
        break;
      case 1:
        //返回右点
        cpoint.x = bspt.x + 1;
        cpoint.y = bspt.y;
        break;
      case 2:
        //返回上点
        cpoint.x = bspt.x;
        cpoint.y = bspt.y - 1;
        break;
      case 3:
        //返回下点
        cpoint.x = bspt.x;
        cpoint.y = bspt.y + 1;
        break;
      }
      //检索左点是否是地图许可范围内
      k = cpoint.x >= 0 && cpoint.x < w && cpoint.y >= 0 && cpoint.y < h;
      if (k) {
        //检查地图是否允许通行
        k = !astarmap[cpoint.y * step + cpoint.x];
        if (k) {
          //检查一个点是否在结构体中
          int _i;
          k = 0;
          //检索左点是否在结构体内
          for (_i = 0; _i < n; _i++) {
            if (CTwoPt(cpoint, cpt[_i].point)) {
              k = 1;
              break;
            }
          }
          if (!k) {
            // 如果不在就添加
            FSPoint(cpoint, TRUE); //填充结构体
            if (CTwoPt(cpoint, aendpoint)) { //判断该点是否是终点
              k = BackPath(backpoint, n, cpt, astarpoint);
              free(cpt);
              return k;
            }
          }
        }
      }
    }
    //检查是否已无点可查
    if (err == n) {
      int e = -1;
      for (i = 0; i <= n; i++) {
        if (!cpt[i].ckclient) {
          e += 1;
        }
      }
      if (e == n) {
        backpoint[0] = astarpoint;
        return 0;
      }
    }
    minpv = SFMinPv(cpt, n);
    cpt[minpv].ckclient = FALSE; //将具有最小pv值的点移出待查点
    bspt = cpt[minpv].point; //将其置为基点
    bn = minpv; //设置基点记录
  }
  free(cpt);
  return 0;
}

