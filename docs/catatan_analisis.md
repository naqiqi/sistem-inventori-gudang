# Catatan Analisis Benchmark — Sistem Inventori Gudang

## Struktur Data yang Dibandingkan
1. **Hash Table** — Separate chaining, polynomial rolling hash, table size 10007
2. **BST** — Binary Search Tree (tidak self-balancing), key = Product ID
3. **Sorted Vector** — vector terurut + binary search

## Hasil Benchmark Kunci (μs)

### INSERT (total N elemen)
| N       | Hash Table | BST    | Sorted Vector |
|---------|-----------|--------|--------------|
| 1.000   | 155       | 231    | 209          |
| 10.000  | 1.247     | 3.762  | 3.396        |
| 50.000  | 9.546     | 26.340 | 14.817       |
| 100.000 | 14.936    | 72.780 | N/A (terlalu lambat) |
| 500.000 | 1.238.602 | 887.996| N/A          |

### SEARCH (200 operasi acak)
| N       | Hash Table | BST  | Sorted Vector |
|---------|-----------|------|--------------|
| 1.000   | 6         | 61   | <1           |
| 10.000  | 36        | 174  | <1           |
| 50.000  | 108       | 270  | <1           |
| 100.000 | 203       | 354  | <1           |
| 500.000 | 1.121     | 574  | <1           |

## Insight Utama untuk Laporan

### 1. Hash Table vs BST — Insert di N=500K
Fenomena menarik: HT (1.24s) lebih LAMBAT dari BST (0.89s) saat N=500K.
**Penyebab:** Table size tetap 10007. Load factor = 500K/10007 ≈ 49.97.
Rata-rata setiap bucket punya 50 elemen → traversal chain = O(LF) ≈ O(n/m).
**Solusi nyata:** Dynamic resizing (rehashing) saat load factor > 0.75.
**Poin laporan:** Ini membuktikan bahwa hash table tanpa resize tetap O(1) hanya jika LF < 1.

### 2. Sorted Vector Search selalu <1μs
Binary search sangat cepat untuk 200 operasi → sub-microsecond.
Namun DELETE sangat mahal (88ms untuk 100 operasi di N=50K) karena shift elemen O(n).
**Kesimpulan:** SV cocok untuk read-heavy workload, tidak cocok untuk frequent delete.

### 3. BST Worst Case (data sequential)
Jika ID diinsert berurutan (PRD-00001, PRD-00002, ...) tanpa shuffle,
BST degenerate menjadi linked list → O(n²) total insert.
Di N=10K tanpa shuffle: BST ≈ 419ms vs dengan shuffle: ≈ 4ms.
**Poin laporan:** Ini adalah worst case klasik BST, solusinya adalah AVL/Red-Black Tree.

### 4. Kesimpulan Rekomendasi
- **Sistem inventori real-world (query by ID):** Hash Table — O(1) avg, TAPI harus implement dynamic resizing
- **Jika perlu ordered traversal / range query:** BST (atau ideally AVL)
- **Jika data kecil & read-heavy:** Sorted Vector cukup memadai
