#ifndef __TRANSLATECANVAS_H__
#define __TRANSLATECANVAS_H__

#include "WarperCanvas.h"

namespace ImageWarper
{
	class DLLEXPORTED TranslateCanvas : public WarperCanvas
	{
	public:
		TranslateCanvas(Rectangle &boundary, int iMaxWidth, int iMaxHeight);
		virtual ~TranslateCanvas();

		virtual void DoForce(Point &ptStart, Point &ptEnd, PointArray2D *gridPoints, Rectangle &boundary);
	};
}

#endif