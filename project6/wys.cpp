#include "wys.h"
#include <cassert>
#include <cstring>
#include <unordered_map>
#include <vector>
constexpr char inf = 100;
constexpr int max_n = 12;

template <int k> struct state {
  constexpr static int len = (k + 1) * 2;
  char data[len]; // first part is lower bounds, second upper bounds
  state(char n) {
    for (int i = 0; i <= k; ++i) {
      data[i] = 1;
      data[i + k + 1] = n + 1;
    }
  }
  inline void lower_insert(char z) {
    for (int j = k + 1; j < len; ++j) {
      if (data[j] > z)
        std::swap(z, data[j]);
    }
  }
  inline void higher_insert(char z) {
    for (int j = k; j >= 0; --j) {
      if (data[j] < z)
        std::swap(z, data[j]);
    }
  }
  inline int fix() {
    char delta = data[0] - 1;
    for (int i = 0; i < len; ++i) {
      data[i] -= delta;
    }
    return delta;
  }
};
template <int k> bool operator==(const state<k> &a, const state<k> &b) {
  return !memcmp(a.data, b.data, state<k>::len);
}

template <int k> struct my_hash {
  long long operator()(const state<k> &s) const {
    long long res = 0;
    for (int i = 0; i < state<k>::len; ++i) {
      res *= inf, res += s.data[i];
    }
    return res;
  }
};

template <int k> struct helper {
  static std::unordered_map<state<k>, std::pair<char, char>, my_hash<k>> dp;
};
template <int k>
std::unordered_map<state<k>, std::pair<char, char>, my_hash<k>> helper<k>::dp;

template <int k> int backtrack(state<k> cr) {
  auto &dp = helper<k>::dp;
  cr.fix();
  if (dp.find(cr) != dp.end())
    return dp[cr].second;
  int cnt = 0, first = 0, res = inf, mv = 0;
  for (char i = cr.data[0]; i < cr.data[state<k>::len - 1]; ++i) {
    int f = 0;
    for (int j = 0; j < k + 1; ++j) {
      if (i < cr.data[j])
        ++f;
      if (i >= cr.data[k + 1 + j])
        ++f;
    }
    if (f > k)
      continue;
    // possibly answer
    ++cnt;
    if (cnt == 1) {
      first = i;
      continue;
    };
    // not first answer, question will give information
    // what if answer true
    int m = 0;
    auto cr2 = cr;
    cr2.lower_insert(i);
    m = std::max(m, backtrack<k>(cr2));
    // what if answer false
    cr2 = cr;
    cr2.higher_insert(i);
    m = std::max(m, backtrack<k>(cr2));
    if (res > m) {
      res = m;
      mv = i;
    }
  }
  // only 1 possible answer, result is known
  if (cnt <= 1) {
    dp[cr] = {first - inf, 0};
    return 0;
  }
  dp[cr] = {mv, res + 1};
  return res + 1;
}
template <int k> void process(state<k> cr) {
  auto &dp = helper<k>::dp;
  auto res = dp[cr];
  int q_move = 0;
  while (res.first >= -(inf / 2)) {
    if (mniejszaNiz(res.first + q_move)) {
      cr.lower_insert(res.first);
      q_move += cr.fix();
      res = dp[cr];
    } else {
      cr.higher_insert(res.first);
      q_move += cr.fix();
      res = dp[cr];
    }
  }
  odpowiedz(res.first + inf + q_move);
}

int main() {
  backtrack<3>({static_cast<char>(max_n)});
  backtrack<2>({static_cast<char>(max_n)});
  backtrack<1>({static_cast<char>(max_n)});
  backtrack<0>({static_cast<char>(max_n)});
  int n, k, g;
  dajParametry(n, k, g);
  for (int i = 0; i < g; ++i) {
    if (k == 3)
      process<3>({static_cast<char>(n)});
    if (k == 2)
      process<2>({static_cast<char>(n)});
    if (k == 1)
      process<1>({static_cast<char>(n)});
    if (k == 0)
      process<0>({static_cast<char>(n)});
  }
}