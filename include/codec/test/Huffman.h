
#include <stdlib.h>
#include <string.h>
#include "../../include/sort.h"

#define TRUE 1
#define FALSE 0

int abab2aabb(void* a0, int len, void* b0) {
  unsigned char* a=(unsigned char*)a0;
  unsigned char* b=(unsigned char*)b0;
  unsigned char* c=b+len/2;
  int i=0, j;
  for (i=0, j=0; i<len; i+=2, ++j) {
    b[j]=a[i];
    c[j]=a[i+1];
  }
  if (len&1) {
    c[j]=a[i+1];
  }
  return 0;
}
int aabb2abab(void* b0, int len, void* a0) {
  unsigned char* a=(unsigned char*)a0;
  unsigned char* b=(unsigned char*)b0;
  unsigned char* c=b+len/2;
  int i=0, j;
  for (i=0, j=0; i<len; i+=2, ++j) {
    a[i]=b[j];
    a[i+1]=c[j];
  }
  if (len&1) {
    a[i+1]=c[j];
  }
  return 0;
}
typedef struct CHuffmanNode {
  //CHuffmanNode() { memset(this, 0, sizeof(CHuffmanNode)); }
  int nFrequency; // must be first for unsigned char align
  unsigned int byAscii;
  unsigned int dwCode;
  int nCodeLength;
  struct CHuffmanNode *pParent, *pLeft, *pRight;
}
CHuffmanNode;

int frequencyCompare( const void *elem1, const void *elem2 ) {
  CHuffmanNode * pNodes1 = ( CHuffmanNode* ) elem1;
  CHuffmanNode * pNodes2 = ( CHuffmanNode* ) elem2;
  if ( pNodes1 ->nFrequency == pNodes2 ->nFrequency )
    return 0;
  return pNodes1 ->nFrequency < pNodes2 ->nFrequency ? 1 : -1;
}

#define FREQUENCYCOMPARE(elem1, elem2)  ((elem1).nFrequency > (elem2).nFrequency)

#define ASCIICOMPARE(elem1, elem2) ((elem1).byAscii < (elem2).byAscii)

int asciiCompare( const void *elem1, const void *elem2 ) {
  return ( ( CHuffmanNode* ) elem1 ) ->byAscii > ( ( CHuffmanNode* ) elem2 ) ->byAscii ? 1 : -1;
}

CHuffmanNode* PopNode( CHuffmanNode *pNodes[], int nIndex, int bRight ) {
  CHuffmanNode * pNode = pNodes[ nIndex ];
  pNode->dwCode = bRight;
  pNode->nCodeLength = 1;
  return pNode;
}
#define POPNODE(PNODES, NINDEX, BRIGHT, OUTNODE) {CHuffmanNode * _PNODE = PNODES[ NINDEX ]; _PNODE->dwCode = BRIGHT,_PNODE->nCodeLength = 1,OUTNODE = _PNODE; }
//#define POPNODE(PNODES, NINDEX, BRIGHT, OUTNODE) OUTNODE = PopNode(PNODES, NINDEX, BRIGHT)

void SetNodeCode( CHuffmanNode* pNode ) {
  CHuffmanNode * pParent = pNode->pParent;
  while ( pParent && pParent->nCodeLength ) {
    pNode->dwCode <<= 1;
    pNode->dwCode |= pParent->dwCode;
    pNode->nCodeLength++;
    pParent = pParent->pParent;
  }
}

#define HUFFMANNODELEN 512

int GetHuffmanTree( CHuffmanNode* nodes, int bSetCodes ) {
  CHuffmanNode * pNodes[ 256 ], *pNode;
  // add used ascii to Huffman queue
  int i, j, nCount, nNodeCount = 0;
  int nParentNode, nBackNode;
  for ( nCount = 0; nCount < 256 && nodes[ nCount ].nFrequency; nCount++ ) {
    pNodes[ nNodeCount++ ] = &nodes[ nCount ];
  }
  nParentNode = nNodeCount, nBackNode = nNodeCount - 1;
  while ( nBackNode > 0 ) {
    //ASSERT(nParentNode<HUFFMANNODELEN);
    // parent node
    pNode = &nodes[ nParentNode++ ];
    // pop first child
    //pNode->pLeft = PopNode( pNodes, nBackNode--, FALSE );
    POPNODE( pNodes, nBackNode--, FALSE, pNode->pLeft );
    // pop second child
    //pNode->pRight = PopNode( pNodes, nBackNode--, TRUE );
    POPNODE( pNodes, nBackNode--, TRUE, pNode->pRight );
    // adjust parent of the two poped nodes
    pNode->pLeft->pParent = pNode->pRight->pParent = pNode;
    // adjust parent frequency
    pNode->nFrequency = pNode->pLeft->nFrequency + pNode->pRight->nFrequency;
    // insert parent node depending on its frequency
    for ( i = nBackNode; i >= 0; i-- ) {
      if ( pNodes[ i ] ->nFrequency >= pNode->nFrequency ) {
        break;
      }
    }
    //memmove( pNodes + i + 2, pNodes + i + 1, ( nBackNode - i ) * sizeof( int ) );
    for (j=( nBackNode - i ); j-->0; ) {
      pNodes[i + 2 + j] = pNodes[i + 1 + j];
    }
    pNodes[ i + 1 ] = pNode;
    nBackNode++;
  }
  // set tree leaves nodes code
  if ( bSetCodes )
    for ( nCount = 0; nCount < nNodeCount; nCount++ )
      SetNodeCode( &nodes[ nCount ] );
      
  return nNodeCount;
}

