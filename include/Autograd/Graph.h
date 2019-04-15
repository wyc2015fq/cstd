
/*
	2018.05.05
	Created by AcrylicShrimp.
*/

#ifndef _CLASS_AUTOGRAD_GRAPH_H

#define _CLASS_AUTOGRAD_GRAPH_H

#include "Shape.h"
#include "Tensor.h"

#include <memory>

namespace Autograd
{
	using GraphNode = std::shared_ptr<class Graph>;

	class Graph
	{
	protected:
		Graph *pNext;

	public:
		Graph();
		Graph(const Graph &sSrc) = default;
		virtual ~Graph() = default;

	public:
		Graph &operator=(const Graph &sSrc) = default;

	public:
		virtual const Shape &shape() const = 0;
		virtual Tensor forward() = 0;
		Tensor backward();
		
	protected:
		inline void beNext(Graph *pGraph);
		virtual Tensor backwardPath(Graph *pPrev) = 0;
	};

	inline void Graph::beNext(Graph *pGraph)
	{
		pGraph->pNext = this;
	}
}

#endif