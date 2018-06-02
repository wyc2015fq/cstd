#include <iostream>
#include <utility>

template <size_t n>
struct BoolMatrix {
  static BoolMatrix identity() {
    BoolMatrix r;

    for (size_t i = 0; i < n; i++)
      for (size_t j = 0; j < n; j++) {
        r.m[i][j] = (i == j);
      }

    return r;
  }

  BoolMatrix operator*(const BoolMatrix& rhs) const {
    BoolMatrix r;

    for (size_t i = 0; i < n; i++)
      for (size_t j = 0; j < n; j++) {
        bool e = false;

        for (size_t k = 0; k < n; k++)
          if (m[i][k] && rhs.m[k][j]) {
            e = true;
            break;
          }

        r.m[i][j] = e;
      }

    return r;
  }

  bool operator==(const BoolMatrix& rhs) const {
    for (size_t i = 0; i < n; i++)
      for (size_t j = 0; j < n; j++)
        if (m[i][j] != rhs.m[i][j]) {
          return false;
        }

    return true;
  }

  bool m[n][n];
};

using namespace std::rel_ops; // defines operator!=() based on operator==()

// https://en.wikipedia.org/wiki/Cycle_detection#Tortoise_and_hare
template <typename F, typename T>
void floyd(F f, const T& x0, int& mu, int& lam)
{
  T tortoise = f(x0);
  T hare = f(tortoise);

  while (tortoise != hare) {
    tortoise = f(tortoise);
    hare = f(f(hare));
  }

  mu = 0;
  tortoise = x0;

  while (tortoise != hare) {
    tortoise = f(tortoise);
    hare = f(hare);
    mu++;
  }

  lam = 1;
  hare = f(tortoise);

  while (tortoise != hare) {
    hare = f(hare);
    lam++;
  }
}

int main()
{
  const BoolMatrix<5> a = {{
      { 0, 1, 0, 0, 0 },
      { 0, 0, 1, 0, 0 },
      { 0, 0, 0, 1, 0 },
      { 0, 0, 0, 0, 1 },
      { 0, 0, 1, 0, 0 }
    }
  };

  int mu, lam;
  floyd(
  [&a](decltype(a)& x) {
    return x * a;
  },
  decltype(a)::identity(), mu, lam);
  std::cout << mu << " " << lam << std::endl;
}

作者：Milo Yip
链接：https://www.zhihu.com/question/39143483/answer/80479581
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。