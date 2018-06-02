// ****************************************************************************************************
// 文件: type.h
// 注释:
// 通用类型定义模块, 定义常用的数据类型; 类型属于Valiant全局命名空间
// ****************************************************************************************************
#ifndef __V32_TYPE_H_
#define __V32_TYPE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ****************************************************************************************************
// Begin namespace
// ****************************************************************************************************
namespace Valiant
{

#ifndef NULL    // Using NULL without windows.h
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef QWORD   // 64b data type
typedef unsigned __int64    QWORD;
#endif
#ifndef PQWORD
typedef unsigned __int64*    PQWORD;
#endif

//#ifdef STRICT
//typedef void *HANDLE;
//#define DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
//#else
//typedef PVOID HANDLE;
//#define DECLARE_HANDLE(name) typedef HANDLE name
//#endif
//typedef HANDLE *PHANDLE;

#pragma pack(1)
typedef struct tagVariant { // smart variant type
  union {
    tagVariant* Value;  // void***...

    struct {
      BYTE bValue1; //  0- 7 bit
      BYTE bValue2; //  8-15 bit
      BYTE bValue3; // 16-23 bit
      BYTE bValue4; // 24-31 bit
    };
    struct {
      WORD wValue1; //  0-15 bit
      WORD wValue2; // 16-31 bit
    };

    char  Value7;   // signed 8 bit
    BYTE  Value8;   // unsigned 8 bit
    short Value15;  // signed 16 bit
    WORD  Value16;  // unsigned 16 bit
    long  Value31;  // signed 32 bit
    UINT  Value32;  // unsigned 32 bit

    char*  pValue7; // signed 8 bit pointer(string)
    BYTE*  pValue8; // unsigned 8 bit pointer
    short* pValue15;  // signed 16 bit pointer
    WORD*  pValue16;  // unsigned 16 bit pointer
    long*  pValue31;  // signed 32 bit pointer
    UINT*  pValue32;  // unsigned 32 bit pointer
    void*  pValue;  // void pointer(handle)
  };
} VAR, *PVAR;
#pragma pack()

typedef struct tagNode1D {
  UINT Size;        // sizeof node
  tagNode1D* Next;    // next node
} NODE1D, *PNODE1D;

typedef struct tagNode2D {
  UINT Size;        // sizeof node
  tagNode2D* Next;    // next node
  tagNode2D* Prev;    // previous node
} NODE2D, *PNODE2D;

typedef struct tagNode3D {
  UINT Size;        // sizeof node
  tagNode3D* Next;    // next node
  tagNode3D* Prev;    // previous node
  tagNode3D* Master;    // host node
} NODE3D, *PNODE3D;

typedef struct tagNode4D {
  UINT Size;        // sizeof node
  tagNode4D* Next;    // next node
  tagNode4D* Prev;    // previous node
  tagNode4D* Master;    // master node
  tagNode4D* Slaves;    // slaves node
} NODE4D, *PNODE4D;

typedef struct tagNode {  // default node type
  NODE1D Node1;
  NODE2D Node2;
  NODE3D Node3;
  NODE4D Node4;
} NODE, *PNODE;

typedef struct tagCache { // cache type
  UINT Flags;       // cache flags
  UINT Volume;      // sizeof applied cache, not including struct size
  UINT Size;        // sizeof used cache, not including struct size
  union {         // available nodes
    PNODE1D PrefixNode;
    PNODE2D PrefixNode2;
    PNODE3D PrefixNode3;
    PNODE4D PrefixNode4;
  };
  union {         // tail nodes
    PNODE1D SubfixNode;
    PNODE2D SubfixNode2;
    PNODE3D SubfixNode3;
    PNODE4D SubfixNode4;
  };
  union {         // recycling nodes
    PNODE1D WastedNode;
    PNODE2D WastedNode2;
    PNODE3D WastedNode3;
    PNODE4D WastedNode4;
  };
  union {         // target node
    PNODE1D ActiveNode;
    PNODE2D ActiveNode2;
    PNODE3D ActiveNode3;
    PNODE4D ActiveNode4;
  };
  union {         // node data
    UINT Data;      // memory alignment
    BYTE* Buffer;   // external buffer
    BYTE Dummy[1];    // internal buffer
  };
} CACHE, *PCACHE;

typedef struct tagBeta {  // Version type, marker, patch
  UINT Major;       // major version, changed every framework modified
  UINT Minor;       // minor version, changed every function modified
  UINT Release;     // compiling version, changed release Build
  UINT Update;      // revison, changed every debug Build
} BETA, *PBETA;

#pragma pack(1)
typedef struct tagDate {  // Date type
  union {
    UINT Date;      // YYYY-MM-DD
    struct {
      WORD Year;    // YYYY
      BYTE Month;   // MM
      BYTE Day;   // DD
    };
  };
} DATE1, *PDATE;
typedef struct tagTime {  // Time type
  union {
    UINT Time;      // HH:MM:SS
    struct {
      BYTE Hour;    // HH
      BYTE Minute;  // MM
      BYTE Second;  // SS
      char Zone;    // +8 Only
    };
  };
} TIME, *PTIME;
typedef struct tagDateTime { // Smart date time
  union {
    QWORD DateTime;   // YYYY-MM-DD HH:MM:SS +Z
    struct {
      UINT Date;    // YYYY-MM-DD
      UINT Time;    // HH:MM:SS
    };
    struct {
      WORD Year;    // YYYY
      BYTE Month;   // MM
      BYTE Day;   // DD
      BYTE Hour;    // HH
      BYTE Minute;  // MM
      BYTE Second;  // SS
      char Zone;    // +8 Only
    };
  };
} DATETIME, *PDATETIME;
#pragma pack()

}
// ****************************************************************************************************
// End namespace
// ****************************************************************************************************

#endif