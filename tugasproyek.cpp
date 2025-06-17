// Login petugas. username: shift1-5 | pass : 123
// Login pengunjung. username: pengunjung11 | pass : 123

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
using namespace std;

const int lantaiparkir = 3;
const int tempatparkir = 4;
int dataparkir[lantaiparkir][tempatparkir];

// Data pengunjung global
struct DataPengunjung {
    string username;
    string password;
    int saldo;
    string nama;
};

DataPengunjung pengunjung = {"pengunjung11", "123", 50000, "Pengunjung Mall"};

bool validasiTanggal(const string& tanggal) {
    regex pola("\\d{2}-\\d{2}-\\d{4}");
    return regex_match(tanggal, pola);
}

bool validasiJam(const string& jam) {
    regex pola("\\d{2}:\\d{2}");
    return regex_match(jam, pola);
}

class parkir {
public:
    int login(); // Return 1 untuk petugas, 2 untuk pengunjung
    void inputPengunjung();
    void history_login();
    void pembayaran();
    void logout();
    void tampilkanparkirKosong();
    
    // Menu pengunjung
    void menuPengunjung();
    void pembayaranPengunjung();
    void lihatSlotKosong();
    void reservasiParkir();
    void akunSaldo();
    void isiSaldo();
    void cekHistoryPembayaran();
    void ubahPassword();
    void laporanAkhirHari();
    void lihatKendaraanKeluar();

};

