
/*
	2018.05.24
	Created by AcrylicShrimp.
*/

#include "MeanGraph.h"

namespace Autograd
{
	MeanGraph::MeanGraph(const GraphNode &sLeft) :
		Operand1Graph(sLeft),
		sShape{1}
	{
		this->forward();
	}

	const Shape &MeanGraph::shape() const
	{
		return this->sShape;
	}

	Tensor MeanGraph::forward()
	{
		Tensor sForward(this->pLeft->forward());
		return Tensor{Shape{1}, {std::accumulate(sForward.tensor().cbegin(), sForward.tensor().cend(), .0f) / sForward.tensor().size()}};
	}

	Tensor MeanGraph::backwardPath(Graph *pPrev)
	{
		return Tensor{Shape{1}, {1.f / this->pLeft->shape().element()}};
	}
}