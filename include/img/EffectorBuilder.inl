#include "StdAfx.h"
#include "EffectorBuilder.h"
#include "math.h"
#include "time.h"


CEffectorBuilder::CEffectorBuilder(void)
{
	// Init members
	m_lpImages = NULL;
	m_iNumberImages = 0;
	m_iCurrentImage = 0;
	m_ScreenBitmap.Create(950, 950);
	m_ScreenBitmap.SetResampleMode(RM_NEARESTNEIGHBOUR);
}

CEffectorBuilder::~CEffectorBuilder(void)
{
	// Free members
	for (int i=0; i<m_iNumberImages; i++)
	{
		delete m_lpImages[i].pBitmap;
		m_lpImages[i].pBitmap = NULL;
	}
}

BOOL CEffectorBuilder::AddImage(LPTSTR lpszImageFile)
{
	BOOL bResult = FALSE;

	// Add new image
	CBitmapEx* pBitmap = new CBitmapEx();
	pBitmap->Load(lpszImageFile);
	if (pBitmap->IsValid())
	{
		m_iNumberImages++;
		if (m_lpImages == NULL)
			m_lpImages = (_LPIMAGEDESC)pmalloc(m_iNumberImages*sizeof(_IMAGEDESC));
		else
			m_lpImages = (_LPIMAGEDESC)prealloc(m_lpImages, m_iNumberImages*sizeof(_IMAGEDESC));
		memset(&m_lpImages[m_iNumberImages-1].animDesc, 0, sizeof(_ANIMDESC));
		memset(&m_lpImages[m_iNumberImages-1].animDesc2, 0, sizeof(_ANIMDESC2));
		m_lpImages[m_iNumberImages-1].count = 0;
		m_lpImages[m_iNumberImages-1].pBitmap = pBitmap;

		bResult = TRUE;
	}
	else
	{
		delete pBitmap;
		pBitmap = NULL;
	}

	return bResult;
}

BOOL CEffectorBuilder::AddImage(CBitmapEx* pBitmap)
{
	BOOL bResult = FALSE;

	// Add new image
	if ((pBitmap != NULL) && (pBitmap->IsValid()))
	{
		m_iNumberImages++;
		if (m_lpImages == NULL)
			m_lpImages = (_LPIMAGEDESC)pmalloc(m_iNumberImages*sizeof(_IMAGEDESC));
		else
			m_lpImages = (_LPIMAGEDESC)prealloc(m_lpImages, m_iNumberImages*sizeof(_IMAGEDESC));
		memset(&m_lpImages[m_iNumberImages-1].animDesc, 0, sizeof(_ANIMDESC));
		memset(&m_lpImages[m_iNumberImages-1].animDesc2, 0, sizeof(_ANIMDESC2));
		m_lpImages[m_iNumberImages-1].count = 0;
		m_lpImages[m_iNumberImages-1].pBitmap = pBitmap;

		bResult = TRUE;
	}

	return bResult;
}

void CEffectorBuilder::SetAnimationInfo(_LPANIMDESC2 lpAnimDesc2, int iCount, int iImage)
{
	// Set image animation info
	int _image = max(min(iImage, m_iNumberImages-1), 0);
	int _count = max(min(iCount, MAX_ANIMATIONS), 0);
	m_lpImages[_image].count = _count;
	m_lpImages[_image].current = 0;
	memcpy(&m_lpImages[_image].animDesc2, lpAnimDesc2, m_lpImages[_image].count*sizeof(_ANIMDESC2));
	if (lpAnimDesc2->dwFilterType == FT_FIRE)
	{
		// Create fire effect
		m_lpImages[_image].pBitmap->CreateFireEffect();
	}
	if (lpAnimDesc2->dwFilterType == FT_WATER)
	{
		// Create water effect
		m_lpImages[_image].pBitmap->CreateWaterEffect();
	}
	if (lpAnimDesc2->dwFilterType == FT_SMOKE)
	{
		// Create smoke effect
		m_lpImages[_image].pBitmap->CreateSmokeEffect();
	}

	// Update image animation info
	DWORD dwAnimationFlags = m_lpImages[_image].animDesc2[m_lpImages[_image].current].dwAnimationFlags;
	DWORD dwAnimationDuration = m_lpImages[_image].animDesc2[m_lpImages[_image].current].dwAnimationDuration;
	DWORD dwAnimationTime = m_lpImages[_image].animDesc2[m_lpImages[_image].current].dwAnimationTime;
	DWORD dwStartTime = m_lpImages[_image].animDesc2[m_lpImages[_image].current].dwStartTime;
	POINT ptStart = m_lpImages[_image].animDesc2[m_lpImages[_image].current].ptStart;
	POINT ptEnd = m_lpImages[_image].animDesc2[m_lpImages[_image].current].ptEnd;
	int scaleStart = m_lpImages[_image].animDesc2[m_lpImages[_image].current].scaleStart;
	int scaleEnd = m_lpImages[_image].animDesc2[m_lpImages[_image].current].scaleEnd;
	int angleStart = m_lpImages[_image].animDesc2[m_lpImages[_image].current].angleStart;
	int angleEnd = m_lpImages[_image].animDesc2[m_lpImages[_image].current].angleEnd;
	BYTE alphaStart = m_lpImages[_image].animDesc2[m_lpImages[_image].current].alphaStart;
	BYTE alphaEnd = m_lpImages[_image].animDesc2[m_lpImages[_image].current].alphaEnd;
	_DRAWMODE drawMode = m_lpImages[_image].animDesc2[m_lpImages[_image].current].drawMode;
	bool bInverse = m_lpImages[_image].animDesc2[m_lpImages[_image].current].bInverse;
	DWORD dwFilterType = m_lpImages[_image].animDesc2[m_lpImages[_image].current].dwFilterType;
	_QUAD _morphQuad = m_lpImages[_image].animDesc2[m_lpImages[_image].current].morphQuad;
	SetAnimationInfo(dwAnimationFlags, dwAnimationDuration, dwAnimationTime, dwStartTime, ptStart, ptEnd, scaleStart, scaleEnd, angleStart, angleEnd, alphaStart, alphaEnd, drawMode, bInverse, dwFilterType, _morphQuad, _image);
}

