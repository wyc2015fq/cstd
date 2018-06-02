#ifndef __VENUS_DEBUG_H_
#define __VENUS_DEBUG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _DEBUG

#define DBGINIT()       fnDebugInit()
#define DBGTEXT(f)        fnGetText(f)
//#define DBGLINE(s, args...) fnPrint(s, ##args)
#define DBGLINE(s)        fnPrint(s)
#define DBGEXIT()       fnDebugExit()

extern int fnDebugInit();
extern const char* fnGetText(unsigned int dwFlags);
extern int fnPrint(const char* szText, ...);
extern void fnPrintBox(PSTAGE pStage);
extern void fnPrintStage(PSTAGE pStage, int printTrees);
extern void fnOutput();
extern int fnDebugExit();

#else

#define DBGINIT()
#define DBGTEXT(f)        NULL
//#define DBGLINE(s, args...)
#define DBGLINE(s)
#define DBGEXIT()

#endif

// ******** 内部函数声明 ********

// alpha.cpp
// *** No internal function defined in this file ***

// alpha_star.cpp
PMAZE fnAlphaInit(long dwSizeX, long dwSizeY, long dwMaxCost, UINT dwFlags);
int fnAlphaEdit(PMAZE pMaze, long dwPosX, long dwPosY, void* lpValue);
int fnAlphaStar(PMAZE pMaze, PSPOT lpStart, PSPOT lpTarget, long* pdwStep);
int fnAlphaProp(PMAZE pMaze, UINT dwPropId, long dwIndex);
int fnAlphaExit(PMAZE pMaze, UINT dwFlags);

// debug.cpp
extern UINT fnStageCode(UINT dwCode);

// box.cpp
// *** No internal function defined in this file ***

// box_query.cpp
extern long fnStageIndex(void* lpVoid);
extern int fnStageEdit(PSTAGE pStage, void* lpVoid, long dwPosX, long dwPosY);
extern int fnStageQuery(PSTAGE pStage, void* lpVoid, long dwPropId, UINT dwFlags);

// box_hash.cpp
extern UINT fnStageHash(PSPOT pStar, PSPOT pSort, UINT dwCount);
extern int fnStageProp(BYTE* pMatrix, PPROP pProp);

// box_stage.cpp
extern UINT fnStageSize(UINT dwCount, UINT dwSizeX, UINT dwSizeY);
extern void fnStagePtr(PSTAGE pStage);
extern int fnStageSort(PSPOT pStar1, PSPOT pStar2, PSPOT pCache, UINT dwCount);
extern BYTE* fnStageFile(void* lpVoid, PSPOT pSpot);
extern PSTAGE fnStageAlloc(BYTE* pData, PPROP pProp);
extern int fnStageErase(PSTAGE pStage, UINT dwFlags);

// box_file.cpp
extern PSTAGE fnStageOpen(void* lpVoid, UINT dwFlags);
extern PSTAGE fnStageApply(void* lpVoid, PSPOT pSpot, UINT dwFlags);
extern int fnStageSave(PSTAGE pStage, void* lpVoid, UINT dwFlags);

// box_loop.cpp
extern void fnFreeScene();
extern int fnStageStars(PSTAGE pStage, PSPOT pStars);
extern int fnStageLoopHash(PQUEUE pQueue, PSTAGE pStage);
extern int fnStageLoopSpot(PQUEUE pQueue, PSTAGE pStage);
extern int fnStageLoop(PQUEUE pQueue, PSTAGE pStage);

// box_queue.cpp
extern PQUEUE fnQueueInit(PSTAGE pStage, PSTACK pStack, PSHOT pShot, UINT dwSize, UINT dwCount, UINT dwFlags);
extern int fnQueueRange(PQUEUE pQueue, PSTAGE pStage);
extern PSTAGE fnQueuePop(PQUEUE pQueue);
extern int fnQueuePush(PQUEUE pQueue, PSTAGE pStage);
extern PSTAGE fnQueueApply(PQUEUE pQueue);
extern PSTAGE fnQueueRemove(PQUEUE pQueue, PSTAGE pStage);

// box_motion.cpp
extern int fnStagePush(PSTAGE pStage, PMOVE pMove, UINT dwFlags);
extern int fnStageProbe(PSTAGE pStage, UINT dwFlags);

// box_scan.cpp
extern int fnStageScan(PQUEUE pQueue, PSTAGE pStage);

// box_solution.cpp
extern UINT fnStageMoves(PSTAGE pStage);
extern PSTAGE fnStageNext(PQUEUE pQueue, PSTAGE pStage, int* pdwCode);
extern int fnStageSnap(PQUEUE pQueue, PSTAGE pStage);
extern void fnStageBest(PSTAGE pStage);
extern void fnStagePath(PQUEUE pQueue, PSTAGE pStage, PSPOT pPath);
extern UINT fnStageSlaves(PQUEUE pQueue, PSTAGE pStage, void* lpVoid, int* pdwCode);
extern int fnStageCalc(PQUEUE pQueue, void* lpVoid);
extern int fnStageSolve(PSTAGE pStage, void* lpVoid, UINT dwCount, UINT dwFlags);

// trapped.cpp
extern int fnStageTrapped(PSQUARE ps);

#endif