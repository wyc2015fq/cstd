
/*
	2018.05.05
	Created by AcrylicShrimp.
*/

#include "Shape.h"

namespace Autograd
{
	Shape::Shape(std::initializer_list<uint32_t> sShape) :
		sShape{sShape.begin(), sShape.end()}
	{
		//Empty.
	}

	bool Shape::operator==(std::initializer_list<uint32_t> sShape) const
	{
		if (this->rank() != sShape.size())
			return false;

		const auto pShape{sShape.begin()};

		for (std::size_t nAxis{0}, nMaxAxis{this->rank()}; nAxis < nMaxAxis; ++nAxis)
			if (this->size(nAxis) != pShape[nAxis])
				return false;

		return true;
	}

	bool Shape::operator==(const Shape &sSrc) const
	{
		if (this->rank() != sSrc.rank())
			return false;

		for (std::size_t nAxis{0}, nMaxAxis{this->rank()}; nAxis < nMaxAxis; ++nAxis)
			if (this->size(nAxis) != sSrc.size(nAxis))
				return false;

		return true;
	}

	bool Shape::operator!=(std::initializer_list<uint32_t> sShape) const
	{
		if (this->rank() != sShape.size())
			return true;

		const auto pShape{sShape.begin()};

		for (std::size_t nAxis{0}, nMaxAxis{this->rank()}; nAxis < nMaxAxis; ++nAxis)
			if (this->size(nAxis) != pShape[nAxis])
				return true;

		return false;
	}

	bool Shape::operator!=(const Shape &sSrc) const
	{
		if (this->rank() != sSrc.rank())
			return true;

		for (std::size_t nAxis{0}, nMaxAxis{this->rank()}; nAxis < nMaxAxis; ++nAxis)
			if (this->size(nAxis) != sSrc.size(nAxis))
				return true;

		return false;
	}

	std::string Shape::toString() const
	{
		std::string sResult{"["};

		for (std::size_t nAxis{0}, nMaxAxis{this->rank()}; nAxis < nMaxAxis; ++nAxis)
		{
			if (nAxis)
				sResult += ", ";

			sResult += std::to_string(this->size(nAxis));
		}

		return sResult += "]";
	}

	void Shape::normalize()
	{
		while (!this->sShape.empty() && this->sShape.front() == 1)
			this->sShape.erase(this->sShape.begin());
	}
}