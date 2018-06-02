

#include "clipper.inl"
#include "clipper_c.inl"


#include "test_clipper_console.inl"
bool Paths_eq(const Paths& a, const Paths& b) {
  if (a.size()!=b.size()) {
    return false;
  }
  int i, j, n;
  for (i=0; i<a.size(); ++i) {
    n = a[i].size();
    if (n!=b[i].size()) {
      return false;
    }
    for (j=0; j<n; ++j) {
      if (a[i][j]!=b[i][j]) {
        return false;
      }
    }
  }
  return true;
}
//---------------------------------------------------------------------------
#include "tic.inl"
int test_clipper() {
  int i, error_cnt=0, edgeCount = 100, loop_cnt = 10;
  Paths subj, clip, solu0, solu1, so0, so1;
  QPOLYGON po_subj[4] = {0};
  QPOLYGON* po_clip = po_subj+1;
  QPOLYGON* po_solu= po_subj+2;
  Clipper clpr;
  Clipper_t clpr1;
  ClipperOffset co;
  ClipperOffset_t co1;
  Clipper_Init(&clpr1, 0);
  for (i = 0; i < loop_cnt; i++) {
    MakeRandomPoly(edgeCount, 400, 400, subj);
    MakeRandomPoly(edgeCount, 400, 400, clip);
    clpr.Clear();
    Clipper_Clear(&clpr1);
    co.Clear();
    co1.Clear();
    tic;
    clpr.AddPaths(subj, ptSubject, true);
    clpr.AddPaths(clip, ptClip, true);
    if (!clpr.Execute(ctIntersection, solu0, pftEvenOdd, pftEvenOdd)) {
      error_cnt++;
    }
    toc;
    tic;
    co.AddPaths(subj, jtRound, etClosedPolygon);
    co.Execute(so0, -7.0);
    toc;
    tic;
    Clipper_AddPaths(&clpr1, subj, ptSubject, true);
    Clipper_AddPaths(&clpr1, clip, ptClip, true);
    if (!Clipper_Execute(&clpr1, ctIntersection, solu1, pftEvenOdd, pftEvenOdd)) {
      error_cnt++;
    }
    toc;
    tic;
    ClipperOffset_AddPaths(&co1, subj, jtRound, etClosedPolygon);
    ClipperOffset_Execute(&co1, so1, -7.0);
    toc;
    //Paths2QPOLYGON(subj, po_subj);
    //Paths2QPOLYGON(clip, po_clip);
    ASSERT(Paths_eq(solu0, solu1));
    ASSERT(Paths_eq(so0, so1));
  }
  Clipper_Free(&clpr1);
  tictoc(0);
  return 0;
}
// virtual