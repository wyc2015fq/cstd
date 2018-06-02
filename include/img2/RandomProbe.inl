// * let N = Width * Height = # of pixels in image
// * let n = # of random probes
// * all n probes may not be distinct. What is the expected # of double/triple/... hits?
// * n probes give C(n,2) combinations
// * let Yij = Xi == Xj
// * E[Yij] = p = 1/N
// * then # of non-unique hits Z = summation Yij
// * E[Z] = summation E[Yij] = C(n,2)*p = n(n-1)/(2*N) I have verifies through simulations that formula is correct c:/c#temp/temp12.cs
// * if E[Z] > a*n where a is in [0,1]; a is fraction of non-unique hits; go in for a complex scheme in which you maintain a book-keeping list and return unique probes
// * E[Z] > a*n gives n > 1+2*a*N with a=0.5 n > 1+N
// * for N large when n = N, E[Z] = N/2 or about 50% probes are non-unique
// * I don't think there is any need to go in for the complex scheme when we maintain a list and make sure we don't return non-unique hits
/// <summary>
/// This probe will randomly probe the source image to find matching blocks. The amount to probe is set
/// using the fractionToProbe parameter.
/// </summary>
struct RandomProbe {
  int counter, width, height, maxAttempts;
  uint32 seed;
};
void RandomProbe_Reset(RandomProbe* s)
{
  s->counter = 0;
}
void RandomProbe_Reset(RandomProbe* s, int Width, int Height)
{
  s->counter = 0;
  s->width = Width;
  s->height = Height;
}
void RandomProbe_set(RandomProbe* s, int Width, int Height, double fractionToProbe)
{
  s->counter = 0;
  s->seed = 15;
  s->width = Width;
  s->height = Height;
  s->maxAttempts = (int)(fractionToProbe * Width * Height);
}
bool RandomProbe_Next(RandomProbe* s, IPOINT* pt)
{
  bool probeCompleted;
  int i = myrand32(&s->seed) % (s->width * s->height);
  int Y = i / s->width;
  int X = i - s->width * Y;
  pt->x = X;
  pt->y = Y;
  if (s->counter >= s->maxAttempts) {
    probeCompleted = true;
  }
  else {
    probeCompleted = false;
  }
  s->counter++;
  return probeCompleted;
}

