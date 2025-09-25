#include "ary.h"

int main() {
    wartosc w = wartosc_dokladnosc(10.0, 5.0); // 10 +/- 5%
    double min = min_wartosc(w);
    double max = max_wartosc(w);
    // Use arithmetic functions: plus, minus, razy, podzielic
}