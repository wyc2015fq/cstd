
/*
	2018.05.24
	Created by AcrylicShrimp.
*/

#include "DivGraph.h"

namespace Autograd
{
	DivGraph::DivGraph(const GraphNode &sLeft, const GraphNode &sRight) :
		Operand2Graph(sLeft, sRight)
	{
		this->forward();
	}

	const Shape &DivGraph::shape() const
	{
		return this->pLeft->shape();
	}

	Tensor DivGraph::forward()
	{
		return this->pLeft->forward() / this->pRight->forward();
	}

	Tensor DivGraph::backwardPath(Graph *pPrev)
	{
		return this->pLeft.get() == pPrev ? this->backward() / this->pRight->forward() : this->pLeft->forward() * this->backward();
	}
}