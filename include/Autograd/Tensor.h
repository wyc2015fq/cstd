
/*
	2018.05.23
	Created by AcrylicShrimp.
*/

#ifndef _CLASS_AUTOGRAD_TENSOR_H

#define _CLASS_AUTOGRAD_TENSOR_H

#include "Shape.h"

#include <algorithm>
#include <cstddef>
#include <exception>
#include <initializer_list>
#include <string>
#include <vector>

namespace Autograd
{
	class Tensor
	{
	private:
		Shape sShape;
		std::vector<float> sTensor;
		
	public:
		Tensor(const Shape &sShape);
		Tensor(const Shape &sShape, const std::vector<float> &sTensor);
		Tensor(const Shape &sShape, std::initializer_list<float> sTensor);
		Tensor(const Tensor &sSrc) = default;
		Tensor(Tensor &&sSrc) = default;
		~Tensor() = default;
		
	public:
		Tensor &operator=(const std::vector<float> &sTensor);
		Tensor &operator=(std::initializer_list<float> sTensor);
		Tensor &operator=(const Tensor &sSrc) = default;
		Tensor &operator=(Tensor &&sSrc) = default;
		Tensor operator+() const;
		Tensor operator-() const;
		Tensor &operator+=(float nRight);
		Tensor &operator+=(const Tensor &sRight);
		Tensor &operator-=(float nRight);
		Tensor &operator-=(const Tensor &sRight);
		Tensor &operator*=(float nRight);
		Tensor &operator*=(const Tensor &sRight);
		Tensor &operator/=(float nRight);
		Tensor &operator/=(const Tensor &sRight);
		bool operator==(const Tensor &sRight) const;
		bool operator!=(const Tensor &sRight) const;
		
	public:
		inline float *data();
		inline const float *data() const;
		inline std::size_t size() const;
		inline const Shape &shape() const;
		inline std::vector<float> &tensor();
		inline const std::vector<float> &tensor() const;
		inline bool isScalar() const;
		inline float scalar() const;
		std::string toString() const;
		std::string toString(std::size_t nDimension, std::size_t nDimensionSize, std::size_t nBeginIndex) const;
		static Tensor zero(const Shape &sShape);
		static Tensor one(const Shape &sShape);
		static Tensor contant(const Shape &sShape, float nConstant);
	};

	Tensor operator+(const Tensor &sLeft, float nRight);
	Tensor operator+(float nLeft, const Tensor &sRight);
	Tensor operator+(const Tensor &sLeft, const Tensor &sRight);
	Tensor operator-(const Tensor &sLeft, float nRight);
	Tensor operator-(float nLeft, const Tensor &sRight);
	Tensor operator-(const Tensor &sLeft, const Tensor &sRight);
	Tensor operator*(const Tensor &sLeft, float nRight);
	Tensor operator*(float nLeft, const Tensor &sRight);
	Tensor operator*(const Tensor &sLeft, const Tensor &sRight);
	Tensor operator/(const Tensor &sLeft, float nRight);
	Tensor operator/(float nLeft, const Tensor &sRight);
	Tensor operator/(const Tensor &sLeft, const Tensor &sRight);

	inline float *Tensor::data()
	{
		return this->sTensor.data();
	}

	inline const float *Tensor::data() const
	{
		return this->sTensor.data();
	}

	inline std::size_t Tensor::size() const
	{
		return this->sTensor.size();
	}

	inline const Shape &Tensor::shape() const
	{
		return this->sShape;
	}

	inline std::vector<float> &Tensor::tensor()
	{
		return this->sTensor;
	}

	inline const std::vector<float> &Tensor::tensor() const
	{
		return this->sTensor;
	}

	inline bool Tensor::isScalar() const
	{
		return this->size() == 1;
	}

	inline float Tensor::scalar() const
	{
		return this->sTensor.front();
	}
}

#endif