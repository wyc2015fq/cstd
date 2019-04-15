
/*
	2018.05.23
	Created by AcrylicShrimp.
*/

#include "AddGraph.h"

namespace Autograd
{
	AddGraph::AddGraph(const GraphNode &sLeft, const GraphNode &sRight) :
		Operand2Graph(sLeft, sRight)
	{
		this->forward();
	}

	const Shape &AddGraph::shape() const
	{
		return this->pLeft->shape();
	}

	Tensor AddGraph::forward()
	{
		return this->pLeft->forward() + this->pRight->forward();
	}

	Tensor AddGraph::backwardPath(Graph *pPrev)
	{
		return this->backward();
	}
}