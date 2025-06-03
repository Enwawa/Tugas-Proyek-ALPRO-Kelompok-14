#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

const int lantaiparkir = 3;
const int tempatparkir = 4;
int dataparkir[lantaiparkir][tempatparkir];

// Fungsi trim untuk menghilangkan spasi di kiri/kanan string
static inline string trim(const string &s) {
    auto start = s.begin();
    while (start != s.end() && isspace(*start)) {
        start++;
    }
    auto end = s.end();
    do {
        end--;
    } while (distance(start, end) > 0 && isspace(*end));
    return string(start, end + 1);
}

class parkir {
private:
    string currentUser;    // *** PERBAIKAN: Simpan username yang sedang login

public:
    void login();
    void inputPengunjung();
    void history_login();
    void pembayaran();
    void logout();
    void tampilkanparkirKosong();
    void riwayatParkir();
};

void parkir::login() {
    struct petugas {
        string username;
        string password;
    };

    petugas akun[5] = {
        {"shift1", "123"},
        {"shift2", "123"},
        {"shift3", "123"},
        {"shift4", "123"},
        {"shift5", "123"}
    };

    bool verifikasi = false;
    string inputUSN, inputPW, jam_login, tanggal_login;

    do {
        cout << "-------------- LOGIN --------------" << endl;
        cout << "| Masukkan Username: ";
        cin >> inputUSN;
        cout << "| Masukkan Password: ";
        cin >> inputPW;
        cout << "-----------------------------------" << endl;

        for (int i = 0; i < 5; i++) {
            if (inputUSN == akun[i].username && inputPW == akun[i].password) {
                verifikasi = true;
                currentUser = inputUSN;    // *** PERBAIKAN: Simpan username ke member
                break;
            }
        }

        if (!verifikasi) {
            cout << "ERROR: Username/Password salah. Coba lagi!" << endl;
            system("pause");
            system("cls");
        }
    } while (!verifikasi);

    system("cls");

    bool verifikasi_format = false;
ulang:
    cout << "Masukkan tanggal login (DD-MM-YYYY): ";
    cin >> tanggal_login;
    cout << "Masukkan jam login (HH:MM): ";
    cin >> jam_login;
    if (tanggal_login.length() == 10 && jam_login.length() == 5) {
        verifikasi_format = true;
    }

    if (!verifikasi_format) {
        cout << "\nERROR: Format tanggal/jam yang dimasukkan salah!" << endl;
        cout << "--- CONTOH FORMAT YANG BENAR --- " << endl;
        cout << "| Tanggal login : 10-03-2025   |" << endl;
        cout << "| Jam login     : 14:05        |" << endl;
        cout << "--------------------------------" << endl << endl;
        goto ulang;
    }

    cout << "Login berhasil, tekan tombol apa saja untuk melanjutkan" << endl;
    system("pause");
    system("cls");

    // *** PERBAIKAN: Simpan history login dengan format yang konsisten
    ofstream waktu_masuk("history_login.txt", ios::app);
    if (waktu_masuk.is_open()) {
        waktu_masuk << "LOGIN  | " 
                    << tanggal_login << " | " 
                    << jam_login     << " | " 
                    << currentUser   << endl;
        waktu_masuk.close();
    }
}

void parkir::tampilkanparkirKosong() {
    cout << "\nStatus Slot Parkir:\n";
    for (int i = 0; i < lantaiparkir; i++) {
        cout << "Lantai " << i + 1 << ": ";
        for (int j = 0; j < tempatparkir; j++) {
            if (dataparkir[i][j] == 0)
                cout << "[" << j + 1 << "] ";
            else
                cout << "[X] ";
        }
        cout << endl;
    }
}

void parkir::riwayatParkir() {
    cout << "----------------- RIWAYAT PARKIR -----------------" << endl;
    cout << "Tanggal       | Jenis      | Jam   | Plat Nomor | Lantai | Tempat" << endl;
    cout << "---------------------------------------------------------------" << endl;

    ifstream riwayat("history_pengunjung.txt");
    if (riwayat.is_open()) {
        string baris;
        while (getline(riwayat, baris)) {
            cout << baris << endl;
        }
        riwayat.close();
    } else {
        cout << "ERROR: Riwayat parkir gagal dimuat." << endl;
    }

    cout << "---------------------------------------------------------------" << endl;
    cout << "Tekan tombol apa saja untuk kembali ke menu" << endl;
    system("pause");
    system("cls");
}

