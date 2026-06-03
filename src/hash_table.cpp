#include "hash_table.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

// ==================== Constructor ====================

HashTable::HashTable(int size) : tableSize(size), count(0) {
    table.resize(tableSize);
}

// ==================== Hash Function ====================
// Polynomial rolling hash: h = (h*31 + c) % tableSize

int HashTable::hashFunction(const string& key) const {
    unsigned long h = 0;
    for (char c : key) {
        h = (h * 31UL + static_cast<unsigned char>(c)) % tableSize;
    }
    return static_cast<int>(h);
}

// ==================== CRUD Operations ====================

void HashTable::insert(const Product& p) {
    int idx = hashFunction(p.id);
    for (const auto& existing : table[idx]) {
        if (existing.id == p.id) return; // Duplikat: skip
    }
    table[idx].push_back(p);
    count++;
}

Product* HashTable::search(const string& id) {
    int idx = hashFunction(id);
    for (auto& p : table[idx]) {
        if (p.id == id) return &p;
    }
    return nullptr;
}

bool HashTable::updateStock(const string& id, int newStock) {
    Product* p = search(id);
    if (!p) return false;
    p->stok   = newStock;
    p->status = (newStock == 0) ? "out_of_stock" : "active";
    return true;
}

bool HashTable::remove(const string& id) {
    int idx = hashFunction(id);
    for (auto it = table[idx].begin(); it != table[idx].end(); ++it) {
        if (it->id == id) {
            table[idx].erase(it);
            count--;
            return true;
        }
    }
    return false;
}

// ==================== Utilities ====================

vector<Product> HashTable::getAllProducts() const {
    vector<Product> result;
    result.reserve(count);
    for (const auto& bucket : table) {
        for (const auto& p : bucket) result.push_back(p);
    }
    return result;
}

void HashTable::displayAll() const {
    if (count == 0) {
        cout << "  [Hash Table kosong]\n";
        return;
    }
    cout << "+-------------+------------------------+--------------+--------+---------------+---------------+\n";
    cout << "| ID          | Nama                   | Kategori     |   Stok |         Harga | Status        |\n";
    cout << "+-------------+------------------------+--------------+--------+---------------+---------------+\n";
    auto products = getAllProducts();
    for (const auto& p : products) p.displayCompact();
    cout << "+-------------+------------------------+--------------+--------+---------------+---------------+\n";
    cout << "  Total: " << count << " produk\n";
}

void HashTable::displayStats() const {
    int empty = 0, maxChain = 0, total = 0, used = 0;
    for (int i = 0; i < tableSize; i++) {
        int len = (int)table[i].size();
        if (len == 0) { empty++; }
        else { used++; total += len; if (len > maxChain) maxChain = len; }
    }
    cout << "\n  === Hash Table Statistics ===\n";
    cout << "  Table Size      : " << tableSize << "\n";
    cout << "  Jumlah Data     : " << count << "\n";
    cout << "  Load Factor     : " << fixed << setprecision(4) << getLoadFactor() << "\n";
    cout << "  Bucket Terisi   : " << used << " / " << tableSize << "\n";
    cout << "  Bucket Kosong   : " << empty << "\n";
    cout << "  Chain Terpanjang: " << maxChain << "\n";
    if (used > 0)
        cout << "  Rata-rata Chain : " << fixed << setprecision(2)
             << (double)total / used << "\n";
    cout << "  Est. Memori     : " << estimateMemory() / 1024 << " KB\n";
}
