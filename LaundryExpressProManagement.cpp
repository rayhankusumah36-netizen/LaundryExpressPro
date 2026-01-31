#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;

const int MAX_ORDER = 100;
int totalOrder = 0;

string STATUS[5] = {
    "Menunggu", "Dicuci", "Dikeringkan", "Disetrika", "Selesai"
};

struct Order {
    int id;
    char nama[50];
    string jenisPakaian;
    float berat;
    string layanan;
    int prioritas;
    double biaya;
    int statusIndex;
    bool batal;
};

Order orders[MAX_ORDER];
string timeline[MAX_ORDER][5];

/* ================= UTILITAS ================= */

int durasiLayanan(string l) {
    if (l == "Express") return 3;
    if (l == "Fast") return 6;
    if (l == "Normal")return 24;
}

/* ================= HITUNG BIAYA ================= */

double hitungBiaya(Order& o) {
    double harga = o.berat * 5000;

    if (o.jenisPakaian == "Celana") harga *= 1.2;
    else if (o.jenisPakaian == "Jaket") harga *= 1.5;
    else if (o.jenisPakaian == "Selimut") harga *= 2.0;
    else if (o.jenisPakaian == "Lainnya") harga *= 1.3;

    if (o.layanan == "Express") harga *= 2.0;
    else if (o.layanan == "Fast") harga *= 1.5;

    if (o.berat > 10) harga *= 0.9;

    int count = 0;
    for (int i = 0; i < totalOrder; i++)
        if (strcmp(orders[i].nama, o.nama) == 0)
            count++;

    if (count >= 2) harga *= 0.85;

    return harga;
}

/* ================= MENU 1 ================= */

void tambahOrder() {
    Order& o = orders[totalOrder];
    o.id = totalOrder + 1;
    o.statusIndex = 0;
    o.batal = false;

    cout << "Nama Pelanggan : ";
    cin.ignore();
    cin.getline(o.nama, 50);

    cout << "Jenis Pakaian (Baju/Celana/Jaket/Selimut/Lainnya): ";
    cin >> o.jenisPakaian;

    do {
        cout << "Berat (0.5 - 20 kg): ";
        cin >> o.berat;
    } while (o.berat < 0.5 || o.berat > 20);

    cout << "Layanan (Express/Fast/Normal): ";
    cin >> o.layanan;

    do {
        cout << "Prioritas (1-5): ";
        cin >> o.prioritas;
    } while (o.prioritas < 1 || o.prioritas > 5);

    o.biaya = hitungBiaya(o);

    for (int i = 0; i < 5; i++)
        timeline[totalOrder][i] = STATUS[i];

    totalOrder++;
    cout << "Order berhasil ditambahkan!\n";
}

/* ================= MENU 2 ================= */

void prosesOrder() {
    for (int i = 0; i < totalOrder; i++) {
        if (orders[i].statusIndex == 4)
            continue;

        orders[i].statusIndex++;

        if (orders[i].prioritas == 1)
            break;
    }
    cout << "Status order diperbarui.\n";
}

/* ================= MENU 3 ================= */

void cariOrder() {
    char key[50];
    cout << "Cari nama pelanggan: ";
    cin >> key;

    for (int i = 0; i < totalOrder; i++) {
        if (strstr(orders[i].nama, key)) {
            cout << "ID: " << orders[i].id
                << " | Nama: " << orders[i].nama
                << " | Status: " << STATUS[orders[i].statusIndex] << endl;
        }
    }
}

/* ================= MENU 4 ================= */

void estimasiWaktuBiaya() {
    int id;
    cout << "Masukkan ID Order: ";
    cin >> id;

    if (id < 1 || id > totalOrder) {
        cout << "Order tidak ditemukan!\n";
        return;
    }

    Order& o = orders[id - 1];
    cout << "Estimasi Waktu: "
        << durasiLayanan(o.layanan) << " jam\n";
    cout << "Total Biaya: Rp " << o.biaya << endl;
}

/* ================= MENU 5 ================= */

void laporanHarian() {
    double pendapatan = 0;
    float totalBerat = 0;
    int count = 0;

    for (int i = 0; i < totalOrder; i++) {
        if (orders[i].batal)
            continue;

        pendapatan += orders[i].biaya;
        totalBerat += orders[i].berat;
        count++;

        if (count == 5)
            break;
    }

    if (count == 0) goto error;

    cout << "Total Pendapatan: Rp " << pendapatan << endl;
    cout << "Rata-rata Berat : " << totalBerat / count << " kg\n";
    return;

error:
    cout << "Data tidak valid!\n";
}

/* ================= MENU 6 ================= */

void optimasiPencucian() {
    cout << "Urutan prioritas pencucian:\n";
    for (int p = 1; p <= 5; p++) {
        for (int i = 0; i < totalOrder; i++) {
            if (orders[i].prioritas == p)
                cout << "Order ID " << orders[i].id << endl;
        }
    }
}

/* ================= MENU 7 ================= */

void analisisPelanggan() {
    char nama[50];
    cout << "Nama pelanggan: ";
    cin >> nama;

    int jumlah = 0;
    for (int i = 0; i < totalOrder; i++)
        if (strcmp(orders[i].nama, nama) == 0)
            jumlah++;

    cout << "Total order pelanggan: " << jumlah << endl;
}

/* ================= MENU 8 ================= */

void resetData() {
    totalOrder = 0;
    cout << "Data harian berhasil direset.\n";
}

/* ================= DASHBOARD ================= */

void dashboard() {
    int selesai = 0;
    for (int i = 0; i < totalOrder; i++)
        if (orders[i].statusIndex == 4)
            selesai++;

    cout << "\n=== LAUNDRYEXPRESS PRO ===\n";
    cout << "Total Order Hari Ini : " << totalOrder << endl;
    cout << "Order Selesai       : " << selesai << endl;
    cout << "Order Dalam Proses  : " << totalOrder - selesai << endl;
}

/* ================= MAIN ================= */

int main() {
    int menu;
    do {
        dashboard();
        cout << "\nMENU UTAMA\n";
        cout << "1. Tambah Order Baru\n";
        cout << "2. Proses Order (Update Status)\n";
        cout << "3. Cari & Lihat Detail Order\n";
        cout << "4. Hitung Estimasi Waktu & Biaya\n";
        cout << "5. Generate Laporan Harian\n";
        cout << "6. Optimasi Urutan Pencucian\n";
        cout << "7. Analisis Data Pelanggan\n";
        cout << "8. Reset Data Harian\n";
        cout << "9. Keluar\n";
        cout << "Pilih Menu: ";
        cin >> menu;

        switch (menu) {
        case 1: tambahOrder(); break;
        case 2: prosesOrder(); break;
        case 3: cariOrder(); break;
        case 4: estimasiWaktuBiaya(); break;
        case 5: laporanHarian(); break;
        case 6: optimasiPencucian(); break;
        case 7: analisisPelanggan(); break;
        case 8: resetData(); break;
        }
    } while (menu != 9);

    return 0;
}