CHuffmanNode nodes[ HUFFMANNODELEN ];
int Huffman_Encode( const void *pSrc0, int nSrcLen, void *pDes0 ) {
  const unsigned char * pSrc = ( const unsigned char * ) pSrc0;
  unsigned char *pDes = ( unsigned char * ) pDes0;
  int nDesLen, nCount, nNodeCount, nNodeSize, nDesIndex;
  unsigned char *pDesPtr;
  if ( NULL == pDes ) {
    return nSrcLen;
  }
  memset( nodes, 0, HUFFMANNODELEN * sizeof( CHuffmanNode ) );
  // initialize nodes ascii
  for ( nCount = 0; nCount < 256; nCount++ )
    nodes[ nCount ].byAscii = nCount;
  // get ascii frequencies
  for ( nCount = 0; nCount < nSrcLen; nCount++ )
    nodes[ pSrc[ nCount ] ].nFrequency++;
  // sort ascii chars depending on frequency
  //qsort(nodes, 256, sizeof(CHuffmanNode), frequencyCompare);
  QSORT( nodes, nodes + 256, FREQUENCYCOMPARE, CHuffmanNode );
  // construct Huffman tree
  nNodeCount = GetHuffmanTree( nodes, TRUE );
  // construct compressed buffer
  nNodeSize = sizeof( unsigned int ) + sizeof( unsigned char );
  //nDesLen = nSrcLen+nNodeCount*nNodeSize;
  //pDes = (unsigned char*)malloc(nDesLen);
  pDesPtr = pDes;
  memset( pDesPtr, 0, nSrcLen );
  // save source buffer length at the first unsigned int
  *( unsigned int* ) pDesPtr = nSrcLen;
  pDesPtr += sizeof( unsigned int );
  // save Huffman tree leaves count-1 (as it may be 256)
  *pDesPtr = nNodeCount - 1;
  pDesPtr += sizeof( unsigned char );
  // save Huffman tree used leaves nodes
  for ( nCount = 0; nCount < nNodeCount; nCount++ ) {
    // the array sorted on frequency so used nodes come first
    memcpy( pDesPtr, &nodes[ nCount ], nNodeSize );
    pDesPtr += nNodeSize;
  }
  // sort nodes depending on ascii to can index nodes with its ascii value
  //qsort(nodes, 256, sizeof(CHuffmanNode), asciiCompare);
  QSORT( nodes, nodes + 256, ASCIICOMPARE, CHuffmanNode );
  
  nDesIndex = 0;
  // loop to write codes
  for ( nCount = 0; nCount < nSrcLen; nCount++ ) {
    *( unsigned int* ) ( pDesPtr + ( nDesIndex >> 3 ) ) |= nodes[ pSrc[ nCount ] ].dwCode << ( nDesIndex & 7 );
    nDesIndex += nodes[ pSrc[ nCount ] ].nCodeLength;
  }
  // update destination length
  nDesLen = ( pDesPtr - pDes ) + ( nDesIndex + 7 ) / 8;
  //pDes = (unsigned char*)realloc(pDes, nDesLen);
  
  return nDesLen;
}

int Huffman_Decode( const void *pSrc0, int nSrcLen, void *pDes0 ) {
  const unsigned char * pSrc = ( const unsigned char * ) pSrc0;
  unsigned char *pDes = ( unsigned char * ) pDes0;
  int nDesLen, nCount, nNodeSize, nNodeCount, nSrcIndex, nDesIndex;
  unsigned int nCode;
  CHuffmanNode *pRoot, *pNode;
  //CHuffmanNode nodes[ HUFFMANNODELEN ];
  memset( nodes, 0, HUFFMANNODELEN * sizeof( CHuffmanNode ) );
  // copy destination final length
  nDesLen = *( unsigned int* ) pSrc;
  // allocate buffer for decompressed buffer
  //pDes = (unsigned char*)malloc(nDesLen+1);
  nNodeCount = *( pSrc + sizeof( unsigned int ) ) + 1;
  // initialize Huffman nodes with frequency and ascii
  nNodeSize = sizeof( unsigned int ) + sizeof( unsigned char ), nSrcIndex = nNodeSize;
  for ( nCount = 0; nCount < nNodeCount; nCount++ ) {
    memcpy( &nodes[ nCount ], pSrc + nSrcIndex, nNodeSize );
    nSrcIndex += nNodeSize;
  }
  // construct Huffman tree
  GetHuffmanTree( nodes, FALSE );
  // get Huffman tree root
  pRoot = &nodes[ 0 ];
  while ( pRoot->pParent )
    pRoot = pRoot->pParent;
    
  nDesIndex = 0;
  nSrcIndex <<= 3; // convert from bits to unsigned chars
  while ( nDesIndex < nDesLen ) {
    nCode = ( *( unsigned int* ) ( pSrc + ( nSrcIndex >> 3 ) ) ) >> ( nSrcIndex & 7 );
    pNode = pRoot;
    while ( pNode->pLeft ) {
      // if node has pLeft then it must has pRight
      // node not leaf
      pNode = ( nCode & 1 ) ? pNode->pRight : pNode->pLeft;
      nCode >>= 1;
      nSrcIndex++;
    }
    pDes[ nDesIndex++ ] = pNode->byAscii;
  }
  
  return nDesLen;
}

#undef HUFFMANNODELEN