void parkir::inputPengunjung() {
    tampilkanparkirKosong();

    int lantai, tempat;
    string jam_datang, tanggal_datang, plat_nomor, jenis_kendaraan;

    while (true) {
        cout << "----------------- INPUT DATA PENGUNJUNG -----------------" << endl;
        cout << "| Masukkan tanggal datang (DD-MM-YYYY): ";
        cin >> tanggal_datang;
        cout << "| Masukkan jam datang (HH:MM): ";
        cin >> jam_datang;
        cout << "| Masukkan plat nomor: ";
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        getline(cin, plat_nomor);
        cout << "| Masukkan jenis kendaraan (motor/mobil): ";
        cin >> jenis_kendaraan;
        cout << "Pendataan lantai parkir (1-" << lantaiparkir << "): ";
        cin >> lantai;
        cout << "Pendataan nomor tempat parkir (1-" << tempatparkir << "): ";
        cin >> tempat;

        if (tanggal_datang.length() == 10 && jam_datang.length() == 5 &&
            (jenis_kendaraan == "motor" || jenis_kendaraan == "mobil") &&
            lantai >= 1 && lantai <= lantaiparkir &&
            tempat >= 1 && tempat <= tempatparkir) {

            if (dataparkir[lantai - 1][tempat - 1] == 1) {
                cout << "Slot sudah terisi! Silakan pilih slot lain." << endl;
                continue;
            }

            dataparkir[lantai - 1][tempat - 1] = 1;

            ofstream pengunjung("history_pengunjung.txt", ios::app);
            if (pengunjung.is_open()) {
                // *** PERBAIKAN: Simpan juga Lantai dan Tempat di file history
                pengunjung << tanggal_datang << " | "
                           << jenis_kendaraan << " | "
                           << jam_datang      << " | "
                           << plat_nomor      << " | "
                           << "L" << lantai    << " | "
                           << "T" << tempat    << endl;
                cout << "Data berhasil disimpan." << endl;
            } else {
                cout << "ERROR: Data gagal disimpan!" << endl;
            }
            pengunjung.close();

            cout << "\n=== Perubahan Data Terbaru ===\n";
            tampilkanparkirKosong();
            break;
        } else {
            cout << "\nERROR: Format atau posisi parkir tidak valid!" << endl;
        }
    }

    system("pause");
    system("cls");
}

void parkir::history_login() {
    cout << "----------------- HISTORY LOGIN PETUGAS -----------------" << endl;
    cout << "Keterangan | Tanggal        | Jam    | Nama Akun" << endl;
    cout << "---------------------------------------------------------" << endl;
    ifstream history("history_login.txt");

    if (history.is_open()) {
        string baris;
        while (getline(history, baris)) {
            cout << baris << endl;
        }
        cout << "---------------------------------------------------------" << endl;
    } else {
        cout << "ERROR: History gagal dimuat." << endl;
    }
    history.close();
    cout << "Tekan tombol apa saja untuk kembali ke menu" << endl;
    system("pause");
    system("cls");
}

