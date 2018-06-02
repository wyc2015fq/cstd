#ifndef __VENUS_BOX_H_
#define __VENUS_BOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct tagProp {
  UINT Flags;        // 属性标识
  UINT Count;
  UINT Value;
  UINT Hash;
  union {
    SPOT Role;      // 角色位置
    struct {
      UINT RoleX;    // 角色水平坐标
      UINT RoleY;    // 角色垂直坐标
    };
  };
  union {
    SPOT Size;      // 角色位置
    struct {
      UINT SizeX;    // 角色水平坐标
      UINT SizeY;    // 角色垂直坐标
    };
  };
  PSPOT Spots;
} PROP, *PPROP;

// Stage property flags
#define SPF_SPOT_ARRAY    0x00000001    // 要求返回箱子坐标

// ①②③
// ④◎⑤
// ⑥⑦⑧
#pragma pack(1)
typedef union tagSquare {
  struct {
    BYTE UpperLeft;    // ①
    BYTE UpperTop;   // ②
    BYTE UpperRight; // ③
    BYTE LeftSide;   // ④
    BYTE RightSide;    // ⑤
    BYTE BottomLeft; // ⑥
    BYTE BottomDown; // ⑦
    BYTE BottomRight;  // ⑧
  };
  struct {
    BYTE Left;     // 目标左侧
    BYTE Core;     // 目标中心
    BYTE Right;      // 目标右侧
    BYTE Front;      // 目标正前
    BYTE UpHill;   // 目标右前
    BYTE DownHill;   // 目标左前
    //WORD Reserved;
  };
  __int64 Value;      // 缓存僵局九宫格组, 快速判断僵局: 按位掩码大幅降低数组尺寸
} SQUARE, *PSQUARE;

//typedef struct tagIdle{
//
//} IDLE, *PIDLE;
#pragma pack()

typedef struct tagMove {
  UINT Index;        // 箱子索引
  UINT Value;        // 归位数量
  union {
    SPOT Porter;    // 角色位置
    struct {
      UINT PortX;    // 角色水平坐标
      UINT PortY;    // 角色垂直坐标
    };
  };
  union {
    SPOT Movement;    // 寻路位置
    struct {
      UINT MoveX;    // 寻路水平坐标
      UINT MoveY;    // 寻路垂直坐标
    };
  };
  union {
    SPOT Object;    // 箱子位置
    struct {
      UINT ObjX;   // 寻路水平坐标
      UINT ObjY;   // 寻路垂直坐标
    };
  };
  UINT DestId;     // 目标单元索引
  UINT CubeId;     // 箱子单元索引
  UINT RoleId;     // 角色单元索引
  union {
    UINT Flags;      // 标识数值任选
    struct {
      BYTE Dest;   // 目标位置的值(AL)
      BYTE Cube;   // 箱子位置的值(AX)
      BYTE Move;   // 寻路位置的值
      BYTE Role;   // 角色位置的值
    };
  };
} MOVE, *PMOVE;

#pragma pack(4)       // SPOT.Value默认以64位对齐
typedef struct tagSnap {
  union {
    SPOT Position;    // 局面角色位置
    struct {
      UINT PosX;   // 角色水平坐标
      UINT PosY;   // 角色垂直坐标
    };
  };
  UINT Hash;       // 快照布局指纹
  SPOT Stars[1];      // 箱子位置列表(0元素数组为非标准扩展)
} SNAP, *PSNAP;
#pragma pack()

typedef struct tagShot {
  UINT Count;        // 快照局面数量
  UINT Value;        // *** 动态内存频繁重分配效率低, 预分配以提速 ***
  UINT Size;       // 单位快照尺寸
  PSNAP Snaps;      // 快照局面列表
} SHOT, *PSHOT;

typedef struct tagStack {
  UINT Count;        // 堆栈单元数量
  PSPOT Stars;      // 堆栈坐标列表(数量为单元数, 用于扫描线计算)
} STACK, *PSTACK;

