#ifndef __VENUS_ALPHA_H_
#define __VENUS_ALPHA_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef union tagSpot { // Point type(8B)
  struct {
    long X;
    long Y;
  };
  __int64 Value;
} SPOT, *PSPOT;

typedef struct tagKnot { // Node type(28B)
  SPOT Spot;    // location
  long Prev;    // parent id, -1 is start pos
  long Step;    // level, relate to get path
  long Cost;    // costs to get here
  long Last;    // costs to target
  long Flags;   // is this a wrong way?
} KNOT, *PKNOT;

// star node flags
#define SNF_PROP_READY    0x00000000
#define SNF_PROP_ERROR    0x00000001

typedef union tagMark {
  struct {
    long Flags;   // not detach < 0, open = 0, close > 0
    long Index;   // open list index
  };
  __int64 Value;    // for fast data copy & compare
} MARK, *PMARK;

// star step flags
#define SSF_PROP_CLOSED   -1  // closed < 0
#define SSF_PROP_UNKNOW   0 // unknow = 0
#define SSF_PROP_OPENED   1 // opened > 0

// alpha star cell
//#define ASC_MASK_VALUE    0x11  // mask(边界和起点不可通行)
//#define ASC_TYPE_BLANK    0x00  // blank
//#define ASC_TYPE_BLOCK    0x01  // obstacle
//#define ASC_TYPE_START    0x10  // start
//#define ASC_TYPE_POINT    0x20  // target

// cell distance constants
#define CDC_PROP_NEAR   10;   // 相邻点距离x10，两点距离使用麦哈顿法(dX+dY)
#define CDC_PROP_AWAY   14;   // 对角不是10根号2, 而是14，近似计算(避免浮点运算)

#pragma pack(4)
typedef struct tagMaze {
  UINT Flags;        // robot thinking & cell type
  union {
    SPOT Size;
    struct {
      long SizeX;   // how many columns, Width
      long SizeY;   // how many rows, Height
    };
  };
  long Range;       // 最大的路程(消耗)
  long Count;       // 寻路分析节点上限(初始化分配的节点数量: KnotCount)
  long Value;       // 寻路结果位置数量(寻路后占用的节点数量: SpotCount)
  PKNOT Knots;      // 寻路分析节点数组(内部指针, 数量=.Count)
  PSPOT Spots;      // 寻路结果位置数组(内部指针, 数量=.Value, 上限=.Count)
  PMARK Marks;      // 寻路分析节点属性数组(内部指针, 数量=.Count)
  BYTE* Scene;     // 场景点阵, 数据的指针(指向实际点阵, 不可释放)
  union {
    BYTE Matrix[1];    // 场景点阵, 单元的寻址(改用[Y * Width + X]代替[X][Y])
    UINT Data;
  };
} MAZE, *PMAZE;
#pragma pack()

// ******** API ********
V32API PMAZE  AlphaInit(long dwSizeX, long dwSizeY, long dwMaxCost, UINT dwFlags);
V32API int  AlphaEdit(PMAZE pMaze, long dwPosX, long dwPosY, void* lpValue);
V32API int  AlphaStar(PMAZE pMaze, PSPOT lpStart, PSPOT lpTarget, long* pdwStep);
V32API int  AlphaData(PMAZE pMaze, UINT dwPropId, long dwIndex);
V32API int  AlphaExit(PMAZE pMaze, UINT dwFlags);

#endif