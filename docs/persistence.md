# Persistence Design

# Overview

This project supports persistent Bloom Filters.

The Bloom Filter can be:

* saved to disk
* reloaded later
* reused across program executions

This simulates real-world production systems.

---

# Why Persistence Matters

Without persistence:

```text
all Bloom Filter data is lost after program termination
```

Persistence allows:

* faster startup
* reusable filters
* long-term caching
* large-scale deployment

---

# Binary Serialization

The Bloom Filter is stored in binary format.

Advantages:

* compact storage
* fast reading
* fast writing
* direct memory representation

---

# File Layout

The file structure is:

```text
[metadata][bit_array]
```

---

# Stored Metadata

The following fields are serialized:

```text
m
bytes
k
expected_n
inserted_n
set_bits_count
collision_count
p
seed
hash_type
```

After metadata:

```text
bit_array
```

is written directly to disk.

---

# Save Workflow

## Step 1

Open file in binary write mode:

```c
fopen(filename, \"wb\")
```

---

## Step 2

Write metadata using:

```c
fwrite()
```

---

## Step 3

Write entire bit array.

---

## Step 4

Close file.

---

# Load Workflow

## Step 1

Open file in binary read mode:

```c
fopen(filename, \"rb\")
```

---

## Step 2

Allocate BloomFilter structure.

---

## Step 3

Read metadata.

---

## Step 4

Allocate memory for bit array.

---

## Step 5

Load bit array from disk.

---

# Future Improvements

Future persistence optimizations include:

* mmap-based persistence
* lazy loading
* compressed bit arrays
* distributed persistence
* concurrent access support

---

# mmap-Based Design

Future versions may use:

```c
mmap()
```

This maps disk files directly into virtual memory.

Advantages:

* reduced copy overhead
* faster startup
* OS-managed paging
* improved scalability

This is commonly used in high-performance systems.

