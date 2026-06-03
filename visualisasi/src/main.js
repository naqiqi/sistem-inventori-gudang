import Plotly from "plotly.js-dist-min";
import Papa from "papaparse";

const colors = {
  blue: "#2f5f98",       // BST (O(log N))
  green: "#10b981",      // Hash Table (O(1))
  orange: "#f59e0b",     // Sorted Vector (Mixed)
  gray: "#64748b"
};

const plotConfig = {
  responsive: true,
  displayModeBar: true
};

const baseLayout = {
  paper_bgcolor: "rgba(0,0,0,0)",
  plot_bgcolor: "rgba(0,0,0,0)",
  font: {
    family: "Inter, Arial, sans-serif",
    color: "#172033"
  },
  margin: {
    t: 70,
    r: 30,
    b: 70,
    l: 70
  },
  hovermode: "closest"
};

// Membaca file benchmark dari folder results/ atau root public
fetch("/benchmark.csv")
  .then(response => response.text())
  .then(csvText => {
    const parsed = Papa.parse(csvText, {
      header: true,
      skipEmptyLines: true,
      dynamicTyping: true
    });

    const data = parsed.data;
    console.log("Data Benchmark C++ berhasil dibaca:", data);
    
    // Memanggil fungsi render grafik baru sesuai dataset Struktur Data
    renderSummaryCards(data);
    renderOperationComparisonBar(data, "INSERT", "chartInsertBar");
    renderOperationComparisonBar(data, "SEARCH", "chartSearchBar");
    renderScalabilityLineChart(data);
  })
  .catch(error => {
    console.error("Gagal membaca data CSV:", error);
  });

// --- FUNGSI UTILITAS STATISTIK ---
function average(values) {
  const validValues = values.filter(v => v !== null && v !== undefined && v !== -1);
  if (validValues.length === 0) return 0;
  return validValues.reduce((sum, value) => sum + Number(value), 0) / validValues.length;
}

/**
 * 1. Menampilkan Ringkasan Performa Rata-rata Kecepatan (Metric Cards)
 */
function renderSummaryCards(data) {
  // Mengambil rata-rata dari seluruh baris data (kecuali nilai -1 yang menandakan skenario error/TTO)
  const avgBST = average(data.map(item => item.bst_us));
  const avgHashTable = average(data.map(item => item.hashtable_us));
  const avgSortedVector = average(data.map(item => item.sorted_vector_us));

  const setStatValue = (id, value) => {
    const element = document.getElementById(id);
    if (element) element.textContent = value;
  };

  setStatValue("avgHashTable", `${avgHashTable.toFixed(2)} μs`);
  setStatValue("avgBST", `${avgBST.toFixed(2)} μs`);
  setStatValue("avgSortedVector", `${avgSortedVector.toFixed(2)} μs`);
}

/**
 * 2. BAR CHART: Membandingkan Kecepatan Struktur Data pada Ukuran Data Terbesar (N Maksimal)
 * Sesuai permintaan Anda untuk memvisualisasikannya lewat Plotly Bar Chart.
 */
function renderOperationComparisonBar(data, operationName, elementId) {
  // Ambil ukuran N terbesar yang tersedia di dalam berkas untuk perbandingan ekstrem
  const sizes = [...new Set(data.map(item => item.n))].filter(Number.isFinite);
  const maxN = Math.max(...sizes);

  // Filter baris data berdasarkan Nama Operasi dan N Terbesar
  const targetData = data.find(item => item.operasi === operationName && item.n === maxN);

  if (!targetData) return;

  // Membuat trace Bar Chart tunggal / grouped untuk Plotly
  const trace = {
    x: ["Hash Table", "Binary Search Tree (BST)", "Sorted Vector"],
    y: [targetData.hashtable_us, targetData.bst_us, targetData.sorted_vector_us],
    type: "bar",
    marker: {
      color: [colors.green, colors.blue, colors.orange]
    },
    text: [
      `${targetData.hashtable_us} μs`, 
      `${targetData.bst_us} μs`, 
      targetData.sorted_vector_us === -1 ? "N/A (Error/Time Out)" : `${targetData.sorted_vector_us} μs`
    ],
    textposition: "auto",
    hovertemplate: `Struktur: %{x}<br>Waktu: %{y} μs<br>N: ${maxN.toLocaleString('id-ID')}<extra></extra>`
  };

  const layout = {
    ...baseLayout,
    title: `Perbandingan Waktu Operasi ${operationName} (N = ${maxN.toLocaleString('id-ID')})`,
    xaxis: { title: "Struktur Data" },
    yaxis: { 
      title: "Waktu Eksekusi (μs)",
      type: "linear" // Jika perbedaan angka us sangat jomplang, Anda bisa menggantinya dengan "log"
    }
  };

  Plotly.newPlot(elementId, [trace], layout, plotConfig);
}

/**
 * 3. LINE CHART: Skalabilitas Pertumbuhan Kompleksitas Waktu (Growth Curve Chart)
 * Sangat krusial untuk membuktikan performa O(1) vs O(log N) vs O(N) seiring bertambahnya N
 */
function renderScalabilityLineChart(data) {
  // Ambil data khusus operasi SEARCH untuk melihat kurva pencarian murni
  const searchPoints = data.filter(item => item.operasi === "SEARCH").sort((a, b) => a.n - b.n);
  
  const xValues = searchPoints.map(item => item.n);

  const traceHashTable = {
    x: xValues,
    y: searchPoints.map(item => item.hashtable_us),
    name: "Hash Table (Chaining)",
    type: "scatter",
    mode: "lines+markers",
    line: { color: colors.green, width: 3, shape: "spline" },
    marker: { size: 8 }
  };

  const traceBST = {
    x: xValues,
    y: searchPoints.map(item => item.bst_us),
    name: "BST",
    type: "scatter",
    mode: "lines+markers",
    line: { color: colors.blue, width: 3, shape: "spline" },
    marker: { size: 8 }
  };

  const traceVector = {
    x: xValues,
    y: searchPoints.map(item => item.sorted_vector_us === -1 ? null : item.sorted_vector_us), // Abaikan jika -1
    name: "Sorted Vector",
    type: "scatter",
    mode: "lines+markers",
    line: { color: colors.orange, width: 3, shape: "spline" },
    marker: { size: 8 }
  };

  const layout = {
    ...baseLayout,
    title: "Kurva Skalabilitas Kompleksitas Waktu Operasi SEARCH",
    xaxis: { 
      title: "Ukuran Elemen Data (N)",
      type: "linear"
    },
    yaxis: { 
      title: "Waktu Eksekusi (μs)",
      type: "linear" // Gunakan 'log' jika nilai Sorted Vector meroket terlalu tinggi dibanding Hash Table
    }
  };

  Plotly.newPlot("scalabilityLineChart", [traceHashTable, traceBST, traceVector], layout, plotConfig);
}