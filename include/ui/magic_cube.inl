//一个三阶魔方由六个面组成, 颜色分别是
//0 白色（W）
//1 对面为黄色（Y）
//2 红色（R）
//3 对面为橙色（O）
//4 绿色（G）
//5 对面为蓝色（B）
//如果手持魔方, 白色向上, 红色朝向自己, 则绿色在左手侧, 蓝右, 黄下, 橙背。
//请写一个程序, 对一个处于还原状态（各面各块同色）的魔方, 进行操作,
//打印操作后的魔方状态。操作指令为单个操作指令组成的字符串。
//单个操作指令包括：
//U：白色面顺时针旋转90度
//D：黄色面顺时针旋转90度
//L：绿色面顺时针旋转90度
//R：蓝色面顺时针旋转90度
//F：红色面顺时针旋转90度
//B：橙色面顺时针旋转90度
//顺时针90
//6, 7, 0 4, 5, 6
//5, , 1 ==> 3, , 7
//4, 3, 2 2, 1, 0
//右下左上 => 0123
// 魔方 48 小面展开
// B面
// 46,47,40
// 45, ,41
// 44,43,42
//
// U面
// 22,23,16
// 21, ,17
// 20,19,18
//
// L面 F面 R面
// 14,15, 8 6, 7, 0 30,31,24
// 13, , 9 5, , 1 29, ,25
// 12,11,10 4, 3, 2 28,27,26
//
// D面
// 38,39,32
// 37, ,33
// 36,35,34
uchar opt_group[12 * 48] = {0};
int magic_cube_group(const unsigned char* input_pos, int opt, unsigned char* output_pos)
{
  static int inited = 0;
  if (!inited) {
    int i, j, k;
    const char* r90 = "67012345";
    const char* optstr = "FLURDB";
    // 上右下左
    const char* side[6] = {
      "UDRLDULR",//F
      "ULFLDLBL",//L
      "BDRUFULU",//U
      "URBRDRFR",//R
      "FDRDBULD",//D
      "DDRRUULL",//B
    };
    char aa[6 * 3] = {
      0, 0, 0, //前
      4, 5, 6, //左
      6, 7, 0, //上
      0, 1, 2, //右
      2, 3, 4, //下
      0, 0, 0, //后
    };
    for (i = 0; i < 6; ++i) {
      unsigned char* g1 = opt_group + i * 6 * 8;
      for (j = 0; j < 6 * 8; ++j) {
        g1[j] = j;
      }
      for (j = 0; j < 8; ++j) {
        g1[i * 8 + j] = (r90[j] - '0') + i * 8;
      }
      {
        const char* ss = side[i];
        uchar mp[128];
        for (j = 0; j < 6; ++j) {
          mp[optstr[j]] = j;
        }
        // 旋转周围四面
        for (j = 0; j < 8; j += 2) {
          int l = (j + 2) % 8;
          int j0 = mp[ss[j + 0]];
          int j1 = mp[ss[j + 1]];
          int l0 = mp[ss[l + 0]];
          int l1 = mp[ss[l + 1]];
          for (k = 0; k < 3; ++k) {
            g1[l0 * 8 + aa[l1 * 3 + k]] = j0 * 8 + aa[j1 * 3 + k];
          }
        }
      }
      {
        for (j = 0; j < 48; ++j) {
          printf("%d ", g1[j]);
        }
        printf("\n");
      }
    }
    inited = 1;
  }
  return 0;
}

