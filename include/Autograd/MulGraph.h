
/*
	2018.05.24
	Created by AcrylicShrimp.
*/

#ifndef _CLASS_AUTOGRAD_MULGRAPH_H

#define _CLASS_AUTOGRAD_MULGRAPH_H

#include "Operand2Graph.h"
#include "Shape.h"
#include "Tensor.h"

namespace Autograd
{
	class MulGraph final : public Operand2Graph
	{
	public:
		MulGraph(const GraphNode &sLeft, const GraphNode &sRight);
		MulGraph(const MulGraph &sSrc) = default;
		~MulGraph() = default;
		
	public:
		MulGraph &operator=(const MulGraph &sSrc) = default;
		
	public:
		virtual const Shape &shape() const override;
		virtual Tensor forward() override;

	protected:
		virtual Tensor backwardPath(Graph *pPrev) override;
	};
}

#endif