void CEffectorBuilder::Update()
{
	// Update images
	DWORD dwCurrentTime = GetTickCount();
	for (int i=0; i<m_iNumberImages; i++)
	{
		if ((dwCurrentTime >= m_lpImages[i].animDesc.dwStartTime) && (dwCurrentTime <= (m_lpImages[i].animDesc.dwStartTime+m_lpImages[i].animDesc.dwAnimationDuration)))
		{
			if ((dwCurrentTime-m_lpImages[i].animDesc.dwCurrentTime) >= m_lpImages[i].animDesc.dwAnimationTime)
			{
				// Update current time
				m_lpImages[i].animDesc.dwCurrentTime = dwCurrentTime;

				// Calculate animation params
				float _offset = (float)(m_lpImages[i].animDesc.dwCurrentTime - m_lpImages[i].animDesc.dwStartTime) / (float)m_lpImages[i].animDesc.dwAnimationDuration;
				int _totalMoveX = m_lpImages[i].animDesc.animationParams[1].rect.left - m_lpImages[i].animDesc.animationParams[0].rect.left;
				int _totalMoveY = m_lpImages[i].animDesc.animationParams[1].rect.top - m_lpImages[i].animDesc.animationParams[0].rect.top;
				int _moveX = (int)(_offset * _totalMoveX);
				int _moveY = (int)(_offset * _totalMoveY);
				int _totalSizeX = (m_lpImages[i].animDesc.animationParams[1].rect.right-m_lpImages[i].animDesc.animationParams[1].rect.left) - (m_lpImages[i].animDesc.animationParams[0].rect.right-m_lpImages[i].animDesc.animationParams[0].rect.left);
				int _totalSizeY = (m_lpImages[i].animDesc.animationParams[1].rect.bottom-m_lpImages[i].animDesc.animationParams[1].rect.top) - (m_lpImages[i].animDesc.animationParams[0].rect.bottom-m_lpImages[i].animDesc.animationParams[0].rect.top);
				int _sizeX = (int)(_offset * _totalSizeX);
				int _sizeY = (int)(_offset * _totalSizeY);

				// Update movement and size
				if ((m_lpImages[i].animDesc.dwAnimationFlags & AT_MOVE) && (m_lpImages[i].animDesc.dwAnimationFlags & AT_SIZE))
				{
					m_lpImages[i].animDesc.animationParams[2].rect = m_lpImages[i].animDesc.animationParams[0].rect;
					OffsetRect(&m_lpImages[i].animDesc.animationParams[2].rect, _moveX, _moveY);
					m_lpImages[i].animDesc.animationParams[2].rect.right = m_lpImages[i].animDesc.animationParams[2].rect.right + _sizeX;
					m_lpImages[i].animDesc.animationParams[2].rect.bottom = m_lpImages[i].animDesc.animationParams[2].rect.bottom + _sizeY;
				}
				else if (m_lpImages[i].animDesc.dwAnimationFlags & AT_MOVE)
				{
					m_lpImages[i].animDesc.animationParams[2].rect = m_lpImages[i].animDesc.animationParams[0].rect;
					OffsetRect(&m_lpImages[i].animDesc.animationParams[2].rect, _moveX, _moveY);
				}
				else if (m_lpImages[i].animDesc.dwAnimationFlags & AT_SIZE)
				{
					m_lpImages[i].animDesc.animationParams[2].rect.right = m_lpImages[i].animDesc.animationParams[0].rect.right + _sizeX;
					m_lpImages[i].animDesc.animationParams[2].rect.bottom = m_lpImages[i].animDesc.animationParams[0].rect.bottom + _sizeY;
				}

				// Update rotation
				if (m_lpImages[i].animDesc.dwAnimationFlags & AT_ROTATE)
				{
					int angle = (int)(_offset * ((int)m_lpImages[i].animDesc.animationParams[1].angle - (int)m_lpImages[i].animDesc.animationParams[0].angle));
					m_lpImages[i].animDesc.animationParams[2].angle = m_lpImages[i].animDesc.animationParams[0].angle + angle;
				}

				// Update alpha
				if (m_lpImages[i].animDesc.dwAnimationFlags & AT_ALPHA)
				{
					int alpha = (int)(_offset * ((int)m_lpImages[i].animDesc.animationParams[1].alpha - (int)m_lpImages[i].animDesc.animationParams[0].alpha));
					m_lpImages[i].animDesc.animationParams[2].alpha = (BYTE)max(0, min(255, ((int)m_lpImages[i].animDesc.animationParams[0].alpha + alpha)));
				}

				// Update morphing
				if (m_lpImages[i].animDesc.dwAnimationFlags & AT_MORPH)
				{
					float _morphOffset = _offset;
					if (m_lpImages[i].animDesc.drawingParams.bInverse)
						_morphOffset = 1.0f - _morphOffset;
					m_lpImages[i].animDesc.morphQuad.p1.x = m_lpImages[i].animDesc.animationParams[2].rect.left + (int)(((float)m_lpImages[i].animDesc2[m_lpImages[i].current].morphQuad.p1.x/100.0f*_morphOffset) * (m_lpImages[i].animDesc.animationParams[2].rect.right - m_lpImages[i].animDesc.animationParams[2].rect.left));
					m_lpImages[i].animDesc.morphQuad.p1.y = m_lpImages[i].animDesc.animationParams[2].rect.top + (int)(((float)m_lpImages[i].animDesc2[m_lpImages[i].current].morphQuad.p1.y/100.0f*_morphOffset) * (m_lpImages[i].animDesc.animationParams[2].rect.bottom - m_lpImages[i].animDesc.animationParams[2].rect.top));
					m_lpImages[i].animDesc.morphQuad.p2.x = m_lpImages[i].animDesc.animationParams[2].rect.right + (int)(((float)m_lpImages[i].animDesc2[m_lpImages[i].current].morphQuad.p2.x/100.0f*_morphOffset) * (m_lpImages[i].animDesc.animationParams[2].rect.right - m_lpImages[i].animDesc.animationParams[2].rect.left));
					m_lpImages[i].animDesc.morphQuad.p2.y = m_lpImages[i].animDesc.animationParams[2].rect.top + (int)(((float)m_lpImages[i].animDesc2[m_lpImages[i].current].morphQuad.p2.y/100.0f*_morphOffset) * (m_lpImages[i].animDesc.animationParams[2].rect.bottom - m_lpImages[i].animDesc.animationParams[2].rect.top));
					m_lpImages[i].animDesc.morphQuad.p3.x = m_lpImages[i].animDesc.animationParams[2].rect.right + (int)(((float)m_lpImages[i].animDesc2[m_lpImages[i].current].morphQuad.p3.x/100.0f*_morphOffset) * (m_lpImages[i].animDesc.animationParams[2].rect.right - m_lpImages[i].animDesc.animationParams[2].rect.left));
					m_lpImages[i].animDesc.morphQuad.p3.y = m_lpImages[i].animDesc.animationParams[2].rect.bottom + (int)(((float)m_lpImages[i].animDesc2[m_lpImages[i].current].morphQuad.p3.y/100.0f*_morphOffset) * (m_lpImages[i].animDesc.animationParams[2].rect.bottom - m_lpImages[i].animDesc.animationParams[2].rect.top));
					m_lpImages[i].animDesc.morphQuad.p4.x = m_lpImages[i].animDesc.animationParams[2].rect.left + (int)(((float)m_lpImages[i].animDesc2[m_lpImages[i].current].morphQuad.p4.x/100.0f*_morphOffset) * (m_lpImages[i].animDesc.animationParams[2].rect.right - m_lpImages[i].animDesc.animationParams[2].rect.left));
					m_lpImages[i].animDesc.morphQuad.p4.y = m_lpImages[i].animDesc.animationParams[2].rect.bottom + (int)(((float)m_lpImages[i].animDesc2[m_lpImages[i].current].morphQuad.p4.y/100.0f*_morphOffset) * (m_lpImages[i].animDesc.animationParams[2].rect.bottom - m_lpImages[i].animDesc.animationParams[2].rect.top));
				}

				// Update fire effect
				if (m_lpImages[i].animDesc.dwFilterType == FT_FIRE)
				{
					// Update fire
					m_lpImages[i].pBitmap->UpdateFireEffect();
				}

				// Update water effect
				if (m_lpImages[i].animDesc.dwFilterType == FT_WATER)
				{
					// Update water
					m_lpImages[i].pBitmap->UpdateWaterEffect();
				}

				// Update smoke effect
				if (m_lpImages[i].animDesc.dwFilterType == FT_SMOKE)
				{
					// Update smoke
					m_lpImages[i].pBitmap->UpdateSmokeEffect(40, 400, 40);
				}

				// Update drawing
				m_lpImages[i].animDesc.drawingParams.drawPercent = _offset;
			}
		}
		else if (dwCurrentTime >= (m_lpImages[i].animDesc.dwStartTime+m_lpImages[i].animDesc.dwAnimationDuration))
		{
			// Update animation
			if (m_lpImages[i].count > 0)
			{
				if ((m_lpImages[i].animDesc.dwAnimationFlags & AT_REPLAY) || ((m_lpImages[i].current+1) < m_lpImages[i].count))
				{
					// Update drawing
					srand(GetTickCount());
					m_lpImages[i].animDesc.drawingParams.drawPercent = 0;

					// Update water effect
					if (m_lpImages[i].animDesc.dwFilterType == FT_WATER)
					{
						// Update water
						long x = max(50, rand()%(m_lpImages[i].pBitmap->GetWidth()-50));
						long y = max(50, rand()%(m_lpImages[i].pBitmap->GetHeight()-50));
						long size = max(5, rand()%20);
						long height = max(20, rand()%100);
						m_lpImages[i].pBitmap->MakeWaterBlob(x, y, size, height);
					}

					m_lpImages[i].current = (m_lpImages[i].current + 1) % m_lpImages[i].count;
					if (m_lpImages[i].animDesc.dwAnimationFlags & AT_RANDOM)
					{
						int current = (m_lpImages[i].current + m_lpImages[i].count - 1) % m_lpImages[i].count;
						int previous = (m_lpImages[i].current + m_lpImages[i].count - 2) % m_lpImages[i].count;
						m_lpImages[i].animDesc2[previous].ptEnd.x = rand() % m_ScreenBitmap.GetWidth();
						m_lpImages[i].animDesc2[previous].ptEnd.y = rand() % m_ScreenBitmap.GetHeight();
						m_lpImages[i].animDesc2[previous].scaleEnd = rand() % 51;
						m_lpImages[i].animDesc2[previous].angleEnd = rand() % 361;
						m_lpImages[i].animDesc2[previous].alphaEnd = rand() % 256;
						m_lpImages[i].animDesc2[current].ptStart.x = m_lpImages[i].animDesc2[previous].ptEnd.x;
						m_lpImages[i].animDesc2[current].ptStart.y = m_lpImages[i].animDesc2[previous].ptEnd.y;
						m_lpImages[i].animDesc2[current].scaleStart = m_lpImages[i].animDesc2[previous].scaleEnd;
						m_lpImages[i].animDesc2[current].angleStart = m_lpImages[i].animDesc2[previous].angleEnd;
						m_lpImages[i].animDesc2[current].alphaStart = m_lpImages[i].animDesc2[previous].alphaEnd;
					}
					DWORD dwAnimationFlags = m_lpImages[i].animDesc2[m_lpImages[i].current].dwAnimationFlags;
					DWORD dwAnimationDuration = m_lpImages[i].animDesc2[m_lpImages[i].current].dwAnimationDuration;
					DWORD dwAnimationTime = m_lpImages[i].animDesc2[m_lpImages[i].current].dwAnimationTime;
					DWORD dwStartTime = m_lpImages[i].animDesc2[m_lpImages[i].current].dwStartTime;
					POINT ptStart = m_lpImages[i].animDesc2[m_lpImages[i].current].ptStart;
					POINT ptEnd = m_lpImages[i].animDesc2[m_lpImages[i].current].ptEnd;
					int scaleStart = m_lpImages[i].animDesc2[m_lpImages[i].current].scaleStart;
					int scaleEnd = m_lpImages[i].animDesc2[m_lpImages[i].current].scaleEnd;
					int angleStart = m_lpImages[i].animDesc2[m_lpImages[i].current].angleStart;
					int angleEnd = m_lpImages[i].animDesc2[m_lpImages[i].current].angleEnd;
					BYTE alphaStart = m_lpImages[i].animDesc2[m_lpImages[i].current].alphaStart;
					BYTE alphaEnd = m_lpImages[i].animDesc2[m_lpImages[i].current].alphaEnd;
					_DRAWMODE drawMode = m_lpImages[i].animDesc2[m_lpImages[i].current].drawMode;
					if (drawMode == DM_RANDOM)
						drawMode = (_DRAWMODE)(rand()%((int)DM_RANDOM));
					bool bInverse = m_lpImages[i].animDesc2[m_lpImages[i].current].bInverse;
					DWORD dwFilterType = m_lpImages[i].animDesc2[m_lpImages[i].current].dwFilterType;
					_QUAD _morphQuad = m_lpImages[i].animDesc.morphQuad;
					SetAnimationInfo(dwAnimationFlags, dwAnimationDuration, dwAnimationTime, dwStartTime, ptStart, ptEnd, scaleStart, scaleEnd, angleStart, angleEnd, alphaStart, alphaEnd, drawMode, bInverse, dwFilterType, _morphQuad, i);
				}
			}
		}
	}
}

