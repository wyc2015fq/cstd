
/*
	2018.05.24
	Created by AcrylicShrimp.
*/

#ifndef _CLASS_AUTOGRAD_MATMULGRAPH_H

#define _CLASS_AUTOGRAD_MATMULGRAPH_H

#include "Operand2Graph.h"
#include "Shape.h"
#include "Tensor.h"

#include <exception>

namespace Autograd
{
	class MatMulGraph final : public Operand2Graph
	{
	protected:
		Shape sShape;
		
	public:
		MatMulGraph(const GraphNode &sLeft, const GraphNode &sRight);
		MatMulGraph(const MatMulGraph &sSrc) = default;
		~MatMulGraph() = default;
		
	public:
		MatMulGraph &operator=(const MatMulGraph &sSrc) = default;
		
	public:
		virtual const Shape &shape() const override;
		virtual Tensor forward() override;

	protected:
		virtual Tensor backwardPath(Graph *pPrev) override;
	};
}

#endif