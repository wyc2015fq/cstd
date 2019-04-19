# plda源码(七) - Arthur的随笔 - CSDN博客
2019年01月07日 18:08:37[largetalk](https://me.csdn.net/largetalk)阅读数：86
## plda源码(七)
FastMatrix
vals和mapped_vec
```
class FastMatrix {
 public:
  struct FElem {
    int col;
    double val;
  };
  class ElemIter {//行迭代器
   public:
    ElemIter(FElem * ptr, int size)
        : ptr_(ptr),
          size_(size),
          index_(0) {
    }
    virtual ~ElemIter() {
    }
    bool HasNext() {
    }
    void GetNext(double &val, int &index) {
    }
    FElem GetNext() {
    }
  };
 public:
  FastMatrix(int row_size, int col_size, int col_capacity)
      : size1_(row_size),
        size2_(col_capacity),
        mapped_vec(row_size) {
    vals = new FElem[row_size * col_capacity];
    linecnts = new int[row_size];
    rowheads = new FElem*[row_size];
    for (int i = 0; i < row_size; i++) {
      linecnts[i] = 0;
      rowheads[i] = vals + i * col_capacity;
    }
  }
  double get(int row, int col) const {
    const map<int, double> &lmap = mapped_vec[row];
    return lmap.at(col);
  }
  void set(int row, int col, double val) {
    //设置vals
    mapped_vec[row][col] = val;
  }
  ElemIter GetRowWithIterUnsorted(int row) const {
    FElem * ptr = rowheads[row];
    return ElemIter(ptr, linecnts[row]);
  }
 private:
  int size1_;
  int size2_;
  FElem * vals;
  int * linecnts;
  FElem ** rowheads;
  vector<map<int, double>> mapped_vec;
};
```
WordSparseMatrix
spare matrix
```
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
class WordSparseMatrix {
  using matrix_type = boost::numeric::ublas::compressed_matrix<double>;
  using matrix_row_type = boost::numeric::ublas::matrix_row<matrix_type>;
 public:
  class ElemIter {
   public:
    ElemIter(matrix_row_type matrix_row_content)
        : row_content(matrix_row_content) {
      elem_iter = row_content.begin();
    }
    bool HasNext() {
    }
    double GetNext() {
    }
    void GetNext(double &val, int &index) {
    }
    
   private:
    matrix_row_type::iterator elem_iter;
    matrix_row_type row_content;
  };
 public:
  WordSparseMatrix(int row_size, int col_size, int col_capacity)
      : mx(matrix_type(row_size, col_size)) {
  }
 private:
  matrix_type mx;
};
double WordSparseMatrix::get(int row, int col) {
	return mx(row, col);
}
void WordSparseMatrix::set(int row, int col, double val) {
	mx(row, col) = val;
}
WordSparseMatrix::ElemIter WordSparseMatrix::GetRowWithIterUnsorted(int row_num) {
	matrix_row_type mat_row = row(mx, row_num);
	return ElemIter(mat_row);
}
```
