"""
Visualisasi Hasil Benchmark - Sistem Manajemen Inventori Gudang
Proyek Akhir Struktur Data, IPB University 2025/2026

Cara pakai:
  python plot_benchmark.py

Output: folder results/ berisi 5 file grafik PNG
"""

import csv
import os
import sys

# Cek matplotlib tersedia
try:
    import matplotlib.pyplot as plt
    import matplotlib
    matplotlib.rcParams['font.family'] = 'DejaVu Sans'
    matplotlib.rcParams['figure.dpi'] = 150
except ImportError:
    print("matplotlib belum terinstall. Jalankan:")
    print("  pip install matplotlib")
    sys.exit(1)

# ── Baca benchmark.csv ──────────────────────────────────────────
CSV_FILE = "results/benchmark.csv"
if not os.path.exists(CSV_FILE):
    print(f"File tidak ditemukan: {CSV_FILE}")
    print("Jalankan program inventori dulu dan export benchmark (menu 11).")
    sys.exit(1)

data = {}   # data[operasi][struktur_data] = {n: waktu}
ns_all = set()

with open(CSV_FILE, newline='') as f:
    reader = csv.DictReader(f)
    for row in reader:
        n   = int(row['n'])
        op  = row['operasi']
        ht  = int(row['hashtable_us'])
        bst = int(row['bst_us'])
        sv  = int(row['sorted_vector_us'])
        ns_all.add(n)
        if op not in data:
            data[op] = {'HashTable': {}, 'BST': {}, 'SortedVector': {}}
        data[op]['HashTable'][n]    = ht
        data[op]['BST'][n]          = bst
        data[op]['SortedVector'][n] = sv

ns = sorted(ns_all)
ops = [op for op in ['INSERT', 'SEARCH', 'UPDATE', 'DELETE'] if op in data]
print(f"Data dimuat: {len(ops)} operasi, N = {ns}")

# ── Warna & style ───────────────────────────────────────────────
COLORS = {
    'HashTable':    '#2196F3',   # Biru
    'BST':          '#FF9800',   # Oranye
    'SortedVector': '#4CAF50',   # Hijau
}
LABELS = {
    'HashTable':    'Hash Table (O(1) avg)',
    'BST':          'BST (O(log n) avg)',
    'SortedVector': 'Sorted Vector',
}
MARKERS = {'HashTable': 'o', 'BST': 's', 'SortedVector': '^'}

os.makedirs("results", exist_ok=True)

# ── Fungsi helper ────────────────────────────────────────────────
def get_values(op, ds):
    return [data[op][ds].get(n, None) for n in ns]

def ns_label(n):
    return f"{n:,}"

# ── Plot 1-4: Satu grafik per operasi (bar chart) ────────────────
for op in ops:
    fig, ax = plt.subplots(figsize=(9, 5))
    x = range(len(ns))
    width = 0.25
    offsets = [-width, 0, width]
    ds_list = ['HashTable', 'BST', 'SortedVector']

    for i, ds in enumerate(ds_list):
        vals = get_values(op, ds)
        # Ganti -1 (N/A) dengan None untuk skip bar
        bar_vals = [v if v >= 0 else 0 for v in vals]
        bars = ax.bar([xi + offsets[i] for xi in x], bar_vals,
                      width=width, label=LABELS[ds],
                      color=COLORS[ds], alpha=0.85, edgecolor='white', linewidth=0.5)
        # Label nilai di atas bar
        for bar, v in zip(bars, vals):
            if v >= 0:
                ax.text(bar.get_x() + bar.get_width()/2,
                        bar.get_height() + max(bar_vals)*0.01,
                        f'{v:,}', ha='center', va='bottom',
                        fontsize=7, color='#333333')

    ax.set_xticks(list(x))
    ax.set_xticklabels([ns_label(n) for n in ns])
    ax.set_xlabel('Jumlah Data (N)', fontsize=11)
    ax.set_ylabel('Waktu Eksekusi (μs)', fontsize=11)
    ax.set_title(f'Perbandingan Performa: {op}\n'
                 f'Sistem Manajemen Inventori Gudang',
                 fontsize=13, fontweight='bold', pad=12)
    ax.legend(fontsize=9)
    ax.grid(axis='y', alpha=0.3, linestyle='--')
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)

    fname = f"results/grafik_{op.lower()}.png"
    plt.tight_layout()
    plt.savefig(fname, bbox_inches='tight')
    plt.close()
    print(f"  Disimpan: {fname}")

# ── Plot 5: Line chart - scaling behavior semua operasi ──────────
fig, axes = plt.subplots(2, 2, figsize=(13, 9))
axes = axes.flatten()

for idx, op in enumerate(ops):
    ax = axes[idx]
    for ds in ['HashTable', 'BST', 'SortedVector']:
        vals = get_values(op, ds)
        # Filter None/-1
        valid = [(n, v) for n, v in zip(ns, vals) if v >= 0]
        if not valid:
            continue
        xv, yv = zip(*valid)
        ax.plot(xv, yv, marker=MARKERS[ds], label=LABELS[ds],
                color=COLORS[ds], linewidth=2, markersize=6)

    ax.set_title(op, fontsize=12, fontweight='bold')
    ax.set_xlabel('N', fontsize=9)
    ax.set_ylabel('Waktu (μs)', fontsize=9)
    ax.legend(fontsize=8)
    ax.grid(alpha=0.3, linestyle='--')
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)

fig.suptitle('Perbandingan Skalabilitas: Hash Table vs BST vs Sorted Vector\n'
             'Sistem Manajemen Inventori Gudang — Struktur Data IPB 2025/2026',
             fontsize=13, fontweight='bold', y=1.01)
plt.tight_layout()
fname = "results/grafik_semua_operasi.png"
plt.savefig(fname, bbox_inches='tight')
plt.close()
print(f"  Disimpan: {fname}")

print("\nSelesai! Semua grafik tersimpan di folder results/")
print("File yang dihasilkan:")
for op in ops:
    print(f"  results/grafik_{op.lower()}.png")
print(f"  results/grafik_semua_operasi.png")
