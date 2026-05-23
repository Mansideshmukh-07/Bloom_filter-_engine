# Bloom Filter System Design

## Overview

This project implements a scalable persistent Bloom Filter engine in C for high-performance probabilistic membership testing.

The system is designed for:

* Fast insertions
* Fast lookups
* Low memory usage
* Persistent storage
* Analytical monitoring

---

# Core Architecture

## Components

### 1. BloomFilter Structure

The main structure stores:

* Bit array
* Total bits
* Number of hash functions
* Collision statistics
* False positive probability
* Persistence metadata

---

## 2. Bit Array

The Bloom Filter uses a compact bit array:

```text
[0 1 0 0 1 1 0 1 ...]
```

Instead of storing full strings, only bits are set.

This enables extremely memory-efficient membership testing.

---

## 3. Hashing Pipeline

The system uses:

* MurmurHash3
* Double hashing

Instead of computing many expensive hashes independently:

```text
hash1(word)
hash2(word)
```

additional hashes are generated using:

```text
combined_hash = h1 + i * h2
```

This reduces computational overhead significantly.

---

# Insertion Workflow

## Step 1

Input word is passed to MurmurHash3.

---

## Step 2

Two base hashes are generated:

```text
h1
h2
```

---

## Step 3

k hash values are derived using double hashing.

---

## Step 4

Corresponding bits are set inside the bit array.

---

# Lookup Workflow

## Step 1

Generate hashes for input word.

---

## Step 2

Check all corresponding bits.

If any bit is zero:

```text
Definitely Not Present
```

If all bits are set:

```text
Possibly Present
```

---

# Persistence Design

The Bloom Filter is serialized in binary format.

File structure:

```text
[metadata][bit_array]
```

Metadata includes:

* m
* bytes
* k
* inserted elements
* collision count
* false positive probability
* hashing seed

The complete bit array is then stored.

---

# Statistical Monitoring

The system tracks:

* Set bits
* Collision count
* Saturation level
* Load factor
* Experimental FPR

This enables analytical performance evaluation.

---

# Scalability Design

The architecture supports future extensions:

* Scalable Bloom Filters
* Distributed Bloom Filters
* mmap persistence
* Concurrent implementations
* SIMD optimizations

---

# Real-World Use Cases

* Web crawling
* Duplicate URL detection
* News aggregation
* Database filtering
* Networking systems
* Distributed caching

