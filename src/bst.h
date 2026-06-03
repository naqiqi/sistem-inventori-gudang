#ifndef BST_H
#define BST_H

#include "product.h"
#include <vector>

// ============================================================
//  BST - Binary Search Tree keyed by product ID
//  Kompleksitas: Insert/Search/Delete O(log n) avg, O(n) worst
//  (catatan: tidak self-balancing, worst case saat data terurut)
// ============================================================

struct BSTNode {
    Product  data;
    BSTNode* left;
    BSTNode* right;
    explicit BSTNode(const Product& p) : data(p), left(nullptr), right(nullptr) {}
};

class BST {
private:
    BSTNode* root;
    int      size;

    // Helper rekursif
    BSTNode* insertHelper(BSTNode* node, const Product& p);
    BSTNode* searchHelper(BSTNode* node, const string& id) const;
    BSTNode* deleteHelper(BSTNode* node, const string& id);
    BSTNode* findMin(BSTNode* node) const;
    void     inorder(BSTNode* node, vector<Product>& out) const;
    void     destroy(BSTNode* node);
    void     searchByNameHelper(BSTNode* node, const string& nama,
                                vector<Product>& out) const;
    void     searchByCategoryHelper(BSTNode* node, const string& kat,
                                    vector<Product>& out) const;

public:
    BST();
    ~BST();

    // Operasi CRUD
    void     insert(const Product& p);
    Product* search(const string& id);
    bool     updateStock(const string& id, int newStock);
    bool     remove(const string& id);

    // Search tambahan
    vector<Product> searchByName(const string& nama) const;
    vector<Product> searchByCategory(const string& kat) const;

    // Utilitas
    vector<Product> getAllProducts() const;
    void            displayAll() const;
    int             getSize()  const { return size; }
    bool            isEmpty()  const { return root == nullptr; }
    // Estimasi penggunaan memori (byte)
    size_t estimateMemory() const {
        return sizeof(*this) + size * (sizeof(BSTNode));
    }
};

#endif // BST_H
