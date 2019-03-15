#include "Quantize.h" 
 
OctreeQuantize::OctreeQuantize() 
{ 
	m_pOctantRoot	= new SOctant; 
	m_pPal			= NULL; 
	m_pbyIndexedImg = NULL; 
	m_iPalIndex		= 0; 
	m_iNumColors	= 0; 
} 
OctreeQuantize::~OctreeQuantize() 
{ 
	delete m_pOctantRoot; 
} 
 
// the pitch is presupposed to be even. 
/*-------------------------?????????---------------------------*/ 
void OctreeQuantize::BuildOctree(BYTE *pwImgBuf, int width, int height, int pitch)//WORD *pwImgBuf?????BYTE *pwImgBuf 
{ 
	int index = 0; 
	for( int row=0; row&lt;height; row++ ) 
	{ 
		int size = row * pitch; 
		/////////pwImgBuf = &pwImgBuf[row * pitch]; 
		for( int col=0; col&lt;width; col++ ) 
		{ 
			int red = pwImgBuf[size + col*3]; 
			int green = pwImgBuf[size + col*3 + 1]; 
			int blue = pwImgBuf[size + col*3 + 2]; 
			//int red = pwImgBuf[index] >> 11; 
			//int green = (pwImgBuf[index] & 0x7e0 )>>6; 
			//int blue = pwImgBuf[index] & 0x1f; 
			index ++; 
 
			SOctant *pOctant = m_pOctantRoot; 
			for( int i=0; i&lt;5; i++ ) //??5?????8 
			{ 
				int indexChildren = ( ((red>>(7-i))&1) &lt;&lt;2 ) |  
									( ((green>>(7-i))&1) &lt;&lt;1 ) |  
									((blue>>(7-i))&1);//??4?????7 
				 
				if( !pOctant->m_pChildren[indexChildren] ) 
				{ 
					SOctant *pChild = new SOctant; 
					pOctant->m_pChildren[indexChildren] = pChild; 
					pChild->m_pParent = pOctant; 
				} 
				 
				pOctant = pOctant->m_pChildren[indexChildren]; 
 
			} 
 
			if( !pOctant->m_iRefCount )  //?????????μ???????????????1 
				m_iNumColors++; 
			 
			pOctant->m_iRefCount++; 
			pOctant->m_iRed += red; 
			pOctant->m_iGreen += green; 
			pOctant->m_iBlue += blue; 
		} 
		index += (pitch>>1)-width; 
	} 
} 
 
int cmp( const void *pEle1, const void *pEle2 ) 
{ 
	if( (*(SOctant**)pEle1)->m_iRefCount &lt; (*(SOctant**)pEle2)->m_iRefCount ) 
		return 1; 
	if( (*(SOctant**)pEle1)->m_iRefCount == (*(SOctant**)pEle2)->m_iRefCount ) 
		return 0; 
	else return -1; 
} 
 
void OctreeQuantize::ReduceTo256()    //??????????????????256????? 
{ 
	if( m_iNumColors &lt; 256 ) 
		return; 
 
	BuildRev2ndLevelVector(m_pOctantRoot); 
	// make it in descending order. 
	qsort( &m_vRev2ndLevelOctants[0], m_vRev2ndLevelOctants.size(),sizeof( SOctant*), cmp ); 
	 
	while( true ) 
	{ 
		SOctant *pOctant = m_vRev2ndLevelOctants.back(); 
		for( int i=0; i&lt;8; i++ ) 
		{ 
			if( pOctant->m_pChildren[i] ) 
			{ 
				m_iNumColors --; 
				SAFE_DELETE( pOctant->m_pChildren[i] );			 
			} 
		} 
		 
		m_iNumColors ++; 
		if( m_iNumColors &lt; 256 )  
			break; 
 
		m_vRev2ndLevelOctants.pop_back(); 
 
		if( pOctant->m_pParent->IsRev2ndLevel() )  
		{ 
			SOctant *pParent = pOctant->m_pParent; 
			for( int i=m_vRev2ndLevelOctants.size()-1; i>=0; i--) 
				if( m_vRev2ndLevelOctants[i]->m_iRefCount >= pParent->m_iRefCount ) 
				{ 
					pParent->SumChildren(); 
					m_vRev2ndLevelOctants.insert(  
						m_vRev2ndLevelOctants.begin()+i+1, pParent ); 
					break; 
				} 
		} 
	} 
} 
 
