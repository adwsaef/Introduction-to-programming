#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "kol.h"

struct office {
  interesant *first = nullptr, *last = nullptr;
  void reset() {
    first = nullptr, last = nullptr;
  }
};
std::vector<office> t;

std::pair<interesant*&, interesant**&> find(interesant* where, interesant* who) {
  if (where->first == &*who)
    return { where->first, where->first_to_me };
  return { where->second, where->second_to_me };
}

interesant* find_non_X(interesant* k, interesant* X = nullptr) {
  if (!k)
    return nullptr;
  if (k->first != X)
    return k->first;
  return k->second;
}

void otwarcie_urzedu(int m) {
  t.resize(m + 1);
}

void append(office& m, interesant* k) {
  if (!m.first) { // append to empty
    m.first = k;
    m.last = k;
    k->first_to_me = &m.first;
    k->second_to_me = &m.last;
    return;
  }

  auto next_to_last = m.last;
  auto tmp = find(next_to_last, nullptr);
  if (m.first == m.last and tmp.second != &m.last) // single element queue corner case
    std::swap(next_to_last->first_to_me, next_to_last->second_to_me);

  tmp = std::make_pair(k, &k->first);
  k->upd(next_to_last, nullptr, &tmp.first, &m.last);
  m.last = k;
}

interesant* nowy_interesant(int k) {
  static int cr = 0;
  interesant* w = (interesant*)malloc(sizeof(interesant));
  w->num = cr++;
  w->reset();
  append(t[k], w);
  return w;
}

int numerek(interesant* i) {
  return i->num;
}

interesant* obsluz(int k) {
  if (!t[k].first) // empty queue
    return nullptr;
  auto res = t[k].first;

  if (!res->first and !res->second) { // single element
    t[k].reset();
    return res;
  }

  t[k].first = find_non_X(res);
  find(t[k].first, res) = std::make_pair(nullptr, &t[k].first);
  return res;
}

void zmiana_okienka(interesant* i, int k) {
  if (i->first) { // update first
    find(i->first, i) = std::make_pair(i->second, i->second_to_me);
  } else {
    *i->first_to_me = i->second;
  }
  if (i->second) { // update_second
    find(i->second, i) = std::make_pair(i->first, i->first_to_me);
  } else {
    *i->second_to_me = i->first;
  }
  i->reset();
  append(t[k], i);
}

void zamkniecie_okienka(int k1, int k2) {
  if (!t[k1].first) // nothing happens
    return;

  if (!t[k2].first) { // second queue is empty
    t[k2].first = t[k1].first;
    t[k2].last = t[k1].last;
    *(t[k2].first->first == nullptr ? &t[k2].first->first_to_me : &t[k2].first->second_to_me) = &t[k2].first;
    *(t[k2].last->second == nullptr ? &t[k2].last->second_to_me : &t[k2].last->first_to_me) = &t[k2].last;
    t[k1].reset();
    return;
  }
  if (t[k1].first == t[k1].last) { // if one element in k1, just add
    zmiana_okienka(t[k1].first, k2);
    return;
  }

  if (t[k2].first == t[k2].last) { // if one element in k2, make sure to update pointer to office's end, not begin
    t[k2].first->upd(t[k2].first->first, t[k1].first, &t[k2].first, &find(t[k1].first, nullptr).first);
  } else {
    find(t[k2].last, nullptr) = std::make_pair(t[k1].first, &find(t[k1].first, nullptr).first);
  }

  find(t[k1].first, nullptr) = std::make_pair(t[k2].last, &find(t[k2].last, t[k1].first).first);
  find(t[k1].last, nullptr).second = &t[k2].last;
  t[k2].last = t[k1].last;
  t[k1].reset();
}

std::vector<interesant*> fast_track(interesant* i1, interesant* i2) {
  if (i1 == i2) { // if one element just process it
    zmiana_okienka(i1, int(t.size() - 1));
    t[t.size() - 1].reset(); // keep last office empty
    return { i1 };
  }

  std::vector<interesant*> l1 { i1, i1->first };
  std::vector<interesant*> l2 { i1, i1->second };
  while (*l1.rbegin() != i2 and *l2.rbegin() != i2) { // move in both directions till i2
    if (*l1.rbegin())
      l1.push_back(find_non_X(*l1.rbegin(), *std::next(l1.rbegin())));
    if (*l2.rbegin())
      l2.push_back(find_non_X(*l2.rbegin(), *std::next(l2.rbegin())));
  }

  if (*l2.rbegin() == i2)
    std::swap(l1, l2);
  for (auto a : l1) // process each element
    zmiana_okienka(a, int(t.size() - 1));
  t[t.size() - 1].reset(); // keep last office empty
  return l1;
}

void naczelnik(int k) {
  if (t[k].first == t[k].last)
    return;
  find(t[k].last, nullptr).second = &t[k].first;
  find(t[k].first, nullptr).second = &t[k].last;
  std::swap(t[k].first, t[k].last);
}

std::vector<interesant*> zamkniecie_urzedu() {
  std::vector<interesant*> res;
  for (size_t i = 0; i < t.size(); ++i) {
    interesant* it = t[i].first;
    interesant* prev = nullptr;
    while (it) {
      res.push_back(it);
      interesant* it2 = find_non_X(it, prev);
      prev = it;
      it = it2;
    }
  }
  t.clear();
  return res;
}