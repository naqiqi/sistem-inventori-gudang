/*
 * ================================================================
 *  SISTEM MANAJEMEN INVENTORI GUDANG
 *  Proyek Akhir Struktur Data - Semester Genap 2025/2026
 *  IPB University
 * ================================================================
 *  Struktur Data yang Diimplementasikan & Dibandingkan:
 *    1. Hash Table   - Separate Chaining, O(1) avg
 *    2. BST          - Binary Search Tree, O(log n) avg
 *    3. Sorted Vector- Sorted array + binary search, O(n) insert
 *
 *  Fitur:
 *    - CRUD produk (Insert, Search, Update Stok, Delete)
 *    - Benchmark otomatis perbandingan 3 struktur data
 *    - Penyimpanan file (load/save CSV) - wajib per spesifikasi
 *    - Export hasil benchmark ke CSV
 * ================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <chrono>
#include <limits>
#include <algorithm>
#include <cstdlib>

#include "product.h"
#include "hash_table.h"
#include "bst.h"
#include "sorted_vector.h"
#include "file_manager.h"
#include "data_generator.h"

using namespace std;
using namespace std::chrono;

// ==================== Global Instances ====================
HashTable    hashTable;
BST          bst;
SortedVector sortedVec;
DataGenerator generator;

const string DATA_FILE      = "data/products.csv";
const string BENCHMARK_FILE = "results/benchmark.csv";

// ==================== Utility Functions ====================

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pressEnter() {
    cout << "\n  Tekan Enter untuk melanjutkan...";
    cin.get();
}

void printHeader() {
    cout << "\n";
    cout << "  +==============================================================+\n";
    cout << "  |       SISTEM MANAJEMEN INVENTORI GUDANG                      |\n";
    cout << "  |       Proyek Akhir Struktur Data - Genap 2025/2026           |\n";
    cout << "  |       IPB University                                         |\n";
    cout << "  +==============================================================+\n";
}

void printMenu() {
    cout << "\n  +--------------------------------------------+\n";
    cout << "  |             MENU UTAMA                      |\n";
    cout << "  +--------------------------------------------+\n";
    cout << "  |  [DATA]                                     |\n";
    cout << "  |  1. Load Data dari File CSV                 |\n";
    cout << "  |  2. Generate Dataset Dummy                  |\n";
    cout << "  |  3. Simpan Data ke File CSV                 |\n";
    cout << "  +--------------------------------------------+\n";
    cout << "  |  [CRUD]                                     |\n";
    cout << "  |  4. Insert Produk                           |\n";
    cout << "  |  5. Search Produk                           |\n";
    cout << "  |  6. Update Stok Produk                      |\n";
    cout << "  |  7. Delete Produk                           |\n";
    cout << "  |  8. Tampilkan Semua Produk                  |\n";
    cout << "  +--------------------------------------------+\n";
    cout << "  |  [ANALISIS]                                 |\n";
    cout << "  |  9. Benchmark Performa (3 Struktur Data)    |\n";
    cout << "  | 10. Statistik Struktur Data                 |\n";
    cout << "  | 11. Export Benchmark ke CSV                 |\n";
    cout << "  +--------------------------------------------+\n";
    cout << "  |  0. Keluar (auto-save)                      |\n";
    cout << "  +--------------------------------------------+\n";
    cout << "  Data di memori: " << hashTable.getSize() << " produk\n";
    cout << "  Pilihan: ";
}

// Sinkronkan semua 3 DS dari satu vector<Product>
void syncAll(const vector<Product>& products) {
    hashTable = HashTable();
    bst       = BST();
    sortedVec = SortedVector();
    for (const auto& p : products) {
        hashTable.insert(p);
        bst.insert(p);
        sortedVec.insert(p);
    }
}

// ==================== Menu 1: Load File ====================

void menuLoadFile() {
    cout << "\n  === LOAD DATA DARI FILE ===\n";
    cout << "  Path file (default: " << DATA_FILE << "): ";
    string path;
    getline(cin, path);
    if (path.empty()) path = DATA_FILE;

    vector<Product> products = FileManager::loadProducts(path);
    if (products.empty()) return;

    cout << "  Menyinkronkan ke 3 struktur data... ";
    syncAll(products);
    cout << "selesai.\n";
}

// ==================== Menu 2: Generate Dataset ====================

void menuGenerateDataset() {
    cout << "\n  === GENERATE DATASET DUMMY ===\n";
    cout << "  Pilih ukuran dataset:\n";
    cout << "  1. 100 produk\n";
    cout << "  2. 1.000 produk\n";
    cout << "  3. 5.000 produk\n";
    cout << "  4. 10.000 produk\n";
    cout << "  5. Custom (masukkan angka)\n";
    cout << "  Pilihan: ";

    int choice; cin >> choice; clearInput();
    int sizes[] = {100, 1000, 5000, 10000};
    int n = 0;

    if (choice >= 1 && choice <= 4)      n = sizes[choice - 1];
    else if (choice == 5) {
        cout << "  Jumlah produk: "; cin >> n; clearInput();
        if (n <= 0) { cout << "  [!] Angka tidak valid.\n"; return; }
    } else { cout << "  [!] Pilihan tidak valid.\n"; return; }

    cout << "  Generating " << n << " produk...\n";
    vector<Product> products = generator.generate(n);

    // Insert ke 3 DS, ukur waktu insert
    hashTable = HashTable();
    bst       = BST();
    sortedVec = SortedVector();

    auto t0 = high_resolution_clock::now();
    for (const auto& p : products) hashTable.insert(p);
    auto t1 = high_resolution_clock::now();
    for (const auto& p : products) bst.insert(p);
    auto t2 = high_resolution_clock::now();
    for (const auto& p : products) sortedVec.insert(p);
    auto t3 = high_resolution_clock::now();

    long long htMs  = duration_cast<milliseconds>(t1-t0).count();
    long long bstMs = duration_cast<milliseconds>(t2-t1).count();
    long long svMs  = duration_cast<milliseconds>(t3-t2).count();

    cout << "\n  [OK] Dataset berhasil digenerate!\n";
    cout << "  +---------------------+-----------------+\n";
    cout << "  | Struktur Data       |   Waktu Insert  |\n";
    cout << "  +---------------------+-----------------+\n";
    cout << "  | Hash Table          | " << setw(8) << htMs  << " ms       |\n";
    cout << "  | BST                 | " << setw(8) << bstMs << " ms       |\n";
    cout << "  | Sorted Vector       | " << setw(8) << svMs  << " ms       |\n";
    cout << "  +---------------------+-----------------+\n";
}

// ==================== Menu 3: Simpan File ====================

void menuSaveFile() {
    cout << "\n  === SIMPAN DATA KE FILE ===\n";
    cout << "  Path file (default: " << DATA_FILE << "): ";
    string path;
    getline(cin, path);
    if (path.empty()) path = DATA_FILE;

    vector<Product> products = hashTable.getAllProducts();
    FileManager::saveProducts(path, products);
}

// ==================== Menu 4: Insert ====================

void menuInsert() {
    cout << "\n  === INSERT PRODUK ===\n";
    string id, nama, kategori, status;
    int stok; double harga;

    cout << "  ID Produk (format PRD-XXXXX): "; getline(cin, id);
    if (hashTable.search(id)) {
        cout << "  [!] ID " << id << " sudah ada.\n"; return;
    }
    cout << "  Nama Produk  : "; getline(cin, nama);
    cout << "  Kategori     : "; getline(cin, kategori);
    cout << "  Stok         : "; cin >> stok; clearInput();
    cout << "  Harga (Rp)   : "; cin >> harga; clearInput();
    status = (stok == 0) ? "out_of_stock" : "active";

    Product p(id, nama, kategori, stok, harga, status);
    hashTable.insert(p);
    bst.insert(p);
    sortedVec.insert(p);

    cout << "\n  [OK] Produk berhasil ditambahkan!\n";
    p.display();
}

// ==================== Menu 5: Search ====================

void menuSearch() {
    cout << "\n  === SEARCH PRODUK ===\n";
    cout << "  1. Cari berdasarkan ID\n";
    cout << "  2. Cari berdasarkan Nama (BST)\n";
    cout << "  3. Cari berdasarkan Kategori (BST)\n";
    cout << "  Pilihan: ";
    int choice; cin >> choice; clearInput();

    if (choice == 1) {
        string id; cout << "  ID Produk: "; getline(cin, id);

        auto s0 = high_resolution_clock::now();
        Product* rHT  = hashTable.search(id);
        auto s1 = high_resolution_clock::now();
        (void)bst.search(id);
        auto s2 = high_resolution_clock::now();
        (void)sortedVec.search(id);
        auto s3 = high_resolution_clock::now();

        long long nsHT  = duration_cast<nanoseconds>(s1-s0).count();
        long long nsBST = duration_cast<nanoseconds>(s2-s1).count();
        long long nsSV  = duration_cast<nanoseconds>(s3-s2).count();

        if (rHT) { cout << "\n  [OK] Produk ditemukan:\n"; rHT->display(); }
        else     cout << "\n  [!] Produk tidak ditemukan.\n";

        cout << "\n  +---------------------+-------------------+\n";
        cout << "  | Struktur Data       |   Waktu Search    |\n";
        cout << "  +---------------------+-------------------+\n";
        cout << "  | Hash Table          | " << setw(10) << nsHT  << " ns       |\n";
        cout << "  | BST                 | " << setw(10) << nsBST << " ns       |\n";
        cout << "  | Sorted Vector       | " << setw(10) << nsSV  << " ns       |\n";
        cout << "  +---------------------+-------------------+\n";

    } else if (choice == 2) {
        string nama; cout << "  Nama Produk: "; getline(cin, nama);
        auto results = bst.searchByName(nama);
        if (results.empty()) { cout << "  [!] Tidak ditemukan.\n"; return; }
        cout << "\n  Ditemukan " << results.size() << " produk:\n";
        cout << "+-------------+------------------------+--------------+--------+---------------+---------------+\n";
        cout << "| ID          | Nama                   | Kategori     |   Stok |         Harga | Status        |\n";
        cout << "+-------------+------------------------+--------------+--------+---------------+---------------+\n";
        for (const auto& p : results) p.displayCompact();
        cout << "+-------------+------------------------+--------------+--------+---------------+---------------+\n";
    } else if (choice == 3) {
        string kat; cout << "  Kategori: "; getline(cin, kat);
        auto results = bst.searchByCategory(kat);
        if (results.empty()) { cout << "  [!] Tidak ditemukan.\n"; return; }
        cout << "\n  Ditemukan " << results.size() << " produk:\n";
        cout << "+-------------+------------------------+--------------+--------+---------------+---------------+\n";
        cout << "| ID          | Nama                   | Kategori     |   Stok |         Harga | Status        |\n";
        cout << "+-------------+------------------------+--------------+--------+---------------+---------------+\n";
        for (const auto& p : results) p.displayCompact();
        cout << "+-------------+------------------------+--------------+--------+---------------+---------------+\n";
    } else {
        cout << "  [!] Pilihan tidak valid.\n";
    }
}

// ==================== Menu 6: Update Stok ====================

void menuUpdate() {
    cout << "\n  === UPDATE STOK PRODUK ===\n";
    string id; cout << "  ID Produk: "; getline(cin, id);

    Product* p = hashTable.search(id);
    if (!p) { cout << "  [!] Produk tidak ditemukan.\n"; return; }
    cout << "  Produk ditemukan:\n"; p->display();

    int newStock; cout << "  Stok baru: "; cin >> newStock; clearInput();
    if (newStock < 0) { cout << "  [!] Stok tidak boleh negatif.\n"; return; }

    auto s0 = high_resolution_clock::now();
    hashTable.updateStock(id, newStock);
    auto s1 = high_resolution_clock::now();
    bst.updateStock(id, newStock);
    auto s2 = high_resolution_clock::now();
    sortedVec.updateStock(id, newStock);
    auto s3 = high_resolution_clock::now();

    cout << "\n  [OK] Stok diupdate!\n";
    hashTable.search(id)->display();
    cout << "  +---------------------+-------------------+\n";
    cout << "  | Struktur Data       |   Waktu Update    |\n";
    cout << "  +---------------------+-------------------+\n";
    cout << "  | Hash Table          | " << setw(10) << duration_cast<nanoseconds>(s1-s0).count() << " ns       |\n";
    cout << "  | BST                 | " << setw(10) << duration_cast<nanoseconds>(s2-s1).count() << " ns       |\n";
    cout << "  | Sorted Vector       | " << setw(10) << duration_cast<nanoseconds>(s3-s2).count() << " ns       |\n";
    cout << "  +---------------------+-------------------+\n";
}

// ==================== Menu 7: Delete ====================

void menuDelete() {
    cout << "\n  === DELETE PRODUK ===\n";
    string id; cout << "  ID Produk: "; getline(cin, id);

    Product* p = hashTable.search(id);
    if (!p) { cout << "  [!] Produk tidak ditemukan.\n"; return; }
    cout << "  Produk yang akan dihapus:\n"; p->display();

    char confirm; cout << "  Yakin? (y/n): "; cin >> confirm; clearInput();
    if (confirm != 'y' && confirm != 'Y') { cout << "  Dibatalkan.\n"; return; }

    auto s0 = high_resolution_clock::now();
    hashTable.remove(id);
    auto s1 = high_resolution_clock::now();
    bst.remove(id);
    auto s2 = high_resolution_clock::now();
    sortedVec.remove(id);
    auto s3 = high_resolution_clock::now();

    cout << "\n  [OK] Produk dihapus!\n";
    cout << "  +---------------------+-------------------+\n";
    cout << "  | Struktur Data       |   Waktu Delete    |\n";
    cout << "  +---------------------+-------------------+\n";
    cout << "  | Hash Table          | " << setw(10) << duration_cast<nanoseconds>(s1-s0).count() << " ns       |\n";
    cout << "  | BST                 | " << setw(10) << duration_cast<nanoseconds>(s2-s1).count() << " ns       |\n";
    cout << "  | Sorted Vector       | " << setw(10) << duration_cast<nanoseconds>(s3-s2).count() << " ns       |\n";
    cout << "  +---------------------+-------------------+\n";
}

// ==================== Menu 8: Display All ====================

void menuDisplayAll() {
    cout << "\n  === TAMPILKAN SEMUA PRODUK ===\n";
    cout << "  1. Hash Table\n  2. BST (terurut by ID)\n  3. Sorted Vector\n";
    cout << "  Pilihan: ";
    int c; cin >> c; clearInput();
    if      (c == 1) hashTable.displayAll();
    else if (c == 2) bst.displayAll();
    else if (c == 3) sortedVec.displayAll();
    else cout << "  [!] Pilihan tidak valid.\n";
}

// ==================== Menu 9: Benchmark ====================

vector<BenchmarkRow> lastBenchmarkResults; // simpan untuk export

void menuBenchmark() {
    cout << "\n  +==================================================================+\n";
    cout << "  |     BENCHMARK: Hash Table vs BST vs Sorted Vector               |\n";
    cout << "  +==================================================================+\n";
    cout << "  Pilih skenario benchmark:\n";
    cout << "  1. Cepat  (100 | 1K | 5K | 10K)\n";
    cout << "  2. Lengkap(1K | 10K | 50K | 100K | 500K) [butuh waktu ~1-2 menit]\n";
    cout << "  Pilihan: ";
    int sc; cin >> sc; clearInput();

    vector<int> sizes;
    if      (sc == 1) sizes = {100, 1000, 5000, 10000};
    else if (sc == 2) sizes = {1000, 10000, 50000, 100000, 500000};
    else { cout << "  [!] Tidak valid.\n"; return; }

    const int SAMPLE = 200; // Jumlah operasi per benchmark search/update/delete

    lastBenchmarkResults.clear();

    // Header tabel
    cout << "\n";
    cout << "  +---------+------------------------------+------------------------------+------------------------------+------------------------------+\n";
    cout << "  |    N    |        INSERT (μs)           |        SEARCH (μs)           |        UPDATE (μs)           |        DELETE (μs)           |\n";
    cout << "  |         |   HT    |  BST   |   SV      |   HT    |  BST   |   SV      |   HT    |  BST   |   SV      |   HT    |  BST   |   SV      |\n";
    cout << "  +---------+---------+--------+-----------+---------+--------+-----------+---------+--------+-----------+---------+--------+-----------+\n";

    for (int n : sizes) {
        vector<Product> data = generator.generate(n);

        // Acak urutan data sebelum insert ke BST supaya mendekati average case O(log n).
        // Catatan: ID sequential PRD-00001, 00002, ... menyebabkan BST degenerate
        // menjadi linked list (worst case O(n)) jika diinsert secara terurut.
        vector<Product> shuffled = data;
        for (int i = (int)shuffled.size()-1; i > 0; i--) {
            int j = rand() % (i + 1);
            swap(shuffled[i], shuffled[j]);
        }

        HashTable    ht;
        BST          bt;
        SortedVector sv;

        // ---- INSERT ----
        auto t0 = high_resolution_clock::now();
        for (const auto& p : data) ht.insert(p);       // HT: urutan tak berpengaruh
        auto t1 = high_resolution_clock::now();
        for (const auto& p : shuffled) bt.insert(p);   // BST: data diacak -> avg case
        auto t2 = high_resolution_clock::now();
        // SortedVector insert O(n), batasi supaya tidak timeout pada N besar
        int insertSV = (n <= 50000) ? n : 50000;
        for (int i = 0; i < insertSV; i++) sv.insert(data[i]);
        auto t3 = high_resolution_clock::now();

        long long insHT  = duration_cast<microseconds>(t1-t0).count();
        long long insBST = duration_cast<microseconds>(t2-t1).count();
        long long insSV  = duration_cast<microseconds>(t3-t2).count();
        if (insertSV < n) insSV = -1; // Tandai tidak dijalankan penuh

        // ---- Ambil sample ID untuk search/update/delete ----
        int s = min(SAMPLE, n);
        vector<string> sampleIds;
        sampleIds.reserve(s);
        for (int i = 0; i < s; i++) sampleIds.push_back(data[rand() % n].id);

        // ---- SEARCH ----
        t0 = high_resolution_clock::now();
        for (const auto& id : sampleIds) ht.search(id);
        t1 = high_resolution_clock::now();
        for (const auto& id : sampleIds) bt.search(id);
        t2 = high_resolution_clock::now();
        for (const auto& id : sampleIds) sv.search(id);
        t3 = high_resolution_clock::now();

        long long schHT  = duration_cast<microseconds>(t1-t0).count();
        long long schBST = duration_cast<microseconds>(t2-t1).count();
        long long schSV  = duration_cast<microseconds>(t3-t2).count();

        // ---- UPDATE ----
        t0 = high_resolution_clock::now();
        for (const auto& id : sampleIds) ht.updateStock(id, rand() % 500);
        t1 = high_resolution_clock::now();
        for (const auto& id : sampleIds) bt.updateStock(id, rand() % 500);
        t2 = high_resolution_clock::now();
        for (const auto& id : sampleIds) sv.updateStock(id, rand() % 500);
        t3 = high_resolution_clock::now();

        long long updHT  = duration_cast<microseconds>(t1-t0).count();
        long long updBST = duration_cast<microseconds>(t2-t1).count();
        long long updSV  = duration_cast<microseconds>(t3-t2).count();

        // ---- DELETE ----
        int d = min(SAMPLE / 2, n);
        t0 = high_resolution_clock::now();
        for (int i = 0; i < d; i++) ht.remove(data[i].id);
        t1 = high_resolution_clock::now();
        for (int i = 0; i < d; i++) bt.remove(data[i].id);
        t2 = high_resolution_clock::now();
        for (int i = 0; i < d; i++) sv.remove(data[i].id);
        t3 = high_resolution_clock::now();

        long long delHT  = duration_cast<microseconds>(t1-t0).count();
        long long delBST = duration_cast<microseconds>(t2-t1).count();
        long long delSV  = duration_cast<microseconds>(t3-t2).count();

        // Print baris
        auto fmt = [](long long v) -> string {
            if (v < 0) return "  N/A  ";
            ostringstream ss; ss << setw(7) << v; return ss.str();
        };
        cout << "  | " << right << setw(7) << n << " |"
             << fmt(insHT)  << " |" << fmt(insBST) << " |" << fmt(insSV)  << "  |"
             << fmt(schHT)  << " |" << fmt(schBST) << " |" << fmt(schSV)  << "  |"
             << fmt(updHT)  << " |" << fmt(updBST) << " |" << fmt(updSV)  << "  |"
             << fmt(delHT)  << " |" << fmt(delBST) << " |" << fmt(delSV)  << "  |\n";

        // Simpan untuk export
        lastBenchmarkResults.push_back({n, "INSERT", insHT,  insBST,  insSV});
        lastBenchmarkResults.push_back({n, "SEARCH", schHT,  schBST,  schSV});
        lastBenchmarkResults.push_back({n, "UPDATE", updHT,  updBST,  updSV});
        lastBenchmarkResults.push_back({n, "DELETE", delHT,  delBST,  delSV});
    }

    cout << "  +---------+---------+--------+-----------+---------+--------+-----------+---------+--------+-----------+---------+--------+-----------+\n";
    cout << "\n  Keterangan:\n";
    cout << "  HT=Hash Table | BST=Binary Search Tree | SV=Sorted Vector\n";
    cout << "  Search/Update/Delete diukur dari " << SAMPLE << " operasi acak\n";
    cout << "  Delete diukur dari " << SAMPLE/2 << " operasi\n";
    cout << "  N/A = tidak dijalankan penuh (SV insert O(n) terlalu mahal untuk N>50K)\n";
    cout << "\n  Kompleksitas Teoritis:\n";
    cout << "  +-----------------+----------------+----------------+----------------+\n";
    cout << "  | Operasi         |   Hash Table   |      BST       |  Sorted Vector |\n";
    cout << "  +-----------------+----------------+----------------+----------------+\n";
    cout << "  | Insert          |   O(1) avg     |  O(log n) avg  |      O(n)      |\n";
    cout << "  | Search          |   O(1) avg     |  O(log n) avg  |    O(log n)    |\n";
    cout << "  | Update          |   O(1) avg     |  O(log n) avg  |    O(log n)    |\n";
    cout << "  | Delete          |   O(1) avg     |  O(log n) avg  |      O(n)      |\n";
    cout << "  +-----------------+----------------+----------------+----------------+\n";

    cout << "\n  Gunakan menu 11 untuk export hasil ke CSV.\n";
}

// ==================== Menu 10: Statistik ====================

void menuStats() {
    cout << "\n  === STATISTIK STRUKTUR DATA ===\n";
    hashTable.displayStats();
    cout << "\n  === BST ===\n";
    cout << "  Jumlah Node    : " << bst.getSize() << "\n";
    cout << "  Est. Memori    : " << bst.estimateMemory() / 1024 << " KB\n";
    cout << "\n  === Sorted Vector ===\n";
    cout << "  Jumlah Elemen  : " << sortedVec.getSize() << "\n";
    cout << "  Est. Memori    : " << sortedVec.estimateMemory() / 1024 << " KB\n";
}

// ==================== Menu 11: Export Benchmark ====================

void menuExportBenchmark() {
    if (lastBenchmarkResults.empty()) {
        cout << "  [!] Jalankan benchmark dulu (menu 9).\n";
        return;
    }
    FileManager::saveBenchmark(BENCHMARK_FILE, lastBenchmarkResults);
    cout << "  Buka file " << BENCHMARK_FILE << " untuk membuat grafik di Excel/Python.\n";
}

// ==================== Main ====================

int main() {
    printHeader();

    // Auto-load jika file sudah ada
    if (FileManager::fileExists(DATA_FILE)) {
        cout << "\n  File " << DATA_FILE << " ditemukan. Load otomatis...\n";
        vector<Product> loaded = FileManager::loadProducts(DATA_FILE);
        if (!loaded.empty()) syncAll(loaded);
    } else {
        cout << "\n  Status: Siap. Gunakan menu 1/2 untuk load/generate data.\n";
    }

    int choice;
    bool running = true;
    while (running) {
        printMenu();
        cin >> choice; clearInput();
        switch (choice) {
            case 1:  menuLoadFile();       break;
            case 2:  menuGenerateDataset();break;
            case 3:  menuSaveFile();       break;
            case 4:  menuInsert();         break;
            case 5:  menuSearch();         break;
            case 6:  menuUpdate();         break;
            case 7:  menuDelete();         break;
            case 8:  menuDisplayAll();     break;
            case 9:  menuBenchmark();      break;
            case 10: menuStats();          break;
            case 11: menuExportBenchmark();break;
                        case 0:
                cout << "\n  Auto-save data...\n";
                {
                    auto all = hashTable.getAllProducts();
                    if (!all.empty()) FileManager::saveProducts(DATA_FILE, all);
                }
                cout << "  Terima kasih! Program selesai.\n\n";
                running = false;
                break;
            default:
                cout << "  [!] Pilihan tidak valid.\n";
        }
        if (running) pressEnter();
    }
    return 0;
}
