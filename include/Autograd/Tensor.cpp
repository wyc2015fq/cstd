
/*
	2018.05.23
	Created by AcrylicShrimp.
*/

#include "Tensor.h"

namespace Autograd
{
	Tensor::Tensor(const Shape &sShape) :
		sShape{sShape},
		sTensor(sShape.element())
	{
		//Empty.
	}

	Tensor::Tensor(const Shape &sShape, const std::vector<float> &sTensor) :
		sShape{sShape},
		sTensor(sTensor)
	{
		//Empty.
	}

	Tensor::Tensor(const Shape &sShape, std::initializer_list<float> sTensor) :
		sShape{sShape},
		sTensor(sTensor)
	{
		//Empty.
	}

	Tensor &Tensor::operator=(const std::vector<float> &sTensor)
	{
		this->sTensor = sTensor;
		return *this;
	}

	Tensor &Tensor::operator=(std::initializer_list<float> sTensor)
	{
		this->sTensor = sTensor;
		return *this;
	}

	Tensor Tensor::operator+() const
	{
		return Tensor{*this};
	}

	Tensor Tensor::operator-() const
	{
		return Tensor{*this} * -1.f;
	}

	Tensor &Tensor::operator+=(float nRight)
	{
		for (auto &nTensor : this->sTensor)
			nTensor += nRight;

		return *this;
	}

	Tensor &Tensor::operator+=(const Tensor &sRight)
	{
		if (sRight.isScalar())
			return this->operator+=(sRight.scalar());

		if (this->sShape != sRight.sShape)
			throw std::exception{"shape mismatch"};

		for (std::size_t nIndex{0}, nSize{this->sTensor.size()}; nIndex < nSize; ++nIndex)
			this->sTensor[nIndex] += sRight.sTensor[nIndex];

		return *this;
	}

	Tensor &Tensor::operator-=(float nRight)
	{
		for (auto &nTensor : this->sTensor)
			nTensor -= nRight;

		return *this;
	}

	Tensor &Tensor::operator-=(const Tensor &sRight)
	{
		if (sRight.isScalar())
			return this->operator-=(sRight.scalar());

		if (this->sShape != sRight.sShape)
			throw std::exception{"shape mismatch"};

		for (std::size_t nIndex{0}, nSize{this->sTensor.size()}; nIndex < nSize; ++nIndex)
			this->sTensor[nIndex] -= sRight.sTensor[nIndex];

		return *this;
	}

	Tensor &Tensor::operator*=(float nRight)
	{
		for (auto &nTensor : this->sTensor)
			nTensor *= nRight;

		return *this;
	}

	Tensor &Tensor::operator*=(const Tensor &sRight)
	{
		if (sRight.isScalar())
			return this->operator*=(sRight.scalar());

		if (this->sShape != sRight.sShape)
			throw std::exception{"shape mismatch"};

		for (std::size_t nIndex{0}, nSize{this->sTensor.size()}; nIndex < nSize; ++nIndex)
			this->sTensor[nIndex] *= sRight.sTensor[nIndex];

		return *this;
	}

	Tensor &Tensor::operator/=(float nRight)
	{
		for (auto &nTensor : this->sTensor)
			nTensor /= nRight;

		return *this;
	}

	Tensor &Tensor::operator/=(const Tensor &sRight)
	{
		if (sRight.isScalar())
			return this->operator/=(sRight.scalar());

		if (this->sShape != sRight.sShape)
			throw std::exception{"shape mismatch"};

		for (std::size_t nIndex{0}, nSize{this->sTensor.size()}; nIndex < nSize; ++nIndex)
			this->sTensor[nIndex] /= sRight.sTensor[nIndex];

		return *this;
	}

	bool Tensor::operator==(const Tensor &sRight) const
	{
		if (this->sShape != sRight.sShape)
			return false;

		for (std::size_t nIndex{0}, nSize{this->sTensor.size()}; nIndex < nSize; ++nIndex)
			if (this->sTensor[nIndex] != sRight.sTensor[nIndex])
				return false;

		return true;
	}

