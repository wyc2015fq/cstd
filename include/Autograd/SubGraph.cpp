
/*
	2018.05.24
	Created by AcrylicShrimp.
*/

#include "SubGraph.h"

namespace Autograd
{
	SubGraph::SubGraph(const GraphNode &sLeft, const GraphNode &sRight) :
		Operand2Graph(sLeft, sRight)
	{
		this->forward();
	}

	const Shape &SubGraph::shape() const
	{
		return this->pLeft->shape();
	}

	Tensor SubGraph::forward()
	{
		return this->pLeft->forward() - this->pRight->forward();
	}

	Tensor SubGraph::backwardPath(Graph *pPrev)
	{
		return this->pLeft.get() == pPrev ? this->backward() : -this->backward();
	}
}