// Path finding callback
typedef int (* PFCB)(void* lpVoid);

#pragma pack(4)   // SPOT.Value(__int64)默认以64位对齐
typedef struct tagStage {
  UINT Volume;     // 结构体实际大小(加速计算)
  UINT Flags;        // 场景标识
  PSPOT Stars;      // 箱子位置列表(内部指针, 不释放, 数量为.Count)
  PSPOT Series;     // 排序箱子坐标(内部指针, 不释放, 数量为.Count)
  UINT Count;        // 箱子数量(目标数量)
  UINT Value;        // 归位数量
  UINT Hash;       // 场景指纹(箱子坐标排序哈希值)
  tagStage* Prev;     // 上个场景(游戏中连接队列操作, 伪指针, 不释放)
  tagStage* Next;     // 下个场景(游戏中连接队列操作, 伪指针, 不释放)
  tagStage* Host;     // 父级场景(求解时反向父级搜索得到解法路径, 伪指针, 不释放)
  union {
    SPOT Size;      // 场景尺寸
    struct {
      long SizeX;   // 场景宽度(列数)
      long SizeY;   // 场景高度(行数)
    };
  };
  union {
    SPOT Position;    // 角色当前位置
    struct {
      long PosX;    // 角色水平位置
      long PosY;    // 角色垂直位置
    };
  };
  union {
    SPOT Automation;  // 自动寻路位置
    struct {
      long AutoX;   // 寻路水平坐标
      long AutoY;   // 寻路垂直坐标
    };
  };
  PMOVE Moves;      // 可行走法列表(内部指针, 不释放, 数量为.Count * 4: 四个方向)
  UINT Range;        // 可行走法数量
  UINT Index;        // 当前测试走法
  UINT Slaves;     // 剩余未分析的子场景数量
  UINT Layer;        // 当前步数
  union {
    BYTE Matrix[1];    // 矩阵数据
    long Data;
  };
} STAGE, *PSTAGE;
#pragma pack()

// 100x100占10000字节, 1000步占10000000B 约10M
// 256x256占65536字节, 1000步占65536000B 约63M
// 取反特定位 ^ 010;
// 置位特定位 | 010;
// 清零特定位 & 101;
#define GetMatrix(s, X, Y)  s->Matrix[Y * s->SizeX + X]

// Stage file header
#define SFH_HEADER    0x786F622E  // .box文件头部

// Stage global flags
#define SGF_PRIME   0x00000000  // 原始场景(新建编辑, 没有坐标数据)
#define SGF_BASIC   0x00000001  // 基础场景(基础数据, 可以游戏)
#define SGF_EXTRA   0x00000002  // 扩展场景(扩展数据, 可以求解)
#define SGF_READY   0x00000003  // 就绪场景(数据完整)
#define SGF_ARRAY   0x00000004  // 子集场景(队列元素, 不可释放)

#define SGF_CRC32   0x10000000  // 哈希校验
#define SGF_DEBUG   0x80000000  // 调试输出

// Stage matrix types
#define SMT_UNKNOW    0x00    // 空位
#define SMT_OBJECT    0x01    // 箱子
#define SMT_TARGET    0x02    // 目的
#define SMT_BORDER    0x04    // 边界
#define SMT_PORTER    0x08    // 角色
#define SMT_FILTER    0x0F    // 掩码

#define SMT_OPENED    0x10    // 可以到达当前单元

#define SMT_MASKED    0x05    // 箱子边界不可通行
#define SMT_OPNMSK    0x15    // ..
#define SMT_MATRIX    0x0F0F0F0F  // 掩码批量

// Path pushing callback
typedef int (__cdecl* PFNPUSH)(PSTAGE pStage, UINT dwIndex, UINT dwFlags);

// Stage move flags
#define SMF_MOVE_NONE 0x00000000  // 无(应用动作)

