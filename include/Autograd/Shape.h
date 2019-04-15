
/*
	2018.05.05
	Created by AcrylicShrimp.
*/

#ifndef _CLASS_AUTOGRAD_SHAPE_H

#define _CLASS_AUTOGRAD_SHAPE_H

#include <cstddef>
#include <cstdint>
#include <exception>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

namespace Autograd
{
	class Shape final
	{
	private:
		std::vector<uint32_t> sShape;

	public:
		Shape(std::initializer_list<uint32_t> sShape);
		Shape(const Shape &sSrc) = default;
		Shape(Shape &&sSrc) = default;
		~Shape() = default;

	public:
		Shape &operator=(const Shape &sSrc) = default;
		Shape &operator=(Shape &&sSrc) = default;
		bool operator==(std::initializer_list<uint32_t> sShape) const;
		bool operator==(const Shape &sSrc) const;
		bool operator!=(std::initializer_list<uint32_t> sShape) const;
		bool operator!=(const Shape &sSrc) const;

	public:
		inline std::size_t rank() const;
		inline uint32_t size(std::size_t nAxis) const;
		inline uint32_t element() const;
		std::string toString() const;
		inline static Shape deduce(float nTensor);
		inline static Shape deduce(std::initializer_list<float> sTensor);
		inline static Shape deduce(std::initializer_list<std::initializer_list<float>> sTensor);
		inline static Shape deduce(std::initializer_list<std::initializer_list<std::initializer_list<float>>> sTensor);
		inline static Shape deduce(std::initializer_list<std::initializer_list<std::initializer_list<std::initializer_list<float>>>> sTensor);
		inline static Shape deduce(std::initializer_list<std::initializer_list<std::initializer_list<std::initializer_list<std::initializer_list<float>>>>> sTensor);
		inline static Shape deduce(std::initializer_list<std::initializer_list<std::initializer_list<std::initializer_list<std::initializer_list<std::initializer_list<float>>>>>> sTensor);

	private:
		template<class T, class ...P> inline static Shape deduceTensor(std::initializer_list<T> sTensor, P &&...sParam);
		template<class T, class ...P> inline static Shape deduceTensor(T sTensor, P &&...sParam);

	private:
		void normalize();
	};

	inline std::size_t Shape::rank() const
	{
		return this->sShape.size();
	}

	inline uint32_t Shape::size(std::size_t nAxis) const
	{
		return nAxis < this->rank() ? this->sShape[nAxis] : 0;
	}

	uint32_t Shape::element() const
	{
		uint32_t nResult{1};

		for (auto nSize : this->sShape)
			nResult *= nSize;

		return nResult;
	}

	inline Shape Shape::deduce(float nTensor)
	{
		return Shape::deduceTensor(nTensor);
	}

	inline Shape Shape::deduce(std::initializer_list<float> sTensor)
	{
		return Shape::deduceTensor(sTensor);
	}

	inline Shape Shape::deduce(std::initializer_list<std::initializer_list<float>> sTensor)
	{
		return Shape::deduceTensor(sTensor);
	}

	inline Shape Shape::deduce(std::initializer_list<std::initializer_list<std::initializer_list<float>>> sTensor)
	{
		return Shape::deduceTensor(sTensor);
	}

	inline Shape Shape::deduce(std::initializer_list<std::initializer_list<std::initializer_list<std::initializer_list<float>>>> sTensor)
	{
		return Shape::deduceTensor(sTensor);
	}

	inline Shape Shape::deduce(std::initializer_list<std::initializer_list<std::initializer_list<std::initializer_list<std::initializer_list<float>>>>> sTensor)
	{
		return Shape::deduceTensor(sTensor);
	}

	inline Shape Shape::deduce(std::initializer_list<std::initializer_list<std::initializer_list<std::initializer_list<std::initializer_list<std::initializer_list<float>>>>>> sTensor)
	{
		return Shape::deduceTensor(sTensor);
	}

	template<class T, class ...P> inline Shape Shape::deduceTensor(std::initializer_list<T> sTensor, P &&...sParam)
	{
		return sTensor.size() ? Shape::deduceTensor(*sTensor.begin(), std::forward<P>(sParam)..., sTensor.size()) : Shape{std::forward<P>(sParam)...};
	}

	template<class T, class ...P> inline Shape Shape::deduceTensor(T sTensor, P &&...sParam)
	{
		return Shape{std::forward<P>(sParam)...};
	}
}

#endif