void CEffectorBuilder::SetAnimationInfo(DWORD dwAnimationFlags, DWORD dwAnimationDuration, DWORD dwAnimationTime, DWORD dwStartTime, POINT ptStart, POINT ptEnd, int scaleStart, int scaleEnd, int angleStart, int angleEnd, BYTE alphaStart, BYTE alphaEnd, _DRAWMODE drawMode, bool bInverse, DWORD dwFilterType, _QUAD morphQuad, int iImage)
{
	// Set image animation info
	DWORD _dwCurrentTime = GetTickCount();
	int _image = max(min(iImage, m_iNumberImages-1), 0);
	int _scaleStart = max(0, scaleStart);
	int _scaleEnd = max(0, scaleEnd);
	int _startWidth = (int)(((float)_scaleStart / 100.0f) * m_lpImages[_image].pBitmap->GetWidth());
	int _startHeight = (int)(((float)_scaleStart / 100.0f) * m_lpImages[_image].pBitmap->GetHeight());
	int _endWidth = (int)(((float)_scaleEnd / 100.0f) * m_lpImages[_image].pBitmap->GetWidth());
	int _endHeight = (int)(((float)_scaleEnd / 100.0f) * m_lpImages[_image].pBitmap->GetHeight());
	m_lpImages[_image].animDesc.dwAnimationFlags = dwAnimationFlags;
	m_lpImages[_image].animDesc.dwAnimationDuration = dwAnimationDuration;
	m_lpImages[_image].animDesc.dwAnimationTime = dwAnimationTime;
	m_lpImages[_image].animDesc.dwStartTime = _dwCurrentTime + dwStartTime;
	m_lpImages[_image].animDesc.dwCurrentTime = m_lpImages[_image].animDesc.dwStartTime;
	m_lpImages[_image].animDesc.animationParams[0].alpha = alphaStart;
	m_lpImages[_image].animDesc.animationParams[0].angle = angleStart;
	SetRect(&m_lpImages[_image].animDesc.animationParams[0].rect, ptStart.x, ptStart.y, ptStart.x+_startWidth, ptStart.y+_startHeight);
	m_lpImages[_image].animDesc.animationParams[1].alpha = alphaEnd;
	m_lpImages[_image].animDesc.animationParams[1].angle = angleEnd;
	SetRect(&m_lpImages[_image].animDesc.animationParams[1].rect, ptEnd.x, ptEnd.y, ptEnd.x+_endWidth, ptEnd.y+_endHeight);
	memcpy(&m_lpImages[_image].animDesc.animationParams[2], &m_lpImages[_image].animDesc.animationParams[0], sizeof(_ANIMPARAMS));
	m_lpImages[_image].animDesc.drawingParams.drawMode = drawMode;
	m_lpImages[_image].animDesc.drawingParams.bInverse = bInverse;
	m_lpImages[_image].animDesc.dwFilterType = dwFilterType;
	float _morphOffset = 0;
	if (m_lpImages[_image].animDesc.drawingParams.bInverse)
		_morphOffset = 1.0f - _morphOffset;
	m_lpImages[_image].animDesc.morphQuad.p1.x = m_lpImages[_image].animDesc.animationParams[2].rect.left + (int)(((float)m_lpImages[_image].animDesc2[m_lpImages[_image].current].morphQuad.p1.x/100.0f*_morphOffset) * (m_lpImages[_image].animDesc.animationParams[2].rect.right - m_lpImages[_image].animDesc.animationParams[2].rect.left));
	m_lpImages[_image].animDesc.morphQuad.p1.y = m_lpImages[_image].animDesc.animationParams[2].rect.top + (int)(((float)m_lpImages[_image].animDesc2[m_lpImages[_image].current].morphQuad.p1.y/100.0f*_morphOffset) * (m_lpImages[_image].animDesc.animationParams[2].rect.bottom - m_lpImages[_image].animDesc.animationParams[2].rect.top));
	m_lpImages[_image].animDesc.morphQuad.p2.x = m_lpImages[_image].animDesc.animationParams[2].rect.right + (int)(((float)m_lpImages[_image].animDesc2[m_lpImages[_image].current].morphQuad.p2.x/100.0f*_morphOffset) * (m_lpImages[_image].animDesc.animationParams[2].rect.right - m_lpImages[_image].animDesc.animationParams[2].rect.left));
	m_lpImages[_image].animDesc.morphQuad.p2.y = m_lpImages[_image].animDesc.animationParams[2].rect.top + (int)(((float)m_lpImages[_image].animDesc2[m_lpImages[_image].current].morphQuad.p2.y/100.0f*_morphOffset) * (m_lpImages[_image].animDesc.animationParams[2].rect.bottom - m_lpImages[_image].animDesc.animationParams[2].rect.top));
	m_lpImages[_image].animDesc.morphQuad.p3.x = m_lpImages[_image].animDesc.animationParams[2].rect.right + (int)(((float)m_lpImages[_image].animDesc2[m_lpImages[_image].current].morphQuad.p3.x/100.0f*_morphOffset) * (m_lpImages[_image].animDesc.animationParams[2].rect.right - m_lpImages[_image].animDesc.animationParams[2].rect.left));
	m_lpImages[_image].animDesc.morphQuad.p3.y = m_lpImages[_image].animDesc.animationParams[2].rect.bottom + (int)(((float)m_lpImages[_image].animDesc2[m_lpImages[_image].current].morphQuad.p3.y/100.0f*_morphOffset) * (m_lpImages[_image].animDesc.animationParams[2].rect.bottom - m_lpImages[_image].animDesc.animationParams[2].rect.top));
	m_lpImages[_image].animDesc.morphQuad.p4.x = m_lpImages[_image].animDesc.animationParams[2].rect.left + (int)(((float)m_lpImages[_image].animDesc2[m_lpImages[_image].current].morphQuad.p4.x/100.0f*_morphOffset) * (m_lpImages[_image].animDesc.animationParams[2].rect.right - m_lpImages[_image].animDesc.animationParams[2].rect.left));
	m_lpImages[_image].animDesc.morphQuad.p4.y = m_lpImages[_image].animDesc.animationParams[2].rect.bottom + (int)(((float)m_lpImages[_image].animDesc2[m_lpImages[_image].current].morphQuad.p4.y/100.0f*_morphOffset) * (m_lpImages[_image].animDesc.animationParams[2].rect.bottom - m_lpImages[_image].animDesc.animationParams[2].rect.top));
}