void parkir::pembayaran() {
    string plat_nomor, jam_keluar;
    cout << "---------- PEMBAYARAN PARKIR ----------" << endl;
    cout << "Masukkan plat nomor kendaraan: ";
    cin.ignore();
    getline(cin, plat_nomor);

    string tanggal, jenis, jam_datang;
    bool ditemukan = false;

    ifstream file("history_pengunjung.txt");
    ofstream temp("temp.txt");
    string line;

    while (getline(file, line)) {
        size_t pos = line.find(plat_nomor);
        if (pos != string::npos) {
            ditemukan = true;

            tanggal = line.substr(0, 10);
            jenis = line.substr(13, line.find("|", 13) - 13);
            jam_datang = line.substr(jenis == "mobil" ? 23 : 22, 5);

            cout << "Data ditemukan:\n";
            cout << "Tanggal datang : " << tanggal << endl;
            cout << "Jam datang     : " << jam_datang << endl;
            cout << "Jenis kendaraan: " << jenis << endl;

            cout << "Masukkan jam keluar (HH:MM): ";
            cin >> jam_keluar;

            char hilang_karcis, titip_barang;
            int denda = 0, biaya_titip = 0;

            cout << "Apakah karcis hilang? (y/n): ";
            cin >> hilang_karcis;
            if (hilang_karcis == 'y' || hilang_karcis == 'Y') {
                denda = 50000;
            }

            cout << "Apakah menitipkan barang? (y/n): ";
            cin >> titip_barang;
            if (titip_barang == 'y' || titip_barang == 'Y') {
                biaya_titip = 5000;
            }

            int jam_in = stoi(jam_datang.substr(0, 2));
            int menit_in = stoi(jam_datang.substr(3, 2));
            int jam_out = stoi(jam_keluar.substr(0, 2));
            int menit_out = stoi(jam_keluar.substr(3, 2));

            int durasi = (jam_out * 60 + menit_out) - (jam_in * 60 + menit_in);
            if (durasi < 0) durasi += 24 * 60;
            int jam_total = (durasi + 59) / 60;

            int tarif = (jenis == "mobil") ? 5000 : 2000;
            int total = (tarif * jam_total) + denda + biaya_titip;

            cout << "\n========== STRUK PARKIR ==========" << endl;
            cout << "Plat Nomor     : " << plat_nomor << endl;
            cout << "Jenis Kendaraan: " << jenis << endl;
            cout << "Jam Datang     : " << jam_datang << endl;
            cout << "Jam Keluar     : " << jam_keluar << endl;
            cout << "Durasi         : " << jam_total << " jam" << endl;
            cout << "Tarif per jam  : Rp " << tarif << endl;
            if (denda > 0) cout << "Denda karcis   : Rp " << denda << endl;
            if (biaya_titip > 0) cout << "Titip barang   : Rp " << biaya_titip << endl;
            cout << "TOTAL BAYAR    : Rp " << total << endl;
            cout << "==================================" << endl;

            // ?? SIMPAN STRUK KE FILE
            ofstream struk("struk_pembayaran.txt");
            if (struk.is_open()) {
                struk << "========== STRUK PARKIR ==========" << endl;
                struk << "Plat Nomor     : " << plat_nomor << endl;
                struk << "Jenis Kendaraan: " << jenis << endl;
                struk << "Jam Datang     : " << jam_datang << endl;
                struk << "Jam Keluar     : " << jam_keluar << endl;
                struk << "Durasi         : " << jam_total << " jam" << endl;
                struk << "Tarif per jam  : Rp " << tarif << endl;
                if (denda > 0) struk << "Denda karcis   : Rp " << denda << endl;
                if (biaya_titip > 0) struk << "Titip barang   : Rp " << biaya_titip << endl;
                struk << "TOTAL BAYAR    : Rp " << total << endl;
                struk << "==================================" << endl;
                struk.close();
                cout << "\nStruk pembayaran telah disimpan ke 'struk_pembayaran.txt'\n";
            } else {
                cout << "ERROR: Gagal membuat file struk.\n";
            }
        } else {
            temp << line << endl;
        }
    }

    file.close();
    temp.close();
    remove("history_pengunjung.txt");
    rename("temp.txt", "history_pengunjung.txt");

    if (!ditemukan) {
        cout << "ERROR: Plat nomor tidak ditemukan dalam riwayat.\n";
    }

    system("pause");
    system("cls");
}

void parkir::logout() {
    string jam_logout, tanggal_logout;

    // Minta tanggal logout dengan format yang benar
    bool verifikasi_format = false;
ulang2:
    cout << "Masukkan tanggal logout (DD-MM-YYYY): ";
    cin >> tanggal_logout;
    cout << "Masukkan jam logout    (HH:MM)    : ";
    cin >> jam_logout;
    if (tanggal_logout.length() == 10 && jam_logout.length() == 5) {
        verifikasi_format = true;
    }

    if (!verifikasi_format) {
        cout << "\nERROR: Format tanggal/jam salah! Contoh: 10-03-2025 15:10\n";
        goto ulang2;
    }

    ofstream waktu_keluar("history_login.txt", ios::app);
    if (waktu_keluar.is_open()) {
        // *** PERBAIKAN: Tulis tanggal/jam/logout + username yang sama seperti saat login
        waktu_keluar << "LOGOUT | "
                     << tanggal_logout << " | "
                     << jam_logout     << " | "
                     << currentUser    << endl;
        waktu_keluar.close();
        cout << "Logout berhasil. Terima kasih!\n";
    } else {
        cout << "ERROR: Logout gagal menyimpan ke file!\n";
    }

    system("pause");
    system("cls");
}

int main() {
    // *** PERBAIKAN: Inisialisasi semua slot parkir jadi 0 (kosong)
    for (int i = 0; i < lantaiparkir; i++) {
        for (int j = 0; j < tempatparkir; j++) {
            dataparkir[i][j] = 0;
        }
    }

    parkir mall;
    mall.login();

    int pilih;
    do {
        cout << "=============== SELAMAT DATANG ===============" << endl;
        cout << "| PILIH MENU:" << endl;
        cout << "| 1. Tambah data pengunjung" << endl;
        cout << "| 2. Lihat riwayat login petugas" << endl;
        cout << "| 3. Lihat riwayat parkir kendaraan" << endl;
        cout << "| 4. Pembayaran" << endl;
        cout << "| 0. Logout" << endl;
        cout << "| Masukkan pilihan (0-4): ";
        cin >> pilih;
        cout << "=============================================" << endl;
        system("cls");

        switch (pilih) {
            case 1: mall.inputPengunjung(); break;
            case 2: mall.history_login();   break;
            case 3: mall.riwayatParkir();   break;
            case 4: mall.pembayaran();      break;
            case 0: mall.logout();          break;
            default: cout << "Pilihan tidak valid.\n"; 
                     system("pause");
                     system("cls");
                     break;
        }
    } while (pilih != 0);

    return 0;
}

