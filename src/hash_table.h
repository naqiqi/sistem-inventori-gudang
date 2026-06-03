#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "product.h"
#include <vector>
#include <list>

// ============================================================
//  HashTable - separate chaining, polynomial rolling hash
//  Kompleksitas: Insert/Search/Delete O(1) rata-rata, O(n) worst
// ============================================================
class HashTable {
private:
    static const int DEFAULT_SIZE = 10007; // Bilangan prima
    int   tableSize;
    int   count;
    vector<list<Product>> table;

    int hashFunction(const string& key) const;

public:
    explicit HashTable(int size = DEFAULT_SIZE);

    // Operasi CRUD
    void     insert(const Product& p);
    Product* search(const string& id);
    bool     updateStock(const string& id, int newStock);
    bool     remove(const string& id);

    // Utilitas
    vector<Product> getAllProducts() const;
    void            displayAll() const;
    void            displayStats() const;
    int             getSize()       const { return count; }
    bool            isEmpty()       const { return count == 0; }
    double          getLoadFactor() const {
        return static_cast<double>(count) / tableSize;
    }
    // Estimasi penggunaan memori (byte)
    size_t estimateMemory() const {
        return sizeof(*this)
             + tableSize * sizeof(list<Product>)
             + count * sizeof(Product);
    }
};

#endif // HASH_TABLE_H
