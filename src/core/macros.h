#pragma once

#define maximum(a, b) ((a) > (b) ? (a) : (b))

#define FABS(x) ((x) < 0.0f ? -(x) : (x))
#define round(x) \
    ( (FABS((x) - (int)(x)) < 0.0001f) ? ((x) + 1.0f) : \
      (FABS((x) - 0.5f) < 0.0001f) ? 1.0f : \
      ((float)((int)((x) + 0.5f))) )
