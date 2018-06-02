//! draws an indexed triangle list
void CTRFlat_drawIndexedTriangleList(ITriangleRenderer* s, S2DVertex* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount)
{
  const S2DVertex* v1, *v2, *v3;
  u16 color;
  f32 tmpDiv; // temporary division factor
  f32 longest; // saves the longest span
  s32 height; // saves height of triangle
  u16* targetSurface; // target pointer where to plot pixels
  s32 spanEnd; // saves end of spans
  f32 leftdeltaxf; // amount of pixels to increase on left side of triangle
  f32 rightdeltaxf; // amount of pixels to increase on right side of triangle
  s32 leftx, rightx; // position where we are
  f32 leftxf, rightxf; // same as above, but as f32 values
  s32 span; // current span
  u16* hSpanBegin, *hSpanEnd; // pointer used when plotting pixels
  IRECT TriangleRect;
  s32 leftZValue, rightZValue;
  s32 leftZStep, rightZStep;
  s32 spanZValue, spanZStep; // ZValues when drawing a span
  TZBufferType* zTarget, *spanZTarget; // target of ZBuffer;
  s32 i;
  s32 triangleHalf;
  for (i = 0; i < triangleCount; ++i) {
    v1 = &vertices[*indexList];
    ++indexList;
    v2 = &vertices[*indexList];
    ++indexList;
    v3 = &vertices[*indexList];
    ++indexList;
    // back face culling
    if (s->BackFaceCullingEnabled) {
      s32 z = ((v3->Pos.X - v1->Pos.X) * (v3->Pos.Y - v2->Pos.Y)) -
          ((v3->Pos.Y - v1->Pos.Y) * (v3->Pos.X - v2->Pos.X));
      if (z < 0) {
        continue;
      }
    }
    //near plane clipping
    if (v1->ZValue < 0 && v2->ZValue < 0 && v3->ZValue < 0) {
      continue;
    }
    // sort for width for inscreen clipping
    if (v1->Pos.X > v2->Pos.X) {
      swapVertices(&v1, &v2);
    }
    if (v1->Pos.X > v3->Pos.X) {
      swapVertices(&v1, &v3);
    }
    if (v2->Pos.X > v3->Pos.X) {
      swapVertices(&v2, &v3);
    }
    if ((v1->Pos.X - v3->Pos.X) == 0) {
      continue;
    }
    TriangleRect.l = v1->Pos.X;
    TriangleRect.r = v3->Pos.X;
    // sort for height for faster drawing.
    if (v1->Pos.Y > v2->Pos.Y) {
      swapVertices(&v1, &v2);
    }
    if (v1->Pos.Y > v3->Pos.Y) {
      swapVertices(&v1, &v3);
    }
    if (v2->Pos.Y > v3->Pos.Y) {
      swapVertices(&v2, &v3);
    }
    TriangleRect.t = v1->Pos.Y;
    TriangleRect.b = v3->Pos.Y;
    if (!isRectCollided(&TriangleRect, &s->ViewPortRect)) {
      continue;
    }
    // calculate height of triangle
    height = v3->Pos.Y - v1->Pos.Y;
    if (!height) {
      continue;
    }
    // calculate longest span
    longest = (v2->Pos.Y - v1->Pos.Y) / (f32)height * (v3->Pos.X - v1->Pos.X) + (v1->Pos.X - v2->Pos.X);
    spanEnd = v2->Pos.Y;
    span = v1->Pos.Y;
    leftxf = (f32)v1->Pos.X;
    rightxf = (f32)v1->Pos.X;
    leftZValue = v1->ZValue;
    rightZValue = v1->ZValue;
    color = v1->Color;
    targetSurface = s->lockedSurface + span * s->SurfaceWidth;
    zTarget = s->lockedZBuffer + span * s->SurfaceWidth;
    if (longest < 0.0f) {
      tmpDiv = 1.0f / (f32)(v2->Pos.Y - v1->Pos.Y);
      rightdeltaxf = (v2->Pos.X - v1->Pos.X) * tmpDiv;
      rightZStep = (s32)((v2->ZValue - v1->ZValue) * tmpDiv);
      tmpDiv = 1.0f / (f32)height;
      leftdeltaxf = (v3->Pos.X - v1->Pos.X) * tmpDiv;
      leftZStep = (s32)((v3->ZValue - v1->ZValue) * tmpDiv);
    }
    else {
      tmpDiv = 1.0f / (f32)height;
      rightdeltaxf = (v3->Pos.X - v1->Pos.X) * tmpDiv;
      rightZStep = (s32)((v3->ZValue - v1->ZValue) * tmpDiv);
      tmpDiv = 1.0f / (f32)(v2->Pos.Y - v1->Pos.Y);
      leftdeltaxf = (v2->Pos.X - v1->Pos.X) * tmpDiv;
      leftZStep = (s32)((v2->ZValue - v1->ZValue) * tmpDiv);
    }
    // do it twice, once for the first half of the triangle,
    // end then for the second half.
    for (triangleHalf = 0; triangleHalf < 2; ++triangleHalf) {
      if (spanEnd > s->ViewPortRect.b) {
        spanEnd = s->ViewPortRect.b;
      }
      // if the span <0, than we can skip these spans,
      // and proceed to the next spans which are really on the screen.
      if (span < s->ViewPortRect.t) {
        // we'll use leftx as temp variable
        if (spanEnd < s->ViewPortRect.t) {
          leftx = spanEnd - span;
          span = spanEnd;
        }
        else {
          leftx = s->ViewPortRect.t - span;
          span = s->ViewPortRect.t;
        }
        leftxf += leftdeltaxf * leftx;
        rightxf += rightdeltaxf * leftx;
        targetSurface += s->SurfaceWidth * leftx;
        zTarget += s->SurfaceWidth * leftx;
        leftZValue += leftZStep * leftx;
        rightZValue += rightZStep * leftx;
      }
      // the main loop. Go through every span and draw it.
      while (span < spanEnd) {
        leftx = (s32)(leftxf);
        rightx = (s32)(rightxf + 0.5f);
        // perform some clipping
        // TODO: clipping is not correct when leftx is clipped.
        if (leftx < s->ViewPortRect.l) {
          leftx = s->ViewPortRect.l;
        }
        else if (leftx > s->ViewPortRect.r) {
          leftx = s->ViewPortRect.r;
        }
        if (rightx < s->ViewPortRect.l) {
          rightx = s->ViewPortRect.l;
        }
        else if (rightx > s->ViewPortRect.r) {
          rightx = s->ViewPortRect.r;
        }
        // draw the span
        if (rightx - leftx != 0) {
          tmpDiv = 1.0f / (rightx - leftx);
          spanZValue = leftZValue;
          spanZStep = (s32)((rightZValue - leftZValue) * tmpDiv);
          hSpanBegin = targetSurface + leftx;
          spanZTarget = zTarget + leftx;
          hSpanEnd = targetSurface + rightx;
          while (hSpanBegin < hSpanEnd) {
            if (spanZValue > *spanZTarget) {
              *spanZTarget = spanZValue;
              *hSpanBegin = color;
            }
            spanZValue += spanZStep;
            ++hSpanBegin;
            ++spanZTarget;
          }
        }
        leftxf += leftdeltaxf;
        rightxf += rightdeltaxf;
        ++span;
        targetSurface += s->SurfaceWidth;
        zTarget += s->SurfaceWidth;
        leftZValue += leftZStep;
        rightZValue += rightZStep;
      }
      if (triangleHalf > 0) { // break, we've gout only two halves
        break;
      }
      // setup variables for second half of the triangle.
      if (longest < 0.0f) {
        tmpDiv = 1.0f / (v3->Pos.Y - v2->Pos.Y);
        rightdeltaxf = (v3->Pos.X - v2->Pos.X) * tmpDiv;
        rightxf = (f32)v2->Pos.X;
        rightZValue = v2->ZValue;
        rightZStep = (s32)((v3->ZValue - v2->ZValue) * tmpDiv);
      }
      else {
        tmpDiv = 1.0f / (v3->Pos.Y - v2->Pos.Y);
        leftdeltaxf = (v3->Pos.X - v2->Pos.X) * tmpDiv;
        leftxf = (f32)v2->Pos.X;
        leftZValue = v2->ZValue;
        leftZStep = (s32)((v3->ZValue - v2->ZValue) * tmpDiv);
      }
      spanEnd = v3->Pos.Y;
    }
  }
}

