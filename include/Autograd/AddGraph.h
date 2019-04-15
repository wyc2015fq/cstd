
/*
	2018.05.23
	Created by AcrylicShrimp.
*/

#ifndef _CLASS_AUTOGRAD_ADDGRAPH_H

#define _CLASS_AUTOGRAD_ADDGRAPH_H

#include "Operand2Graph.h"
#include "Shape.h"
#include "Tensor.h"

namespace Autograd
{
	class AddGraph final : public Operand2Graph
	{
	public:
		AddGraph(const GraphNode &sLeft, const GraphNode &sRight);
		AddGraph(const AddGraph &sSrc) = default;
		~AddGraph() = default;
		
	public:
		AddGraph &operator=(const AddGraph &sSrc) = default;
		
	public:
		virtual const Shape &shape() const override;
		virtual Tensor forward() override;

	protected:
		virtual Tensor backwardPath(Graph *pPrev) override;
	};
}

#endif