#define SMF_MOVE_RISE 0x00000001  // 上
#define SMF_MOVE_DROP 0x00000002  // 下
#define SMF_MOVE_BACK 0x00000003  // 左
#define SMF_MOVE_FORE 0x00000004  // 右
#define SMF_MOVE_MASK 0x0000000F  // 掩码(方向只能有一个)

#define SMF_MOVE_USED 0x00000010  // 锁定(判断比移动高效)
#define SMF_MOVE_TEST 0x00000020  // 尝试(游戏时检测移动)

#define SMF_FLAG_HASH 0x10000000  // 使用哈希(使用CRC32快速校验局面重复)
#define SMF_FLAG_TEST 0x80000000  // 调试输出
#define SMF_FLAG_MASK 0xFF000000  // 标识掩码

// Stage prop flags
#define SPF_SAFE_CHECK    -1
#define SPF_UNIT_VALUE    -2
#define SPF_LAST_ERROR    -3
#define SPF_PREV_STAGE    -4
#define SPF_NEXT_STAGE    -5
#define SPF_HEAP_STAGE    -6
#define SPF_CALC_STAGE    -7
#define SPF_PUSH_STAGE    -8
#define SPF_PATH_STAGE    -9

typedef struct tagQueue { // 与堆栈不同, 先进先出
  UINT Volume;     // 队列容量(字节数)
  UINT Size;       // 元素内存大小
  UINT Count;        // 元素上限索引
  UINT Value;        // 当前元素个数(下个索引)
  UINT Used;       // 已用元素个数
  UINT Step;       // 结果步数
  PSTAGE Active;      // 首个活动场景(从此弹出)
  PSTAGE Backup;      // 末尾活动场景(向此压入)
  PSTAGE Stages;      // 过期场景(压入弹出)
  PSHOT Shots;      // 失败快照列表(外部指针, 外部释放)
  PSTACK Stacks;      // 扫描坐标列表(外部指针, 外部释放)
  union {
    BYTE Dummy[1];
    UINT Data;
  };
} QUEUE, *PQUEUE;

// Solution error codes
// 错误代码分级: 最多四级, 最外层函数设置高位
#define SEC_ERROR_NONE    0x00000000  // 成功完成

#define SEC_FILE_OPEN   0x00000001  // 文件打开失败
#define SEC_FILE_SIGN   0x00000002  // 文件类型无效
#define SEC_FILE_SIZE   0x00000003  // 文件大小无效

#define SEC_CACHE_NULL    0x00000010  // 内存不足

#define SEC_PARAM_NULL    0x00000020  // 参数无效
#define SEC_PARAM_MANY    0x00000030  // 参数越界

#define SEC_STAGE_ROLE    0x00000100  // 场景角色出现复数
#define SEC_STAGE_VARY    0x00000200  // 场景箱子目标不等
#define SEC_STAGE_SAVE    0x00000300  // 场景新建不能保存
#define SEC_STAGE_FLAG    0x00000400  // 场景元素不能释放
#define SEC_STAGE_GAME    0x00000400  // 场景新建不能游戏

#define SEC_STAGE_NULL    1     // 无效指针
#define SEC_SOLVE_DONE    3     // 场景已经归位
#define SEC_STAGE_ECHO    5     // 场景重现

#define SEC_ERROR_DUMP    0xFFFFFFFF  // 只读不写

// ******** API ********
V32API PSTAGE  StageInit(void* lpVoid, PSPOT pSpot, UINT dwFlags);
V32API int  StageEdit(PSTAGE pStage, void* lpVoid, long dwPosX, long dwPosY);
V32API int  StageMove(PSTAGE pStage, void* lpVoid, UINT dwIndex, UINT dwFlags);
V32API int  StageProp(PSTAGE pStage, void* lpVoid, long dwPropId, UINT dwFlags);
V32API int  StageSave(PSTAGE pStage, void* lpVoid, UINT dwFlags);
V32API int  StageExit(PSTAGE pStage, UINT dwFlags);

#endif