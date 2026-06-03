# Sistem Manajemen Inventori Gudang

Proyek Akhir Mata Kuliah Struktur Data — Semester Genap 2025/2026  
IPB University

## Deskripsi

Sistem manajemen inventori berbasis CLI yang mengimplementasikan dan membandingkan tiga struktur data secara simultan untuk operasi CRUD pada data produk gudang skala besar.

## Struktur Data yang Dibandingkan

| Struktur Data | Insert | Search | Update | Delete |
|--------------|--------|--------|--------|--------|
| Hash Table (Separate Chaining) | O(1) avg | O(1) avg | O(1) avg | O(1) avg |
| BST (Binary Search Tree) | O(log n) avg | O(log n) avg | O(log n) avg | O(log n) avg |
| Sorted Vector | O(n) | O(log n) | O(log n) | O(n) |

## Fitur

- **CRUD Produk** — Insert, Search by ID/Nama/Kategori, Update Stok, Delete
- **Benchmark Otomatis** — perbandingan waktu eksekusi 3 struktur data pada N = 100, 1K, 5K, 10K, 50K, 100K, 500K
- **File I/O** — auto-load/save produk ke CSV, export hasil benchmark ke CSV
- **Visualisasi** — script Python untuk generate grafik perbandingan (5 grafik PNG)

## Struktur Folder

```
sistem-inventori-gudang/
├── src/
│   ├── main.cpp            - Program utama + CLI menu
│   ├── product.h           - Struct Product (domain data)
│   ├── hash_table.h/.cpp   - Hash Table custom (separate chaining)
│   ├── bst.h/.cpp          - BST custom (keyed by product ID)
│   ├── sorted_vector.h     - Sorted Vector dengan binary search
│   ├── file_manager.h      - Load/save CSV
│   └── data_generator.h    - Generator data dummy realistis
├── data/
│   └── products.csv        - Data produk (generated at runtime)
├── results/
│   ├── benchmark.csv       - Hasil benchmark
│   └── grafik_*.png        - Grafik perbandingan performa
├── docs/
│   └── catatan_analisis.md - Catatan temuan benchmark
├── plot_benchmark.py       - Script visualisasi Python
└── Makefile
```

## Build & Run

### Kompilasi

```bash
g++ -std=c++17 -O2 -o inventori src/main.cpp src/hash_table.cpp src/bst.cpp
```

### Jalankan

```bash
./inventori        # Linux/Mac
.\inventori.exe    # Windows
```

### Generate Grafik

```bash
pip install matplotlib
python plot_benchmark.py
```

## Hasil Benchmark (Ringkasan)

Dijalankan pada N = 100 hingga 10.000, 200 operasi acak per pengukuran.

**Temuan utama:**
- Hash Table unggul untuk DELETE (9 μs vs Sorted Vector 19.157 μs di N=10K)
- BST unggul untuk SEARCH di N besar karena Hash Table load factor tinggi
- Sorted Vector: search sangat cepat (binary search ~0 μs), tapi DELETE O(n) sangat mahal
- BST worst case terjadi saat data sequential — insert N=10K tanpa shuffle: ~331ms vs ~2ms dengan shuffle

## Domain Data

```
Product:
  id        : string  (PRD-XXXXX)
  nama      : string
  kategori  : string  (Elektronik/Pakaian/Makanan/Minuman/Peralatan/Olahraga)
  stok      : int
  harga     : double  (Rupiah)
  status    : string  (active/out_of_stock/discontinued)
```

## Anggota Kelompok

IPB University — Departemen Ilmu Komputer

| No | Nama | NIM |
|----|------|-----|
| 1 | Khalifah Azka Hikari | M0403241150 |
| 2 | Mochamad Aleandre Moulidouane | M0403241105 |
| 3 | Muhammad Anaqi | M0403241046 |
| 4 | Wardah Nabilah Zahiroh | G6401231127 |
