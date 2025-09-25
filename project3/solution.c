#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define and &&
#define or ||
#define eps 1e-12

bool is_zero(double tmp) { return tmp >= -eps and tmp <= eps; }

typedef struct {
  double x, y;
} point;

typedef struct {
  double a, b, c; // ax+by+c=0
} line;

typedef struct {
  point mid;
  double rad;
} circle;

typedef struct {
  point p1, p2;
} rec;

typedef struct {
  int k;
  point p1, p2;
  line l;
} cut;

typedef struct {
  char type;
  void *data;
} input;

void load_point(point *tmp) { assert(scanf("%lf%lf", &tmp->x, &tmp->y)); }

void load_rec(rec *tmp) { load_point(&tmp->p1), load_point(&tmp->p2); }

void load_circle(circle *tmp) {
  load_point(&tmp->mid);
  assert(scanf("%lf", &tmp->rad));
}

line make_line(const point *p1, const point *p2) {
  line res;
  if (!is_zero(p1->x - p2->x)) {
    res.b = 1;
    res.a = (-p1->y + p2->y) / (p1->x - p2->x);
    res.c = -res.a * p1->x - p1->y;
  } else {
    res.a = 1;
    res.b = 0;
    res.c = -p1->x;
  }
  return res;
}

void load_cut(cut *tmp) {
  assert(scanf("%d", &tmp->k));
  load_point(&tmp->p1), load_point(&tmp->p2);
  tmp->l = make_line(&tmp->p1, &tmp->p2);
}

void load(input *tmp) {
  switch (tmp->type) {
  case 'P':
    tmp->data = (void *)malloc(sizeof(rec));
    load_rec(tmp->data);
    return;
  case 'K':
    tmp->data = (void *)malloc(sizeof(circle));
    load_circle(tmp->data);
    return;
  default:
    tmp->data = (void *)malloc(sizeof(cut));
    load_cut(tmp->data);
    return;
  }
}

double square_range(const point *a, const point *b) {
  return (a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y);
}

bool on_line(const line *l, const point *tmp) {
  return is_zero(l->a * tmp->x + l->b * tmp->y + l->c);
}

bool inside_circle(const circle *a, const point *b) {
  return square_range(&a->mid, b) <= a->rad * a->rad + eps;
}

bool inside_rec(const rec *a, const point *b) {
  return b->x >= a->p1.x - eps and b->x <= a->p2.x + eps and
         b->y >= a->p1.y - eps and b->y <= a->p2.y + eps;
}

bool correct_side(const cut *tmp, const point *c) {
  point a = tmp->p1;
  point b = tmp->p2;
  return (b.x - a.x) * (c->y - a.y) - (b.y - a.y) * (c->x - a.x) > 0;
}

line flip_90(const line *tmp) {
  line res;
  res.c = 0;
  res.a = -tmp->b;
  res.b = tmp->a;
  return res;
}

// finds intersection of two lines, assumes they are not parallel
point cross_line(const line *l1, const line *l2) { // exists
  point res;
  if (!is_zero(l2->a)) {
    res.y =
        ((l2->c * l1->a / l2->a) - l1->c) / (l1->b - (l2->b * l1->a / l2->a));
    res.x = (-l2->c - l2->b * res.y) / l2->a;
  } else {
    res.y = -l2->c / l2->b;
    res.x = (-l1->c - l1->b * res.y) / l1->a;
  }
  return res;
}

// reflect a point relative to a line
point reverse(const line *l, const point *p) {
  line l2 = flip_90(l);
  l2.c = -(l2.a * p->x + l2.b * p->y); // goes through p
  point p2 = cross_line(l, &l2), res;
  res.x = p->x + (p2.x - p->x) + (p2.x - p->x);
  res.y = p->y + (p2.y - p->y) + (p2.y - p->y);
  return res;
}

// For each point p, reqursivly counts how many times it is inside the figure
int solve(const input *t, int k, const point *p) {
  switch (t[k].type) {
  case 'P':
    return inside_rec(t[k].data, p);
  case 'K':
    return inside_circle(t[k].data, p);
  default:
    // Processing bend.
    if (on_line(&((cut *)t[k].data)->l, p)) // Count only once.
      return solve(t, ((cut *)t[k].data)->k, p);
    if (!correct_side(t[k].data, p)) // Outside, do not count.
      return 0;
    point second = reverse(&((cut *)t[k].data)->l, p);
    return solve(t, ((cut *)t[k].data)->k, p) +
           solve(t, ((cut *)t[k].data)->k, &second);
  }
}

void make_free(int n, input *t) {
  for (int i = 0; i <= n; ++i) {
    if (t[i].data != 0)
      free(t[i].data);
  }
  free(t);
}

int main() {
  int n, q;
  assert(scanf("%d%d", &n, &q));
  input *t = (input *)malloc((size_t)(n + 1) * (sizeof(input)));
  for (int i = 0; i <= n; ++i)
    t[i].data = 0;
  for (int i = 1; i <= n; ++i) {
    assert(scanf(" %c", &t[i].type));
    load(&t[i]);
  }
  for (int i = 0; i < q; ++i) {
    int k;
    assert(scanf("%d", &k));
    point tmp;
    load_point(&tmp);
    printf("%d%c", solve(t, k, &tmp), '\n');
  }
  make_free(n, t);
}