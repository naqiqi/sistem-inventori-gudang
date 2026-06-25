import Plotly from "plotly.js-dist-min";
import Papa from "papaparse";

const colors = {
  HashTable: '#2196F3',    // Biru (O(1) avg)
  BST: '#FF9800',          // Oranye (O(log n) avg)
  SortedVector: '#4CAF50'  // Hijau
};

const labels = {
  HashTable: 'Hash Table (O(1) avg)',
  BST: 'BST (O(log n) avg)',
  SortedVector: 'Sorted Vector'
};

const plotConfig = {
  responsive: true,
  displayModeBar: true
};

// Konfigurasi font dan dasar tampilan layout
const baseLayoutStyles = {
  font: { family: "DejaVu Sans, Arial, sans-serif" },
  paper_bgcolor: "rgba(0,0,0,0)",
  plot_bgcolor: "rgba(0,0,0,0)"
};

// Membaca file benchmark dari folder public/ atau root public server Vite
fetch("benchmark.csv")
  .then(response => response.text())
  .then(csvText => {
    const parsed = Papa.parse(csvText, {
      header: true,
      skipEmptyLines: true,
      dynamicTyping: true
    });

    const rawData = parsed.data;
    console.log("Data CSV berhasil dimuat:", rawData);

    // Proses data ke bentuk map terstruktur: data[operasi][struktur][n] = waktu
    const dataStructure = {};
    const nsSet = new Set();

    rawData.forEach(row => {
      const n = intOrNull(row.n);
      const op = row.operasi;
      const ht = intOrNull(row.hashtable_us);
      const bst = intOrNull(row.bst_us);
      const sv = intOrNull(row.sorted_vector_us);

      if (n !== null && op) {
        nsSet.add(n);
        if (!dataStructure[op]) {
          dataStructure[op] = { HashTable: {}, BST: {}, SortedVector: {} };
        }
        dataStructure[op]['HashTable'][n] = ht;
        dataStructure[op]['BST'][n] = bst;
        dataStructure[op]['SortedVector'][n] = sv;
      }
    });

    const nsList = Array.from(nsSet).sort((a, b) => a - b);
    const operations = ['INSERT', 'SEARCH', 'UPDATE', 'DELETE'];

    // ── Render Plot 1-4: Bar Chart Tunggal per Operasi ──
    operations.forEach(op => {
      if (dataStructure[op]) {
        renderSingleOperationBar(dataStructure, nsList, op, `grafik_${op.toLowerCase()}`);
      }
    });

    // ── Render Plot 5: Kurva Line Chart Skalabilitas Gabungan ──
    renderScalabilityLineChart(dataStructure, nsList, operations, "grafik_semua_operasi");
  })
  .catch(error => {
    console.error("Gagal memproses data CSV benchmark:", error);
  });

function intOrNull(val) {
  const num = parseInt(val);
  return isNaN(num) ? null : num;
}

/**
 * RENDER PLOT 1-4: Grouped Bar Chart per Operasi
 */
function renderSingleOperationBar(data, nsList, op, elementId) {
  const dsList = ['HashTable', 'BST', 'SortedVector'];
  
  // Format label sumbu X dengan tanda koma ribuan (e.g. 10,000)
  const xLabels = nsList.map(n => n.toLocaleString('en-US'));

  const traces = dsList.map(ds => {
    const yValues = nsList.map(n => {
      const val = data[op][ds][n];
      // Jika nilai -1 atau kosong, jadikan 0 (skip bar)
      return (val !== undefined && val >= 0) ? val : 0;
    });

    return {
      x: xLabels,
      y: yValues,
      name: labels[ds],
      type: 'bar',
      marker: { color: colors[ds] },
      text: yValues.map(v => v > 0 ? v.toLocaleString('en-US') : ''),
      textposition: 'outside',
      textfont: { size: 9 },
      hovertemplate: `<b>${labels[ds]}</b><br>N: %{x}<br>Waktu: %{y} μs<extra></extra>`
    };
  });

  const layout = {
    ...baseLayoutStyles,
    title: {
      text: `<b>Perbandingan Performa: ${op}</b><br><span style="font-size:11px; color:#64748b;">Sistem Manajemen Inventori Gudang</span>`,
      x: 0.05
    },
    xaxis: { title: "Jumlah Data (N)", tickmode: "array", tickvals: xLabels },
    yaxis: { title: "Waktu Eksekusi (μs)", gridcolor: "#e2e8f0" },
    barmode: 'group',
    bargap: 0.25,
    bargroupgap: 0.1,
    margin: { t: 80, b: 60, l: 60, r: 20 },
    legend: { orientation: "h", y: -0.2 }
  };

  Plotly.newPlot(elementId, traces, layout, plotConfig);
}

/**
 * RENDER PLOT 5: Line Chart Skalabilitas Terintegrasi
 */
function renderScalabilityLineChart(data, nsList, operations, elementId) {
  const dsList = ['HashTable', 'BST', 'SortedVector'];
  const symbols = { HashTable: 'circle', BST: 'square', SortedVector: 'triangle-up' };
  const traces = [];

  operations.forEach(op => {
    if (!data[op]) return;

    dsList.forEach(ds => {
      const validPoints = nsList
        .map(n => ({ n, v: data[op][ds][n] }))
        .filter(pt => pt.v !== undefined && pt.v >= 0);

      if (validPoints.length === 0) return;

      traces.push({
        x: validPoints.map(pt => pt.n),
        y: validPoints.map(pt => pt.v),
        // Kelompokkan legend berdasarkan struktur data agar rapi saat diklik
        name: `${op} - ${ds}`,
        legendgroup: ds,
        type: 'scatter',
        mode: 'lines+markers',
        line: { color: colors[ds], width: 2 },
        marker: { symbol: symbols[ds], size: 7 },
        hovertemplate: `<b>${op} (${ds})</b><br>N: %{x}<br>Waktu: %{y} μs<extra></extra>`
      });
    });
  });

  const layout = {
    ...baseLayoutStyles,
    title: {
      text: `<b>Perbandingan Skalabilitas: Hash Table vs BST vs Sorted Vector</b><br><span style="font-size:11px; color:#64748b;">Sistem Manajemen Inventori Gudang</span>`,
      x: 0.05
    },
    xaxis: { title: "Ukuran Elemen Data (N)", gridcolor: "#e2e8f0" },
    yaxis: { title: "Waktu (μs)", gridcolor: "#e2e8f0" },
    margin: { t: 90, b: 60, l: 60, r: 20 },
    legend: { font: { size: 10 } }
  };

  Plotly.newPlot(elementId, traces, layout, plotConfig);
}