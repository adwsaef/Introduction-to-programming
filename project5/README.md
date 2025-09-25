# PrevInRange Data Structure

Project implements a data structure that supports efficient queries on a sequence of integers `X`.  
The sequence may grow dynamically (elements can be appended online).  

We are interested in the function:

**`prevInRange(i, [lo, hi])`**  
→ returns the largest index `j` such that `0 ≤ j ≤ i` and `x[j] ∈ [lo, hi]`.  
If no such `j` exists, the result is `-1`.

Formally:  
prevInRange(i, [lo, hi]) = max{ j | 0 ≤ j ≤ i, x[j] ∈ [lo, hi] }  
                           or -1 if no such index exists

---

### Other functions:

- **`void init(const vector<int> &x)`**  
  Initialize the data structure with the starting sequence `X`.  
  - `X` can contain any `int` values.

- **`void pushBack(int v)`**  
  Append a new element `v` at the end of the sequence `X`.

- **`void done()`**  
  Free all memory used by the data structure.

---

## My solution
- I used a **persistent treap** as the core data structure.  
- Each prefix of the sequence corresponds to a separate *version*.  
- Every element `x` at index `i` is represented by a node, where:
  - the **key** is the value `x`,  
  - the **extra stored value** is the index `i`.  
- Each node maintains the maximum index in its subtree.  
- To answer a query:
  - Select the appropriate version (prefix),  
  - Query the treap for the maximum index within the key range `[lo, hi]`.  
- Both operations:
  - **`prevInRange`** query → **O(log n)** average time,  
  - **`pushBack`** (adding a new element) → **O(log n)** average time,  
  where `n` is the number of elements in the structure.
  