#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define and &&
#define or ||

typedef struct {
  int val[3];
} triple;

// Resets all values in triple to -1 (used as a sentinel value)
void triple_reset(triple *tmp) {
  tmp->val[0] = -1, tmp->val[1] = -1, tmp->val[2] = -1;
}

// Find first or last three different companies.
triple find_triple(int n, int *company, bool front) {
  int start = 0, step = 1;
  if (!front)
    start = n - 1, step = -1;
  triple res;
  triple_reset(&res);
  for (int i = start; i >= 0 and i < n; i += step) {
    for (int j = 0; j < 3; ++j) {
      if (company[res.val[j]] == company[i])
        break;
      if (res.val[j] == -1) {
        res.val[j] = i;
        break;
      }
    }
  }
  return res;
}

// Check if the three indices are valid and correspond to different companies.
bool correct(int start, int middle, int end, int *company) {
  if (start == -1 or middle == -1 or end == -1)
    return false;
  if (!(start <= middle and middle <= end))
    return false;
  return company[start] != company[middle] and
         company[start] != company[end] and company[middle] != company[end];
}

// Go over all triples containing one element from triple first, middle and one
// element from triple last.
void query_maximalize_min(triple *first, int middle, triple *last, int *res,
                          int *company, int *pos) {
  for (int start = 0; start < 3; ++start) {
    for (int end = 0; end < 3; ++end) {
      if (correct(first->val[start], middle, last->val[end], company)) {
        int tmp = min(pos[middle] - pos[first->val[start]],
                      pos[last->val[end]] - pos[middle]);
        *res = max(*res, tmp);
      }
    }
  }
}

// It can be proven that the optimal solution always contains elements from the
// first triple and last triple. It is enough to iterate over all possible
// middle elements.
int maximalize_min(int n, int *company, int *pos) {
  triple first = find_triple(n, company, true),
         last = find_triple(n, company, false);
  int res = INT_MIN;
  for (int middle = 0; middle < n; ++middle) {
    query_maximalize_min(&first, middle, &last, &res, company, pos);
  }
  return res;
}

// Insert next index to triple (preferring big indexes).
void update_front(triple *tmp, int *company, int next) {
  // If company already exists, update its index.
  for (int i = 0; i < 3; ++i) {
    if (company[tmp->val[i]] == company[next]) {
      tmp->val[i] = next;
      return;
    }
  }
  // Otherwise, evict the maximum index.
  int min_p = INT_MAX;
  for (int i = 0; i < 3; ++i) {
    if (tmp->val[i] == -1) {
      tmp->val[i] = next;
      return;
    }
    min_p = min(min_p, tmp->val[i]);
  }
  for (int i = 0; i < 3; ++i) {
    if (tmp->val[i] == min_p) {
      tmp->val[i] = next;
      return;
    }
  }
}

// Insert next index to triple (preferring small indexes).
void update_back(triple *tmp, int *company, int next) {
  // If company already exists, update its index.
  for (int i = 0; i < 3; ++i) {
    if (company[tmp->val[i]] == company[next]) {
      tmp->val[i] = next;
      return;
    }
  }
  // Otherwise, evict the maximum index.
  int max_p = INT_MIN;
  for (int i = 0; i < 3; ++i) {
    if (tmp->val[i] == -1) {
      tmp->val[i] = next;
      return;
    }
    max_p = max(max_p, tmp->val[i]);
  }
  for (int i = 0; i < 3; ++i) {
    if (tmp->val[i] == max_p) {
      tmp->val[i] = next;
      return;
    }
  }
}

// Check all triples containing one element from triple first, middle and one
// element from triple last.
void query_minimalize_max(triple *first, int middle, triple *last, int *res,
                          int *company, int *pos) {
  for (int start = 0; start < 3; ++start) {
    for (int end = 0; end < 3; ++end) {
      if (correct(first->val[start], middle, last->val[end], company)) {
        int tmp = max(pos[middle] - pos[first->val[start]],
                      pos[last->val[end]] - pos[middle]);
        *res = min(*res, tmp);
      }
    }
  }
}

// For each index we need to store three nearest diffrent companies to left and right.
int minimalize_max(int n, int *company, int *pos) {
  // Precompute three nearest to the right.
  triple *reverse_latest = (triple *)malloc((size_t)n * sizeof(triple));
  triple tmp;
  triple_reset(&tmp);
  int res = INT_MAX;
  for (int i = n - 1; i >= 0; --i) {
    update_back(&tmp, company, i);
    reverse_latest[i] = tmp;
  }
  triple_reset(&tmp);
  update_front(&tmp, company, 0);
  for (int middle = 1; middle < n - 1; ++middle) {
    query_minimalize_max(&tmp, middle, &reverse_latest[middle + 1], &res,
                         company, pos);
    update_front(&tmp, company, middle);
  }
  return res;
}

// Handle corner case of fewer than three different companies
bool less_than_three(int n, int *company) {
  triple tmp = find_triple(n, company, true);
  return tmp.val[2] == -1;
}

int main() {
  int n;
  scanf("%d", &n);
  int *company = (int *)malloc((size_t)n * sizeof(int));
  int *pos = (int *)malloc((size_t)n * sizeof(int));
  for (int i = 0; i < n; ++i)
    scanf("%d%d", &company[i], &pos[i]);
  if (less_than_three(n, company)) {
    printf("0 0");
    return 0;
  }
  int tmp1 = minimalize_max(n, company, pos);
  int tmp2 = maximalize_min(n, company, pos);
  printf("%d %d", tmp1, tmp2);
  free(company);
  free(pos);
}