void CEffectorBuilder::Draw(HDC hDC)
{
	// Clear background bitmap
	m_ScreenBitmap.Clear();

	// Draw images
	for (int i=0; i<m_iNumberImages; i++)
	{
		int _x = m_lpImages[i].animDesc.animationParams[2].rect.left;
		int _y = m_lpImages[i].animDesc.animationParams[2].rect.top;
		int _width = m_lpImages[i].animDesc.animationParams[2].rect.right - m_lpImages[i].animDesc.animationParams[2].rect.left;
		int _height = m_lpImages[i].animDesc.animationParams[2].rect.bottom - m_lpImages[i].animDesc.animationParams[2].rect.top;
		_x = _x - (_width>>1);
		_y = _y - (_height>>1);
		BYTE _alpha = m_lpImages[i].animDesc.animationParams[2].alpha;
		int _angle = m_lpImages[i].animDesc.animationParams[2].angle;
		Draw(i, _x, _y, _width, _height, _alpha, _angle);
	}

	// Reflection
	int width = m_ScreenBitmap.GetWidth();
	int height = m_ScreenBitmap.GetHeight() / 2;
	CBitmapEx tmpBitmap2, tmpBitmap3;
	tmpBitmap2.Create(width, height);
	tmpBitmap2.Clear();
	tmpBitmap3.Create(width, height);
	tmpBitmap3.Draw(0, 0, width, height, m_ScreenBitmap, 0, 0);
	tmpBitmap3.FlipVertical();
	tmpBitmap2.DrawBlended(0, 0, width, height, tmpBitmap3, 0, 0, 25, 0, GM_VERTICAL);
	m_ScreenBitmap.Draw(0, height, width, height, tmpBitmap2, 0, 0);

	// Draw on screen DC
	m_ScreenBitmap.Draw(hDC);
}

