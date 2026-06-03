#ifndef DATA_GENERATOR_H
#define DATA_GENERATOR_H

#include "product.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>

// ============================================================
//  DataGenerator - membuat dataset produk dummy realistis
//  ID sequential -> PRD-00001 s.d. PRD-NNNNN (tidak collision)
// ============================================================
class DataGenerator {
private:
    static const vector<string> NAMA_DEPAN;
    static const vector<string> NAMA_BELAKANG;
    static const vector<string> KATEGORI;

    string generateId(int idx) const {
        ostringstream ss;
        ss << "PRD-" << setw(5) << setfill('0') << idx;
        return ss.str();
    }

    string generateNama() const {
        return NAMA_DEPAN[rand() % NAMA_DEPAN.size()] + " "
             + NAMA_BELAKANG[rand() % NAMA_BELAKANG.size()];
    }

    string generateKategori() const {
        return KATEGORI[rand() % KATEGORI.size()];
    }

    int generateStok() const {
        return (rand() % 10 == 0) ? 0 : rand() % 1000 + 1;
    }

    double generateHarga(const string& kat) const {
        if (kat == "Elektronik") return (rand() % 9000 + 1000) * 1000.0;
        if (kat == "Pakaian")    return (rand() % 450  + 50)  * 1000.0;
        if (kat == "Makanan")    return (rand() % 90   + 10)  * 1000.0;
        if (kat == "Minuman")    return (rand() % 45   + 5)   * 1000.0;
        if (kat == "Peralatan")  return (rand() % 2000 + 200) * 1000.0;
        return                          (rand() % 3000 + 300) * 1000.0; // Olahraga
    }

    string generateStatus(int stok) const {
        if (stok == 0)            return "out_of_stock";
        if (rand() % 20 == 0)    return "discontinued";
        return "active";
    }

public:
    DataGenerator() { srand(static_cast<unsigned>(time(nullptr))); }

    Product generateOne(int idx) const {
        string kat  = generateKategori();
        int    stok = generateStok();
        return Product(generateId(idx), generateNama(), kat,
                       stok, generateHarga(kat), generateStatus(stok));
    }

    vector<Product> generate(int count) const {
        vector<Product> out;
        out.reserve(count);
        for (int i = 1; i <= count; i++) out.push_back(generateOne(i));
        return out;
    }
};

// Definisi static member di header agar header-only
inline const vector<string> DataGenerator::NAMA_DEPAN = {
    "Premium", "Super", "Ultra", "Mega", "Pro", "Classic", "Royal",
    "Golden", "Silver", "Diamond", "Fresh", "Natural", "Eco", "Smart",
    "Digital", "Mini", "Max", "Elite", "Prime", "Original"
};

inline const vector<string> DataGenerator::NAMA_BELAKANG = {
    "Widget", "Gadget", "Device", "Tool", "Kit", "Pack", "Set", "Box",
    "Bottle", "Shirt", "Pants", "Jacket", "Cable", "Charger", "Speaker",
    "Snack", "Drink", "Rice", "Ball", "Racket", "Glove", "Mat", "Bag",
    "Mouse", "Keyboard", "Monitor", "Case", "Jar", "Tube", "Cup"
};

inline const vector<string> DataGenerator::KATEGORI = {
    "Elektronik", "Pakaian", "Makanan", "Minuman", "Peralatan", "Olahraga"
};

#endif // DATA_GENERATOR_H
