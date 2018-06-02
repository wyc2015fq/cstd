

#include "macro.h"



#define real double

#define MIN_OPT(a, b)  (a < b)
#define MAX_OPT(a, b)  (a > b)

#define FIND_ELEMENT(h, w, A, al, ai, out_i, OPT)                             \
  do {                                                                          \
    int a0, a1, _w=w*ai;                                                        \
    for (a1=0; a1<(h*al); a1+=al)                                               \
      for (a0=a1; a0<(_w+a1); a0+=ai)                                           \
        if (OPT(*(A+out_i), *(A+a0)))                                           \
          out_i = a0;                                                           \
  } while(0)

unsigned char* min_element(int h, int w, unsigned char* A, int al, int ai)
{
  int i = 0;
  FIND_ELEMENT(h, w, A, al, ai, i, MIN_OPT);
  return A + i;
}

unsigned char* max_element(int h, int w, unsigned char* A, int al, int ai)
{
  int i = 0;
  FIND_ELEMENT(h, w, A, al, ai, i, MAX_OPT);
  return A + i;
}

int bilateralFilter(int height, int width, unsigned char* img, int al, int ai, unsigned char* B, int bl, int bi,
    real space_sigma, real range_sigma, real space_sampling, real range_sampling)
{

  real sigma_r, sigma_s, input_min, input_max, input_delta;
  int padding_xy, padding_z, small_width, small_height, small_depth, small_size;
  int half_width, half_height, half_depth;
  int x, y, z, x_end, y_end, z_end;
  real* w;
  real* iw;

  sigma_r = range_sigma / range_sampling;
  sigma_s = space_sigma / space_sampling;

  padding_xy = (int)(2.0 * sigma_s) + 1;
  padding_z = (int)(2.0 * sigma_r) + 1;

  input_min = *min_element(height, width, img, al, ai);
  input_max = *max_element(height, width, img, al, ai);
  input_delta = input_max - input_min;

  small_width = (int)((width - 1) / space_sampling) + 1 + 2 * padding_xy;
  small_height = (int)((height - 1) / space_sampling) + 1 + 2 * padding_xy;
  small_depth = (int)(input_delta / range_sampling) + 1 + 2 * padding_z;
  small_size = small_width * small_height * small_depth;
#if 0

  w = MALLOC(real, small_size);
  iw = MALLOC(real, small_size);

  for (int x = 0, x_end = width; x < x_end; x++) {
    for (int y = 0, y_end = height; y < y_end; y++) {
      const int small_x = (int)(1.0 * x / space_sampling + 0.5) + padding_xy;
      const int small_y = (int)(1.0 * y / space_sampling + 0.5) + padding_xy;
      const int small_z = (int)((input(x, y) - input_min) / range_sampling + 0.5) + padding_z;
      w(small_x, small_y, small_z) += 1.0;
      iw(small_x, small_y, small_z) += input(x, y);
    }
  }

#endif
}


void fast_color_BF(int height, int width, unsigned char* img, int al, int ai,
    real space_sigma, real range_sigma, real* weight, real* result)
{
  enum { dimension = 5 };
  real base_min[ 3 ];
  int size[ dimension ], index[ dimension ];
  const int padding_s = 2;
  const int padding_r = 2;
  int padding[ dimension ];
  padding[ 0 ] = padding_s;
  padding[ 1 ] = padding_s;
  padding[ 2 ] = padding_r;
  padding[ 3 ] = padding_r;
  padding[ 4 ] = padding_r;

#if 0

  for (int c = 0; c < 3; ++c) {
    real_type m = base[ index ][ c ];
    real_type M = m;

    for (typename base_array_2D_type::const_iterator
        b = base.begin(),
        b_end = base.end();
        b != b_end; ++b) {
      const real_type r = (*b) [ c ];
      m = min(m, r);
      M = max(M, r);
    }

    base_min[ c ] = m;
    size[ 2 + c ] = static_cast<int>((M - m) / range_sigma)
        + 1 + 2 * padding[ 2 + c ];
  }

  size[ 0 ] = static_cast<int>((width - 1) / space_sigma) + 1 + 2 * padding[ 0 ];
  size[ 1 ] = static_cast<int>((height - 1) / space_sigma) + 1 + 2 * padding[ 1 ];

  mixed_array_ND_type data(size, mixed_type());

  for (int x = 0, x_end = width; x < x_end; ++x) {
    index[ 0 ] = static_cast<int>(1.0 * x / space_sigma + 0.5) + padding[ 0 ];

    for (int y = 0, y_end = height; y < y_end; ++y) {
      index[ 1 ] = static_cast<int>(1.0 * y / space_sigma + 0.5) + padding[ 1 ];

      for (int c = 0; c < 3; ++c) {
        index[ 2 + c ] =
            static_cast<int>((base(x, y) [ c ] - base_min[ c ]) / range_sigma + 0.5)
            + padding[ 2 + c ];
      }

      mixed_type& d = data[ index ];
      d.first += input(x, y);
      d.second += 1.0;
    } // END OF for y
  } // END OF for x

  mixed_array_ND_type buffer(size);

  const mixed_type* const origin = &(data[ key_type() ]);
  key_type start_index, end_index;

  for (int dim = 0; dim < dimension; ++dim) {
    start_index[ dim ] = 1;
    end_index[ dim ] = (dim == dimension - 1) ? 2 : (size[ dim ] - 1);
  }

  for (int dim = 0; dim < dimension; ++dim) {

    key_type offset_proxy;
    offset_proxy[ dim ] = 1;
    const int off = &(data[ offset_proxy ]) - origin;

    for (int n_iter = 0; n_iter < 2; ++n_iter) {

      swap(buffer, data);

      key_type index = start_index;

      do {
        mixed_type* d_ptr = &(data[ index ]);
        mixed_type* b_ptr = &(buffer[ index ]);

        for (int
            z = 1,
            z_end = size[ dimension - 1 ] - 1;
            z < z_end;
            ++z, ++d_ptr, ++b_ptr) {

          *d_ptr = (*(b_ptr - off) + * (b_ptr + off) + 2.0 * (*b_ptr)) / 4.0;

        } // END OF for z
      }
      while (data.advance(&index, start_index, end_index));
    } // END OF for n_iter
  } // END OF for dim

  result->resize(width, height);

  vector<real_type> pos(dimension);
  weight->resize(width, height);

  for (int x = 0, x_end = width; x < x_end; ++x) {
    pos[ 0 ] = static_cast<real_type>(x) / space_sigma + padding[ 0 ];

    for (int y = 0, y_end = height; y < y_end; ++y) {
      pos[ 1 ] = static_cast<real_type>(y) / space_sigma + padding[ 1 ];

      for (int c = 0; c < 3; ++c) {
        pos[ 2 + c ] = (base(x, y) [ c ] - base_min[ c ]) / range_sigma + padding[ 2 + c ];
      }

      const mixed_type D = Math_tools::Nlinear_interpolation(data, pos);
      (*weight)(x, y) = D.second;
      (*result)(x, y) = D.first / D.second;
    } // END OF for y
  } // END OF for x

#endif

  return 0;
}