void CEffectorBuilder::Draw(int iImage, int x, int y, int width, int height, BYTE alpha, int angle)
{
	// Check for image rotation
	CBitmapEx tmpBitmap1;
	tmpBitmap1.Create(width, height);
	switch(m_iQuality)
	{
		case QT_LOW:
			{
				// Set low quality
				tmpBitmap1.SetResampleMode(RM_NEARESTNEIGHBOUR);
			}
			break;

		case QT_NORMAL:
			{
				// Set normal quality
				tmpBitmap1.SetResampleMode(RM_BILINEAR);
			}
			break;

		case QT_HIGH:
			{
				// Set high quality
				tmpBitmap1.SetResampleMode(RM_BICUBIC);
			}
			break;
	}
	tmpBitmap1.Draw(0, 0, width, height, *m_lpImages[iImage].pBitmap, 0, 0, m_lpImages[iImage].pBitmap->GetWidth(), m_lpImages[iImage].pBitmap->GetHeight());
	long horizontalExtent=0, verticalExtent=0;
	if ((m_lpImages[iImage].animDesc.drawingParams.drawMode == DM_NONE) && (m_lpImages[iImage].animDesc.dwAnimationFlags & AT_ROTATE))
	{
		float _angle1 = -((float)m_lpImages[iImage].animDesc.animationParams[2].angle/180.0f) * _PI;
		long _width = (long)(abs((float)width*cos(_angle1)) + abs((float)height*sin(_angle1)) + 0.5f);
		long _height = (long)(abs((float)width*sin(_angle1)) + abs((float)height*cos(_angle1)) + 0.5f);
		horizontalExtent = _width - width;
		verticalExtent = _height - height;
		tmpBitmap1.Rotate(angle);
		x -= (horizontalExtent>>1);
		y -= (verticalExtent>>1);
		width += horizontalExtent;
		height += verticalExtent;
	}
	if ((m_lpImages[iImage].animDesc.drawingParams.drawMode == DM_NONE) && (m_lpImages[iImage].animDesc.dwAnimationFlags & AT_MORPH))
	{
		long minX = min(min(m_lpImages[iImage].animDesc.morphQuad.p1.x, m_lpImages[iImage].animDesc.morphQuad.p2.x), min(m_lpImages[iImage].animDesc.morphQuad.p3.x, m_lpImages[iImage].animDesc.morphQuad.p4.x));
		long maxX = max(max(m_lpImages[iImage].animDesc.morphQuad.p1.x, m_lpImages[iImage].animDesc.morphQuad.p2.x), max(m_lpImages[iImage].animDesc.morphQuad.p3.x, m_lpImages[iImage].animDesc.morphQuad.p4.x));
		long minY = min(min(m_lpImages[iImage].animDesc.morphQuad.p1.y, m_lpImages[iImage].animDesc.morphQuad.p2.y), min(m_lpImages[iImage].animDesc.morphQuad.p3.y, m_lpImages[iImage].animDesc.morphQuad.p4.y));
		long maxY = max(max(m_lpImages[iImage].animDesc.morphQuad.p1.y, m_lpImages[iImage].animDesc.morphQuad.p2.y), max(m_lpImages[iImage].animDesc.morphQuad.p3.y, m_lpImages[iImage].animDesc.morphQuad.p4.y));
		width = maxX - minX;
		height = maxY - minY;
	}

	// Check for image filtering
	if (m_lpImages[iImage].animDesc.dwFilterType & FT_NEGATIVE)
	{
		// Apply negative filter
		tmpBitmap1.Negative();
	}
	if (m_lpImages[iImage].animDesc.dwFilterType & FT_GRAYSCALE)
	{
		// Apply grayscale filter
		tmpBitmap1.Grayscale();
	}
	if (m_lpImages[iImage].animDesc.dwFilterType & FT_SEPIA)
	{
		// Apply sepia filter
		tmpBitmap1.Sepia();
	}
	if (m_lpImages[iImage].animDesc.dwFilterType & FT_EMBOSS)
	{
		// Apply emboss filter
		tmpBitmap1.Emboss();
	}
	if (m_lpImages[iImage].animDesc.dwFilterType & FT_ENGRAVE)
	{
		// Apply engrave filter
		tmpBitmap1.Engrave();
	}
	if (m_lpImages[iImage].animDesc.dwFilterType & FT_PIXELIZE)
	{
		// Apply pixelize filter
		tmpBitmap1.Pixelize();
	}
	if (m_lpImages[iImage].animDesc.dwFilterType & FT_FLIPHORIZONTAL)
	{
		// Apply flip horizontal filter
		tmpBitmap1.FlipHorizontal();
	}
	if (m_lpImages[iImage].animDesc.dwFilterType & FT_FLIPVERTICAL)
	{
		// Apply flip vertical filter
		tmpBitmap1.FlipVertical();
	}

	// Create temporary bitmap
	long _alpha = (long)((float)alpha / 255.0f * 100.0f);
	CBitmapEx tmpBitmap;
	tmpBitmap.Create(width, height);
	switch(m_iQuality)
	{
		case QT_LOW:
			{
				// Set low quality
				tmpBitmap.SetResampleMode(RM_NEARESTNEIGHBOUR);
			}
			break;

		case QT_NORMAL:
			{
				// Set normal quality
				tmpBitmap.SetResampleMode(RM_BILINEAR);
			}
			break;

		case QT_HIGH:
			{
				// Set high quality
				tmpBitmap.SetResampleMode(RM_BICUBIC);
			}
			break;
	}

	// Draw image
	switch (m_lpImages[iImage].animDesc.drawingParams.drawMode)
	{
		case DM_NONE:
			{
				tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y);
				if (m_lpImages[iImage].animDesc.dwAnimationFlags & AT_ROTATE)
					tmpBitmap.DrawTransparent(0, 0, width, height, tmpBitmap1, 0, 0, _alpha, tmpBitmap1.GetPixel(0, 0));
				else if (m_lpImages[iImage].animDesc.dwAnimationFlags & AT_MORPH)
				{
					_QUAD morphQuad = m_lpImages[iImage].animDesc.morphQuad;
					morphQuad.p1.x -= m_lpImages[iImage].animDesc.animationParams[2].rect.left;
					morphQuad.p1.y -= m_lpImages[iImage].animDesc.animationParams[2].rect.top;
					morphQuad.p2.x -= m_lpImages[iImage].animDesc.animationParams[2].rect.left;
					morphQuad.p2.y -= m_lpImages[iImage].animDesc.animationParams[2].rect.top;
					morphQuad.p3.x -= m_lpImages[iImage].animDesc.animationParams[2].rect.left;
					morphQuad.p3.y -= m_lpImages[iImage].animDesc.animationParams[2].rect.top;
					morphQuad.p4.x -= m_lpImages[iImage].animDesc.animationParams[2].rect.left;
					morphQuad.p4.y -= m_lpImages[iImage].animDesc.animationParams[2].rect.top;
					tmpBitmap.Draw(morphQuad, tmpBitmap1, _alpha);
				}
				else
					tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				m_ScreenBitmap.Draw(x, y, width, height, tmpBitmap, 0, 0);
			}
			break;

		case DM_DRAWLEFT:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = height;
				tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x, y, newWidth, newHeight, tmpBitmap, 0, 0);
				else
					m_ScreenBitmap.Draw(x, y, width-newWidth, newHeight, tmpBitmap, 0, 0);
			}
			break;

		case DM_DRAWRIGHT:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = height;
				tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x+width-newWidth, y, newWidth, newHeight, tmpBitmap, width-newWidth, 0);
				else
					m_ScreenBitmap.Draw(x+newWidth, y, width-newWidth, newHeight, tmpBitmap, newWidth, 0);
			}
			break;

		case DM_DRAWTOP:
			{
				int newWidth = width;
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x, y, newWidth, newHeight, tmpBitmap, 0, 0);
				else
					m_ScreenBitmap.Draw(x, y, newWidth, height-newHeight, tmpBitmap, 0, 0);
			}
			break;

		case DM_DRAWBOTTOM:
			{
				int newWidth = width;
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x, y+height-newHeight, newWidth, newHeight, tmpBitmap, 0, height-newHeight);
				else
					m_ScreenBitmap.Draw(x, y+newHeight, newWidth, height-newHeight, tmpBitmap, 0, newHeight);
			}
			break;

		case DM_DRAWDIAG1:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x, y, newWidth, newHeight, tmpBitmap, 0, 0);
				else
					m_ScreenBitmap.Draw(x, y, width-newWidth, height-newHeight, tmpBitmap, 0, 0);
			}
			break;

		case DM_DRAWDIAG2:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(width-newWidth, height-newHeight, newWidth, newHeight, m_ScreenBitmap, x+width-newWidth, y+height-newHeight);
				else
					tmpBitmap.Draw(newWidth, newHeight, width-newWidth, height-newHeight, m_ScreenBitmap, x+newWidth, y+newHeight);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x+width-newWidth, y+height-newHeight, newWidth, newHeight, tmpBitmap, width-newWidth, height-newHeight);
				else
					m_ScreenBitmap.Draw(x+newWidth, y+newHeight, width-newWidth, height-newHeight, tmpBitmap, newWidth, newHeight);
			}
			break;

		case DM_DRAWDIAG3:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(width-newWidth, 0, newWidth, newHeight, m_ScreenBitmap, x+width-newWidth, y);
				else
					tmpBitmap.Draw(newWidth, 0, width-newWidth, height-newHeight, m_ScreenBitmap, x+newWidth, y);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x+width-newWidth, y, newWidth, newHeight, tmpBitmap, width-newWidth, 0);
				else
					m_ScreenBitmap.Draw(x+newWidth, y, width-newWidth, height-newHeight, tmpBitmap, newWidth, 0);
			}
			break;

		case DM_DRAWDIAG4:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(0, height-newHeight, newWidth, newHeight, m_ScreenBitmap, x, y+height-newHeight);
				else
					tmpBitmap.Draw(0, newHeight, width-newWidth, height-newHeight, m_ScreenBitmap, x, y+newHeight);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x, y+height-newHeight, newWidth, newHeight, tmpBitmap, 0, height-newHeight);
				else
					m_ScreenBitmap.Draw(x, y+newHeight, width-newWidth, height-newHeight, tmpBitmap, 0, newHeight);
			}
			break;

		case DM_DRAWINBOX:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw((width-newWidth)/2, (height-newHeight)/2, newWidth, newHeight, m_ScreenBitmap, x+(width-newWidth)/2, y+(height-newHeight)/2);
				else
					tmpBitmap.Draw(newWidth/2, newHeight/2, width-newWidth, height-newHeight, m_ScreenBitmap, x+newWidth/2, y+newHeight/2);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x+(width-newWidth)/2, y+(height-newHeight)/2, newWidth, newHeight, tmpBitmap, (width-newWidth)/2, (height-newHeight)/2);
				else
					m_ScreenBitmap.Draw(x+newWidth/2, y+newHeight/2, width-newWidth, height-newHeight, tmpBitmap, newWidth/2, newHeight/2);
			}
			break;

		case DM_DRAWOUTBOX:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(newWidth/2, newHeight/2, width-newWidth, height-newHeight, m_ScreenBitmap, x+newWidth/2, y+newHeight/2);
				else
					tmpBitmap.Draw((width-newWidth)/2, (height-newHeight)/2, newWidth, newHeight, m_ScreenBitmap, x+(width-newWidth)/2, y+(height-newHeight)/2);
				m_ScreenBitmap.Draw(x, y, width, height, tmpBitmap, 0, 0);
			}
			break;

		case DM_DRAWINCIRCLE:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.DrawBlended((width-newWidth)/2, (height-newHeight)/2, newWidth, newHeight, tmpBitmap1, (width-newWidth)/2, (height-newHeight)/2, 0, 100, GM_RADIAL);
				else
					tmpBitmap.DrawBlended(newWidth/2, newHeight/2, width-newWidth, height-newHeight, tmpBitmap1, newWidth/2, newHeight/2, 0, 100, GM_RADIAL);
				m_ScreenBitmap.Draw(x, y, width, height, tmpBitmap, 0, 0, _alpha);
			}
			break;

		case DM_DRAWOUTCIRCLE:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.DrawBlended(newWidth/2, newHeight/2, width-newWidth, height-newHeight, m_ScreenBitmap, x+newWidth/2, y+newHeight/2, 0, 100, GM_RADIAL);
				else
					tmpBitmap.DrawBlended((width-newWidth)/2, (height-newHeight)/2, newWidth, newHeight, m_ScreenBitmap, x+(width-newWidth)/2, y+(height-newHeight)/2, 0, 100, GM_RADIAL);
				m_ScreenBitmap.Draw(x, y, width, height, tmpBitmap, 0, 0);
			}
			break;

		case DM_SLIDELEFT:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = height;
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(width-newWidth, 0, width, height, m_ScreenBitmap, x, y);
				else
					tmpBitmap.Draw(newWidth, 0, width-newWidth, height, m_ScreenBitmap, x, y);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x, y, newWidth, newHeight, tmpBitmap, width-newWidth, 0);
				else
					m_ScreenBitmap.Draw(x, y, width-newWidth, newHeight, tmpBitmap, newWidth, 0);
			}
			break;

		case DM_SLIDERIGHT:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = height;
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x+width-newWidth, y);
				else
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x+newWidth, y);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x+width-newWidth, y, newWidth, newHeight, tmpBitmap, 0, 0);
				else
					m_ScreenBitmap.Draw(x+newWidth, y, width-newWidth, newHeight, tmpBitmap, 0, 0);
			}
			break;

		case DM_SLIDETOP:
			{
				int newWidth = width;
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(0, height-newHeight, width, height, m_ScreenBitmap, x, y);
				else
					tmpBitmap.Draw(0, newHeight, width, height-newHeight, m_ScreenBitmap, x, y);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x, y, newWidth, newHeight, tmpBitmap, 0, height-newHeight);
				else
					m_ScreenBitmap.Draw(x, y, newWidth, height-newHeight, tmpBitmap, 0, newHeight);
			}
			break;

		case DM_SLIDEBOTTOM:
			{
				int newWidth = width;
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y+height-newHeight);
				else
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y+newHeight);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x, y+height-newHeight, newWidth, newHeight, tmpBitmap, 0, 0);
				else
					m_ScreenBitmap.Draw(x, y+newHeight, newWidth, height-newHeight, tmpBitmap, 0, 0);
			}
			break;

		case DM_SLIDEDIAG1:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(width-newWidth, height-newHeight, newWidth, newHeight, m_ScreenBitmap, x, y);
				else
					tmpBitmap.Draw(newWidth, newHeight, width-newWidth, height-newHeight, m_ScreenBitmap, x, y);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x, y, newWidth, newHeight, tmpBitmap, width-newWidth, height-newHeight);
				else
					m_ScreenBitmap.Draw(x, y, width-newWidth, height-newHeight, tmpBitmap, newWidth, newHeight);
			}
			break;

		case DM_SLIDEDIAG2:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(0, 0, newWidth, newHeight, m_ScreenBitmap, x+width-newWidth, y+height-newHeight);
				else
					tmpBitmap.Draw(0, 0, width-newWidth, height-newHeight, m_ScreenBitmap, x+newWidth, y+newHeight);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x+width-newWidth, y+height-newHeight, newWidth, newHeight, tmpBitmap, 0, 0);
				else
					m_ScreenBitmap.Draw(x+newWidth, y+newHeight, width-newWidth, height-newHeight, tmpBitmap, 0, 0);
			}
			break;

		case DM_SLIDEDIAG3:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(0, height-newHeight, newWidth, newHeight, m_ScreenBitmap, x+width-newWidth, y);
				else
					tmpBitmap.Draw(0, newHeight, width-newWidth, height-newHeight, m_ScreenBitmap, x+newWidth, y);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x+width-newWidth, y, newWidth, newHeight, tmpBitmap, 0, height-newHeight);
				else
					m_ScreenBitmap.Draw(x+newWidth, y, width-newWidth, height-newHeight, tmpBitmap, 0, newHeight);
			}
			break;

		case DM_SLIDEDIAG4:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(width-newWidth, 0, newWidth, newHeight, m_ScreenBitmap, x, y+height-newHeight);
				else
					tmpBitmap.Draw(newWidth, 0, width-newWidth, height-newHeight, m_ScreenBitmap, x, y+newHeight);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x, y+height-newHeight, newWidth, newHeight, tmpBitmap, width-newWidth, 0);
				else
					m_ScreenBitmap.Draw(x, y+newHeight, width-newWidth, height-newHeight, tmpBitmap, newWidth, 0);
			}
			break;

		case DM_SCALELEFT:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = height;
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y, newWidth, height);
				else
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y, width-newWidth, newHeight);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x, y, newWidth, newHeight, tmpBitmap, 0, 0, width, height);
				else
					m_ScreenBitmap.Draw(x, y, width-newWidth, newHeight, tmpBitmap, 0, 0, width, height);
			}
			break;

		case DM_SCALERIGHT:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = height;
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x+width-newWidth, y, newWidth, height);
				else
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x+newWidth, y, width-newWidth, newHeight);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x+width-newWidth, y, newWidth, newHeight, tmpBitmap, 0, 0, width, height);
				else
					m_ScreenBitmap.Draw(x+newWidth, y, width-newWidth, newHeight, tmpBitmap, 0, 0, width, height);
			}
			break;

		case DM_SCALETOP:
			{
				int newWidth = width;
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y, width, newHeight);
				else
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y, newWidth, height-newHeight);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x, y, newWidth, newHeight, tmpBitmap, 0, 0, width, height);
				else
					m_ScreenBitmap.Draw(x, y, newWidth, height-newHeight, tmpBitmap, 0, 0, width, height);
			}
			break;

		case DM_SCALEBOTTOM:
			{
				int newWidth = width;
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y+height-newHeight, width, newHeight);
				else
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y+newHeight, newWidth, height-newHeight);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x, y+height-newHeight, newWidth, newHeight, tmpBitmap, 0, 0, width, height);
				else
					m_ScreenBitmap.Draw(x, y+newHeight, newWidth, height-newHeight, tmpBitmap, 0, 0, width, height);
			}
			break;

		case DM_SCALEDIAG1:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y, newWidth, newHeight);
				else
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y, width-newWidth, height-newHeight);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x, y, newWidth, newHeight, tmpBitmap, 0, 0, width, height);
				else
					m_ScreenBitmap.Draw(x, y, width-newWidth, height-newHeight, tmpBitmap, 0, 0, width, height);
			}
			break;

		case DM_SCALEDIAG2:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x+width-newWidth, y+height-newHeight, newWidth, newHeight);
				else
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x+newWidth, y+newHeight, width-newWidth, height-newHeight);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x+width-newWidth, y+height-newHeight, newWidth, newHeight, tmpBitmap, 0, 0, width, height);
				else
					m_ScreenBitmap.Draw(x+newWidth, y+newHeight, width-newWidth, height-newHeight, tmpBitmap, 0, 0, width, height);
			}
			break;

		case DM_SCALEDIAG3:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x+width-newWidth, y, newWidth, newHeight);
				else
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x+newWidth, y, width-newWidth, height-newHeight);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x+width-newWidth, y, newWidth, newHeight, tmpBitmap, 0, 0, width, height);
				else
					m_ScreenBitmap.Draw(x+newWidth, y, width-newWidth, height-newHeight, tmpBitmap, 0, 0, width, height);
			}
			break;

		case DM_SCALEDIAG4:
			{
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y+height-newHeight, newWidth, newHeight);
				else
					tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y+newHeight, width-newWidth, height-newHeight);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					m_ScreenBitmap.Draw(x, y+height-newHeight, newWidth, newHeight, tmpBitmap, 0, 0, width, height);
				else
					m_ScreenBitmap.Draw(x, y+newHeight, width-newWidth, height-newHeight, tmpBitmap, 0, 0, width, height);
			}
			break;

		case DM_VSTRIPS:
			{
				int stripSize = max((int)((float)width * 0.01f), 1);
				int numberStrips = width / stripSize;
				if ((width % stripSize) != 0)
					numberStrips++;
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				for (int i=0; i<numberStrips; i++)
				{
					if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					{
						if ((i % 2) == 0)
							m_ScreenBitmap.Draw(x+i*stripSize, y, stripSize, newHeight, tmpBitmap, i*stripSize, 0);
						else
							m_ScreenBitmap.Draw(x+i*stripSize, y+height-newHeight, stripSize, newHeight, tmpBitmap, i*stripSize, height-newHeight);
					}
					else
					{
						if ((i % 2) == 0)
							m_ScreenBitmap.Draw(x+i*stripSize, y, stripSize, height-newHeight, tmpBitmap, i*stripSize, 0);
						else
							m_ScreenBitmap.Draw(x+i*stripSize, y+newHeight, stripSize, height-newHeight, tmpBitmap, i*stripSize, newHeight);
					}
				}
			}
			break;

		case DM_HSTRIPS:
			{
				int stripSize = max((int)((float)height * 0.01f), 1);
				int numberStrips = height / stripSize;
				if ((height % stripSize) != 0)
					numberStrips++;
				int newWidth = (int)(width * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = (int)(height * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				for (int i=0; i<numberStrips; i++)
				{
					if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
					{
						if ((i % 2) == 0)
							m_ScreenBitmap.Draw(x, y+i*stripSize, newWidth, stripSize, tmpBitmap, 0, i*stripSize);
						else
							m_ScreenBitmap.Draw(x+width-newWidth, y+i*stripSize, newWidth, stripSize, tmpBitmap, width-newWidth, i*stripSize);
					}
					else
					{
						if ((i % 2) == 0)
							m_ScreenBitmap.Draw(x, y+i*stripSize, width-newWidth, stripSize, tmpBitmap, 0, i*stripSize);
						else
							m_ScreenBitmap.Draw(x+newWidth, y+i*stripSize, width-newWidth, stripSize, tmpBitmap, newWidth, i*stripSize);
					}
				}
			}
			break;

		case DM_VBLINDS:
			{
				int stripSize = max((int)((float)width * 0.05f), 1);
				int numberStrips = width / stripSize;
				if ((width % stripSize) != 0)
					numberStrips++;
				int newWidth = (int)(stripSize * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				int newHeight = height;
				tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				for (int i=0; i<numberStrips; i++)
				{
					if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
						m_ScreenBitmap.Draw(x+i*stripSize, y, newWidth, newHeight, tmpBitmap, i*stripSize, 0);
					else
						m_ScreenBitmap.Draw(x+i*stripSize, y, stripSize-newWidth, newHeight, tmpBitmap, i*stripSize, 0);
				}
			}
			break;

		case DM_HBLINDS:
			{
				int stripSize = max((int)((float)height * 0.05f), 1);
				int numberStrips = height / stripSize;
				if ((height % stripSize) != 0)
					numberStrips++;
				int newWidth = width;
				int newHeight = (int)(stripSize * m_lpImages[iImage].animDesc.drawingParams.drawPercent);
				tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y);
				tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				for (int i=0; i<numberStrips; i++)
				{
					if (!m_lpImages[iImage].animDesc.drawingParams.bInverse)
						m_ScreenBitmap.Draw(x, y+i*stripSize, newWidth, newHeight, tmpBitmap, 0, i*stripSize);
					else
						m_ScreenBitmap.Draw(x, y+i*stripSize, newWidth, stripSize-newHeight, tmpBitmap, 0, i*stripSize);
				}
			}
			break;

		default:
			{
				tmpBitmap.Draw(0, 0, width, height, m_ScreenBitmap, x, y);
				if (m_lpImages[iImage].animDesc.dwAnimationFlags & AT_ROTATE)
					tmpBitmap.DrawTransparent(0, 0, width, height, tmpBitmap1, 0, 0, _alpha, tmpBitmap1.GetPixel(0, 0));
				else if (m_lpImages[iImage].animDesc.dwAnimationFlags & AT_MORPH)
				{
					_QUAD morphQuad = m_lpImages[iImage].animDesc.morphQuad;
					morphQuad.p1.x -= m_lpImages[iImage].animDesc.animationParams[2].rect.left;
					morphQuad.p1.y -= m_lpImages[iImage].animDesc.animationParams[2].rect.top;
					morphQuad.p2.x -= m_lpImages[iImage].animDesc.animationParams[2].rect.left;
					morphQuad.p2.y -= m_lpImages[iImage].animDesc.animationParams[2].rect.top;
					morphQuad.p3.x -= m_lpImages[iImage].animDesc.animationParams[2].rect.left;
					morphQuad.p3.y -= m_lpImages[iImage].animDesc.animationParams[2].rect.top;
					morphQuad.p4.x -= m_lpImages[iImage].animDesc.animationParams[2].rect.left;
					morphQuad.p4.y -= m_lpImages[iImage].animDesc.animationParams[2].rect.top;
					tmpBitmap.Draw(morphQuad, tmpBitmap1, _alpha);
				}
				else
					tmpBitmap.Draw(0, 0, width, height, tmpBitmap1, 0, 0, _alpha);
				m_ScreenBitmap.Draw(x, y, width, height, tmpBitmap, 0, 0);
			}
	}
}

void CEffectorBuilder::SetQuality(_QUALITYTYPE quality)
{
	// Set quality
	m_iQuality = quality;

	switch(m_iQuality)
	{
		case QT_LOW:
			{
				// Set low quality
				m_ScreenBitmap.SetResampleMode(RM_NEARESTNEIGHBOUR);
			}
			break;

		case QT_NORMAL:
			{
				// Set normal quality
				m_ScreenBitmap.SetResampleMode(RM_BILINEAR);
			}
			break;

		case QT_HIGH:
			{
				// Set high quality
				m_ScreenBitmap.SetResampleMode(RM_BICUBIC);
			}
			break;
	}
}

_QUALITYTYPE CEffectorBuilder::GetQuality()
{
	// Get quality
	return m_iQuality;
}