/*----------------------????????????????????????-----------------------------*/ 
// build the internal vector( recording reverse 2nd-level octant ) 
void OctreeQuantize::BuildRev2ndLevelVector(SOctant *pOctant) 
{ 
	if( pOctant->IsRev2ndLevel() ) 
	{ 
		pOctant->SumChildren(); 
		m_vRev2ndLevelOctants.push_back( pOctant ); 
	} 
	else 
		for( int i=0; i&lt;8; i++ ) 
			if( pOctant->m_pChildren[i] ) 
				BuildRev2ndLevelVector( pOctant->m_pChildren[i] ); 
} 
 
void OctreeQuantize::BuildPal() 
{ 
	m_pPal = new RGB[256]; 
	memset( m_pPal, 0, 768 );//??512?????1024 
	m_iPalIndex = 0; 
	BuildPalIt( m_pOctantRoot ); 
} 
 
void OctreeQuantize::BuildPalIt( SOctant *pOctant )//????????? 
{ 
	if( pOctant->IsLeaf() )  
	{ 
		pOctant->m_iRed /= pOctant->m_iRefCount; 
		pOctant->m_iGreen /= pOctant->m_iRefCount; 
		pOctant->m_iBlue /= pOctant->m_iRefCount; 
		/*m_pPal[m_iPalIndex] = ( pOctant->m_iRed &lt;&lt; 11 ) | 
							  ( pOctant->m_iGreen &lt;&lt; 6 ) | 
							  ( pOctant->m_iBlue ); 
		*/ 
		m_pPal[m_iPalIndex].Red = pOctant->m_iRed; 
		m_pPal[m_iPalIndex].Green = pOctant->m_iGreen; 
		m_pPal[m_iPalIndex].Blue = pOctant->m_iBlue; 
	 
		pOctant->m_iPalIndex = m_iPalIndex; 
 
		m_iPalIndex ++; 
	} 
	else 
	{		 
		for( int i=0; i&lt;8; i++ ) 
		{ 
			if( pOctant->m_pChildren[i] ) 
				BuildPalIt( pOctant->m_pChildren[i] ); 
		} 
	} 
} 
 
/*-------------------------??????????????????????-------------------------------*/ 
 
void OctreeQuantize::MapImgTo256(BYTE *pwImgBuf, int width, int height, int pitch)//WORD *pwImgBuf?????BYTE *pwImgBuf 
{ 
	m_pbyIndexedImg = new BYTE[ width*height ]; 
	int indexSrc=0; 
	int indexDst=0; 
	for( int row=0; row&lt;height; row++ ) 
	{ 
		int size = row *pitch; 
		////////pwImgBuf = &pwImgBuf[row * pitch]; 
		for( int col=0; col&lt;width; col++ ) 
		{ 
			int red = pwImgBuf[size + col*3]; 
			int green = pwImgBuf[size + col*3 + 1]; 
			int blue = pwImgBuf[size + col*3 + 2]; 
 
			//int red = pwImgBuf[indexSrc] >> 11; 
			//int green = (pwImgBuf[indexSrc] & 0x7e0 )>>6; 
			//int blue = pwImgBuf[indexSrc] & 0x1f; 
			indexSrc ++; 
 
			SOctant *pOctant = m_pOctantRoot; 
			for( int i=0; i&lt;5; i++ ) //??5?????8 
			{ 
				int indexChild = ( ((red>>(7-i))&1) &lt;&lt;2 ) |  
								 ( ((green>>(7-i))&1) &lt;&lt;1 ) |  
								 ( (blue>>(7-i))&1);//??4?????7 
 
				pOctant = pOctant->m_pChildren[indexChild]; 
 
				if( pOctant->IsLeaf() ) 
				{ 
					m_pbyIndexedImg[indexDst++] = pOctant->m_iPalIndex;//????????? 
			   		break; 
				} 
			}			 
		} 
		indexSrc += (pitch>>1)-width; 
	} 
} 
 
BOOL SOctant::IsRev2ndLevel() 
{ 
	BOOL bHasChild = false; 
	for( int i=0; i&lt;8; i++ ) 
	{ 
		if( m_pChildren[i] ) 
		{ 
			bHasChild = true; 
			if( !m_pChildren[i]->IsLeaf() ) 
				return false; 
		} 
	} 
 
	if( bHasChild ) return true; 
	else return false; 
} 
 
 
//16λ??256??????????? 
void OctreeQuantize::Chg16bitTo256(BYTE *pwImgBuf, int width, int height, int pitch)//WORD *pwImgBuf?????BYTE *pwImgBuf 
{ 
	BuildOctree( pwImgBuf, width, height, pitch );    //????????? 
	ReduceTo256();                                    //??????????????256????? 
	BuildPal();                                       //????????? 
	MapImgTo256( pwImgBuf, width, height, pitch );    //??????????????????????? 
} 
