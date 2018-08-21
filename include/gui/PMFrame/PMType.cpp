#include "StdAfx.h"
#include "PMType.h"

void Assign( IntPoint &sourcePoint,const POINT &targetPoint )
{
	sourcePoint.x=targetPoint.x;
	sourcePoint.y=targetPoint.y;
}

void Assign( IntSize &sourceSize,const SIZE &targetSize )
{
	sourceSize.width=targetSize.cx;
	sourceSize.height=targetSize.cy;
}

void Assign( IntRect &sourceRect,const RECT &targetRect )
{
	sourceRect.top=targetRect.top;
	sourceRect.left=targetRect.left;
	sourceRect.right=targetRect.right;
	sourceRect.bottom=targetRect.bottom;
}

void Assign( RECT &sourceRect,const IntRect &targetRect )
{
	sourceRect.top=targetRect.top;
	sourceRect.left=targetRect.left;
	sourceRect.right=targetRect.right;
	sourceRect.bottom=targetRect.bottom;
}

IntRect CreateRect( const IntSize &sourceSize )
{
	return IntRect(0,0,sourceSize.width,sourceSize.height);
}

IntRect CreateRect( RECT &source )
{
	return IntRect(source.left,source.top,source.right,source.bottom);
}

IntSize CreateSize(SIZE &size)
{
	return IntSize(size.cx,size.cy);
}