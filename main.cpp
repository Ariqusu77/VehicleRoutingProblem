#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
#include<iomanip>

using namespace std;

// Definisikan struktur untuk menyimpan data.
struct TargetData {
    int x;
    int y;
    int permintaan;
    int waktu_buka;
    int waktu_tutup;
    vector<double> data_jarak; // Vektor untuk menyimpan jarak ke target lain
};

struct kendaraan {
    int id;
    int kapasitas;
};

struct Murid {
    int id;
};

class DataJarak {
public:
    vector<TargetData> Data;

    // Fungsi untuk menghitung dan menetapkan jarak antar target
    void AssignDataJarak() {
        // Inisialisasi vektor data_jarak untuk setiap TargetData
        for (auto& target : Data) {
            target.data_jarak.resize(Data.size(), 0.0);
        }

        // Hitung jarak antar target dengan rumus jarak Euclidean
        for (size_t i = 0; i < Data.size(); ++i) {
            for (size_t j = i + 1; j < Data.size(); ++j) {
                double jarak = sqrt(pow(Data[i].x - Data[j].x, 2) + pow(Data[i].y - Data[j].y, 2));
                Data[i].data_jarak[j] = jarak;
                Data[j].data_jarak[i] = jarak; // Jarak simetris
            }
        }
    }

    // Fungsi untuk menampilkan data jarak antar target
    void PrintDataJarak() {
        for (size_t i = 0; i < Data.size(); ++i) {
            cout << "Target " << i << " jarak ke target lain: ";
            for (size_t j = 0; j < Data[i].data_jarak.size(); ++j) {
                cout << fixed << setprecision(3) <<Data[i].data_jarak[j];
                if (j < Data[i].data_jarak.size() - 1) cout << ", ";
            }
            cout << endl;
        }
    }
};
// Fungsi menambah kendaraan
vector<kendaraan> inputKendaraan(){
    vector<kendaraan> vehicles;
    int jumlahKendaraan;
    cout << "Masukkan jumlah kendaraan: ";
    cin >> jumlahKendaraan;
    for(int i= 0; i < jumlahKendaraan; ++i){
        kendaraan v;
        cout << "kendaraan " << i + 1 << " - Masukkan kapasitas: ";
        cin >> v.kapasitas;
        v.id = i + 1;
        vehicles.push_back(v);
    }
    return vehicles;
}

// Fungsi menambah murid
vector<Murid> inputMurid(){
    vector<Murid> students;
    int jumlahMurid;
    cout << "Masukkan jumlah Murid: ";
    cin >> jumlahMurid;
    for(int i = 0; i < jumlahMurid; i++){
        Murid m;
        m.id = i + 1;
        students.push_back(m);
    }
    return students;
}
// Fungsi untuk membaca data dari file CSV.
vector<TargetData> readDataFromCSV(const string& filePath) {
    vector<TargetData> data;
    ifstream file(filePath);
    string line;
    getline(file, line); // Melewati header

    while (getline(file, line)) {
        stringstream iss(line);
        TargetData targetData;
        string value;
        getline(iss, value, ','); // Melewati kolom 'Target'
        getline(iss, value, ','); targetData.x = stoi(value);
        getline(iss, value, ','); targetData.y = stoi(value);
        getline(iss, value, ','); targetData.permintaan = stoi(value);
        getline(iss, value, ','); targetData.waktu_buka = stoi(value);
        getline(iss, value, ','); targetData.waktu_tutup = stoi(value);

        data.push_back(targetData);
    }
    file.close();
    return data;
}

int main() {
    string csv_file = "sample.csv"; // Pastikan file ini ada di direktori yang sama dengan executable atau berikan path lengkap.
    vector<TargetData> targets = readDataFromCSV(csv_file);


    cout<< "DATA AWAL"<< endl;
    int number = 0;
    // Tampilkan data untuk verifikasi
    for (const auto& target : targets) {
        cout << "No " << number
                  << ", X: " << target.x
                  << ", Y: " << target.y
                  << ", Permintaan: " << target.permintaan
                  << ", Waktu Buka: " << target.waktu_buka
                  << ", Waktu Tutup: " << target.waktu_tutup << endl;
                  number++;
    }

    cout<< "============================================================="<< endl;

    DataJarak dataJarak;
    dataJarak.Data = targets; // Memasukkan data target ke dalam class DataJarak
    dataJarak.AssignDataJarak(); // Menghitung jarak antar target

    // Tampilkan jarak antar target
    cout<< "DATA JARAK: " << endl;
    dataJarak.PrintDataJarak();

    cout<< "============================================================="<< endl;

    cout<< "INPUT KENDARAAN DAN MURID: "<< endl;
    vector<kendaraan> vehicles = inputKendaraan();
    vector<Murid> students = inputMurid();

    return 0;
}
