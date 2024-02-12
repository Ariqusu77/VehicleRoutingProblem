#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>

using namespace std;

// Definisikan struktur untuk menyimpan data.
struct TargetData {
    int x;
    int y;
    int permintaan;
    int waktu_buka;
    int waktu_tutup;
};

class DataJarak {

};

// Fungsi untuk membaca data dari file CSV.
vector<TargetData> readDataFromCSV(const string& filePath) {
    vector<TargetData> data;
    ifstream file(filePath);
    string line;
    // Melewati header
    getline(file, line);

    while (getline(file, line)) {
        istringstream iss(line);
        string value;
        TargetData targetData;

        // Melewati kolom 'Target'
        getline(iss, value, ',');

        // Membaca data x, y, permintaan, waktu_buka, waktu_tutup
        getline(iss, value, ','); targetData.x = stoi(value);
        getline(iss, value, ','); targetData.y = stoi(value);
        getline(iss, value, ','); targetData.permintaan = stoi(value);
        getline(iss, value, ','); targetData.waktu_buka = stoi(value);
        getline(iss, value, ','); targetData.waktu_tutup = stoi(value);

        data.push_back(targetData);
    }

    return data;
}

// Changed void to int for the return type since you're returning an integer on failure
int bacadata(string csv_file)
{
    cout << "Ini Sample \n";
    ifstream file(csv_file);
    string line;
    if (!file.is_open()) {
        cerr << "Gagal membuka file!" << endl;
        return 1;
    }
    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> row;
        string data;
        while (getline(ss, data, ',')) {
            row.push_back(data);
        }
        for (const auto& field : row) {
            cout << field << ",";
        }
        cout << endl;
    }
    file.close();
    // Added return statement since the function expects an integer return type
    return 0;
}

int main() {
    // Added semicolon at the end of the function call

    string csv_file = "sample.csv";
    //bacadata(csv_file);

    vector<TargetData> targets = readDataFromCSV(csv_file);

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

    return 0;
}
