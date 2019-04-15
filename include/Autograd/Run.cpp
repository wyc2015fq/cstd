
/*
	2018.05.05
	Created by AcrylicShrimp.
*/

#include "GraphOp.h"
#include "Shape.h"
#include "Tensor.h"

#include <cstdio>
#include <cstdlib>
#include <initializer_list>
#include <memory>
#include <random>

using namespace Autograd;

int main()
{
	auto x{GraphOp::tensor(Shape{2, 1})};
	auto y{GraphOp::tensor(Shape{1})};

	auto w{GraphOp::tensor(Shape{1, 2})};
	auto b{GraphOp::tensor(Shape{1})};

	auto output{GraphOp::matMul(w, x) + b};
	auto loss{GraphOp::mean((y - output) * (y - output))};

	std::mt19937_64 sEngine{std::random_device{}()};
	std::uniform_real<float> sRandom{-1.f, 1.f};

	w->value() = {sRandom(sEngine), sRandom(sEngine)};
	b->value() = {.0f};

	for (;;)
	{
		system("cls");

		x->value() = {.0f, .0f};
		y->value() = {.0f};
		printf("output :\n%s\n\n", output->forward().toString().c_str());
		printf("loss :\n%s\n\n", loss->forward().toString().c_str());
		printf("weight_gradient :\n%s\n\n", w->backward().toString().c_str());
		printf("bias_gradient :\n%s\n\n", b->backward().toString().c_str());

		puts("============================================");

		x->value() = {1.f, .0f};
		y->value() = {1.f};
		printf("output :\n%s\n\n", output->forward().toString().c_str());
		printf("loss :\n%s\n\n", loss->forward().toString().c_str());
		printf("weight_gradient :\n%s\n\n", w->backward().toString().c_str());
		printf("bias_gradient :\n%s\n\n", b->backward().toString().c_str());

		puts("============================================");

		x->value() = {.0f, 1.f};
		y->value() = {1.f};
		printf("output :\n%s\n\n", output->forward().toString().c_str());
		printf("loss :\n%s\n\n", loss->forward().toString().c_str());
		printf("weight_gradient :\n%s\n\n", w->backward().toString().c_str());
		printf("bias_gradient :\n%s\n\n", b->backward().toString().c_str());

		puts("============================================");

		x->value() = {1.f, 1.f};
		y->value() = {1.f};
		printf("output :\n%s\n\n", output->forward().toString().c_str());
		printf("loss :\n%s\n\n", loss->forward().toString().c_str());
		printf("weight_gradient :\n%s\n\n", w->backward().toString().c_str());
		printf("bias_gradient :\n%s\n\n", b->backward().toString().c_str());

		system("pause");

		{
			x->value() = {.0f, .0f};
			y->value() = {.0f};

			auto gradient_w{w->backward()};
			auto gradient_b{b->backward()};

			w->value() -= gradient_w * .1f;
			b->value() -= gradient_b * .1f;
		}

		{
			x->value() = {1.f, .0f};
			y->value() = {1.f};

			auto gradient_w{w->backward()};
			auto gradient_b{b->backward()};

			w->value() -= gradient_w * .1f;
			b->value() -= gradient_b * .1f;
		}

		{
			x->value() = {.0f, 1.f};
			y->value() = {1.f};

			auto gradient_w{w->backward()};
			auto gradient_b{b->backward()};

			w->value() -= gradient_w * .1f;
			b->value() -= gradient_b * .1f;
		}

		{
			x->value() = {1.f, 1.f};
			y->value() = {1.f};

			auto gradient_w{w->backward()};
			auto gradient_b{b->backward()};

			w->value() -= gradient_w * .1f;
			b->value() -= gradient_b * .1f;
		}
	}

	return 0;
}



















#include "AddGraph.cpp"
#include "DivGraph.cpp"
#include "Graph.cpp"
#include "GraphOp.cpp"
#include "MatMulGraph.cpp"
#include "MeanGraph.cpp"
#include "MulGraph.cpp"
#include "Operand1Graph.cpp"
#include "Operand2Graph.cpp"
#include "Shape.cpp"
#include "SubGraph.cpp"
#include "Tensor.cpp"
#include "ValueGraph.cpp"
//#include "Run.cpp"