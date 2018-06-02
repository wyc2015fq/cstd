enum {
  ASTAR_NODE_IN_OPEN = 1,
  ASTAR_NODE_IN_CLOSE = 2,
};
typedef struct astarnode_t_ astarnode_t;
struct astarnode_t_ {
  int x, y;
  int g, h;
  int f; // = g + h
  int stage;
  int v;
  astarnode_t* parent; // 父节点
};
typedef struct {
  int nopen;
  astarnode_t* opentable[100];
} astar_t;
void astar_adjust_heap(astar_t* a, int nIndex)
{
  astarnode_t* t;
  int curr = nIndex;
  int child = curr * 2 + 1; // 得到左孩子idx( 下标从0开始，所有做孩子是curr*2+1 )
  int parent = (curr - 1) / 2; // 得到双亲idx
  if (nIndex < 0 || nIndex >= a->nopen) {
    return;
  }
  // 往下调整( 要比较左右孩子和cuur parent )
  while (child < a->nopen) {
    // 小根堆是双亲值小于孩子值
    if (child + 1 < a->nopen && a->opentable[child]->f > a->opentable[child + 1]->f) {
      ++child; // 判断左右孩子大小
    }
    if (a->opentable[curr]->f <= a->opentable[child]->f) {
      break;
    }
    else {
      CV_SWAP(a->opentable[child], a->opentable[curr], t); // 交换节点
      curr = child; // 再判断当前孩子节点
      child = curr * 2 + 1; // 再判断左孩子
    }
  }
  if (curr != nIndex) {
    return;
  }
  // 往上调整( 只需要比较cuur child和parent )
  while (curr != 0) {
    if (a->opentable[curr]->f >= a->opentable[parent]->f) {
      break;
    }
    else {
      CV_SWAP(a->opentable[curr], a->opentable[parent], t);
      curr = parent;
      parent = (curr - 1) / 2;
    }
  }
}
int astar_push_heap(astar_t* a, astarnode_t* node)
{
  node->f = node->h + node->g;
  a->opentable[a->nopen++] = node;
  astar_adjust_heap(a, a->nopen - 1); // 调整堆
  return 0;
}
astarnode_t* astar_pop_heap(astar_t* a)
{
  astarnode_t* node = NULL;
  if (a->nopen > 0) {
    node = a->opentable[0]; // open表的第一个点一定是f值最小的点(通过堆排序得到的)
    a->opentable[0] = a->opentable[--(a->nopen)]; // 最后一个点放到第一个点，然后进行堆调整
    astar_adjust_heap(a, 0); // 调整堆
  }
  return node;
}
astarnode_t* astar_find(void* user, int (*get_neighbors)(void* user, astar_t* a, astarnode_t* curr_node))
{
  //PUSH_HEAP
  astar_t a[1] = {0};
  int n = get_neighbors(user, a, NULL);
  astarnode_t* curr_node = NULL;
  while ((curr_node = astar_pop_heap(a)) && curr_node->h > 0) {
    get_neighbors(user, a, curr_node);
  }
  return curr_node;
}
typedef struct {
  int h, w;
  astarnode_t* node; // all
  astarnode_t* node1; // start
  astarnode_t* node2; // end
} test_astar_t;
int test_astar_print(test_astar_t* g)
{
  int x, y, step = g->w;
  astarnode_t* node = NULL;
  printf("\n");
  for (y = 0; y < g->h; ++y) {
    for (x = 0; x < g->w; ++x) {
      node = &g->node[y * step + x];
      printf("%d", node->stage == ASTAR_NODE_IN_OPEN ? node->f : 0);
    }
    printf("\n");
  }
  return 0;
}
int get_neighbors1(void* user, astar_t* a, astarnode_t* curr_node)
{
  test_astar_t* g = (test_astar_t*)user;
  astarnode_t* node = NULL;
  int i, j, step = g->w, x, y, n = 0;
  if (NULL == curr_node) {
    for (y = 0; y < g->h; ++y) {
      for (x = 0; x < g->w; ++x) {
        node = &g->node[y * step + x];
        node->x = x;
        node->y = y;
        node->parent = NULL;
        node->stage = 0;
        node->g = node->h = node->f = 0;
        if (1 == node->v) {
          g->node1 = &g->node[y * step + x];
        }
        else if (2 == node->v) {
          g->node2 = node;
        }
      }
    }
    if (g->node1) {
      g->node1->stage = 0; // 加入open表
      g->node1->g = 0;
      g->node1->h = abs(g->node2->x - g->node1->x) + abs(g->node2->y - g->node1->y);
      g->node1->parent = NULL;
      n = 1;
      astar_push_heap(a, g->node1);
    }
  }
  else {
    IPOINT pt[4] = {1, 0, -1, 0, 0, 1, 0, -1};
    int w = 1;
    for (i = 0; i < 4; ++i) {
      int x = curr_node->x + pt[i].x, y = curr_node->y + pt[i].y;
      if (x >= 0 && x < g->w && y >= 0 && y < g->h) {
        node = &g->node[y * step + x];
        if ((0 == node->v || 2 == node->v) && ASTAR_NODE_IN_CLOSE != node->stage) {
          if (ASTAR_NODE_IN_OPEN == node->stage) {
            if (node->g > curr_node->g + w) { // 如果更优化
              node->g = curr_node->g + w;
              node->parent = curr_node;
              for (j = 0; j < a->nopen; ++j) {
                if (node == a->opentable[i]) {
                  break;
                }
              }
              astar_adjust_heap(a, j); // 下面调整点
            }
          }
          else {
            node->g = curr_node->g + w;
            node->h = abs(g->node2->x - x) + abs(g->node2->y - y);
            node->parent = curr_node;
            node->stage = ASTAR_NODE_IN_OPEN;
            ++n;
            astar_push_heap(a, node);
          }
        }
      }
    }
  }
  //test_astar_print(g);
  return n;
}
int test_astar()
{
  int maze[10 * 10] = { // 仅仅为了好赋值给map_maze
    1, 0, 0, 3, 0, 3, 0, 0, 0, 0,
    0, 0, 3, 0, 0, 3, 0, 3, 0, 3,
    3, 0, 0, 0, 0, 3, 3, 3, 0, 3,
    3, 0, 3, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 3, 0, 0, 0, 3,
    3, 0, 0, 3, 0, 0, 0, 3, 0, 3,
    3, 0, 0, 0, 0, 3, 3, 0, 0, 0,
    0, 0, 2, 0, 0, 0, 0, 0, 0, 0,
    3, 3, 3, 0, 0, 3, 0, 3, 0, 3,
    3, 0, 0, 0, 0, 3, 3, 3, 0, 3,
  };
  test_astar_t a[1] = {0};
  astarnode_t node[countof(maze)] = {0};
  astarnode_t* end = NULL;
  int i;
  for (i = 0; i < countof(maze); ++i) {
    node[i].v = maze[i];
  }
  a->h = 10;
  a->w = 10;
  a->node = node;
  end = astar_find(a, get_neighbors1);
  return 0;
}

