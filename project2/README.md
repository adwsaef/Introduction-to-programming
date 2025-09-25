# Motel Triples Problem

Along a straight highway, there are **n** motels, each at a specific distance from the starting point. Every motel belongs to a chain, identified by an integer between `1` and `n`.

A traveller stays overnight in **three motels**, each from a **different chain**. The task is to find:

- The **closest triple**: three motels (A, B, C in order along the highway, each from a different chain) such that the **largest** of the two distances between consecutive motels (`A → B` and `B → C`) is as **small** as possible.
- The **furthest triple**: three motels (A, B, C in order, each from a different chain) such that the **smallest** of the two distances between consecutive motels is as **large** as possible.

If no such triple exists, output `0 0`.

## Input

- The first line contains an integer `n` — the number of motels.
- Each of the next `n` lines contains two integers:
  - the motel’s **chain number** (`1` to `n`)
  - the motel’s **distance** from the start of the highway

Motels are listed in the order they appear along the highway.

## Output

Print two integers separated by a space:
1. The **maximum of the minimum distances** for the furthest triple.
2. The **minimum of the maximum distances** for the closest triple.

## Example

### Input
```
9
1 2
2 6
2 9
1 13
1 17
3 20
1 26
3 27
1 30
```

### Output
```
7 10
```
