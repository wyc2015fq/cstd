
/*
	2018.05.24
	Created by AcrylicShrimp.
*/

#ifndef _CLASS_AUTOGRAD_MEANGRAPH_H

#define _CLASS_AUTOGRAD_MEANGRAPH_H

#include "Operand1Graph.h"
#include "Shape.h"
#include "Tensor.h"

#include <numeric>

namespace Autograd
{
	class MeanGraph final : public Operand1Graph
	{
	protected:
		Shape sShape;
		
	public:
		MeanGraph(const GraphNode &sLeft);
		MeanGraph(const MeanGraph &sSrc) = default;
		~MeanGraph() = default;
		
	public:
		MeanGraph &operator=(const MeanGraph &sSrc) = default;
		
	public:
		virtual const Shape &shape() const override;
		virtual Tensor forward() override;

	protected:
		virtual Tensor backwardPath(Graph *pPrev) override;
	};
}

#endif