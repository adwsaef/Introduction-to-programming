#include "prev.h"
#include <cassert>
#include <chrono>
#include <climits>
#include <iostream>
#include <random>
auto seed =
    std::chrono::high_resolution_clock::now().time_since_epoch().count();
std::mt19937 gen{static_cast<long unsigned int>(seed)};
struct node {
  using key = std::pair<int, int>;
  const key x; // key, tiebreaker
  const int y;
  const int id;
  int max_id = id;
  int L = -1, R = -1;
};
std::vector<node> data;
std::vector<int> version;

struct data_ref {
  int val;
  enum side { left, right };
  data_ref(int id, side s) : val{((s == left) ? -1 : 1) * (id + 1)} {}
  operator int &() const {
    if (val < 0)
      return data[-val - 1].L;
    return data[val - 1].R;
  }
};

int create_copy(int treap) {
  data.push_back(data[treap]);
  return (int)data.size() - 1;
}

void update_up(int treap) {
  if (treap == -1)
    return;
  data[treap].max_id = data[treap].id;
  if (data[treap].L != -1)
    data[treap].max_id =
        std::max(data[data[treap].L].max_id, data[treap].max_id);
  if (data[treap].R != -1)
    data[treap].max_id =
        std::max(data[data[treap].R].max_id, data[treap].max_id);
}

void split(int treap, node::key key, data_ref L, data_ref R) {
  if (treap == -1) {
    (int &)L = -1, (int &)R = -1;
    return;
  }
  treap = create_copy(treap);
  if (data[treap].x <= key) {
    split(data[treap].R, key, {treap, data_ref::side::right}, R);
    (int &)L = treap;
  } else {
    split(data[treap].L, key, L, {treap, data_ref::side::left});
    (int &)R = treap;
  }
  update_up(L);
  update_up(R);
}

int insert(int treap, int val) {
  if (treap == -1)
    return val;
  if (data[treap].y > data[val].y) {
    treap = create_copy(treap);
    if (data[val].x <= data[treap].x) {
      data[treap].L = insert(data[treap].L, val);
    } else {
      data[treap].R = insert(data[treap].R, val);
    }
    update_up(treap);
    return treap;
  }
  split(treap, data[val].x, {val, data_ref::side::left},
        {val, data_ref::side::right});
  update_up(val);
  return val;
}

int read(int treap, node::key L, node::key R,
         node::key cr_L = {INT_MIN, INT_MIN},
         node::key cr_R = {INT_MAX, INT_MAX}) {
  if (treap == -1 or cr_L > cr_R)
    return -1;
  if (cr_L >= L and cr_R <= R)
    return data[treap].max_id;
  if (data[treap].x < L)
    return read(data[treap].R, L, R, std::max(cr_L, data[treap].x), cr_R);
  if (data[treap].x > R)
    return read(data[treap].L, L, R, cr_L, std::min(cr_R, data[treap].x));
  return std::max(data[treap].id,
                  std::max(read(data[treap].L, L, R, cr_L, data[treap].x),
                           read(data[treap].R, L, R, data[treap].x, cr_R)));
}

void pushBack(int value) {
  data.emplace_back(node{{value, (int)gen()}, (int)gen(), (int)version.size()});
  if (version.empty()) {
    version.push_back(0);
  } else {
    version.push_back(insert(*version.rbegin(), (int)data.size() - 1));
  }
}

void init(const std::vector<int> &seq) {
  for (auto a : seq)
    pushBack(a);
}
int prevInRange(int i, int lo, int hi) {
  return read(version[i], {lo, INT_MIN}, {hi, INT_MAX});
} // max{ j : 0 <= j <= i && seq[j] \in [lo..hi] } or -1

void done() {
  data.clear();
  version.clear();
}