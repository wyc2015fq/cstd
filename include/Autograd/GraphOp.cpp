
/*
	2018.05.24
	Created by AcrylicShrimp.
*/

#include "GraphOp.h"

#include "AddGraph.h"
#include "SubGraph.h"
#include "MulGraph.h"
#include "DivGraph.h"
#include "MeanGraph.h"
#include "MatMulGraph.h"

namespace Autograd
{
	GraphNode GraphOp::mean(const GraphNode &sLeft)
	{
		return GraphOp::make<MeanGraph>(sLeft);
	}

	GraphNode GraphOp::matMul(const GraphNode &sLeft, const GraphNode &sRight)
	{
		return GraphOp::make<MatMulGraph>(sLeft, sRight);
	}

	std::shared_ptr<ValueGraph> GraphOp::tensor(const Shape &sShape)
	{
		return GraphOp::make<ValueGraph>(sShape);
	}

	std::shared_ptr<ValueGraph> GraphOp::tensor(const Shape &sShape, std::initializer_list<float> &sTensor)
	{
		return GraphOp::make<ValueGraph>(Tensor{sShape, sTensor});
	}

	GraphNode operator+(const GraphNode &sLeft, const GraphNode &sRight)
	{
		return GraphOp::make<AddGraph>(sLeft, sRight);
	}

	GraphNode operator-(const GraphNode &sLeft, const GraphNode &sRight)
	{
		return GraphOp::make<SubGraph>(sLeft, sRight);
	}

	GraphNode operator*(const GraphNode &sLeft, const GraphNode &sRight)
	{
		return GraphOp::make<MulGraph>(sLeft, sRight);
	}

	GraphNode operator/(const GraphNode &sLeft, const GraphNode &sRight)
	{
		return GraphOp::make<DivGraph>(sLeft, sRight);
	}
}