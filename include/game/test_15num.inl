// IDA 解决 十五数码问题
// 15数码问题
// 1878年，美国最伟大的谜题专家"Sam Loyd发明了15数码谜题。这个谜题由一个正方形盒子和15个编号为1，2，...，15
//的正方形滑块组成，每个滑块的边长是盒子边长的1/4，因此可以把它们拼成如下面左图所示的形式，留下一个空格。
//每次可以把任何一个与空格相邻的滑块移动到空格中（因此在左图中可以把滑块12和15移动到空格中），
//而此滑块原来的位置变成新的空格。左图和右图非常接近，那么是否可以从左图的状态出发，在若干步之后变成右图的状态呢？
//Sam Loyd悬赏"1000美元奖给第一个解决这个问题的人。
//游戏规则很容易理解，因此这个问题引起了很多不同职业不同阶层的的人的兴趣，可惜始终没有一个人拿到这1000美元。
//事实上，这个特殊的15数码问题是无解的，但在很多情况下，是可以从一个布局A转化成另一个布局B的。
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#define SIZE1 4
typedef char board[SIZE1][SIZE1];
board target = {
  1, 2, 3, 4,
  12, 13, 14, 5,
  11, 0, 15, 6,
  10, 9, 8, 7
};
board start;
long add[4][2] = { -1, 0, 0, 1, 1, 0, 0, -1};
long targetplace[SIZE1* SIZE1][2]; // 这个估价函数是用来剪枝的
long CAL_H(board& node)
{
  long i, j;
  long re = 0;
  for (i = 0; i < SIZE1; i++) {
    for (j = 0; j < SIZE1; j++) {
      if (node[i][j]) {
        re += abs(i - targetplace[node[i][j]][0])
            + abs(j - targetplace[node[i][j]][1]);
      }
    }
  }
  return re;
}
bool can_solve() // 搜索前判断是否可解
{
  long i, j, k1, k2;
  for (i = 0; i < SIZE1; i++) {
    for (j = 0; j < SIZE1; j++) {
      if (start[i][j] == 0) {
        start[i][j] = SIZE1 * SIZE1;
        k1 = (SIZE1 - 1 - i) + (SIZE1 - 1 - j);
      }
      if (target[i][j] == 0) {
        target[i][j] = SIZE1 * SIZE1;
        k2 = (SIZE1 - 1 - i) + (SIZE1 - 1 - j);
      }
    }
  }
  for (i = 0; i < SIZE1 * SIZE1; i++) {
    for (j = i + 1; j < SIZE1 * SIZE1; j++) {
      if (start[i / SIZE1][i % SIZE1] > start[j / SIZE1][j % SIZE1]) {
        k1++;
      }
      if (target[i / SIZE1][i % SIZE1] > target[j / SIZE1][j % SIZE1]) {
        k2++;
      }
    }
  }
  for (i = 0; i < SIZE1; i++) {
    for (j = 0; j < SIZE1; j++) {
      if (start[i][j] == SIZE1 * SIZE1) {
        start[i][j] = 0;
      }
      if (target[i][j] == SIZE1 * SIZE1) {
        target[i][j] = 0;
      }
    }
  }
  return (k1 % 2) == (k2 % 2);
}
void out_board(board state, long step)
{
  long i, j;
  printf("STEP %ld:\n", step);
  for (i = 0; i < SIZE1; i++) {
    for (j = 0; j < SIZE1; j++) {
      printf("%ld ", state[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}
void output(long dep, char path[]) // 输出答案
{
  long i, j, x1, y1, x0, y0;
  board state;
  memcpy(state, start, sizeof(state));
  out_board(state, 0);
  for (i = 0; i < SIZE1; i++) {
    for (j = 0; j < SIZE1; j++) {
      if (state[i][j] == 0) {
        x0 = i, y0 = j;
      }
    }
  }
  for (i = 0; i < dep; i++) {
    x1 = x0 + add[path[i]][0];
    y1 = y0 + add[path[i]][1];
    state[x0][y0] = state[x1][y1];
    state[x1][y1] = 0;
    x0 = x1, y0 = y1;
    out_board(state, i + 1);
  }
  printf("The minimum number of steps is %ld.\n", dep);
}
long ans;
char path[100000];
bool ida(board& node, long x0, long y0, long dep, long d, long h)
{
  long i, j, k, x1, y1, h1;
  if (memcmp(node, target, sizeof(target)) == 0) {
    output(dep, path);
    return 1;
  }
  if (dep == ans) {
    return 0;
  }
  board node1;
  for (i = 0; i < 4; i++) {
    if (((i % 2) == (d % 2)) && i != d) {
      continue;
    }
    x1 = x0 + add[i][0];
    y1 = y0 + add[i][1];
    if (x1 < 0 || y1 < 0 || x1 == SIZE1 || y1 == SIZE1) {
      continue;
    }
    memcpy(node1, node, sizeof(node1));
    node1[x1][y1] = 0;
    node1[x0][y0] = node[x1][y1];
    if (i == 3 && y1 < targetplace[node[x1][y1]][1]) {
      h1 = h - 1;
    }
    else if (i == 1 && y1 > targetplace[node[x1][y1]][1]) {
      h1 = h - 1;
    }
    else if (i == 0 && x1 < targetplace[node[x1][y1]][0]) {
      h1 = h - 1;
    }
    else if (i == 2 && x1 > targetplace[node[x1][y1]][0]) {
      h1 = h - 1;
    }
    else {
      h1 = h + 1;
    }
    if (h1 + dep + 1 > ans) {
      continue; // 根据估价值(h1+dep)
    }
    // 和假定的解的步数(ans)来剪枝
    path[dep] = i;
    if (ida(node1, x1, y1, dep + 1, i, h1)) {
      return 1;
    }
  }
  return 0;
}
#include "cstd.h"
int test_15num()
{
  long i, j, k, x0, y0;
  long cs;
  //scanf("%ld", &cs);
  cs = 1;
  //printf("input=\n");
  while (cs--) {
    for (i = 0; i < SIZE1 * SIZE1; i++) {
      start[i / SIZE1][i % SIZE1] = i;
    }
    CV_SWAP(start[0][2], start[0][1], i);
    //RAND_SHUFFLE()
    for (i = 0; i < SIZE1; i++) {
      for (j = 0; j < SIZE1; j++) {
        //scanf("%ld", &k);
        //start[i][j] = target[i][j];
        if (start[i][j] == 0) {
          x0 = i;
          y0 = j;
        }
      }
    }
    for (k = 1, i = 0; i < SIZE1; i++) {
      for (j = 0; j < SIZE1; j++) {
        targetplace[target[i][j]][0] = i;
        targetplace[target[i][j]][1] = j;
      }
    }
    if (!can_solve()) {
      printf("This puzzle is not solvable.\n");
      continue;
    }
    i = -1;
    j = CAL_H(start);
    for (ans = j; ; ans += 1) {
      if (ida(start, x0, y0, 0, i, j)) {
        break;
      }
    }
    getch();
  }
  return 0;
}
#undef SIZE1

