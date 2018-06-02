
#if 0
struct naive_group_intersections< DSEGMENT2 > {
public:

  template<typename InputIterator, typename OutputIterator>
  se2_naive_group_intersections(const DSEGMENT2* begin, InputIterator end, OutputIterator out) {
    for (InputIterator i = begin; i != end; ++i) {
      for (InputIterator j = (i + 1); j != end; ++j) {
        if (intersect((*j), (*i))) {
          (*out++) = intersection_point((*j), (*i));
        }
      }
    }
  }
};

struct naive_group_intersections< DSEGMENT3 > {
public:

  template<typename InputIterator, typename OutputIterator>
  naive_group_intersections(InputIterator begin, InputIterator end, OutputIterator out) {
    for (InputIterator i = begin; i != end; ++i) {
      for (InputIterator j = (i + 1); j != end; ++j) {
        if (intersect((*j), (*i))) {
          (*out++) = intersection_point((*j), (*i));
        }
      }
    }
  }
};

struct naive_group_intersections< DCIRCLE > {
public:

  template<typename InputIterator, typename OutputIterator>
  naive_group_intersections(InputIterator begin, InputIterator end, OutputIterator out) {
    for (InputIterator i = begin; i != end; ++i) {
      for (InputIterator j = (i + 1); j != end; ++j) {

        if ((distance((*i).x, (*i).y, (*j).x, (*j).y) >= abs((*i).radius - (*j).radius)) &&
            intersect((*j), (*i))) {
          DPOINT2 p1;
          DPOINT2 p2;
          intersection_point((*j), (*i), p1, p2);
          (*out++) = p1;
          (*out++) = p2;
        }
      }
    }
  }
};

#endif

