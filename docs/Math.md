# Bloom Filter Mathematics

# Optimal Bit Array Size

The optimal number of bits is:

[
m = -\frac{n \ln p}{(\ln 2)^2}
]

Where:

* m = total bits
* n = expected inserted elements
* p = false positive probability

---

# Why This Formula Exists

Bloom Filters balance:

* Memory usage
* False positive probability

Increasing memory reduces false positives.

Reducing memory increases saturation and collisions.

This formula mathematically minimizes memory usage for a target false positive probability.

---

# Optimal Number of Hash Functions

[
k = \frac{m}{n}\ln 2
]

Where:

* k = number of hash functions

This minimizes false positive probability.

Too many hash functions:

* increase collisions
* increase CPU cost

Too few hash functions:

* reduce randomness
* increase false positives

---

# False Positive Probability

[
p = \left(1 - e^{-kn/m}\right)^k
]

Where:

* p = false positive probability
* k = hash functions
* n = inserted elements
* m = total bits

---

# Load Factor

Load factor measures Bloom Filter saturation.

[
LoadFactor = \frac{setBits}{totalBits}
]

High saturation increases false positives.

---

# Collision Analysis

A collision occurs when:

```text
a bit that is already set is set again
```

Collisions increase naturally as saturation grows.

---

# Experimental FPR Validation

Experimental false positive rate:

[
ExperimentalFPR =
\frac{falsePositives}{totalQueries}
]

Example:

```text
False Positives : 2139
Total Queries   : 214112
Experimental FPR: 0.00999
Expected FPR    : 0.01
```

This validates the correctness of the implementation.

---

# Double Hashing

Instead of computing many independent hashes:

[
g_i(x) = h_1(x) + i \cdot h_2(x)
]

This significantly improves performance while maintaining good hash distribution.

---

# Space Complexity

[
O(m)
]

Where:

* m = total bits

---

# Time Complexity

Insertion:

[
O(k)
]

Lookup:

[
O(k)
]

Where:

* k = number of hash functions

