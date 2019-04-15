
/*
	2018.05.24
	Created by AcrylicShrimp.
*/

#ifndef _CLASS_AUTOGRAD_SUBGRAPH_H

#define _CLASS_AUTOGRAD_SUBGRAPH_H

#include "Operand2Graph.h"
#include "Shape.h"
#include "Tensor.h"

namespace Autograd
{
	class SubGraph final : public Operand2Graph
	{
	public:
		SubGraph(const GraphNode &sLeft, const GraphNode &sRight);
		SubGraph(const SubGraph &sSrc) = default;
		~SubGraph() = default;
		
	public:
		SubGraph &operator=(const SubGraph &sSrc) = default;
		
	public:
		virtual const Shape &shape() const override;
		virtual Tensor forward() override;

	protected:
		virtual Tensor backwardPath(Graph *pPrev) override;
	};
}

#endif