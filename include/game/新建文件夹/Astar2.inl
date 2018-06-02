//通用Astar算法 通用A星算法 通用A_算法
//版权声明：本文为博主原创文章，未经博主允许不得转载。
//写RPG游戏的时候，解决怪物寻路问题的时候写的，寻路是正确的，效率虽然说不高，但是还是满足了游戏里面的寻路要求。
//提供了4个算法，其中2个是用POINT结构体来输入输出的，还有一个是用自定的结构体来输入输出的，同时函数接受一个数据源，只要自己实现那个Adapter的GetData接口就行了，所以无论你的数据是怎么样的，不用改动原先的代码即可寻路。
//SEARCH_OFFSET_X和SEARCH_OFFSET_Y是确定搜索范围的，因为游戏效率要求，所以可以设置的小点，不然的话设大点也行。
#include <Windows.h>
#include <stack>
#include <vector>
#include <list>
#include <math.h>
#include <algorithm>
#define SEARCH_OFFSET_X 5
#define SEARCH_OFFSET_Y 5
enum ASTAR_DIRECTION {
  ADT_TOPLEFT,
  ADT_TOP,
  ADT_TOPRIGHT,
  ADT_LEFT,
  ADT_RIGHT,
  ADT_BOTTOMLEFT,
  ADT_BOTTOM,
  ADT_BOTTOMRIGHT,
  ADT_TOTAL,
};
struct SAstarElement {
  int nX;
  int nY;
  BOOL bOpen;
  // 计算信息
  // 和
  int nFn;
  // 开始点到当前位置移动距离
  UINT nGn;
  // 当前位置到目标坐标的距离
  UINT nHn;
  SAstarElement* pParent;
};
typedef std::list<SAstarElement> SAstarElements;
typedef std::list<SAstarElement*> SAstarPelements;
typedef std::list<POINT> SAstarPoints;
class SAstarDataAdaptor
{
public:
  SAstarDataAdaptor() {
    m_pSrc = NULL;
  }
  virtual ~SAstarDataAdaptor() {}
public:
  void SetDataSource(void* _psrc) {
    m_pSrc = _psrc;
  }
  // 返回元素nFn为-1表示元素不存在
  virtual SAstarElement GetData(int _x, int _y) const = 0;
protected:
  void* m_pSrc;
};
// Element
BOOL Calculate8DirectionNormal(const SAstarDataAdaptor* _psrc, SAstarElements* _pret, const SAstarElement* _pfrom, const SAstarElement* _pto);
BOOL Calculate8DirectionNoDiag(const SAstarDataAdaptor* _psrc, SAstarElements* _pret, const SAstarElement* _pfrom, const SAstarElement* _pto);
// POINT
BOOL Calculate8DirectionNormal(const SAstarDataAdaptor* _psrc, SAstarPoints* _pret, const POINT* _pfrom, const POINT* _pto);
BOOL Calculate8DirectionNoDiag(const SAstarDataAdaptor* _psrc, SAstarPoints* _pret, const POINT* _pfrom, const POINT* _pto);
// 启发式计算Hn
int GetElementHn(const SAstarElement* _pcur, const SAstarElement* _pdest);
int GetElementHn(const SAstarElement* _pcur, const POINT* _pdest);
bool operator== (const SAstarElement& ele1, const SAstarElement& ele2);
struct SAstarCompGreater {
  bool operator()(const SAstarElement* e1, const SAstarElement* e2) {
    if (e1->nFn < e2->nFn) {
      return true;
    }
    return false;
  }
};
struct SAstarEqual {
  bool operator()(const SAstarElement& e1, const SAstarElement& e2) {
    if (e1.nX == e2.nX &&
        e2.nY == e1.nY) {
      return true;
    }
    return false;
  }
};
BOOL Calculate8DirectionNormal(const SAstarDataAdaptor* _psrc, SAstarElements* _pret, const SAstarElement* _pfrom, const SAstarElement* _pto)
{
  if (_psrc == NULL ||
      _pret == NULL) {
    return FALSE;
  }
  static const int nCalTable[] = { -1, -1, 0, -1, 1, -1, -1, 0, 1, 0, -1, 1, 0, 1, 1, 1};
  static const int nCostNormal = 10;
  static const int nCostDiag = 14;
  SAstarPelements m_OpenTable;
  SAstarPelements m_CloseTable;
  int nLeftX = min(_pfrom->nX, _pto->nX);
  int nTopY = min(_pfrom->nY, _pto->nY);
  int nArrayWidth = (nLeftX < SEARCH_OFFSET_X ? nLeftX : SEARCH_OFFSET_X) + SEARCH_OFFSET_X + abs(_pfrom->nX - _pto->nX) + 1;
  int nArrayHeight = (nTopY < SEARCH_OFFSET_Y ? nTopY : SEARCH_OFFSET_Y) + SEARCH_OFFSET_Y + abs(_pfrom->nY - _pto->nY) + 1;
  int nArraySize = (nArrayWidth * nArrayHeight);
  nLeftX -= SEARCH_OFFSET_X;
  if (nLeftX < 0) {
    nLeftX = 0;
  }
  nTopY -= SEARCH_OFFSET_Y;
  if (nTopY < 0) {
    nTopY = 0;
  }
  SAstarElement* pSearchEle = new SAstarElement[nArraySize];
  ZeroMemory(pSearchEle, sizeof(SAstarElement) * nArraySize);
  SAstarElement ardEles[ADT_TOTAL];
  ZeroMemory(ardEles, sizeof(ardEles));
  SAstarElement getEle;
  SAstarElement* pTst;
  // _pfrom->pParent = NULL;
  // _pfrom->nGn = 0;
  // _pfrom->nHn = 0;
  // _pfrom->nFn = 0;
  pSearchEle[(_pfrom->nY - nTopY) * nArrayWidth + _pfrom->nX - nLeftX] = *_pfrom;
  pSearchEle[(_pfrom->nY - nTopY) * nArrayWidth + _pfrom->nX - nLeftX].pParent = NULL;
  pSearchEle[(_pfrom->nY - nTopY) * nArrayWidth + _pfrom->nX - nLeftX].nGn = 0;
  pSearchEle[(_pfrom->nY - nTopY) * nArrayWidth + _pfrom->nX - nLeftX].nFn = 0;
  pSearchEle[(_pfrom->nY - nTopY) * nArrayWidth + _pfrom->nX - nLeftX].nHn = 0;
  m_CloseTable.push_back(&pSearchEle[(_pfrom->nY - nTopY) * nArrayWidth + _pfrom->nX - nLeftX]);
  getEle = *m_CloseTable.back();
  SAstarPelements::iterator iter;
  while (1) {
    for (int i = ADT_TOPLEFT; i < ADT_TOTAL; ++i) {
      ardEles[i] = _psrc->GetData((*m_CloseTable.back()).nX + nCalTable[2 * i],
          m_CloseTable.back()->nY + nCalTable[2 * i + 1]);
      if ((*m_CloseTable.back()).nX + nCalTable[2 * i] < nLeftX ||
          (*m_CloseTable.back()).nX + nCalTable[2 * i] >= nLeftX + nArrayWidth ||
          (*m_CloseTable.back()).nY + nCalTable[2 * i + 1] < nTopY ||
          (*m_CloseTable.back()).nY + nCalTable[2 * i + 1] >= nTopY + nArrayHeight) {
        // 不检索在Buffer区域外的点
        ardEles[i].nFn = -1;
        continue;
      }
      for (iter = m_CloseTable.begin();
          iter != m_CloseTable.end();
          ++iter) {
        if ((*iter)->nX == ardEles[i].nX &&
            (*iter)->nY == ardEles[i].nY) {
          // Close表不检测
          break;
        }
      }
      if (iter != m_CloseTable.end()) {
        // 在Close表中 跳出
        continue;
      }
      if (ardEles[i].nFn == -1 ||
          ardEles[i].bOpen == 0) {
        // 不存在 或者不可通过
        ardEles[i].nFn = -1;
      }
      else {
        // 存在 有两种情况 一种在Open表中 一种不在
        for (iter = m_OpenTable.begin();
            iter != m_OpenTable.end();
            ++iter) {
          if ((*iter)->nX == ardEles[i].nX &&
              (*iter)->nY == ardEles[i].nY) {
            break;
          }
        }
        //ardEles[i].pParent = &pSearchEle[(m_CloseTable.back()->nY - nTopY) * nArrayWidth + m_CloseTable.back()->nX - nLeftX];
        ardEles[i].pParent = m_CloseTable.back();
        // 假设该元素与目的元素相同 则找到
        if (ardEles[i].nX == _pto->nX &&
            ardEles[i].nY == _pto->nY) {
          // 依照parent寻找路径
          SAstarElement* pEle = ardEles[i].pParent;
          while (pEle) {
            _pret->push_back(*pEle);
            pEle = pEle->pParent;
          }
          _pret->reverse();
          delete[] pSearchEle;
          return TRUE;
        }
        if (iter == m_OpenTable.end()) {
          // 没有重复元素 直接添加入Open表
          //ardEles[i].pParent = m_CloseTable.back();
          ardEles[i].nGn = ardEles[i].pParent->nGn + ((i == 1 || i == 3 || i == 4 || i == 6) ? nCostNormal : nCostDiag);
          ardEles[i].nHn = GetElementHn(&ardEles[i], _pto) * 10;
          ardEles[i].nFn = ardEles[i].nGn + ardEles[i].nHn;
          pSearchEle[(ardEles[i].nY - nTopY) * nArrayWidth + ardEles[i].nX - nLeftX] = ardEles[i];
          m_OpenTable.push_back(&pSearchEle[(ardEles[i].nY - nTopY) * nArrayWidth + ardEles[i].nX - nLeftX]);
        }
        else {
          // 有重复元素 判断Gn 若走到重复方块的Gn小于原先方块的Gn 则修改
          int nTestGn = ardEles[i].pParent->nGn + ((i == 1 || i == 3 || i == 4 || i == 6) ? nCostNormal : nCostDiag);
          if (nTestGn < (*iter)->nGn) {
            (*iter)->nGn = nTestGn;
            (*iter)->nFn = (*iter)->nHn + nTestGn;
            //(*iter)->pParent = &pSearchEle[(m_CloseTable.back()->nY - nTopY) * nArrayWidth + m_CloseTable.back()->nX - nLeftX];
            (*iter)->pParent = m_CloseTable.back();
          }
        }
      }
    }
    // 从Open表中找出最小Fn，加入Close表搜索
    if (m_OpenTable.empty()) {
      // Open表搜索完了 没有找到路劲
      delete[] pSearchEle;
      return FALSE;
    }
    else {
      m_OpenTable.sort(SAstarCompGreater());
      pTst = *m_OpenTable.begin();
      m_CloseTable.push_back(*m_OpenTable.begin());
      m_OpenTable.erase(m_OpenTable.begin());
    }
  }
}
BOOL Calculate8DirectionNoDiag(const SAstarDataAdaptor* _psrc, SAstarElements* _pret, const SAstarElement* _pfrom, const SAstarElement* _pto)
{
  if (_psrc == NULL ||
      _pret == NULL) {
    return FALSE;
  }
  if (_pfrom->nX == _pto->nX &&
      _pfrom->nY == _pto->nY) {
    return FALSE;
  }
  static const int nCalTable[] = { -1, -1, 0, -1, 1, -1, -1, 0, 1, 0, -1, 1, 0, 1, 1, 1};
  static const int nCostNormal = 10;
  static const int nCostDiag = 14;
  SAstarPelements m_OpenTable;
  SAstarPelements m_CloseTable;
  int nLeftX = min(_pfrom->nX, _pto->nX);
  int nTopY = min(_pfrom->nY, _pto->nY);
  int nArrayWidth = (nLeftX < SEARCH_OFFSET_X ? nLeftX : SEARCH_OFFSET_X) + SEARCH_OFFSET_X + abs(_pfrom->nX - _pto->nX) + 1;
  int nArrayHeight = (nTopY < SEARCH_OFFSET_Y ? nTopY : SEARCH_OFFSET_Y) + SEARCH_OFFSET_Y + abs(_pfrom->nY - _pto->nY) + 1;
  int nArraySize = (nArrayWidth * nArrayHeight);
  nLeftX -= SEARCH_OFFSET_X;
  if (nLeftX < 0) {
    nLeftX = 0;
  }
  nTopY -= SEARCH_OFFSET_Y;
  if (nTopY < 0) {
    nTopY = 0;
  }
  SAstarElement* pSearchEle = new SAstarElement[nArraySize];
  ZeroMemory(pSearchEle, sizeof(SAstarElement) * nArraySize);
  SAstarElement ardEles[ADT_TOTAL];
  ZeroMemory(ardEles, sizeof(ardEles));
  SAstarElement* pTst;
  pSearchEle[(_pfrom->nY - nTopY) * nArrayWidth + _pfrom->nX - nLeftX] = *_pfrom;
  pSearchEle[(_pfrom->nY - nTopY) * nArrayWidth + _pfrom->nX - nLeftX].pParent = NULL;
  pSearchEle[(_pfrom->nY - nTopY) * nArrayWidth + _pfrom->nX - nLeftX].nGn = 0;
  pSearchEle[(_pfrom->nY - nTopY) * nArrayWidth + _pfrom->nX - nLeftX].nFn = 0;
  pSearchEle[(_pfrom->nY - nTopY) * nArrayWidth + _pfrom->nX - nLeftX].nHn = 0;
  m_CloseTable.push_back(&pSearchEle[(_pfrom->nY - nTopY) * nArrayWidth + _pfrom->nX - nLeftX]);
  //getEle = *m_CloseTable.back();
  SAstarPelements::iterator iter;
  while (1) {
    for (int i = ADT_TOPLEFT; i < ADT_TOTAL; ++i) {
      ardEles[i] = _psrc->GetData((*m_CloseTable.back()).nX + nCalTable[2 * i],
          m_CloseTable.back()->nY + nCalTable[2 * i + 1]);
      if ((*m_CloseTable.back()).nX + nCalTable[2 * i] < nLeftX ||
          (*m_CloseTable.back()).nX + nCalTable[2 * i] >= nLeftX + nArrayWidth ||
          (*m_CloseTable.back()).nY + nCalTable[2 * i + 1] < nTopY ||
          (*m_CloseTable.back()).nY + nCalTable[2 * i + 1] >= nTopY + nArrayHeight) {
        // 不检索在Buffer区域外的点
        ardEles[i].nFn = -1;
        continue;
      }
      for (iter = m_CloseTable.begin();
          iter != m_CloseTable.end();
          ++iter) {
        if ((*iter)->nX == ardEles[i].nX &&
            (*iter)->nY == ardEles[i].nY) {
          // Close表不检测
          break;
        }
      }
      if (iter != m_CloseTable.end()) {
        // 在Close表中 跳出
        continue;
      }
      // 对角线行走判断周围两点是否可以行走
      if (i == 0 ||
          i == 2 ||
          i == 5 ||
          i == 7) {
        SAstarElement getEle1;
        SAstarElement getEle2;
        POINT ptCenter = {m_CloseTable.back()->nX, m_CloseTable.back()->nY};
        switch (i) {
        case 0: {
          getEle1 = _psrc->GetData(ptCenter.x - 1, ptCenter.y);
          getEle2 = _psrc->GetData(ptCenter.x, ptCenter.y - 1);
        }
        break;
        case 2: {
          getEle1 = _psrc->GetData(ptCenter.x, ptCenter.y - 1);
          getEle2 = _psrc->GetData(ptCenter.x + 1, ptCenter.y);
        }
        break;
        case 5: {
          getEle1 = _psrc->GetData(ptCenter.x - 1, ptCenter.y);
          getEle2 = _psrc->GetData(ptCenter.x, ptCenter.y + 1);
        }
        break;
        case 7: {
          getEle1 = _psrc->GetData(ptCenter.x, ptCenter.y + 1);
          getEle2 = _psrc->GetData(ptCenter.x + 1, ptCenter.y);
        }
        break;
        }
        if (!getEle1.bOpen ||
            !getEle2.bOpen) {
          // 该元素不作检测 不加入Open表
          continue;
        }
      }
      if (ardEles[i].nFn == -1 ||
          ardEles[i].bOpen == 0) {
        // 不存在 或者不可通过
        ardEles[i].nFn = -1;
      }
      else {
        // 存在 有两种情况 一种在Open表中 一种不在
        for (iter = m_OpenTable.begin();
            iter != m_OpenTable.end();
            ++iter) {
          if ((*iter)->nX == ardEles[i].nX &&
              (*iter)->nY == ardEles[i].nY) {
            break;
          }
        }
        //ardEles[i].pParent = &pSearchEle[(m_CloseTable.back()->nY - nTopY) * nArrayWidth + m_CloseTable.back()->nX - nLeftX];
        ardEles[i].pParent = m_CloseTable.back();
        // 假设该元素与目的元素相同 则找到
        if (ardEles[i].nX == _pto->nX &&
            ardEles[i].nY == _pto->nY) {
          // 依照parent寻找路径
          _pret->push_back(ardEles[i]);
          SAstarElement* pEle = ardEles[i].pParent;
          while (pEle) {
            _pret->push_back(*pEle);
            pEle = pEle->pParent;
          }
          _pret->reverse();
          delete[] pSearchEle;
          return TRUE;
        }
        if (iter == m_OpenTable.end()) {
          // 没有重复元素 直接添加入Open表
          //ardEles[i].pParent = m_CloseTable.back();
          ardEles[i].nGn = ardEles[i].pParent->nGn + ((i == 1 || i == 3 || i == 4 || i == 6) ? nCostNormal : nCostDiag);
          ardEles[i].nHn = GetElementHn(&ardEles[i], _pto) * 10;
          ardEles[i].nFn = ardEles[i].nGn + ardEles[i].nHn;
          pSearchEle[(ardEles[i].nY - nTopY) * nArrayWidth + ardEles[i].nX - nLeftX] = ardEles[i];
          m_OpenTable.push_back(&pSearchEle[(ardEles[i].nY - nTopY) * nArrayWidth + ardEles[i].nX - nLeftX]);
        }
        else {
          // 有重复元素 判断Gn 若走到重复方块的Gn小于原先方块的Gn 则修改
          int nTestGn = ardEles[i].pParent->nGn + ((i == 1 || i == 3 || i == 4 || i == 6) ? nCostNormal : nCostDiag);
          if (nTestGn < (*iter)->nGn) {
            (*iter)->nGn = nTestGn;
            (*iter)->nFn = (*iter)->nHn + nTestGn;
            //(*iter)->pParent = &pSearchEle[(m_CloseTable.back()->nY - nTopY) * nArrayWidth + m_CloseTable.back()->nX - nLeftX];
            (*iter)->pParent = m_CloseTable.back();
          }
        }
      }
    }
    // 从Open表中找出最小Fn，加入Close表搜索
    if (m_OpenTable.empty()) {
      // Open表搜索完了 没有找到路劲
      delete[] pSearchEle;
      return FALSE;
    }
    else {
      m_OpenTable.sort(SAstarCompGreater());
      pTst = *m_OpenTable.begin();
      m_CloseTable.push_back(*m_OpenTable.begin());
      m_OpenTable.erase(m_OpenTable.begin());
    }
  }
}
// POINT
BOOL Calculate8DirectionNormal(const SAstarDataAdaptor* _psrc, SAstarPoints* _pret, const POINT* _pfrom, const POINT* _pto)
{
  if (_psrc == NULL ||
      _pret == NULL) {
    return FALSE;
  }
  if (_pfrom->x == _pto->x &&
      _pfrom->y == _pto->y) {
    return FALSE;
  }
  static const int nCalTable[] = { -1, -1, 0, -1, 1, -1, -1, 0, 1, 0, -1, 1, 0, 1, 1, 1};
  static const int nCostNormal = 10;
  static const int nCostDiag = 14;
  SAstarPelements m_OpenTable;
  SAstarPelements m_CloseTable;
  int nLeftX = min(_pfrom->x, _pto->x);
  int nTopY = min(_pfrom->y, _pto->y);
  int nArrayWidth = (nLeftX < SEARCH_OFFSET_X ? nLeftX : SEARCH_OFFSET_X) + SEARCH_OFFSET_X + abs(_pfrom->x - _pto->y) + 1;
  int nArrayHeight = (nTopY < SEARCH_OFFSET_Y ? nTopY : SEARCH_OFFSET_Y) + SEARCH_OFFSET_Y + abs(_pfrom->x - _pto->y) + 1;
  int nArraySize = (nArrayWidth * nArrayHeight);
  nLeftX -= SEARCH_OFFSET_X;
  if (nLeftX < 0) {
    nLeftX = 0;
  }
  nTopY -= SEARCH_OFFSET_Y;
  if (nTopY < 0) {
    nTopY = 0;
  }
  SAstarElement* pSearchEle = new SAstarElement[nArraySize];
  ZeroMemory(pSearchEle, sizeof(SAstarElement) * nArraySize);
  SAstarElement ardEles[ADT_TOTAL];
  ZeroMemory(ardEles, sizeof(ardEles));
  SAstarElement getEle;
  SAstarElement* pTst;
  // _pfrom->pParent = NULL;
  // _pfrom->nGn = 0;
  // _pfrom->nHn = 0;
  // _pfrom->nFn = 0;
  // pSearchEle[(_pfrom->nY - nTopY) * nArrayWidth + _pfrom->nX - nLeftX] = *_pfrom;
  pSearchEle[(_pfrom->y - nTopY) * nArrayWidth + _pfrom->x - nLeftX].nX = _pfrom->x;
  pSearchEle[(_pfrom->y - nTopY) * nArrayWidth + _pfrom->x - nLeftX].nY = _pfrom->y;
  pSearchEle[(_pfrom->y - nTopY) * nArrayWidth + _pfrom->x - nLeftX].pParent = NULL;
  pSearchEle[(_pfrom->y - nTopY) * nArrayWidth + _pfrom->x - nLeftX].nGn = 0;
  pSearchEle[(_pfrom->y - nTopY) * nArrayWidth + _pfrom->x - nLeftX].nFn = 0;
  pSearchEle[(_pfrom->y - nTopY) * nArrayWidth + _pfrom->x - nLeftX].nHn = 0;
  m_CloseTable.push_back(&pSearchEle[(_pfrom->y - nTopY) * nArrayWidth + _pfrom->x - nLeftX]);
  getEle = *m_CloseTable.back();
  SAstarPelements::iterator iter;
  while (1) {
    for (int i = ADT_TOPLEFT; i < ADT_TOTAL; ++i) {
      ardEles[i] = _psrc->GetData((*m_CloseTable.back()).nX + nCalTable[2 * i],
          m_CloseTable.back()->nY + nCalTable[2 * i + 1]);
      if ((*m_CloseTable.back()).nX + nCalTable[2 * i] < nLeftX ||
          (*m_CloseTable.back()).nX + nCalTable[2 * i] >= nLeftX + nArrayWidth ||
          (*m_CloseTable.back()).nY + nCalTable[2 * i + 1] < nTopY ||
          (*m_CloseTable.back()).nY + nCalTable[2 * i + 1] >= nTopY + nArrayHeight) {
        // 不检索在Buffer区域外的点
        ardEles[i].nFn = -1;
        continue;
      }
      for (iter = m_CloseTable.begin();
          iter != m_CloseTable.end();
          ++iter) {
        if ((*iter)->nX == ardEles[i].nX &&
            (*iter)->nY == ardEles[i].nY) {
          // Close表不检测
          break;
        }
      }
      if (iter != m_CloseTable.end()) {
        // 在Close表中 跳出
        continue;
      }
      if (ardEles[i].nFn == -1 ||
          ardEles[i].bOpen == 0) {
        // 不存在 或者不可通过
        ardEles[i].nFn = -1;
      }
      else {
        // 存在 有两种情况 一种在Open表中 一种不在
        for (iter = m_OpenTable.begin();
            iter != m_OpenTable.end();
            ++iter) {
          if ((*iter)->nX == ardEles[i].nX &&
              (*iter)->nY == ardEles[i].nY) {
            break;
          }
        }
        //ardEles[i].pParent = &pSearchEle[(m_CloseTable.back()->nY - nTopY) * nArrayWidth + m_CloseTable.back()->nX - nLeftX];
        ardEles[i].pParent = m_CloseTable.back();
        // 假设该元素与目的元素相同 则找到
        if (ardEles[i].nX == _pto->x &&
            ardEles[i].nY == _pto->y) {
          // 依照parent寻找路径
          SAstarElement* pEle = ardEles[i].pParent;
          POINT tPoint = {ardEles[i].nX, ardEles[i].nY};
          _pret->push_back(tPoint);
          while (pEle) {
            tPoint.x = pEle->nX;
            tPoint.y = pEle->nY;
            _pret->push_back(tPoint);
            pEle = pEle->pParent;
          }
          _pret->reverse();
          delete[] pSearchEle;
          return TRUE;
        }
        if (iter == m_OpenTable.end()) {
          // 没有重复元素 直接添加入Open表
          //ardEles[i].pParent = m_CloseTable.back();
          ardEles[i].nGn = ardEles[i].pParent->nGn + ((i == 1 || i == 3 || i == 4 || i == 6) ? nCostNormal : nCostDiag);
          ardEles[i].nHn = GetElementHn(&ardEles[i], _pto) * 10;
          ardEles[i].nFn = ardEles[i].nGn + ardEles[i].nHn;
          pSearchEle[(ardEles[i].nY - nTopY) * nArrayWidth + ardEles[i].nX - nLeftX] = ardEles[i];
          m_OpenTable.push_back(&pSearchEle[(ardEles[i].nY - nTopY) * nArrayWidth + ardEles[i].nX - nLeftX]);
        }
        else {
          // 有重复元素 判断Gn 若走到重复方块的Gn小于原先方块的Gn 则修改
          int nTestGn = ardEles[i].pParent->nGn + ((i == 1 || i == 3 || i == 4 || i == 6) ? nCostNormal : nCostDiag);
          if (nTestGn < (*iter)->nGn) {
            (*iter)->nGn = nTestGn;
            (*iter)->nFn = (*iter)->nHn + nTestGn;
            //(*iter)->pParent = &pSearchEle[(m_CloseTable.back()->nY - nTopY) * nArrayWidth + m_CloseTable.back()->nX - nLeftX];
            (*iter)->pParent = m_CloseTable.back();
          }
        }
      }
    }
    // 从Open表中找出最小Fn，加入Close表搜索
    if (m_OpenTable.empty()) {
      // Open表搜索完了 没有找到路劲
      delete[] pSearchEle;
      return FALSE;
    }
    else {
      m_OpenTable.sort(SAstarCompGreater());
      pTst = *m_OpenTable.begin();
      m_CloseTable.push_back(*m_OpenTable.begin());
      m_OpenTable.erase(m_OpenTable.begin());
    }
  }
}
BOOL Calculate8DirectionNoDiag(const SAstarDataAdaptor* _psrc, SAstarPoints* _pret, const POINT* _pfrom, const POINT* _pto)
{
  if (_psrc == NULL ||
      _pret == NULL) {
    return FALSE;
  }
  if (_pfrom->x == _pto->x &&
      _pfrom->y == _pto->y) {
    return FALSE;
  }
  static const int nCalTable[] = { -1, -1, 0, -1, 1, -1, -1, 0, 1, 0, -1, 1, 0, 1, 1, 1};
  static const int nCostNormal = 10;
  static const int nCostDiag = 14;
  SAstarPelements m_OpenTable;
  SAstarPelements m_CloseTable;
  int nLeftX = min(_pfrom->x, _pto->x);
  int nTopY = min(_pfrom->y, _pto->y);
  int nArrayWidth = (nLeftX < SEARCH_OFFSET_X ? nLeftX : SEARCH_OFFSET_X) + SEARCH_OFFSET_X + abs(_pfrom->x - _pto->x) + 1;
  int nArrayHeight = (nTopY < SEARCH_OFFSET_Y ? nTopY : SEARCH_OFFSET_Y) + SEARCH_OFFSET_Y + abs(_pfrom->y - _pto->y) + 1;
  int nArraySize = (nArrayWidth * nArrayHeight);
  nLeftX -= SEARCH_OFFSET_X;
  if (nLeftX < 0) {
    nLeftX = 0;
  }
  nTopY -= SEARCH_OFFSET_Y;
  if (nTopY < 0) {
    nTopY = 0;
  }
  SAstarElement* pSearchEle = new SAstarElement[nArraySize];
  ZeroMemory(pSearchEle, sizeof(SAstarElement) * nArraySize);
  SAstarElement ardEles[ADT_TOTAL];
  ZeroMemory(ardEles, sizeof(ardEles));
  SAstarElement* pTst;
  pSearchEle[(_pfrom->y - nTopY) * nArrayWidth + _pfrom->x - nLeftX].nX = _pfrom->x;
  pSearchEle[(_pfrom->y - nTopY) * nArrayWidth + _pfrom->x - nLeftX].nY = _pfrom->y;
  pSearchEle[(_pfrom->y - nTopY) * nArrayWidth + _pfrom->x - nLeftX].pParent = NULL;
  pSearchEle[(_pfrom->y - nTopY) * nArrayWidth + _pfrom->x - nLeftX].nGn = 0;
  pSearchEle[(_pfrom->y - nTopY) * nArrayWidth + _pfrom->x - nLeftX].nFn = 0;
  pSearchEle[(_pfrom->y - nTopY) * nArrayWidth + _pfrom->x - nLeftX].nHn = 0;
  m_CloseTable.push_back(&pSearchEle[(_pfrom->y - nTopY) * nArrayWidth + _pfrom->x - nLeftX]);
  //getEle = *m_CloseTable.back();
  SAstarPelements::iterator iter;
  while (1) {
    for (int i = ADT_TOPLEFT; i < ADT_TOTAL; ++i) {
      ardEles[i] = _psrc->GetData((*m_CloseTable.back()).nX + nCalTable[2 * i],
          m_CloseTable.back()->nY + nCalTable[2 * i + 1]);
      if ((*m_CloseTable.back()).nX + nCalTable[2 * i] < nLeftX ||
          (*m_CloseTable.back()).nX + nCalTable[2 * i] >= nLeftX + nArrayWidth ||
          (*m_CloseTable.back()).nY + nCalTable[2 * i + 1] < nTopY ||
          (*m_CloseTable.back()).nY + nCalTable[2 * i + 1] >= nTopY + nArrayHeight) {
        // 不检索在Buffer区域外的点
        ardEles[i].nFn = -1;
        continue;
      }
      for (iter = m_CloseTable.begin();
          iter != m_CloseTable.end();
          ++iter) {
        if ((*iter)->nX == ardEles[i].nX &&
            (*iter)->nY == ardEles[i].nY) {
          // Close表不检测
          break;
        }
      }
      if (iter != m_CloseTable.end()) {
        // 在Close表中 跳出
        continue;
      }
      // 对角线行走判断周围两点是否可以行走
      if (i == 0 || i == 2 || i == 5 || i == 7) {
        SAstarElement getEle1;
        SAstarElement getEle2;
        POINT ptCenter = {m_CloseTable.back()->nX, m_CloseTable.back()->nY};
        switch (i) {
        case 0: {
          getEle1 = _psrc->GetData(ptCenter.x - 1, ptCenter.y);
          getEle2 = _psrc->GetData(ptCenter.x, ptCenter.y - 1);
        }
        break;
        case 2: {
          getEle1 = _psrc->GetData(ptCenter.x, ptCenter.y - 1);
          getEle2 = _psrc->GetData(ptCenter.x + 1, ptCenter.y);
        }
        break;
        case 5: {
          getEle1 = _psrc->GetData(ptCenter.x - 1, ptCenter.y);
          getEle2 = _psrc->GetData(ptCenter.x, ptCenter.y + 1);
        }
        break;
        case 7: {
          getEle1 = _psrc->GetData(ptCenter.x, ptCenter.y + 1);
          getEle2 = _psrc->GetData(ptCenter.x + 1, ptCenter.y);
        }
        break;
        }
        if (!getEle1.bOpen ||
            !getEle2.bOpen) {
          // 该元素不作检测 不加入Open表
          continue;
        }
      }
      if (ardEles[i].nFn == -1 ||
          ardEles[i].bOpen == 0) {
        // 不存在 或者不可通过
        ardEles[i].nFn = -1;
      }
      else {
        // 存在 有两种情况 一种在Open表中 一种不在
        for (iter = m_OpenTable.begin();
            iter != m_OpenTable.end();
            ++iter) {
          if ((*iter)->nX == ardEles[i].nX &&
              (*iter)->nY == ardEles[i].nY) {
            break;
          }
        }
        //ardEles[i].pParent = &pSearchEle[(m_CloseTable.back()->nY - nTopY) * nArrayWidth + m_CloseTable.back()->nX - nLeftX];
        ardEles[i].pParent = m_CloseTable.back();
        // 假设该元素与目的元素相同 则找到
        if (ardEles[i].nX == _pto->x &&
            ardEles[i].nY == _pto->y) {
          // 依照parent寻找路径
          //_pret->push_back(ardEles[i]);
          POINT tPoint = {ardEles[i].nX, ardEles[i].nY};
          _pret->push_back(tPoint);
          SAstarElement* pEle = ardEles[i].pParent;
          while (pEle) {
            tPoint.x = pEle->nX;
            tPoint.y = pEle->nY;
            _pret->push_back(tPoint);
            pEle = pEle->pParent;
          }
          _pret->reverse();
          delete[] pSearchEle;
          return TRUE;
        }
        if (iter == m_OpenTable.end()) {
          // 没有重复元素 直接添加入Open表
          //ardEles[i].pParent = m_CloseTable.back();
          ardEles[i].nGn = ardEles[i].pParent->nGn + ((i == 1 || i == 3 || i == 4 || i == 6) ? nCostNormal : nCostDiag);
          ardEles[i].nHn = GetElementHn(&ardEles[i], _pto) * 10;
          ardEles[i].nFn = ardEles[i].nGn + ardEles[i].nHn;
          pSearchEle[(ardEles[i].nY - nTopY) * nArrayWidth + ardEles[i].nX - nLeftX] = ardEles[i];
          m_OpenTable.push_back(&pSearchEle[(ardEles[i].nY - nTopY) * nArrayWidth + ardEles[i].nX - nLeftX]);
        }
        else {
          // 有重复元素 判断Gn 若走到重复方块的Gn小于原先方块的Gn 则修改
          int nTestGn = ardEles[i].pParent->nGn + ((i == 1 || i == 3 || i == 4 || i == 6) ? nCostNormal : nCostDiag);
          if (nTestGn < (*iter)->nGn) {
            (*iter)->nGn = nTestGn;
            (*iter)->nFn = (*iter)->nHn + nTestGn;
            //(*iter)->pParent = &pSearchEle[(m_CloseTable.back()->nY - nTopY) * nArrayWidth + m_CloseTable.back()->nX - nLeftX];
            (*iter)->pParent = m_CloseTable.back();
          }
        }
      }
    }
    // 从Open表中找出最小Fn，加入Close表搜索
    if (m_OpenTable.empty()) {
      // Open表搜索完了 没有找到路径
      delete[] pSearchEle;
      return FALSE;
    }
    else {
      m_OpenTable.sort(SAstarCompGreater());
      pTst = *m_OpenTable.begin();
      m_CloseTable.push_back(*m_OpenTable.begin());
      m_OpenTable.erase(m_OpenTable.begin());
    }
  }
}
// Helper
int GetElementHn(const SAstarElement* _pcur, const SAstarElement* _pdest)
{
  int nHn = 0;
  nHn = abs((int)_pdest->nX - (int)_pcur->nX) + abs((int)_pdest->nY - (int)_pcur->nY);
  return nHn;
}
int GetElementHn(const SAstarElement* _pcur, const POINT* _pdest)
{
  int nHn = 0;
  nHn = abs((int)_pdest->x - (int)_pcur->nX) + abs((int)_pdest->y - (int)_pcur->nY);
  return nHn;
}
bool operator== (const SAstarElement& ele1, const SAstarElement& ele2)
{
  if (ele1.nX == ele2.nX &&
      ele2.nY == ele1.nY) {
    return true;
  }
  return false;
}

