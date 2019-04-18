# Boost.Hana - zwvista - 博客园

## [Boost.Hana](https://www.cnblogs.com/zwvista/p/7886763.html)

### Boost.Hana

Boost.Hana 是一个元编程的库。它为不同种类数据的集合以及类型的集合提供了容器和算法。

```
#include <boost/hana.hpp>
namespace hana = boost::hana;

#include <cassert>
#include <iostream>
#include <string>
struct Fish { std::string name; };
struct Cat  { std::string name; };
struct Dog  { std::string name; };

auto animals = hana::make_tuple(Fish{"Nemo"}, Cat{"Garfield"}, Dog{"Snoopy"});

int main()
{
    using namespace hana::literals;
    // Access tuple elements with operator[] instead of std::get.
    Cat garfield = animals[1_c];
    // Perform high level algorithms on tuples (this is like std::transform)
    auto names = hana::transform(animals, [](auto a) {
      return a.name;
    });
    assert(hana::reverse(names) == hana::make_tuple("Snoopy", "Garfield", "Nemo"));

    auto animal_types = hana::make_tuple(hana::type_c<Fish*>, hana::type_c<Cat&>, hana::type_c<Dog>);
    auto no_pointers = hana::remove_if(animal_types, [](auto a) {
      return hana::traits::is_pointer(a);
    });
    static_assert(no_pointers == hana::make_tuple(hana::type_c<Cat&>, hana::type_c<Dog>), "");

    auto has_name = hana::is_valid([](auto&& x) -> decltype((void)x.name) { });
    static_assert(has_name(garfield), "");
    static_assert(!has_name(1), "");
}
```


