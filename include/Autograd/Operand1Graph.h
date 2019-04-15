
/*
	2018.05.24
	Created by AcrylicShrimp.
*/

#ifndef _CLASS_AUTOGRAD_OPERAND1GRAPH_H

#define _CLASS_AUTOGRAD_OPERAND1GRAPH_H

#include "Graph.h"

namespace Autograd
{
	class Operand1Graph : public Graph
	{
	protected:
		GraphNode pLeft;
		
	public:
		Operand1Graph(const GraphNode &pLeft);
		Operand1Graph(const Operand1Graph &sSrc) = default;
		~Operand1Graph() = default;
		
	public:
		Operand1Graph &operator=(const Operand1Graph &sSrc) = default;
		
	public:
		inline Graph *left();
		inline const Graph *left() const;
	};

	inline Graph *Operand1Graph::left()
	{
		return this->pLeft.get();
	}

	inline const Graph *Operand1Graph::left() const
	{
		return this->pLeft.get();
	}
}

#endif