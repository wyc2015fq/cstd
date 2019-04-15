
/*
	2018.05.24
	Created by AcrylicShrimp.
*/

#ifndef _CLASS_AUTOGRAD_GRAPHOP_H

#define _CLASS_AUTOGRAD_GRAPHOP_H

#include "Graph.h"
#include "Shape.h"
#include "Tensor.h"
#include "ValueGraph.h"

#include <initializer_list>
#include <utility>

namespace Autograd
{
	class GraphOp final
	{
	public:
		GraphOp() = delete;
		GraphOp(const GraphOp &sSrc) = delete;
		~GraphOp() = delete;
		
	public:
		GraphOp &operator=(const GraphOp &sSrc) = delete;
		
	public:
		template<class T, class ...P> inline static std::shared_ptr<T> make(P &&...sParam);
		static GraphNode mean(const GraphNode &sLeft);
		static GraphNode matMul(const GraphNode &sLeft, const GraphNode &sRight);
		static std::shared_ptr<ValueGraph> tensor(const Shape &sShape);
		static std::shared_ptr<ValueGraph> tensor(const Shape &sShape, std::initializer_list<float> &sTensor);
	};

	GraphNode operator+(const GraphNode &sLeft, const GraphNode &sRight);
	GraphNode operator-(const GraphNode &sLeft, const GraphNode &sRight);
	GraphNode operator*(const GraphNode &sLeft, const GraphNode &sRight);
	GraphNode operator/(const GraphNode &sLeft, const GraphNode &sRight);

	template<class T, class ...P> inline std::shared_ptr<T> GraphOp::make(P &&...sParam)
	{
		return std::make_shared<T>(std::forward<P>(sParam)...);
	}
}

#endif