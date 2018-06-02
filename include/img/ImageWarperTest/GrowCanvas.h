#ifndef __GROWCANVAS_H__
#define __GROWCANVAS_H__

#include "WarperCanvas.h"

namespace ImageWarper
{
	class DLLEXPORTED GrowCanvas : public WarperCanvas
	{
		float m_GrowFactor;
		bool m_bGrow;

	public:
		GrowCanvas(Rectangle &boundary, int iMaxWidth, int iMaxHeight, bool bGrow = true);
		virtual ~GrowCanvas();

		virtual void DoForce(Point &ptStart, Point &ptEnd, PointArray2D *gridPoints, Rectangle &boundary);
	};
}

#endif