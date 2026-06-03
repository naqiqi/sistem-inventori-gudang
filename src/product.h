#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;

// ============================================================
//  Struct Product - domain data inventori gudang
// ============================================================
struct Product {
    string id;        // Format: "PRD-XXXXX"
    string nama;
    string kategori;  // Elektronik, Pakaian, Makanan, dll.
    int    stok;
    double harga;
    string status;    // "active" | "out_of_stock" | "discontinued"

    Product() : stok(0), harga(0.0) {}

    Product(const string& _id, const string& _nama, const string& _kategori,
            int _stok, double _harga, const string& _status)
        : id(_id), nama(_nama), kategori(_kategori),
          stok(_stok), harga(_harga), status(_status) {}

    // Perbandingan berdasarkan ID (untuk BST & SortedVector)
    bool operator<(const Product& o) const { return id < o.id; }
    bool operator==(const Product& o) const { return id == o.id; }

    // Tampilkan satu produk secara detail
    void display() const {
        cout << "  +-----------------------+----------------------------------------+\n";
        cout << "  | " << left << setw(21) << "ID"       << " | " << setw(38) << id       << " |\n";
        cout << "  | " << left << setw(21) << "Nama"     << " | " << setw(38) << nama     << " |\n";
        cout << "  | " << left << setw(21) << "Kategori" << " | " << setw(38) << kategori << " |\n";
        cout << "  | " << left << setw(21) << "Stok"     << " | " << setw(38) << stok     << " |\n";
        cout << "  | " << left << setw(21) << "Harga"    << " | Rp "
             << setw(35) << fixed << setprecision(0) << harga << " |\n";
        cout << "  | " << left << setw(21) << "Status"   << " | " << setw(38) << status   << " |\n";
        cout << "  +-----------------------+----------------------------------------+\n";
    }

    // Tampilkan ringkas (satu baris tabel)
    void displayCompact() const {
        string namaStr = (nama.length() > 22) ? nama.substr(0, 19) + "..." : nama;
        cout << "| " << left  << setw(11) << id
             << " | " << left  << setw(22) << namaStr
             << " | " << left  << setw(12) << kategori
             << " | " << right << setw(6)  << stok
             << " | " << right << setw(13) << fixed << setprecision(0) << harga
             << " | " << left  << setw(13) << status
             << " |\n";
    }

    // Konversi ke format CSV (satu baris)
    string toCSV() const {
        ostringstream oss;
        oss << id << "," << nama << "," << kategori << ","
            << stok << "," << fixed << setprecision(2) << harga << "," << status;
        return oss.str();
    }

    // Parse dari baris CSV
    static Product fromCSV(const string& line) {
        istringstream ss(line);
        string token;
        vector<string> tok;
        while (getline(ss, token, ',')) tok.push_back(token);
        if (tok.size() < 6) return Product();
        try {
            return Product(tok[0], tok[1], tok[2],
                           stoi(tok[3]), stod(tok[4]), tok[5]);
        } catch (...) {
            return Product();
        }
    }
};

#endif // PRODUCT_H
