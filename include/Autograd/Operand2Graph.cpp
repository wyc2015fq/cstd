
/*
	2018.05.24
	Created by AcrylicShrimp.
*/

#include "Operand2Graph.h"

namespace Autograd
{
	Operand2Graph::Operand2Graph(const GraphNode &pLeft, const GraphNode &pRight) :
		Operand1Graph(pLeft),
		pRight{pRight}
	{
		this->beNext(this->pRight.get());
	}
}