int parkir::login() {
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
    string simpan_username;
    int tipe_user = 0; // 0: tidak valid, 1: petugas, 2: pengunjung

    do {
        cout << "-------------- LOGIN --------------" << endl;
        cout << "| Masukkan Username: ";
        cin >> inputUSN;
        cout << "| Masukkan Password: ";
        cin >> inputPW;
        cout << "| (Ketik '0' untuk keluar program) " << endl;
        cout << "-----------------------------------" << endl;

        // Cek untuk keluar program
        if (inputUSN == "0") {
            cout << "Terima kasih! Program akan ditutup." << endl;
            exit(0);
        }

        // Cek login petugas
        for (int i = 0; i < 5; i++) {
            if (inputUSN == akun[i].username && inputPW == akun[i].password) {
                verifikasi = true;
                simpan_username = inputUSN;
                tipe_user = 1;
                break;
            }
        }

        // Cek login pengunjung
        if (!verifikasi && inputUSN == pengunjung.username && inputPW == pengunjung.password) {
            verifikasi = true;
            simpan_username = inputUSN;
            tipe_user = 2;
        }

        if (!verifikasi) {
            cout << "ERROR: Username/Password salah. Coba lagi!" << endl;
            system("pause");
            system("cls");
        }
    } while (!verifikasi);

    system("cls");

    if (tipe_user == 1) {
        // Login petugas - minta tanggal dan jam
        bool verifikasi_format = false;
    ulang:
        cout << "Masukkan tanggal login(TANGGAL-BULAN-TAHUN): ";
        cin >> tanggal_login;
        cout << "Masukkan jam login(JAM:MENIT): ";
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

        ofstream waktu_masuk("history_login.txt", ios::app);
        waktu_masuk << "LOGIN     | " << tanggal_login << "    | " << jam_login << "       | " << simpan_username << endl;
        waktu_masuk.close();
    }

    cout << "Login berhasil sebagai " << (tipe_user == 1 ? "Petugas" : "Pengunjung") << "!" << endl;
    cout << "Tekan tombol apa saja untuk melanjutkan" << endl;
    
    system("pause");
    system("cls");

    return tipe_user;
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

void parkir::inputPengunjung() {
    tampilkanparkirKosong();

    int lantai, tempat;
    string jam_datang, tanggal_datang, plat_nomor, jenis_kendaraan;

    while (true) {
        cout << "----------------- INPUT DATA PENGUNJUNG -----------------" << endl;
        cout << "| Masukkan tanggal datang(TANGGAL-BULAN-TAHUN): ";
        cin >> tanggal_datang;
        cout << "| Masukkan jam datang(JAM:MENIT): ";
        cin >> jam_datang;
        cout << "| Masukkan plat nomor: ";
        cin.ignore();
        getline(cin, plat_nomor);
        cout << "| Masukkan jenis kendaraan(motor/mobil): ";
        cin >> jenis_kendaraan;
        cout << "Pendataan lantai parkir (1-" << lantaiparkir << "): ";
        cin >> lantai;
        cout << "Pendataan nomor tempat parkir (1-" << tempatparkir << "): ";
        cin >> tempat;

        if (tanggal_datang.length() == 10 && jam_datang.length() == 5 &&
            lantai >= 1 && lantai <= lantaiparkir &&
            tempat >= 1 && tempat <= tempatparkir) {

            // Tandai slot parkir sebagai terisi
            dataparkir[lantai - 1][tempat - 1] = 1;

            ofstream pengunjung("history_pengunjung.txt", ios::app);
            if (pengunjung.is_open()) {
                pengunjung << tanggal_datang << " | " << jenis_kendaraan << " | " << jam_datang << " | " << plat_nomor << endl;
                cout << "Data berhasil disimpan." << endl;
            } else {
                cout << "ERROR : Data gagal disimpan!" << endl;
            }
            pengunjung.close();

            cout << "\n=== Perubahan Data Akhir ===\n";
            tampilkanparkirKosong();
            break;
        } else {
            cout << "\nERROR: Format atau posisi parkir tidak valid!" << endl;
        }
        
int totalSlot = lantaiparkir * tempatparkir;
int slotTerisi = 0;

for (int i = 0; i < lantaiparkir; i++) {
    for (int j = 0; j < tempatparkir; j++) {
        if (dataparkir[i][j] == 1) slotTerisi++;
    }
}

int slotKosong = totalSlot - slotTerisi;

if (slotKosong <= 2) {
    cout << "\n??  Peringatan: Slot parkir hampir penuh! Tersisa hanya " << slotKosong << " slot.\n";
}

    }

    system("pause");
    system("cls");
}

void parkir::history_login() {
    cout << "----------------- HISTORY LOGIN PETUGAS -----------------" << endl;
    cout << "Keterangan | Tanggal login | waktu login | Nama akun " << endl;
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
    string platCari;
    cout << "===== PEMBAYARAN PARKIR =====" << endl;
    cout << "Masukkan plat nomor: ";
    cin.ignore();
    getline(cin, platCari);

    ifstream fileIn("history_pengunjung.txt");
    ofstream fileOut("temp.txt");
    string baris;
    bool ditemukan = false;

    if (!fileIn.is_open() || !fileOut.is_open()) {
        cout << "Gagal membuka file data pengunjung." << endl;
        return;
    }

    while (getline(fileIn, baris)) {
        if (baris.find(platCari) != string::npos) {
            ditemukan = true;
            cout << "\nData ditemukan:\n" << baris << endl;

            string tanggal, jenis, jam_masuk, plat;
            istringstream ss(baris);
            getline(ss, tanggal, '|');
            getline(ss, jenis, '|');
            getline(ss, jam_masuk, '|');
            getline(ss, plat);

            // Trim whitespace
            tanggal.erase(0, tanggal.find_first_not_of(" "));
            jenis.erase(0, jenis.find_first_not_of(" "));
            jam_masuk.erase(0, jam_masuk.find_first_not_of(" "));
            plat.erase(0, plat.find_first_not_of(" "));

            string jam_keluar;
            do {
                cout << "Masukkan jam keluar (HH:MM): ";
                getline(cin, jam_keluar);
            } while (!validasiJam(jam_keluar));

            // Pisah jam dan menit
            int jam1 = stoi(jam_masuk.substr(0, 2));
            int jam2 = stoi(jam_keluar.substr(0, 2));
            int menit1 = stoi(jam_masuk.substr(3, 2));
            int menit2 = stoi(jam_keluar.substr(3, 2));

            int durasi = (jam2 * 60 + menit2) - (jam1 * 60 + menit1);
            if (durasi < 0) durasi += 1440; // Lewat tengah malam

            int jam_total = (durasi + 59) / 60; // pembulatan ke atas

            // Hitung tarif
            int tarif_per_jam = (jenis.find("mobil") != string::npos) ? 5000 : 2000;
            int tarif = tarif_per_jam * jam_total;

            // Karcis hilang?
            char hilang;
            int denda = 0;
            cout << "Karcis hilang? (Y/N): ";
            cin >> hilang;
            if (hilang == 'Y' || hilang == 'y') denda = 50000;

            // Titip barang?
            char titip;
            int biaya_titip = 0;
            cout << "Titip barang? (Y/N): ";
            cin >> titip;
            if (titip == 'Y' || titip == 'y') biaya_titip = 2000 * jam_total;

            int total = tarif + denda + biaya_titip;

            cout << "\n===== STRUK =====" << endl;
            cout << "Plat Nomor   : " << plat << endl;
            cout << "Jam Masuk    : " << jam_masuk << endl;
            cout << "Jam Keluar   : " << jam_keluar << endl;
            cout << "Durasi       : " << jam_total << " jam" << endl;
            cout << "Tarif        : Rp " << tarif << endl;
            if (denda) cout << "Denda        : Rp " << denda << endl;
            if (biaya_titip) cout << "Penitipan    : Rp " << biaya_titip << endl;
            cout << "TOTAL        : Rp " << total << endl;
            cout << "==========================" << endl;

            ofstream struk("struk_pembayaran.txt", ios::app);
            struk << "STRUK PEMBAYARAN\n";
            struk << "Plat Nomor : " << plat << "\n";
            struk << "Jam Masuk  : " << jam_masuk << "\n";
            struk << "Jam Keluar : " << jam_keluar << "\n";
            struk << "Durasi     : " << jam_total << " jam\n";
            struk << "TOTAL      : Rp " << total << "\n\n";
            struk.close();

            // Tandai slot sebagai kosong
            for (int i = 0; i < lantaiparkir; i++) {
                for (int j = 0; j < tempatparkir; j++) {
                    if (dataparkir[i][j] == 1) {
                        dataparkir[i][j] = 0; // Kosongkan (sederhana, tanpa lokasi spesifik)
                        goto selesai;
                    }
                }
            }
        selesai:
            continue; // lewati penyalinan baris ini
        } else {
            fileOut << baris << endl;
        }
    }

    fileIn.close();
    fileOut.close();

    if (ditemukan) {
        remove("history_pengunjung.txt");
        rename("temp.txt", "history_pengunjung.txt");
        cout << "\nPembayaran berhasil dan slot dikosongkan!" << endl;
    } else {
        cout << "Plat nomor tidak ditemukan!" << endl;
        remove("temp.txt");
    }

    system("pause");
    system("cls");
}

void parkir::logout() {
    string jam_logout;
    while (true) {
        cout << "| Masukkan jam logout(JAM:MENIT): ";
        cin >> jam_logout;

        if (validasiJam(jam_logout)) break;
        cout << "\nERROR: Format jam salah! Cth: 14:05\n";
    }

    // Ambil tanggal login terakhir dari file history_login
    string tanggal_terakhir = "--/--/----";
    ifstream history("history_login.txt");
    string line;
    while (getline(history, line)) {
        if (line.find("LOGIN") != string::npos) {
            tanggal_terakhir = line.substr(12, 10);
        }
    }
    history.close();

    // Hitung kendaraan masuk
    int jumlahMotor = 0, jumlahMobil = 0, jumlahKeluar = 0, totalUang = 0;
    ifstream pengunjung("history_pengunjung.txt");
    while (getline(pengunjung, line)) {
        if (line.find("motor") != string::npos) jumlahMotor++;
        else if (line.find("mobil") != string::npos) jumlahMobil++;
    }
    pengunjung.close();

    // Hitung pendapatan dan kendaraan keluar dari struk
    ifstream struk("struk_pembayaran.txt");
    while (getline(struk, line)) {
        if (line.find("TOTAL") != string::npos) {
            size_t pos = line.find("Rp ");
            if (pos != string::npos) {
                int uang = stoi(line.substr(pos + 3));
                totalUang += uang;
                jumlahKeluar++;
            }
        }
    }
    struk.close();

    // Hitung slot kosong dan terisi
    int totalSlot = lantaiparkir * tempatparkir;
    int slotTerisi = 0;
    for (int i = 0; i < lantaiparkir; i++) {
        for (int j = 0; j < tempatparkir; j++) {
            if (dataparkir[i][j] == 1) slotTerisi++;
        }
    }
    int slotKosong = totalSlot - slotTerisi;

    // Cetak laporan akhir saat logout
    cout << "\n======== LAPORAN OTOMATIS SAAT LOGOUT ========" << endl;
    cout << "Tanggal Hari Ini       : " << tanggal_terakhir << endl;
    cout << "Jam Logout             : " << jam_logout << endl;
    cout << "Jumlah Kendaraan Masuk : " << (jumlahMotor + jumlahMobil) << endl;
    cout << " - Motor               : " << jumlahMotor << endl;
    cout << " - Mobil               : " << jumlahMobil << endl;
    cout << "Jumlah Kendaraan Keluar: " << jumlahKeluar << endl;
    cout << "Total Pendapatan       : Rp " << totalUang << endl;
    cout << "Slot Parkir Kosong     : " << slotKosong << " dari " << totalSlot << endl;
    cout << "Slot Parkir Terisi     : " << slotTerisi << " dari " << totalSlot << endl;
    cout << "===============================================" << endl;

    // Simpan juga ke file laporan
    ofstream file("laporan_logout_otomatis.txt", ios::app);
    file << "===== LOGOUT SHIFT =====" << endl;
    file << "Tanggal      : " << tanggal_terakhir << endl;
    file << "Jam Logout   : " << jam_logout << endl;
    file << "Motor Masuk  : " << jumlahMotor << endl;
    file << "Mobil Masuk  : " << jumlahMobil << endl;
    file << "Keluar       : " << jumlahKeluar << endl;
    file << "Pendapatan   : Rp " << totalUang << endl;
    file << "Slot Kosong  : " << slotKosong << endl;
    file << "Slot Terisi  : " << slotTerisi << endl;
    file << "=========================" << endl << endl;
    file.close();

    // Simpan juga logout ke history
    ofstream waktu_keluar("history_login.txt", ios::app);
    if (waktu_keluar.is_open()) {
        waktu_keluar << "LOGOUT    | " << tanggal_terakhir << "    | " << jam_logout << "       | --" << endl;
        waktu_keluar.close();
        cout << "\nLogout berhasil. Terima kasih telah bertugas!" << endl;
    } else {
        cout << "ERROR: Gagal menyimpan waktu logout!\n";
    }

    system("pause");
}


// === MENU PENGUNJUNG ===
void parkir::menuPengunjung() {
    int pilih;
    do {
        cout << "========== MENU PENGUNJUNG ==========" << endl;
        cout << "| Selamat datang, " << pengunjung.nama << "!" << endl;
        cout << "| Saldo Anda: Rp " << pengunjung.saldo << endl;
        cout << "| 1. Pembayaran Parkir" << endl;
        cout << "| 2. Lihat Slot Parkir Kosong" << endl;
        cout << "| 3. Reservasi Parkir" << endl;
        cout << "| 4. Akun & Saldo" << endl;
        cout << "| 0. Logout" << endl;
        cout << "| Pilihan Anda: ";
        cin >> pilih;
        cout << "=====================================" << endl;
        system("cls");

        switch (pilih) {
            case 1: pembayaranPengunjung(); break;
            case 2: lihatSlotKosong(); break;
            case 3: reservasiParkir(); break;
            case 4: akunSaldo(); break;
            case 0: 
                cout << "Logout berhasil! Kembali ke menu login..." << endl;
                system("pause");
                break;
            default: 
                cout << "Pilihan tidak valid!" << endl;
                system("pause");
                system("cls");
                break;
        }
    } while (pilih != 0);
     
    int totalSlot = lantaiparkir * tempatparkir;
    int slotTerisi = 0;

    for (int i = 0; i < lantaiparkir; i++) {
        for (int j = 0; j < tempatparkir; j++) {
            if (dataparkir[i][j] == 1) slotTerisi++;
    }
}

int slotKosong = totalSlot - slotTerisi;

if (slotKosong <= 2) {
    cout << "\n??  Peringatan: Slot parkir hampir penuh! Tersisa hanya " << slotKosong << " slot.\n";
}

}

void parkir::pembayaranPengunjung() {
    int biaya_parkir;
    cout << "===== PEMBAYARAN PARKIR PENGUNJUNG =====" << endl;
    cout << "Saldo Anda saat ini: Rp " << pengunjung.saldo << endl;
    cout << "Masukkan jumlah yang harus dibayar: Rp ";
    cin >> biaya_parkir;

    if (biaya_parkir <= 0) {
        cout << "Jumlah pembayaran tidak valid!" << endl;
        system("pause");
        system("cls");
        return;
    }

    // Tanya reservasi dan nitip barang
    char reservasi, nitip;
    int diskon = 0;
    
    cout << "Apakah Anda punya reservasi parkir? (Y/N): ";
    cin >> reservasi;
    if (reservasi == 'Y' || reservasi == 'y') {
        diskon += biaya_parkir * 0.1; // Diskon 10%
        cout << "Diskon reservasi 10% diterima!" << endl;
    }

    cout << "Apakah Anda menitip barang? (Y/N): ";
    cin >> nitip;
    if (nitip == 'Y' || nitip == 'y') {
        diskon += biaya_parkir * 0.05; // Diskon tambahan 5%
        cout << "Diskon penitipan barang 5% diterima!" << endl;
    }

    int total_bayar = biaya_parkir - diskon;

    cout << "\n===== DETAIL PEMBAYARAN =====" << endl;
    cout << "Biaya Parkir     : Rp " << biaya_parkir << endl;
    cout << "Total Diskon     : Rp " << diskon << endl;
    cout << "Total Pembayaran : Rp " << total_bayar << endl;
    cout << "==============================" << endl;

    if (pengunjung.saldo >= total_bayar) {
        pengunjung.saldo -= total_bayar;
        cout << "Pembayaran berhasil!" << endl;
        cout << "Saldo tersisa: Rp " << pengunjung.saldo << endl;

        // Simpan ke history
        ofstream history("history_pembayaran_pengunjung.txt", ios::app);
        history << "Pembayaran Parkir | Rp " << total_bayar << " | Saldo: Rp " << pengunjung.saldo << endl;
        history.close();
    } else {
        cout << "Saldo tidak mencukupi!" << endl;
        cout << "Kekurangan: Rp " << (total_bayar - pengunjung.saldo) << endl;
    }
    
int totalSlot = lantaiparkir * tempatparkir;
int slotTerisi = 0;

for (int i = 0; i < lantaiparkir; i++) {
    for (int j = 0; j < tempatparkir; j++) {
        if (dataparkir[i][j] == 1) slotTerisi++;
    }
}

int slotKosong = totalSlot - slotTerisi;

if (slotKosong <= 2) {
    cout << "\n??  Peringatan: Slot parkir hampir penuh! Tersisa hanya " << slotKosong << " slot.\n";
}


    system("pause");
    system("cls");
}

void parkir::lihatSlotKosong() {
    cout << "===== SLOT PARKIR TERSEDIA =====" << endl;
    tampilkanparkirKosong();
    cout << "=================================" << endl;
    system("pause");
    system("cls");
}

void parkir::reservasiParkir() {
    cout << "===== RESERVASI PARKIR =====" << endl;
    lihatSlotKosong();
    
    int lantai, slot;
    cout << "Pilih lantai (1-" << lantaiparkir << "): ";
    cin >> lantai;
    cout << "Pilih slot (1-" << tempatparkir << "): ";
    cin >> slot;

    if (lantai >= 1 && lantai <= lantaiparkir && slot >= 1 && slot <= tempatparkir) {
        if (dataparkir[lantai-1][slot-1] == 0) {
            dataparkir[lantai-1][slot-1] = 1; // Tandai sebagai terisi
            cout << "Reservasi berhasil untuk Lantai " << lantai << " Slot " << slot << endl;
            cout << "Anda mendapat diskon 10% untuk pembayaran nanti!" << endl;
            
            // Simpan reservasi
            ofstream reservasi("reservasi_pengunjung.txt", ios::app);
            reservasi << pengunjung.username << " | Lantai " << lantai << " Slot " << slot << endl;
            reservasi.close();
        } else {
            cout << "Slot sudah terisi! Pilih slot lain." << endl;
        }
    } else {
        cout << "Pilihan tidak valid!" << endl;
    }

    system("pause");
    system("cls");
}

void parkir::akunSaldo() {
    int pilih;
    do {
        cout << "======== AKUN & SALDO ========" << endl;
        cout << "| Nama    : " << pengunjung.nama << endl;
        cout << "| Username: " << pengunjung.username << endl;
        cout << "| Saldo   : Rp " << pengunjung.saldo << endl;
        cout << "| 1. Isi Saldo" << endl;
        cout << "| 2. Cek History Pembayaran" << endl;
        cout << "| 3. Ubah Password" << endl;
        cout << "| 0. Kembali" << endl;
        cout << "| Pilihan: ";
        cin >> pilih;
        cout << "==============================" << endl;
        system("cls");

        switch (pilih) {
            case 1: isiSaldo(); break;
            case 2: cekHistoryPembayaran(); break;
            case 3: ubahPassword(); break;
            case 0: break;
            default: 
                cout << "Pilihan tidak valid!" << endl;
                system("pause");
                system("cls");
                break;
        }
    } while (pilih != 0);
}

void parkir::isiSaldo() {
    int tambah_saldo;
    cout << "===== ISI SALDO =====" << endl;
    cout << "Saldo saat ini: Rp " << pengunjung.saldo << endl;
    cout << "Masukkan jumlah saldo yang ingin ditambah: Rp ";
    cin >> tambah_saldo;

    if (tambah_saldo > 0) {
        pengunjung.saldo += tambah_saldo;
        cout << "Saldo berhasil ditambah!" << endl;
        cout << "Saldo baru: Rp " << pengunjung.saldo << endl;

        // Simpan ke history
        ofstream history("history_pembayaran_pengunjung.txt", ios::app);
        history << "Top Up Saldo | +Rp " << tambah_saldo << " | Saldo: Rp " << pengunjung.saldo << endl;
        history.close();
    } else {
        cout << "Jumlah tidak valid!" << endl;
    }

    system("pause");
    system("cls");
}

void parkir::cekHistoryPembayaran() {
    cout << "===== HISTORY PEMBAYARAN =====" << endl;
    ifstream history("history_pembayaran_pengunjung.txt");
    string baris;
    
    if (history.is_open()) {
        bool ada_data = false;
        while (getline(history, baris)) {
            cout << baris << endl;
            ada_data = true;
        }
        if (!ada_data) {
            cout << "Belum ada history pembayaran." << endl;
        }
        history.close();
    } else {
        cout << "File history tidak ditemukan." << endl;
    }
    
    cout << "===============================" << endl;
    system("pause");
    system("cls");
}

void parkir::ubahPassword() {
    string password_lama, password_baru;
    cout << "===== UBAH PASSWORD =====" << endl;
    cout << "Masukkan password lama: ";
    cin >> password_lama;

    if (password_lama == pengunjung.password) {
        cout << "Masukkan password baru: ";
        cin >> password_baru;
        pengunjung.password = password_baru;
        cout << "Password berhasil diubah!" << endl;
    } else {
        cout << "Password lama salah!" << endl;
    }

    system("pause");
    system("cls");
}

// === FUNGSI LAINNYA ===
void lihatRiwayatParkir() {
    cout << "------------- RIWAYAT PARKIR KENDARAAN -------------" << endl;
    cout << "Tanggal | Jenis | Jam | Plat Nomor" << endl;
    cout << "----------------------------------------------------" << endl;
    ifstream file("history_pengunjung.txt");
    string baris;
    if (file.is_open()) {
        while (getline(file, baris)) {
            cout << baris << endl;
        }
        file.close();
    } else {
        cout << "File riwayat tidak ditemukan.";
    }
    cout << "----------------------------------------------------" << endl;
    system("pause");
    system("cls");
}

void cariDataParkir() {
    string cariPlat;
    cout << "===== CARI DATA PARKIR =====" << endl;
    cout << "Masukkan plat nomor: ";
    cin.ignore();
    getline(cin, cariPlat);

    ifstream file("history_pengunjung.txt");
    string baris;
    bool ditemukan = false;

    cout << "Hasil pencarian untuk plat nomor '" << cariPlat << "':" << endl;
    cout << "---------------------------------------------" << endl;

    if (file.is_open()) {
        while (getline(file, baris)) {
            if (baris.find(cariPlat) != string::npos) {
                cout << baris << endl;
                ditemukan = true;
            }
        }
        file.close();
    }

    if (!ditemukan) {
        cout << "Data tidak ditemukan." << endl;
    }

    cout << "---------------------------------------------" << endl;
    system("pause");
    system("cls");
}

void tambahShiftPetugas() {
    string nama, tanggal, jamMasuk, jamKeluar;
    cout << "===== TAMBAH JADWAL SHIFT PETUGAS =====" << endl;
    cout << "Nama petugas: ";
    cin.ignore();
    getline(cin, nama);

    do {
        cout << "Tanggal shift (DD-MM-YYYY): ";
        getline(cin, tanggal);
    } while (!validasiTanggal(tanggal));

    do {
        cout << "Jam masuk (HH:MM): ";
        getline(cin, jamMasuk);
    } while (!validasiJam(jamMasuk));

    do {
        cout << "Jam keluar (HH:MM): ";
        getline(cin, jamKeluar);
    } while (!validasiJam(jamKeluar));

    ofstream fileShift("jadwal_shift.txt", ios::app);
    fileShift << nama << " | " << tanggal << " | " << jamMasuk << " - " << jamKeluar << endl;
    fileShift.close();

    cout << "Jadwal shift berhasil disimpan!" << endl;
    system("pause");
    system("cls");
}

void lihatShiftPetugas() {
    cout << "===== JADWAL SHIFT PETUGAS =====" << endl;
    cout << "Nama | Tanggal | Jam Shift" << endl;
    cout << "-------------------------------" << endl;

    ifstream fileShift("jadwal_shift.txt");
    string baris;
    if (fileShift.is_open()) {
        while (getline(fileShift, baris)) {
            cout << baris << endl;
        }
        fileShift.close();
    } else {
        cout << "File jadwal shift tidak ditemukan." << endl;
    }

    cout << "-------------------------------" << endl;
    system("pause");
    system("cls");
}

void parkir::laporanAkhirHari() {
    cout << "===== LAPORAN AKHIR HARI =====" << endl;

    int jumlahMasuk = 0, jumlahKeluar = 0, totalUang = 0;
    string tanggal;
    
    // Ambil tanggal dari login terakhir
    ifstream history("history_login.txt");
    string line;
    while (getline(history, line)) {
        if (line.find("LOGIN") != string::npos) {
            tanggal = line.substr(12, 10); // Ambil tanggal dari format
        }
    }
    history.close();

    // Hitung jumlah masuk (dari history pengunjung)
    ifstream pengunjung("history_pengunjung.txt");
    while (getline(pengunjung, line)) {
        if (!line.empty()) jumlahMasuk++;
    }
    pengunjung.close();

    // Hitung jumlah pembayaran dan uang masuk
    ifstream struk("struk_pembayaran.txt");
    while (getline(struk, line)) {
        if (line.find("TOTAL") != string::npos) {
            size_t pos = line.find("Rp");
            if (pos != string::npos) {
                int uang = stoi(line.substr(pos + 3));
                totalUang += uang;
                jumlahKeluar++;
            }
        }
    }
    struk.close();

    // Slot tersibuk (lantai terisi paling banyak)
    int lantai_terbanyak = 0, jumlahSlotTerisi = 0;
    for (int i = 0; i < lantaiparkir; i++) {
        int hitung = 0;
        for (int j = 0; j < tempatparkir; j++) {
            if (dataparkir[i][j] == 1) hitung++;
        }
        if (hitung > jumlahSlotTerisi) {
            jumlahSlotTerisi = hitung;
            lantai_terbanyak = i + 1;
        }
    }

    cout << "Tanggal Hari Ini   : " << tanggal << endl;
    cout << "Total Kendaraan Masuk : " << jumlahMasuk << endl;
    cout << "Total Kendaraan Keluar: " << jumlahKeluar << endl;
    cout << "Total Pendapatan       : Rp " << totalUang << endl;
    cout << "Lantai Paling Sering Terisi: Lantai " << lantai_terbanyak << endl;

    // Simpan ke file
    ofstream laporan("laporan_akhir_hari.txt", ios::app);
    laporan << "===== LAPORAN AKHIR HARI =====" << endl;
    laporan << "Tanggal           : " << tanggal << endl;
    laporan << "Masuk Kendaraan   : " << jumlahMasuk << endl;
    laporan << "Keluar Kendaraan  : " << jumlahKeluar << endl;
    laporan << "Total Pendapatan  : Rp " << totalUang << endl;
    laporan << "Lantai Tersibuk   : " << lantai_terbanyak + 1 << endl;
    laporan << "===============================" << endl << endl;
    laporan.close();

    system("pause");
    system("cls");
}

void lihatKendaraanKeluar() {
    cout << "========== DAFTAR KENDARAAN KELUAR ==========" << endl;

    ifstream file("struk_pembayaran.txt");
    string baris;
    int nomor = 1;
    bool sedangCetak = false;

    if (file.is_open()) {
        while (getline(file, baris)) {
            if (baris.find("STRUK PEMBAYARAN") != string::npos) {
                cout << "\n[" << nomor++ << "] ------------------------------" << endl;
                sedangCetak = true;
            } 
            else if (sedangCetak) {
                if (baris.empty()) {
                    sedangCetak = false;  // selesai cetak satu struk
                } else {
                    cout << baris << endl;
                }
            }
        }
        file.close();
        
    } else {
        cout << "File struk tidak ditemukan." << endl;
    }

    cout << "==============================================" << endl;
    
    system("pause");  
    system("cls");  
}

void bubbleSortDataParkir() {
    vector<string> data;
    ifstream file("history_pengunjung.txt");
    string baris;

    while (getline(file, baris)) {
        data.push_back(baris);
    }
    file.close();

    if (data.empty()) {
        cout << "Tidak ada data untuk diurutkan." << endl;
        system("pause");
        return;
    }

    // Bubble Sort berdasarkan plat nomor (kolom ke-4)
    for (size_t i = 0; i < data.size() - 1; i++) {
        for (size_t j = 0; j < data.size() - i - 1; j++) {
            // Ambil plat dari baris j dan j+1
            stringstream ss1(data[j]), ss2(data[j + 1]);
            string t1, j1, k1, plat1, t2, j2, k2, plat2;

            getline(ss1, t1, '|');
            getline(ss1, k1, '|');
            getline(ss1, j1, '|');
            getline(ss1, plat1);

            getline(ss2, t2, '|');
            getline(ss2, k2, '|');
            getline(ss2, j2, '|');
            getline(ss2, plat2);

            plat1.erase(0, plat1.find_first_not_of(" "));
            plat2.erase(0, plat2.find_first_not_of(" "));

            if (plat1 > plat2) {
                swap(data[j], data[j + 1]);
            }
        }
    }

    cout << "===== DATA PENGUNJUNG TERURUT BERDASARKAN PLAT =====" << endl;
    cout << "Tanggal | Jenis | Jam | Plat Nomor" << endl;
    cout << "-----------------------------------------------" << endl;
    for (const auto& d : data) {
        cout << d << endl;
    }
    cout << "-----------------------------------------------" << endl;

    system("pause");
    system("cls");
}

void bubbleSortByJam() {
    vector<string> data;
    ifstream file("history_pengunjung.txt");
    string baris;

    while (getline(file, baris)) {
        data.push_back(baris);
    }
    file.close();

    if (data.empty()) {
        cout << "Tidak ada data untuk diurutkan." << endl;
        system("pause");
        return;
    }

    // Bubble Sort berdasarkan jam datang (kolom ke-3)
    for (size_t i = 0; i < data.size() - 1; i++) {
        for (size_t j = 0; j < data.size() - i - 1; j++) {
            stringstream ss1(data[j]), ss2(data[j + 1]);
            string t1, k1, jam1, plat1, t2, k2, jam2, plat2;

            getline(ss1, t1, '|');
            getline(ss1, k1, '|');
            getline(ss1, jam1, '|');
            getline(ss1, plat1);

            getline(ss2, t2, '|');
            getline(ss2, k2, '|');
            getline(ss2, jam2, '|');
            getline(ss2, plat2);

            jam1.erase(0, jam1.find_first_not_of(" "));
            jam2.erase(0, jam2.find_first_not_of(" "));

            if (jam1 > jam2) {
                swap(data[j], data[j + 1]);
            }
        }
    }

    cout << "===== DATA PENGUNJUNG DIURUTKAN BERDASARKAN JAM =====" << endl;
    cout << "Tanggal | Jenis | Jam | Plat Nomor" << endl;
    cout << "-----------------------------------------------------" << endl;
    for (const auto& d : data) {
        cout << d << endl;
    }
    cout << "-----------------------------------------------------" << endl;

    system("pause");
    system("cls");
}


int main() {
    parkir mall;
    
    while (true) {
        int tipe_login = mall.login();
        
        if (tipe_login == 1) {
            // Menu Petugas
            int pilih;
            do {
            	
                cout << "=============== MENU PETUGAS ===============" << endl;
                cout << "| 1. Tambah data pengunjung" << endl;
                cout << "| 2. Lihat riwayat login petugas" << endl;
                cout << "| 3. Lihat riwayat parkir kendaraan" << endl;
                cout << "| 4. Pembayaran" << endl;
                cout << "| 5. Cari data parkir berdasarkan plat nomor" << endl;
                cout << "| 6. Tambah jadwal shift petugas" << endl;
                cout << "| 7. Lihat jadwal shift petugas" << endl;
                cout << "| 8. Informasi Sementara Laporan Pendapatan" << endl ;
		cout << "| 9. Lihat Daftar Kendaraan Keluar" << endl; 
		cout << "| 10. Urutkan data parkir berdasarkan plat" << endl;
		cout << "| 11. Urutkan data berdasarkan jam datang" << endl;
                cout << "| 0. Logout" << endl;
                cout << "| Masukkan pilihan: ";
                cin >> pilih;
                cout << "===========================================" << endl;
                system("cls");

                switch (pilih) {
                	
                    case 1: mall.inputPengunjung(); break;
                    case 2: mall.history_login(); break;
                    case 3: lihatRiwayatParkir(); break;
                    case 4: mall.pembayaran(); break;
                    case 5: cariDataParkir(); break;
                    case 6: tambahShiftPetugas(); break;
                    case 7: lihatShiftPetugas(); break;
                    case 8: mall.laporanAkhirHari(); break;
                    case 9: lihatKendaraanKeluar(); break;
                    case 10: bubbleSortDataParkir(); break;
                    case 11: bubbleSortByJam(); break;
                    case 0: mall.logout(); break;
                    default: 
                        cout << "Pilihan tidak valid." << endl; 
                        system("pause");
                        system("cls");
                        break;
                }
                
            } while (pilih != 0);
        }
        
        else if (tipe_login == 2) {
            // Menu Pengunjung
            mall.menuPengunjung();
            
        }
        
        system("cls");
        
    }
    return 0;
}
