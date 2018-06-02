int test_QShortestArc()
{
  float3 v1 = {0.1, 0.2, 0.3};
  float3 v2 = {0.4, 0.5, 0.6}, v3;
  float4 q, q1;
  float9 rot;
  if (1) {
    QShortestArc(v1, v2, q);
    QToMatrix9(q, rot);
    MultiplyPoint9(rot, v1, v3);
    Normalize3(v2);
    Normalize3(v3);
    ASSERT(Dist3(v2, v3) < FLT_EPSILON);
  }
  if (1) {
    float3 v1 = {1, 1, 1};
    float3 v2 = { -1, 1, 1}, v3;
    QShortestArc(v1, v2, q);
    QToMatrix9(q, rot);
    MultiplyPoint9(rot, v1, v3);
    Normalize3(v2);
    Normalize3(v3);
    ASSERT(Dist3(v2, v3) < FLT_EPSILON);
    QRotatingVector(q, v1, v3);
    Normalize3(v3);
    ASSERT(Dist3(v2, v3) < FLT_EPSILON);
  }
  if (1) {
    float3 v1 = {0, 0, 1};
    float3 d1 = {0, 1, 0};
    float3 v2 = {1, 0, 0}, v3, d3;
    float3 d2 = {0, 0, 1};
    QPlaneTrans(v1, d1, v2, d2, q);
    QRotatingVector(q, v1, v3);
    QRotatingVector(q, d1, d3);
    Normalize3(v3);
    Normalize3(d3);
    ASSERT(Dist3(v2, v3) < FLT_EPSILON);
    ASSERT(Dist3(d2, d3) < FLT_EPSILON);
  }
  if (1) {
    int i, n = 100000;
    {
      utime_start(_start_time);
      for (i = 0; i < n; ++i) {
        QShortestArc(v1, v2, q);
      }
      printf("%f\n", utime_elapsed(_start_time));
    }
    {
      utime_start(_start_time);
      for (i = 0; i < n; ++i) {
        QShortestArc1(v1, v2, q1);
      }
      printf("%f\n", utime_elapsed(_start_time));
    }
  }
  return 0;
}
#include "Quaternion2.inl"

