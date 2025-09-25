#ifndef _ARY_H_
#define _ARY_H_

#include "stdbool.h"


typedef struct wartosc {
    double MIN, MAX;
    bool excluded, EMPTY;
  } wartosc;

/* We implicitly assume that all arguments of type double are real numbers,  */
/* i.e. they are different from HUGE_VAL, -HUGE_VAL and NAN.                 */

/* wartosc_dokladnosc(x, p) returns a value representing  */
/* x +/- p%                                               */
/* precondition: p > 0                                    */
wartosc wartosc_dokladnosc(double x, double p);

/* wartosc_od_do(x, y) returns a value representing [x;y] */
/* precondition: x <= y                                   */
wartosc wartosc_od_do(double x, double y);

/* wartosc_dokladna(x) returns a value representing [x;x] */
wartosc wartosc_dokladna(double x);

/* in_wartosc(w, x) = x \in w                             */
bool in_wartosc(wartosc w, double x);

/* min_wartosc(w) = the smallest possible value of w,      */
/* or -HUGE_VAL if there is no lower bound.                */
double min_wartosc(wartosc w);

/* max_wartosc(w) = the largest possible value of w,       */
/* or HUGE_VAL if there is no upper bound.                 */
double max_wartosc(wartosc w);

/* center of the interval from min_wartosc to max_wartosc, */
/* or nan if min and max_wartosc are not defined.          */
double sr_wartosc(wartosc w);

/* Arithmetic operations on imprecise values.              */
wartosc plus(wartosc a, wartosc b);
wartosc minus(wartosc a, wartosc b);
wartosc razy(wartosc a, wartosc b);
wartosc podzielic(wartosc a, wartosc b);

#endif
