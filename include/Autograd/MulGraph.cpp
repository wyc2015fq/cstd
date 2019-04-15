
/*
	2018.05.24
	Created by AcrylicShrimp.
*/

#include "MulGraph.h"

namespace Autograd
{
	MulGraph::MulGraph(const GraphNode &sLeft, const GraphNode &sRight) :
		Operand2Graph(sLeft, sRight)
	{
		this->forward();
	}

	const Shape &MulGraph::shape() const
	{
		return this->pLeft->shape();
	}

	Tensor MulGraph::forward()
	{
		return this->pLeft->forward() * this->pRight->forward();
	}

	Tensor MulGraph::backwardPath(Graph *pPrev)
	{
		return this->pLeft.get() == pPrev ? this->pRight->forward() * this->backward() : this->pLeft->forward() * this->backward();
	}
}