	bool Tensor::operator!=(const Tensor &sRight) const
	{
		if (this->sShape != sRight.sShape)
			return true;

		for (std::size_t nIndex{0}, nSize{this->sTensor.size()}; nIndex < nSize; ++nIndex)
			if (this->sTensor[nIndex] != sRight.sTensor[nIndex])
				return true;

		return false;
	}

	std::string Tensor::toString() const
	{
		std::string sResult{"["};
		std::size_t nNextDimensionSize{this->size() / this->sShape.size(0)};

		for (std::size_t nIndex{0}, nSize{static_cast<std::size_t>(this->sShape.size(0))}; nIndex < nSize; ++nIndex)
		{
			if (nIndex)
				sResult += ",\n";

			sResult += this->toString(0, nNextDimensionSize, nNextDimensionSize * nIndex);
		}

		return sResult += "]";
	}

	std::string Tensor::toString(std::size_t nDimension, std::size_t nDimensionSize, std::size_t nBeginIndex) const
	{
		if (nDimension + 1 == this->sShape.rank())
			return std::to_string(this->sTensor[nBeginIndex]);

		std::string sResult{"["};
		std::size_t nNextDimensionSize{nDimensionSize / this->sShape.size(nDimension + 1)};

		for (std::size_t nIndex{0}, nSize{static_cast<std::size_t>(this->sShape.size(nDimension + 1))}; nIndex < nSize; ++nIndex)
		{
			if (nIndex)
				sResult += ", ";

			sResult += this->toString(nDimension + 1, nNextDimensionSize, nBeginIndex + nNextDimensionSize * nIndex);
		}

		return sResult += "]";
	}

	Tensor Tensor::zero(const Shape &sShape)
	{
		Tensor sTensor{sShape};

		std::fill(sTensor.sTensor.begin(), sTensor.sTensor.end(), .0f);

		return sTensor;
	}

	Tensor Tensor::one(const Shape &sShape)
	{
		Tensor sTensor{sShape};

		std::fill(sTensor.sTensor.begin(), sTensor.sTensor.end(), 1.f);

		return sTensor;
	}

	Tensor Tensor::contant(const Shape &sShape, float nConstant)
	{
		Tensor sTensor{sShape};

		std::fill(sTensor.sTensor.begin(), sTensor.sTensor.end(), nConstant);

		return sTensor;
	}

	Tensor operator+(const Tensor &sLeft, float nRight)
	{
		return Tensor{sLeft} += nRight;
	}

	Tensor operator+(float nLeft, const Tensor &sRight)
	{
		return Tensor{sRight} += nLeft;
	}

	Tensor operator+(const Tensor &sLeft, const Tensor &sRight)
	{
		return Tensor{sLeft} += sRight;
	}

	Tensor operator-(const Tensor &sLeft, float nRight)
	{
		return Tensor{sLeft} -= nRight;
	}

	Tensor operator-(float nLeft, const Tensor &sRight)
	{
		return Tensor::contant(sRight.shape(), nLeft) -= sRight;
	}

	Tensor operator-(const Tensor &sLeft, const Tensor &sRight)
	{
		return Tensor{sLeft} -= sRight;
	}

	Tensor operator*(const Tensor &sLeft, float nRight)
	{
		return Tensor{sLeft} *= nRight;
	}

	Tensor operator*(float nLeft, const Tensor &sRight)
	{
		return Tensor{sRight} *= nLeft;
	}

	Tensor operator*(const Tensor &sLeft, const Tensor &sRight)
	{
		return Tensor{sLeft} *= sRight;
	}

	Tensor operator/(const Tensor &sLeft, float nRight)
	{
		return Tensor{sLeft} /= nRight;
	}

	Tensor operator/(float nLeft, const Tensor &sRight)
	{
		return Tensor::contant(sRight.shape(), nLeft) /= sRight;
	}

	Tensor operator/(const Tensor &sLeft, const Tensor &sRight)
	{
		return Tensor{sLeft} /= sRight;
	}
}