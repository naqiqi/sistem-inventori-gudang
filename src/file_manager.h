#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "product.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

// ============================================================
//  FileManager - penyimpanan file CSV 
//
//  Format CSV produk:
//    id,nama,kategori,stok,harga,status
//
//  Format CSV benchmark:
//    n,op,bst_us,ht_us,sv_us
// ============================================================

struct BenchmarkRow {
    int         n;          // Ukuran data
    string      operation;  // INSERT | SEARCH | UPDATE | DELETE
    long long   bst_us;     // Waktu BST (microseconds)
    long long   ht_us;      // Waktu Hash Table (microseconds)
    long long   sv_us;      // Waktu Sorted Vector (microseconds)
};

class FileManager {
public:
    // ---- Produk ----

    // Simpan semua produk ke file CSV
    static bool saveProducts(const string& filepath,
                             const vector<Product>& products) {
        ofstream f(filepath);
        if (!f.is_open()) {
            cerr << "  [ERROR] Tidak bisa membuka file: " << filepath << "\n";
            return false;
        }
        f << "id,nama,kategori,stok,harga,status\n"; // Header
        for (const auto& p : products) f << p.toCSV() << "\n";
        cout << "  [OK] " << products.size() << " produk disimpan ke " << filepath << "\n";
        return true;
    }

    // Load produk dari file CSV; kembalikan vector<Product>
    static vector<Product> loadProducts(const string& filepath) {
        vector<Product> result;
        ifstream f(filepath);
        if (!f.is_open()) {
            cerr << "  [ERROR] File tidak ditemukan: " << filepath << "\n";
            return result;
        }
        string line;
        bool header = true;
        while (getline(f, line)) {
            if (header) { header = false; continue; } // Skip header
            if (line.empty()) continue;
            Product p = Product::fromCSV(line);
            if (!p.id.empty()) result.push_back(p);
        }
        cout << "  [OK] " << result.size() << " produk dimuat dari " << filepath << "\n";
        return result;
    }

    // ---- Benchmark ----

    // Export hasil benchmark ke CSV
    static bool saveBenchmark(const string& filepath,
                              const vector<BenchmarkRow>& rows) {
        ofstream f(filepath);
        if (!f.is_open()) {
            cerr << "  [ERROR] Tidak bisa membuka file: " << filepath << "\n";
            return false;
        }
        f << "n,operasi,bst_us,hashtable_us,sorted_vector_us\n";
        for (const auto& r : rows) {
            f << r.n << "," << r.operation << ","
              << r.bst_us << "," << r.ht_us << "," << r.sv_us << "\n";
        }
        cout << "  [OK] Hasil benchmark disimpan ke " << filepath << "\n";
        return true;
    }

    // Cek apakah file ada
    static bool fileExists(const string& filepath) {
        ifstream f(filepath);
        return f.good();
    }
};

#endif // FILE_MANAGER_H
