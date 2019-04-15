
/*
	2018.05.24
	Created by AcrylicShrimp.
*/

#ifndef _CLASS_AUTOGRAD_OPERAND2GRAPH_H

#define _CLASS_AUTOGRAD_OPERAND2GRAPH_H

#include "Operand1Graph.h"

namespace Autograd
{
	class Operand2Graph : public Operand1Graph
	{
	protected:
		GraphNode pRight;
		
	public:
		Operand2Graph(const GraphNode &pLeft, const GraphNode &pRight);
		Operand2Graph(const Operand2Graph &sSrc) = default;
		~Operand2Graph() = default;
		
	public:
		Operand2Graph &operator=(const Operand2Graph &sSrc) = default;
		
	public:
		inline Graph *right();
		inline const Graph *right() const;
	};

	inline Graph *Operand2Graph::right()
	{
		return this->pRight.get();
	}

	inline const Graph *Operand2Graph::right() const
	{
		return this->pRight.get();
	}
}

#endif