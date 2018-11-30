#ifndef _RNG_CPP_HPP_
#define _RNG_CPP_HPP_

#include <algorithm>
#include <iterator>
#include <random>
#ifdef _WIN32
#include <process.h> // _getpid
#else
#include <unistd.h>
#endif // _WIN32
//#include <direct.h>

typedef std::mt19937 rng_t;

inline rng_t* caffe_rng()
{
  static rng_t _rng_;
  static unsigned int inited_rng = 0;
  if (0 == inited_rng) {
    inited_rng = std::random_device()();
    _rng_.seed(inited_rng);
  }
  return &_rng_;
}

// Fisher¨CYates algorithm
template <class RandomAccessIterator, class RandomGenerator>
inline void shuffle(RandomAccessIterator begin, RandomAccessIterator end,
  RandomGenerator* gen)
{
  typedef typename std::iterator_traits<RandomAccessIterator>::difference_type
    difference_type;
  typedef typename std::uniform_int_distribution<difference_type> dist_type;
  difference_type length = std::distance(begin, end);
  if (length <= 0) { return; }
  for (difference_type i = length - 1; i > 0; --i) {
    dist_type dist(0, i);
    std::iter_swap(begin + i, begin + dist(*gen));
  }
}

template <class RandomAccessIterator>
inline void shuffle(RandomAccessIterator begin, RandomAccessIterator end)
{
  shuffle(begin, end, caffe_rng());
}

// random seeding
static int64_t cluster_seedgen(void)
{
  int64_t s, seed, pid;
#ifndef _MSC_VER
  FILE* f = fopen("/dev/urandom", "rb");
  if (f && fread(&seed, 1, sizeof(seed), f) == sizeof(seed)) {
    fclose(f);
    return seed;
  }
  LOG(INFO) << "System entropy source not available, "
    "using fallback algorithm to generate seed instead.";
  if (f) {
    fclose(f);
  }
  pid = getpid();
#else
  pid = _getpid();
#endif
  s = time(NULL);
  seed = std::abs(((s * 181) * ((pid - 83) * 359)) % 104729);
  return seed;
}


#endif  // CAFFE_RNG_HPP_
