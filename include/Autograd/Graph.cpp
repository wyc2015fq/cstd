
/*
	2018.05.05
	Created by AcrylicShrimp.
*/

#include "Graph.h"

namespace Autograd
{
	Graph::Graph() :
		pNext{nullptr}
	{
		//Empty.
	}

	Tensor Graph::backward()
	{
		if (this->pNext)
			return this->pNext->backwardPath(this);

		return Tensor::one(this->shape());
	}
}