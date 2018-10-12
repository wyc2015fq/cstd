
template <typename II, typename C, typename P, , typename Q, typename OI>
int kmeans(II arr, int n, P center_dist_fun, Q calc_center_fun, OI labels, double thd, int nclass) {
  int i;
  for (i = 0; i < nclass; ++i) {
    labels[i] = rand() % n;
  }
  for (i = 0; i < n; i) {
    int idx = -1;
    double mindist = thd;
    for (int j = 0; j < nclass; ++j) {
      center_dist_fun(arr, labels, i);
    }
  }
  return n;
}
 
int test_kmeans()
{
	return 0;
}
