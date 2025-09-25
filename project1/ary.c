#include "ary.h"
#include <math.h>
#include <stdio.h>
#define swap(x, y)                                                             \
  do {                                                                         \
    double temp = x;                                                           \
    x = y, y = temp;                                                           \
  } while (0)
#define EPS 1e-10
#define and &&
#define or ||

wartosc wartosc_dokladnosc(double x, double p) {
  return (wartosc){fmin(x * ((100.0 - p) / 100.0), x * ((100.0 + p) / 100.0)),
                   fmax(x * ((100.0 - p) / 100.0), x * ((100.0 + p) / 100.0)),
                   false, false};
}

wartosc wartosc_od_do(double x, double y) {
  return (wartosc){x, y, false, false};
}

wartosc wartosc_dokladna(double x) { return (wartosc){x, x, false, false}; }

bool in_wartosc(wartosc w, double x) {
  if (w.EMPTY)
    return false;
  if (w.excluded)
    return x < w.MIN + EPS or x > w.MAX - EPS;
  return w.MIN - EPS < x and w.MAX + EPS > x;
}

double min_wartosc(wartosc w) {
  if (w.EMPTY)
    return NAN;
  if (w.excluded)
    return -HUGE_VAL;
  return w.MIN;
}

double max_wartosc(wartosc w) {
  if (w.EMPTY)
    return NAN;
  if (w.excluded)
    return HUGE_VAL;
  return w.MAX;
}

double sr_wartosc(wartosc w) {
  if (w.EMPTY or w.excluded or w.MIN < -HUGE_VAL + EPS or
      w.MAX > HUGE_VAL - EPS)
    return NAN;
  return (w.MIN + w.MAX) / 2.0;
}

bool is_zeroW(wartosc x) {
  return x.EMPTY == false and x.excluded == false and x.MIN > 0.0 - EPS and
         x.MAX < 0.0 + EPS;
}

bool is_zeroD(double x) { return x > 0.0 - EPS and x < 0.0 + EPS; }

double mul(double a, double b) {
  if (is_zeroD(a) or is_zeroD(b))
    return 0;
  return a * b;
}

wartosc plus_NE_NE(wartosc a, wartosc b) {
  return (wartosc){a.MIN + b.MIN, a.MAX + b.MAX, false, false};
}

wartosc plus_NE_E(wartosc a, wartosc b) {
  wartosc res = {b.MIN + a.MAX, b.MAX + a.MIN, true, false};
  if (res.MIN + EPS >= res.MAX)
    return (wartosc){-HUGE_VAL, HUGE_VAL, false, false};
  return res;
}

wartosc plus_E_E(/*wartosc a, wartosc b*/) {
  return (wartosc){-HUGE_VAL, HUGE_VAL, false, false};
}

wartosc plus(wartosc a, wartosc b) {
  if (a.EMPTY or b.EMPTY)
    return (wartosc){0, 0, false, true};
  if (a.excluded == b.excluded)
    return a.excluded ? plus_E_E(/*a,b*/) : plus_NE_NE(a, b);
  if (a.excluded)
    return plus_NE_E(b, a);
  return plus_NE_E(a, b);
}

wartosc minus(wartosc a, wartosc b) {
  if (a.EMPTY or b.EMPTY)
    return (wartosc){0, 0, false, true};
  return plus(a, (wartosc){b.MAX * -1.0, b.MIN * -1.0, b.excluded, false});
}

wartosc razy_NE_NE(wartosc a, wartosc b) {
  return (wartosc){fmin(fmin(mul(a.MIN, b.MIN), mul(a.MIN, b.MAX)),
                        fmin(mul(a.MAX, b.MIN), mul(a.MAX, b.MAX))),
                   fmax(fmax(mul(a.MIN, b.MIN), mul(a.MIN, b.MAX)),
                        fmax(mul(a.MAX, b.MIN), mul(a.MAX, b.MAX))),
                   false, false};
}

wartosc razy_NE_E(wartosc a, wartosc b) {
  if (a.MAX < 0.0 - EPS) {
    b.MIN *= -1;
    b.MAX *= -1;
    a.MIN *= -1;
    a.MAX *= -1;
    swap(b.MIN, b.MAX);
    swap(a.MIN, a.MAX);
  }
  if (a.MIN > 0.0 + EPS) {
    wartosc res = {fmax(mul(b.MIN, a.MIN), mul(b.MIN, a.MAX)),
                   fmin(mul(b.MAX, a.MIN), mul(b.MAX, a.MAX)), true, false};
    if (res.MIN + EPS > res.MAX)
      return (wartosc){-HUGE_VAL, HUGE_VAL, false, false};
    return res;
  }
  if (a.MIN > 0.0 - EPS and a.MAX < 0.0 + EPS)
    return (wartosc){0, 0, false, false};
  return (wartosc){-HUGE_VAL, HUGE_VAL, false, false};
}

wartosc razy_E_E(wartosc a, wartosc b) {
  if (a.MIN > 0.0 + EPS or a.MAX < 0.0 - EPS)
    return (wartosc){-HUGE_VAL, HUGE_VAL, false, false};
  if (b.MIN > 0.0 + EPS or b.MAX < 0.0 - EPS)
    return (wartosc){-HUGE_VAL, HUGE_VAL, false, false};
  return (wartosc){fmax(mul(a.MIN, b.MAX), mul(b.MIN, a.MAX)),
                   fmin(mul(a.MAX, b.MAX), mul(a.MIN, b.MIN)), true, false};
}

wartosc razy(wartosc a, wartosc b) {
  if (a.EMPTY or b.EMPTY)
    return (wartosc){0, 0, false, true};
  if (a.excluded == b.excluded)
    return a.excluded ? razy_E_E(a, b) : razy_NE_NE(a, b);
  if (a.excluded)
    return razy_NE_E(b, a);
  return razy_NE_E(a, b);
}

wartosc inv(wartosc a) {
  if (a.excluded) {
    if (a.MIN < 0.0 - EPS and a.MAX > 0.0 + EPS)
      return (wartosc){1.0 / a.MIN, 1.0 / a.MAX, false, false};
    if (a.MIN > 0.0 + EPS or a.MAX < 0.0 - EPS)
      return (wartosc){1.0 / a.MAX, 1.0 / a.MIN, true, false};
    if (a.MIN > 0.0 - EPS and a.MIN < 0.0 + EPS)
      return (wartosc){-HUGE_VAL, 1.0 / a.MAX, false, false};
    return (wartosc){1 / a.MIN, HUGE_VAL, false, false};
  }
  if (a.MIN > 0.0 + EPS or a.MAX < 0.0 - EPS)
    return (wartosc){1.0 / a.MAX, 1.0 / a.MIN, false, false};
  if (a.MIN < 0.0 - EPS and a.MAX > 0.0 + EPS)
    return (wartosc){1.0 / a.MIN, 1.0 / a.MAX, true, false};
  if (a.MAX > 0.0 + EPS)
    return (wartosc){1.0 / a.MAX, HUGE_VAL, false, false};
  return (wartosc){-HUGE_VAL, 1.0 / a.MIN, false, false};
}

wartosc podzielic(wartosc a, wartosc b) {
  if (a.EMPTY)
    return a;
  if (b.EMPTY)
    return b;
  if (is_zeroW(b))
    return (wartosc){0, 0, false, true};
  return razy(a, inv(b));
}
