
/*
	2018.05.24
	Created by AcrylicShrimp.
*/

#ifndef _CLASS_AUTOGRAD_DIVGRAPH_H

#define _CLASS_AUTOGRAD_DIVGRAPH_H

#include "Operand2Graph.h"
#include "Shape.h"
#include "Tensor.h"

namespace Autograd
{
	class DivGraph final : public Operand2Graph
	{
	public:
		DivGraph(const GraphNode &sLeft, const GraphNode &sRight);
		DivGraph(const DivGraph &sSrc) = default;
		~DivGraph() = default;
		
	public:
		DivGraph &operator=(const DivGraph &sSrc) = default;
		
	public:
		virtual const Shape &shape() const override;
		virtual Tensor forward() override;

	protected:
		virtual Tensor backwardPath(Graph *pPrev) override;
	};
}

#endif