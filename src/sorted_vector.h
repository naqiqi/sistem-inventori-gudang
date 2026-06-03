#ifndef SORTED_VECTOR_H
#define SORTED_VECTOR_H

#include "product.h"
#include <vector>
#include <algorithm>

// ============================================================
//  SortedVector - vector yang selalu terurut berdasarkan ID
//
//  Kompleksitas:
//    Insert : O(n)     - perlu menggeser elemen (menjaga urutan)
//    Search : O(log n) - binary search karena terurut
//    Update : O(log n) - cari dulu via binary search
//    Delete : O(n)     - erase menyebabkan shift
//
//  Digunakan sebagai BASELINE perbandingan terhadap BST & Hash Table.
//  Menunjukkan trade-off: search cepat tapi insert/delete mahal.
// ============================================================
class SortedVector {
private:
    vector<Product> data; // Selalu terurut ascending by ID

    // Cari index via binary search; kembalikan -1 jika tidak ada
    int binarySearch(const string& id) const {
        int lo = 0, hi = (int)data.size() - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if      (data[mid].id == id) return mid;
            else if (data[mid].id <  id) lo = mid + 1;
            else                         hi = mid - 1;
        }
        return -1;
    }

public:
    SortedVector() = default;

    // Insert - cari posisi via lower_bound, insert di sana
    void insert(const Product& p) {
        auto pos = lower_bound(data.begin(), data.end(), p,
                               [](const Product& a, const Product& b) {
                                   return a.id < b.id;
                               });
        // Cek duplikat
        if (pos != data.end() && pos->id == p.id) return;
        data.insert(pos, p); // O(n) karena shift
    }

    // Search via binary search - O(log n)
    Product* search(const string& id) {
        int idx = binarySearch(id);
        return (idx >= 0) ? &data[idx] : nullptr;
    }

    // Update stok - O(log n) cari + O(1) update
    bool updateStock(const string& id, int newStock) {
        int idx = binarySearch(id);
        if (idx < 0) return false;
        data[idx].stok   = newStock;
        data[idx].status = (newStock == 0) ? "out_of_stock" : "active";
        return true;
    }

    // Delete - O(log n) cari + O(n) shift
    bool remove(const string& id) {
        int idx = binarySearch(id);
        if (idx < 0) return false;
        data.erase(data.begin() + idx);
        return true;
    }

    vector<Product> getAllProducts() const { return data; }

    void displayAll() const {
        if (data.empty()) { cout << "  [Sorted Vector kosong]\n"; return; }
        cout << "+-------------+------------------------+--------------+--------+---------------+---------------+\n";
        cout << "| ID          | Nama                   | Kategori     |   Stok |         Harga | Status        |\n";
        cout << "+-------------+------------------------+--------------+--------+---------------+---------------+\n";
        for (const auto& p : data) p.displayCompact();
        cout << "+-------------+------------------------+--------------+--------+---------------+---------------+\n";
        cout << "  Total: " << data.size() << " produk\n";
    }

    int    getSize()  const { return (int)data.size(); }
    bool   isEmpty()  const { return data.empty(); }
    size_t estimateMemory() const {
        return sizeof(*this) + data.capacity() * sizeof(Product);
    }
};

#endif // SORTED_VECTOR_H
