#include "bst.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

// ==================== Constructor & Destructor ====================

BST::BST() : root(nullptr), size(0) {}

BST::~BST() { destroy(root); }

void BST::destroy(BSTNode* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// ==================== Helper Functions ====================

BSTNode* BST::insertHelper(BSTNode* node, const Product& p) {
    if (!node) { size++; return new BSTNode(p); }
    if (p.id < node->data.id)      node->left  = insertHelper(node->left,  p);
    else if (p.id > node->data.id) node->right = insertHelper(node->right, p);
    // Duplikat: abaikan
    return node;
}

BSTNode* BST::searchHelper(BSTNode* node, const string& id) const {
    if (!node || node->data.id == id) return node;
    if (id < node->data.id) return searchHelper(node->left,  id);
    return                         searchHelper(node->right, id);
}

BSTNode* BST::findMin(BSTNode* node) const {
    while (node && node->left) node = node->left;
    return node;
}

BSTNode* BST::deleteHelper(BSTNode* node, const string& id) {
    if (!node) return nullptr;
    if      (id < node->data.id) node->left  = deleteHelper(node->left,  id);
    else if (id > node->data.id) node->right = deleteHelper(node->right, id);
    else {
        // Node ditemukan
        if (!node->left) {
            BSTNode* tmp = node->right; delete node; size--; return tmp;
        }
        if (!node->right) {
            BSTNode* tmp = node->left;  delete node; size--; return tmp;
        }
        // Dua anak: ganti dengan inorder successor
        BSTNode* succ = findMin(node->right);
        node->data = succ->data;
        node->right = deleteHelper(node->right, succ->data.id);
    }
    return node;
}

void BST::inorder(BSTNode* node, vector<Product>& out) const {
    if (!node) return;
    inorder(node->left, out);
    out.push_back(node->data);
    inorder(node->right, out);
}

void BST::searchByNameHelper(BSTNode* node, const string& nama,
                              vector<Product>& out) const {
    if (!node) return;
    searchByNameHelper(node->left,  nama, out);
    string a = node->data.nama, b = nama;
    transform(a.begin(), a.end(), a.begin(), ::tolower);
    transform(b.begin(), b.end(), b.begin(), ::tolower);
    if (a.find(b) != string::npos) out.push_back(node->data);
    searchByNameHelper(node->right, nama, out);
}

void BST::searchByCategoryHelper(BSTNode* node, const string& kat,
                                  vector<Product>& out) const {
    if (!node) return;
    searchByCategoryHelper(node->left,  kat, out);
    string a = node->data.kategori, b = kat;
    transform(a.begin(), a.end(), a.begin(), ::tolower);
    transform(b.begin(), b.end(), b.begin(), ::tolower);
    if (a.find(b) != string::npos) out.push_back(node->data);
    searchByCategoryHelper(node->right, kat, out);
}

// ==================== Public Methods ====================

void BST::insert(const Product& p) { root = insertHelper(root, p); }

Product* BST::search(const string& id) {
    BSTNode* n = searchHelper(root, id);
    return n ? &n->data : nullptr;
}

bool BST::updateStock(const string& id, int newStock) {
    BSTNode* n = searchHelper(root, id);
    if (!n) return false;
    n->data.stok   = newStock;
    n->data.status = (newStock == 0) ? "out_of_stock" : "active";
    return true;
}

bool BST::remove(const string& id) {
    if (!search(id)) return false;
    root = deleteHelper(root, id);
    return true;
}

vector<Product> BST::searchByName(const string& nama) const {
    vector<Product> out;
    searchByNameHelper(root, nama, out);
    return out;
}

vector<Product> BST::searchByCategory(const string& kat) const {
    vector<Product> out;
    searchByCategoryHelper(root, kat, out);
    return out;
}

vector<Product> BST::getAllProducts() const {
    vector<Product> out;
    inorder(root, out);
    return out;
}

void BST::displayAll() const {
    if (!root) { cout << "  [BST kosong]\n"; return; }
    cout << "+-------------+------------------------+--------------+--------+---------------+---------------+\n";
    cout << "| ID          | Nama                   | Kategori     |   Stok |         Harga | Status        |\n";
    cout << "+-------------+------------------------+--------------+--------+---------------+---------------+\n";
    auto products = getAllProducts();
    for (const auto& p : products) p.displayCompact();
    cout << "+-------------+------------------------+--------------+--------+---------------+---------------+\n";
    cout << "  Total: " << size << " produk\n";
}
