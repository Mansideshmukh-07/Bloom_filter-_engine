# Advanced Scalable Persistent Bloom Filter

Advanced scalable persistent Bloom Filter implementation in C using MurmurHash3 and double hashing, featuring saturation analysis, load-factor monitoring, and experimental false positive validation.

---

## Features

* Persistent Bloom Filter storage
* MurmurHash3 hashing
* Double hashing optimization
* Configurable false positive probability
* Batch dataset insertion
* Batch lookup processing
* Collision tracking
* Load-factor monitoring
* Saturation analysis
* Experimental FPR validation
* Binary serialization/deserialization
* Interactive CLI support
* Scalable architecture design

---

## Mathematical Foundation

Optimal bit array size:

[
m = -\frac{n \ln p}{(\ln 2)^2}
]

Optimal number of hash functions:

[
k = \frac{m}{n}\ln 2
]

False positive probability:

[
p = \left(1 - e^{-kn/m}\right)^k
]

Where:

* (m) = total bits
* (n) = expected inserted elements
* (k) = number of hash functions
* (p) = false positive probability

---

## Build

```bash
gcc Bloom.c murmur3.c -lm -o bloom
```

---

## Usage

### Create Bloom Filter

```bash
./bloom create wordlist.txt 300000 0.01 words.bloom
```

### Load Existing Bloom Filter

```bash
./bloom load words.bloom
```

---

## Interactive Operations

```text
1. Insert Word
2. Lookup Word
3. Insert File
4. Lookup File
5. Statistics
6. Exit
```

---

## Experimental False Positive Validation

Example result:

```text
False Positives : 2139
Total Queries   : 214112
Experimental FPR: 0.00999
Expected FPR    : 0.01
```

---

## Example Statistics

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

## Performance

### Time Complexity

| Operation | Complexity |
| --------- | ---------- |
| Insert    | O(k)       |
| Lookup    | O(k)       |

### Space Complexity

```text
O(m)
```

---

## Real-World Applications

* Web crawlers
* News aggregation systems
* Duplicate URL detection
* Database query optimization
* Network packet filtering
* Distributed systems

---

## Future Improvements

* Scalable Bloom Filter
* mmap-based persistence
* Counting Bloom Filter
* SIMD optimization
* Concurrent Bloom Filter
* Distributed Bloom Filters

---

## Repository Structure

```text
.
├── Bloom.c
├── murmur3.c
├── murmur3.h
├── README.md
├── result.txt
├── unseen_words.txt
└── .gitignore
```

---
## Documentation

Detailed technical documentation is available inside the `docs/` directory.

| File                  | Description                                                                                              |
| --------------------- | -------------------------------------------------------------------------------------------------------- |
| `docs/design.md`      | System architecture, insertion/lookup workflow, scalability design, and internal Bloom Filter structure  |
| `docs/math.md`        | Mathematical derivations, false positive probability analysis, load-factor equations, and hashing theory |
| `docs/persistence.md` | Binary serialization format, persistence workflow, and future mmap-based storage design                  |
| `docs/benchmarks.md`  | Experimental results, saturation analysis, false positive validation, and performance evaluation         |

---
## Author

Manasi Deshmukh

