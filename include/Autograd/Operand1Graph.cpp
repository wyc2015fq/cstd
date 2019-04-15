
/*
	2018.05.24
	Created by AcrylicShrimp.
*/

#include "Operand1Graph.h"

namespace Autograd
{
	Operand1Graph::Operand1Graph(const GraphNode &pLeft) :
		pLeft{pLeft}
	{
		this->beNext(this->pLeft.get());
	}
}