# Origami Problem

The goal is to build a program for origami fans that calculates **how many paper layers** exist at a given point after folding operations.  

The input consists of:
- A set of paper sheets (`n`),
- A set of queries (`q`) asking for the number of layers at specific points.  

Each sheet can be:
- **Rectangle** `P x1 y1 x2 y2`  
- **Circle** `K x y r`  
- **Folded sheet** `Z k x1 y1 x2 y2` (folds sheet `k` along line through `(x1,y1)` → `(x2,y2)`)

Each query is in the form:  
`k x y` → "How many layers are at point `(x,y)` on sheet `k`?"

## Example

### Input
```
4 5
P 0 0 2.5 1
Z 1 0.5 0 0.5 1
K 0 1 5
Z 3 0 1 -1 0
1 1.5 0.5
2 1.5 0.5
2 0 0.5
3 4 4
4 4 4
```

### Output
```
1
0
2
1
2
```
