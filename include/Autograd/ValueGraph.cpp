
/*
	2018.05.24
	Created by AcrylicShrimp.
*/

#include "ValueGraph.h"

namespace Autograd
{
	ValueGraph::ValueGraph(const Shape &sShape) :
		sValue{sShape}
	{
		//Empty.
	}

	ValueGraph::ValueGraph(const Tensor &sValue) :
		sValue{sValue}
	{
		//Empty.
	}

	const Shape &ValueGraph::shape() const
	{
		return this->sValue.shape();
	}

	Tensor ValueGraph::forward()
	{
		return this->sValue;
	}

	Tensor ValueGraph::backwardPath(Graph *pPrev)
	{
		return this->sValue * this->backward();
	}
}