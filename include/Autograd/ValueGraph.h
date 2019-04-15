
/*
	2018.05.24
	Created by AcrylicShrimp.
*/

#ifndef _CLASS_AUTOGRAD_VALUEGRAPH_H

#define _CLASS_AUTOGRAD_VALUEGRAPH_H

#include "Graph.h"
#include "Shape.h"
#include "Tensor.h"

namespace Autograd
{
	class ValueGraph final : public Graph
	{
	protected:
		Tensor sValue;
		
	public:
		ValueGraph(const Shape &sShape);
		ValueGraph(const Tensor &sValue);
		ValueGraph(const ValueGraph &sSrc) = default;
		~ValueGraph() = default;
		
	public:
		ValueGraph &operator=(const ValueGraph &sSrc) = default;
		
	public:
		inline Tensor &value();
		inline const Tensor &value() const;
		virtual const Shape & shape() const override;
		virtual Tensor forward() override;

	protected:
		virtual Tensor backwardPath(Graph *pPrev) override;
	};

	inline Tensor &ValueGraph::value()
	{
		return this->sValue;
	}

	inline const Tensor &ValueGraph::value() const
	{
		return this->sValue;
	}
}

#endif