
#define  HLSMAX   240 /* lhs.H,lhs.L, and lhs.S vary over 0-HLSMAX */ 
#define  RGBMAX   255   /* R,G, and B vary over 0-RGBMAX */ 
/* HLSMAX BEST IF DIVISIBLE BY 6 */ 
/* RGBMAX, HLSMAX must each fit in a byte. */ 

/* Hue is undefined if Saturation is 0 (grey-scale) */ 
/* This value determines where the Hue scrollbar is */ 
/* initially set for achromatic colors */ 
#define UNDEFINED (HLSMAX*2/3)

HLS RGBtoHLS(DWORD lRGBColor)
{
  HLS lhs;
  //WORD R,G,B;          /* input RGB values */ 
  BYTE cMax,cMin;      /* MAX and min RGB values */ 
  WORD  Rdelta,Gdelta,Bdelta; /* intermediate value: % of spread from MAX

                */ 
  /* get R, G, and B out of DWORD */ 
  WORD R = GetRValue(lRGBColor);
  WORD G = GetGValue(lRGBColor);
  WORD B = GetBValue(lRGBColor);

  /* calculate lightness */ 
  cMax = (BYTE)MAX( MAX(R,G), B);
  cMin = (BYTE)MIN( MIN(R,G), B);
  lhs.L =(BYTE)( ( ((cMax+cMin)*HLSMAX) + RGBMAX )/(2*RGBMAX));

  if (cMax == cMin) {           /* r=g=b --> achromatic case */ 
    lhs.S= 0;                     /* saturation */ 
    lhs.H = UNDEFINED;             /* hue */ 
  }
  else {                        /* chromatic case */ 
    /* saturation */ 
    if (lhs.L <= (HLSMAX/2))
      lhs.S =(BYTE) ( ((cMax-cMin)*HLSMAX) + ((cMax+cMin)/2) ) / (cMax+cMin);
    else
      lhs.S =(BYTE) ( ((cMax-cMin)*HLSMAX) + ((2*RGBMAX-cMax-cMin)/2) )
      / (2*RGBMAX-cMax-cMin);

    /* hue */ 
    Rdelta = ( ((cMax-R)*(HLSMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);
    Gdelta = ( ((cMax-G)*(HLSMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);
    Bdelta = ( ((cMax-B)*(HLSMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);

    if (R == cMax)
      lhs.H = Bdelta - Gdelta;
    else if (G == cMax)
      lhs.H = (HLSMAX/3) + Rdelta - Bdelta;
    else /* B == cMax */ 
      lhs.H = ((2*HLSMAX)/3) + Gdelta - Rdelta;

    if (lhs.H < 0)
      lhs.H += HLSMAX;
    if (lhs.H > HLSMAX)
      lhs.H -= HLSMAX;
  }
  return lhs;
}
/* utility routine for HLStoRGB */ 
WORD HueToRGB(WORD n1,WORD n2,WORD hue)
{
  /* range check: note values passed add/subtract thirds of range */ 
  if (hue < 0)
    hue += HLSMAX;

  if (hue > HLSMAX)
    hue -= HLSMAX;

  /* return r,g, or b value from this tridrant */ 
  if (hue < (HLSMAX/6))
    return ( n1 + (((n2-n1)*hue+(HLSMAX/12))/(HLSMAX/6)) );
  if (hue < (HLSMAX/2))
    return ( n2 );
  if (hue < ((HLSMAX*2)/3))
    return ( n1 +    (((n2-n1)*(((HLSMAX*2)/3)-hue)+(HLSMAX/12))/(HLSMAX/6))
    );
  else
    return ( n1 );
}

DWORD HLStoRGB(WORD hue,WORD lum,WORD sat)
{
  WORD R,G,B;                /* RGB component values */ 
  WORD  Magic1,Magic2;       /* calculated magic numbers (really!) */ 

  if (sat == 0) {            /* achromatic case */ 
    R=G=B=(lum*RGBMAX)/HLSMAX;
    if (hue != UNDEFINED) {
      /* ERROR */ 
    }
  }
  else  {                    /* chromatic case */ 
    /* set up magic numbers */ 
    if (lum <= (HLSMAX/2))
      Magic2 = (lum*(HLSMAX + sat) + (HLSMAX/2))/HLSMAX;
    else
      Magic2 = lum + sat - ((lum*sat) + (HLSMAX/2))/HLSMAX;
    Magic1 = 2*lum-Magic2;

    /* get RGB, change units from HLSMAX to RGBMAX */ 
    R = (HueToRGB(Magic1,Magic2,hue+(HLSMAX/3))*RGBMAX +
      (HLSMAX/2))/HLSMAX;
    G = (HueToRGB(Magic1,Magic2,hue)*RGBMAX + (HLSMAX/2)) / HLSMAX;
    B = (HueToRGB(Magic1,Magic2,hue-(HLSMAX/3))*RGBMAX +
      (HLSMAX/2))/HLSMAX;
  }
  return(RGB(R,G,B));
}