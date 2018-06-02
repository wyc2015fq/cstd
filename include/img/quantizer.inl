
#ifndef _QUANTIZER_INL_
#define _QUANTIZER_INL_

// 八叉树颜色量化
typedef struct OCtreeNode {
  BOOL bIsLeaf;             // TRUE if node has no children
  UINT nPixelCount;         // Number of pixels represented by this leaf
  UINT nRSum;               // Sum of red components
  UINT nGSum;               // Sum of green components
  UINT nBSum;               // Sum of blue components
  UINT nASum;               // Sum of alpha components
  int index;
  struct OCtreeNode* pChild[8];    // Pointers to child nodes
  struct OCtreeNode* pNext;        // Pointer to next reducible node
} OCtreeNode;

static int OCtree_GetPaletteColors(OCtreeNode* pTree,  PaletteEntry* prgb, UINT* pIndex, UINT* pSum)
{
  int i, index;
  if (pTree) {
    if (pTree->bIsLeaf)  {
      index = *pIndex;
      prgb[index].r = (BYTE)((pTree->nRSum) / (pTree->nPixelCount));
      prgb[index].g = (BYTE)((pTree->nGSum) / (pTree->nPixelCount));
      prgb[index].b = (BYTE)((pTree->nBSum) / (pTree->nPixelCount));
      prgb[index].a = (BYTE)((pTree->nASum) / (pTree->nPixelCount));
      
      if (pSum) {
        pSum[index] = pTree->nPixelCount;
      }
      pTree->index = index;
      (*pIndex)++;
    }
    else {
      for (i = 0; i < 8; i++) {
        if (pTree->pChild[i] !=  NULL) {
          OCtree_GetPaletteColors(pTree->pChild[i],  prgb, pIndex, pSum);
        }
      }
    }
  }
  return 0;
}

static void OCtree_SetColorTable(OCtreeNode* m_pTree, int m_nLeafCount, int nMaxColors, PaletteEntry* prgb)
{
  UINT nIndex = 0;
  
  if (nMaxColors < 16) {
    UINT nSum[16];
    PaletteEntry tmppal[16];
    OCtree_GetPaletteColors(m_pTree, tmppal, &nIndex, nSum);
    
    if (m_nLeafCount > nMaxColors) {
      int j, k, nr, ng, nb, na, ns, a, b;
      
      for (j = 0; j < nMaxColors; j++) {
        a = (j * m_nLeafCount) / nMaxColors;
        b = ((j + 1) * m_nLeafCount) / nMaxColors;
        nr = ng = nb = na = ns = 0;
        
        for (k = a; k < b; k++) {
          nr += tmppal[k].r * nSum[k];
          ng += tmppal[k].g * nSum[k];
          nb += tmppal[k].b * nSum[k];
          na += tmppal[k].a * nSum[k];
          ns += nSum[k];
        }
        
        prgb[j].r = nr / ns;
        prgb[j].g = ng / ns;
        prgb[j].b = nb / ns;
        prgb[j].a = na / ns;
      }
    }
    else {
      memcpy(prgb, tmppal, m_nLeafCount * sizeof(PaletteEntry));
    }
  }
  else {
    OCtree_GetPaletteColors(m_pTree, prgb, &nIndex, 0);
  }
}

static void* OCtree_CreateNode(UINT nLevel, UINT  nColorBits, UINT* pLeafCount,
                        OCtreeNode** pReducibleNodes)
{
  OCtreeNode* pNode = (OCtreeNode*)pmalloc(sizeof(OCtreeNode));
  memset(pNode, 0, sizeof(OCtreeNode));
  
  if (pNode == NULL) {
    return NULL;
  }
  
  pNode->bIsLeaf = (nLevel ==  nColorBits) ? TRUE : FALSE;
  
  if (pNode->bIsLeaf) {
    (*pLeafCount)++;
  }
  else {
    pNode->pNext = pReducibleNodes[nLevel];
    pReducibleNodes[nLevel] = pNode;
  }
  
  return pNode;
}

