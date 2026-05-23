# Benchmarks and Experimental Analysis

# Experimental Setup

Dataset:

```text
wordlist.txt
```

Configuration:

```text
Expected Elements : 300000
Target FPR        : 0.01
```

---

# Generated Bloom Filter

Example output:

```text
************** BLOOM FILTER STATS ************
Expected Elements : 300395
Inserted Elements : 300394
Total Bits        : 2879303
Total Bytes       : 359913
Hash Functions    : 7
Set Bits          : 1491507
Collision Count   : 611251
Load Factor       : 0.518010
False Positive p  : 0.010000
```

---

# Experimental False Positive Validation

Unseen test dataset generated using:

```bash
grep -vxFf wordlist.txt words_alpha.txt > unseen_words.txt
```

This extracts words not present in the original dataset.

---

# Lookup Experiment

Batch lookup performed on unseen dataset.

Results:

```text
False Positives : 2139
Total Queries   : 214112
Experimental FPR: 0.00999
Expected FPR    : 0.01
```

---

# Analysis

The experimental false positive rate closely matches the theoretical target probability.

This validates:

* hashing correctness
* bit distribution quality
* parameter calculation formulas
* persistence correctness

---

# Saturation Analysis

Observed load factor:

[
0.518
]

This indicates:

* approximately 51.8% of bits are set
* filter is operating near optimal saturation

Extremely high saturation increases false positives rapidly.

---

# Collision Analysis

Observed collisions:

```text
611251
```

Collisions are expected behavior in Bloom Filters and increase naturally as the filter becomes saturated.

---

# Performance Characteristics

## Insert Complexity

[
O(k)
]

## Lookup Complexity

[
O(k)
]

Where:

* k = number of hash functions

---

# Memory Efficiency

The system stores:

```text
300000+ elements
```

using only:

```text
~360 KB
```

This demonstrates the memory efficiency of probabilistic data structures compared to exact storage methods.

---

# Future Benchmarking

Future experiments:

* insertion throughput
* lookup throughput
* scalable Bloom Filter benchmarking
* mmap performance comparison
* SIMD optimization analysis
* concurrent workload testing