static BYTE mask[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

static OCtreeNode* OCtree_FindColor(OCtreeNode* pNode, BYTE r, BYTE g, BYTE b, BYTE a) {
  int nLevel=0;
  // Update color information if it's a leaf node.
  for (; !pNode->bIsLeaf; ++nLevel)  {
    int shift = 7 - nLevel;
    int nIndex = (((r & mask[nLevel]) >> shift) << 2) | (((g & mask[nLevel]) >> shift) << 1) | ((b & mask[nLevel]) >> shift);
    if(NULL==pNode->pChild[nIndex]) {
      int k;
      for (k=0; k<8; ++k) {
        if (pNode->pChild[k]) {
          nIndex = k;
          break;
        }
      }
    }
    pNode = pNode->pChild[nIndex];
    ASSERT(nLevel<8);
  }
  return pNode;
}
static OCtreeNode* OCtree_AddColor(OCtreeNode** ppNode, BYTE r, BYTE g, BYTE b, BYTE a, UINT nColorBits, UINT nLevel, UINT*  pLeafCount, OCtreeNode** pReducibleNodes)
{
  OCtreeNode* pNode = NULL;

  // If the node doesn't exist, create it.
  if (*ppNode  == NULL) {
    *ppNode = (OCtreeNode*)OCtree_CreateNode(nLevel, nColorBits, pLeafCount, pReducibleNodes);
  }
  
  // Update color information if it's a leaf node.
  if ((*ppNode)->bIsLeaf)  {
    (*ppNode)->nPixelCount++;
    (*ppNode)->nRSum += r;
    (*ppNode)->nGSum += g;
    (*ppNode)->nBSum += b;
    (*ppNode)->nASum += a;
    pNode = *ppNode;
  }
  else {    // Recurse a level deeper if the node is not a leaf.
    int shift = 7 - nLevel;
    int nIndex = (((r & mask[nLevel]) >> shift) << 2) | (((g & mask[nLevel]) >> shift) << 1) | ((b & mask[nLevel]) >> shift);
    pNode = OCtree_AddColor(&((*ppNode)->pChild[nIndex]),  r, g, b, a, nColorBits, nLevel + 1, pLeafCount, pReducibleNodes);
  }
  return pNode;
}

static void OCtree_DeleteTree(OCtreeNode** ppNode)
{
  int i;
  for (i = 0; i < 8; i++) {
    if ((*ppNode)->pChild[i] !=  NULL) {
      OCtree_DeleteTree(&((*ppNode)->pChild[i]));
    }
  }
  
  pfree(*ppNode);
  *ppNode = NULL;
}

static OCtreeNode* OCtree_ReduceTree(UINT nColorBits, UINT* pLeafCount, OCtreeNode** pReducibleNodes)
{
  int i;
  UINT nRSum = 0;
  UINT nGSum = 0;
  UINT nBSum = 0;
  UINT nASum = 0;
  UINT nChildren = 0;
  OCtreeNode* pNode;

  // Find the deepest level containing at least one reducible node.
  for (i = nColorBits - 1; (i > 0) && (pReducibleNodes[i] == NULL); i--);

  // Reduce the node most recently added to the list at level i.
  pNode = pReducibleNodes[i];
  pReducibleNodes[i] = pNode->pNext;


  for (i = 0; i < 8; i++) {
    if (pNode->pChild[i] !=  NULL) {
      nRSum += pNode->pChild[i]->nRSum;
      nGSum += pNode->pChild[i]->nGSum;
      nBSum += pNode->pChild[i]->nBSum;
      nASum += pNode->pChild[i]->nASum;
      pNode->nPixelCount += pNode->pChild[i]->nPixelCount;
      //pfree(pNode->pChild[i]);
      OCtree_DeleteTree(&pNode->pChild[i]);
      pNode->pChild[i] = NULL;
      nChildren++;
    }
  }

  pNode->bIsLeaf = TRUE;
  pNode->nRSum = nRSum;
  pNode->nGSum = nGSum;
  pNode->nBSum = nBSum;
  pNode->nASum = nASum;
  *pLeafCount -= (nChildren -  1);
  return pNode;
}

static BYTE OCtree_GetPixelIndex(long x, long y, int nbit, long effwdt, BYTE* pimage)
{
  if (nbit == 8) {
    return pimage[y * effwdt + x];
  }
  else {
    BYTE pos;
    BYTE iDst = pimage[y * effwdt + (x * nbit >> 3)];

    if (nbit == 4) {
      pos = (BYTE)(4 * (1 - x % 2));
      iDst &= (0x0F << pos);
      return(iDst >> pos);
    }
    else if (nbit == 1) {
      pos = (BYTE)(7 - x % 8);
      iDst &= (0x01 << pos);
      return(iDst >> pos);
    }
  }

  return 0;
}

static BOOL img_quantizer(UINT nMaxColors, int h, int w, const BYTE* A, int al, int ai, uchar* B, int bl, PaletteEntry* prgb)
{
  int i, j;
  UINT m_nColorBits = (nMaxColors > 16 ? 7 : 8);
  OCtreeNode* m_pTree;
  UINT m_nLeafCount, Index=0, Sum=0;
  OCtreeNode* m_pReducibleNodes[9];
  OCtreeNode* pNode;
  int* eB = (int*)pmalloc((w+2)*4*sizeof(int));
  memset(eB, 0, (w+2)*4*sizeof(int));

  m_pTree = NULL;
  m_nLeafCount = 0;
  
  for (i = 0; i <= (int) m_nColorBits; i++) {
    m_pReducibleNodes[i] = NULL;
  }
  
  for (i = 0; i < h;  i++) {
    const BYTE* A1 = A + i * al;
    uchar* B1 = B + i * bl;
    
    for (j = 0; j < w; j++) {
      const BYTE* A2 = A1 + j * ai;
      pNode = OCtree_AddColor(&m_pTree,  A2[2], A2[1], A2[0], 0, m_nColorBits, 0, &m_nLeafCount, m_pReducibleNodes);
      
      while (m_nLeafCount  > nMaxColors) {
        pNode = OCtree_ReduceTree(m_nColorBits, &m_nLeafCount, m_pReducibleNodes);
      }
      ASSERT(pNode->bIsLeaf);
      B1[j] = m_nLeafCount-1;
    }
  }
  
  OCtree_GetPaletteColors(m_pTree, prgb, &Index, 0);
  
  if (0) {
    for (i = 0; i < h;  i++) {
      const BYTE* A1 = A + i * al;
      uchar* B1 = B + i * bl;
      
      for (j = 0; j < w; j++) {
        const BYTE* A2 = A1 + j * ai;
        pNode = OCtree_FindColor(m_pTree, A2[2], A2[1], A2[0], 0);
        ASSERT(pNode->bIsLeaf);
        B1[j] = pNode->index;
      }
    }
  } else {
    // 抖动
    // X7
    //351
    for (i = 0; i < h;  i++) {
      const BYTE* A1 = A + i * al;
      uchar* B1 = B + i * bl;
      int k, xx[] = {3, 5, 1};
      int eR[4] = {0};
      int ss=50;
      
      for (j = 0; j < w; j++) {
        const BYTE* A2 = A1 + j * ai;
        int* eB1 = eB + (j+1)*4;
        eR[2] = A2[2]+eR[2]+eB1[2];
        eR[1] = A2[1]+eR[1]+eB1[2];
        eR[0] = A2[0]+eR[0]+eB1[0];
        eR[2] = BOUND(eR[2], 0, 255);
        eR[1] = BOUND(eR[1], 0, 255);
        eR[0] = BOUND(eR[0], 0, 255);
        //ASSERT(abs());
        pNode = OCtree_FindColor(m_pTree, eR[2], eR[1], eR[0], 0);
        ASSERT(pNode->bIsLeaf);
        eR[2] = A2[2] - prgb[pNode->index].r;
        eR[1] = A2[1] - prgb[pNode->index].g;
        eR[0] = A2[0] - prgb[pNode->index].b;
        //if (abs(eR[2])>50) {          printf("%d\n", eR[2]);        }
        if (1) {
          for (k=0; k<3; ++k) {
            eB1 = eB + (j+k)*4;
            eB1[2] = eR[2]*xx[k]/16;
            eB1[1] = eR[1]*xx[k]/16;
            eB1[0] = eR[0]*xx[k]/16;
            eB1[2] = BOUND(eB1[2], -ss, ss);
            eB1[1] = BOUND(eB1[1], -ss, ss);
            eB1[0] = BOUND(eB1[0], -ss, ss);
          }
          eR[2] = eR[2]*7/16;
          eR[1] = eR[1]*7/16;
          eR[0] = eR[0]*7/16;
          eR[2] = BOUND(eR[2], -ss, ss);
          eR[1] = BOUND(eR[1], -ss, ss);
          eR[0] = BOUND(eR[0], -ss, ss);
        }
        B1[j] = pNode->index;
      }
    }
  }
  if (m_pTree  != NULL) {
    OCtree_DeleteTree(&m_pTree);
  }
  
  pfree(eB);
  return TRUE;
}

int index2img(int h, int w, const uchar* B, int bl, uchar* A, int al, int ai, const PaletteEntry* ppal) {
  int i, j;
  for (i=0; i<h; ++i) {
    const uchar* B1 = B + i*bl;
    uchar* A1 = A + i*al;
    for (j=0; j<w; ++j) {
      memcpy(A1 + j*ai, ppal+B1[j], ai);
    }
  }
  return 0;
}

int index2im(const img_t* im, const PaletteEntry* ppal, img_t* im1, int cn) {
  img_t im2[1] = {0};
  if (im==im1) {
    imclone2(im, im2);
    im = im2;
  }
  imsetsize(im1, im->h, im->w, cn, im->f);
  index2img(im->h*im->f, im->w, im->tt.data, im->s, im1->tt.data, im1->s, im1->c, ppal);
  imfree(im2);
  return 0;
}

int index2im2(const img_t* im, const PaletteEntry* ppal, int f, img_t* im1, int cn) {
  img_t im2[1] = {0};
  if (im->f>0) {
    if (im==im1) {
      imclone2(im, im2);
      im = im2;
    }
    f %= im->f;
    imsetsize(im1, im->h, im->w, cn, 1);
    index2img(im->h, im->w, im->tt.data+im->h*im->s*f, im->s, im1->tt.data, im1->s, im1->c, ppal);
    imfree(im2);
  }
  return 0;
}

//#include "debug.h"
static BOOL imquantizer(UINT nMaxColors, const img_t* im, img_t* im1, PaletteEntry* ppal) {
  img_t im2[1] = {0};
  if (im==im1) {
    imclone2(im, im2);
    im = im2;
  }
  imsetsize(im1, im->h, im->w, 1, im->f);
  // MEM_CHECK_BEGIN();
  
  //{utime_start(_start_time);
  img_quantizer(nMaxColors, im->h*im->f, im->w, im->tt.data, im->s, im->c, im1->tt.data, im1->s, ppal);
//printf("%f\n", utime_elapsed(_start_time));}
  // MEM_CHECK_END();
  imfree(im2);
  return 0;
}

#if 0

int test_quantizer() {
  img_t im[3] = {0};
  img_t* im1 = im+1;
  img_t* im2 = im+2;
  PaletteEntry pal[256] = {0};
  _chdir("E:/pub/pic/color");
  imread("mm.jpg", 3, 1, im);
  imsetsize(im1, im->h, im->w, 1, 1);
  imsetsize(im2, im->h, im->w, 3, 1);
  imquantizer(255, im, im1, pal);
  index2im(im1, pal, im2, 4);
  imshow(im);
  imshow(im2);
  cvWaitKey(-1);
  //quantizer_image();
  imfrees(im, 3);
  return 0;
}

#endif

#endif // _QUANTIZER_INL_
