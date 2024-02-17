#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
#include<regex>
#include<iomanip>
#include<random>

using namespace std;

std::random_device rd;
std::mt19937 gen(rd());

// Definisikan struktur untuk menyimpan data.
struct TargetData {
    int x;
    int y;
    int permintaan;
    int waktu_buka;
    int waktu_tutup;
    vector<double> data_jarak; // Vektor untuk menyimpan jarak ke target lain
    vector<double> data_waktu_tempuh; // Vektor untuk menyimpan data waktu tempuh
};

class Kendaraan {
public:
    int id;
    int kapasitas = 0;
    int max;
    int waktu = 0;
    vector<int> routing;

    Kendaraan(){
        routing.push_back(0);
    }

    Kendaraan(int Id, int kapasitas) : id(Id), max(kapasitas) {
        routing.push_back(0);
    }

    void Update(){

    }
};

class DataJarak {
public:
    vector<TargetData> Data;

    DataJarak(){}

    DataJarak(vector<TargetData> data) : Data(data) {
        AssignDataJarak();
        AssignDataWaktuTempuh();
    }

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

    void AssignDataWaktuTempuh() {
        // Pastikan setiap target memiliki vektor data_waktu_tempuh dengan ukuran yang tepat
        for (auto& target : Data) {
            target.data_waktu_tempuh.resize(Data.size(), 0.0);
        }

        // Hitung waktu tempuh antar target dengan rumus yang diberikan
        for (size_t i = 0; i < Data.size(); ++i) {
            for (size_t j = 0; j < Data[i].data_jarak.size(); ++j) {
                // Waktu tempuh adalah jarak (dalam satuan yang sama dengan kecepatan) dikalikan 60 dan dibagi 40
                double waktu_tempuh = Data[i].data_jarak[j] * 60.0 / 40.0;
                Data[i].data_waktu_tempuh[j] = waktu_tempuh;
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

    void PrintDataWaktuTempuh() {
        cout << "Waktu tempuh antar target (dalam menit):" << endl;
        for (size_t i = 0; i < Data.size(); ++i) {
            cout << "Dari Target " << i << " ke target lain: ";
            for (size_t j = 0; j < Data[i].data_waktu_tempuh.size(); ++j) {
                cout << fixed << setprecision(3) << Data[i].data_waktu_tempuh[j];
                if (j < Data[i].data_waktu_tempuh.size() - 1) cout << ", ";
            }
            cout << endl;
        }
    }
};

class Siswa {
public:
    int id; 
    int destination;
    int kapasitas;
    int point;
    int prefpoint;
    int status;
    vector<double> initial;
    vector<double> bestpoint;
    vector<int> routes;
    vector<Kendaraan> vechicle;
 
    Siswa(){}

    Siswa(int i, int n,int kapasitas) : destination(n), id(i), kapasitas(kapasitas) {
        Setup();
    }

    void Setup() {
        // Resize Temp to the size of destination
        vector<double> Temp(destination);

        for (int i = 0; i < destination; i++) {
            double minr = 0.0;
            double maxr = 1.0;
            std::uniform_real_distribution<double> dis(minr, maxr);
            double random = dis(gen);
            Temp[i] = random;  // Store the random number in Temp
        }

        initial = Temp;  
    }

    void GetRoute(){
        vector<double> Temp = initial;
        routes.resize(destination);

        for (int i = 0; i < destination; i++) {
            double min = Temp[i];

            int num = i; // Initialize num to i
            for (int j = 0; j < destination; j++) {
                if (Temp[j] > min) {
                    continue;
                }
                min = Temp[j];
                num = j;
            }
            Temp[num] = 1.1;  // Set Temp[num] to a value not less than 1.0 to avoid being selected again
            routes[num] = i + 1;  // Assign the route index
        }
    }

    void CleanUp(){
        routes.clear();
        vechicle.clear();
    }

    void TryRoute(DataJarak jarak){
        vector<int> temp = routes; 
        int id = 1;
        cout << endl << "Murid " << this->id  << " Lets goo!!" << endl << endl;
        while(temp.size() != 0){
            Kendaraan TempVechicle(id , kapasitas);
            TempVechicle.kapasitas = 0;
            cout << "Kendaraan " << TempVechicle.id << " berangkat ======"<< endl << endl;
            //cout << TempVechicle.id << " " << TempVechicle.kapasitas << endl;
            for(auto it = temp.begin(); it != temp.end();){
                int current = *it;
                //cout << "Mencoba route : " << current << endl;

                if(TempVechicle.kapasitas + jarak.Data[current].permintaan > TempVechicle.max){
                    TempVechicle.waktu += jarak.Data[current].data_waktu_tempuh[TempVechicle.routing.back()];
                    TempVechicle.kapasitas = 0;
                    TempVechicle.routing.push_back(0);
                    //cout << "Kendaraan mengalami overload dan akan kembali ke depot" << endl << endl;
                    continue; 
                }

                if(TempVechicle.waktu + jarak.Data[current].data_waktu_tempuh[TempVechicle.routing.back()] + jarak.Data[current].permintaan >= jarak.Data[current].waktu_tutup){
                    if(TempVechicle.waktu + jarak.Data[current].data_waktu_tempuh[TempVechicle.routing.back()] < jarak.Data[current].waktu_tutup){
                        int muatan = jarak.Data[current].waktu_tutup - ( TempVechicle.waktu + jarak.Data[current].data_waktu_tempuh[TempVechicle.routing.back()] );
                        jarak.Data[current].permintaan -= muatan;
                        TempVechicle.waktu = jarak.Data[current].waktu_tutup;
                        TempVechicle.routing.push_back(current);
                        TempVechicle.kapasitas += muatan;
                        //cout << "Sebagian muatan pada rute akan dilanjutkan kendaraan selanjutanya" << endl << endl;
                        ++it;
                        continue;
                    }
                    //cout << "Route akan diambil kendaraan berikutnya" << endl << jarak.Data[current].data_waktu_tempuh[TempVechicle.routing.back()] << endl;
                    ++it;
                    continue;
                }

                if(TempVechicle.waktu <= jarak.Data[current].waktu_buka){
                    TempVechicle.waktu = jarak.Data[current].waktu_buka + jarak.Data[current].permintaan;
                    TempVechicle.routing.push_back(current);
                    TempVechicle.kapasitas += jarak.Data[current].permintaan;
                    //cout << "Route telah diamankan oleh kendaraan ini" << endl << endl;
                    it = temp.erase(it);
                    continue;
                }

                TempVechicle.waktu += jarak.Data[current].data_waktu_tempuh[TempVechicle.routing.back()] + jarak.Data[current].permintaan;
                TempVechicle.routing.push_back(current);
                TempVechicle.kapasitas += jarak.Data[current].permintaan;
                //cout << "Route telah diamankan oleh kendaraan ini" << endl << endl;
                it = temp.erase(it);
            }
            id++;
            vechicle.push_back(TempVechicle);
            cout << "route : " ;
            for(auto route : TempVechicle.routing){
                 cout << route << " - ";
             }
            cout << "0" << endl;
        }
    }

    void Assess(){
        int tdt = 0;
        for(auto vehicle : vechicle){
            tdt += vehicle.waktu;
        } 
        int rdt = vechicle.front().waktu - vechicle.back().waktu;
        point = vechicle.size() * 100000 + tdt * 100 + rdt * 0.00005;
    }

    void Evaluate(const vector<Siswa>& para_siswa){
        vector<double> temp(initial.size());
        vector<int> j = randomData(initial.size(),1,para_siswa.size());
        vector<int> k = randomData(initial.size(),1,2);
        vector<double> rand = randomDouble(initial.size());
        vector<double> min = minData(para_siswa, initial.size());
        vector<double> max = maxData(para_siswa, initial.size());
        vector<double> mean = meanData(para_siswa, initial.size());
        vector<double> best = bestData(para_siswa);
        switch (status)
        {
        case 0: //BEST   
            for(int i = 0; i < initial.size(); i++){
                temp[i] = initial[i] + pow(-1,k[i]) * rand[i] * (initial[i] - para_siswa[j[i]].initial[i]); 
            }
            break;
        case 1: //GOOD
            for(int i = 0; i < initial.size(); i++){
                temp[i] = initial[i] + (rand[i] * (best[i] - initial[i])) + (rand[i] * (mean[i] - initial[i]));
            }
            break;
        case 2: //AVERAGE
            for(int i = 0; i < initial.size(); i++){
                temp[i] = initial[i] + (rand[i] * (mean[i] - initial[i]));
            }
            break;
        case 3: //RANDOM
            for(int i = 0; i < initial.size(); i++){
                temp[i] = initial[i] + (rand[i] * (max[i] - min[i]));
            }
            break;
        default:
            break;
        }
        initial = temp;
    }

    vector<double> randomDouble(int size){
    vector<double> Temp(size);
    for (int i = 0; i < size; i++) {
        
        double minr = 0.0;
        double maxr = 1.0;
        std::uniform_real_distribution<double> dis(minr, maxr);
        double random = dis(gen);
        Temp[i] = random;  // Store the random number in Temp
    }
    return Temp;
    }

    vector<int> randomData(int size, int min, int max){
        vector<int> temp;
        for(int i = 0; i < size; i++){
            uniform_int_distribution<int> dit(min,max);

            temp.push_back(dit(gen));
        }
        return temp;
    }

    vector<double> minData(const vector<Siswa>& para_siswa, int size){
        vector<double> temparray;
        for(int i=0; i < size; i++){
            double temp = para_siswa[0].bestpoint[i];
            for(int j = 0; j < para_siswa.size(); j++){
                if(para_siswa[j].bestpoint[i] < temp){
                    temp = para_siswa[j].bestpoint[i];
                }
            }
            temparray.push_back(temp);
        }
        return temparray;
    }

    vector<double> maxData(const vector<Siswa>& para_siswa, int size){
        vector<double> temparray;
        for(int i=0; i < size; i++){
            double temp = para_siswa[0].bestpoint[i];
            for(int j = 0; j < para_siswa.size(); j++){
                if(para_siswa[j].bestpoint[i] > temp){
                    temp = para_siswa[j].bestpoint[i];
                }
            }
            temparray.push_back(temp);
        }
        return temparray;
    }

    vector<double> meanData(const vector<Siswa>& para_siswa, int size){
        vector<double> temparray;
        for(int i=0; i < size; i++){
            double temp=0;
            for(int j = 0; j < para_siswa.size(); j++){
                temp += para_siswa[j].bestpoint[i];    
            }
            temp /= para_siswa.size();
            temparray.push_back(temp);
        }
        return temparray;
    }

    vector<double> bestData(const vector<Siswa>& para_siswa){
        vector<double> temparray;
        for(auto student : para_siswa){
            if(student.status == 0){
                temparray = student.bestpoint;
            }
        }
        return temparray;
    }
};

class Alghorithm {
public:
    int num_of_siswa;
    int iteration;
    int kapasitas;
    vector<Siswa> para_siswa;
    DataJarak Jarak;

    Alghorithm() {}

    Alghorithm(DataJarak dist) : Jarak(dist){
    }

    void set_siswa() {
        for (int i = 0; i < num_of_siswa; i++){
            para_siswa.push_back(Siswa(i + 1 , Jarak.Data.size() - 1, kapasitas));
        }
    }

    void Input(){
        cout << "Inputkan Jumlah Siswa : " ;
        cin >> num_of_siswa;
        cout << "Inputkan Jumlah Iterasi : " ;
        cin >>  iteration;
        cout << "Inputkan Kapasitas Kendaraan : " ;
        cin >> kapasitas;
    }

    void Iterate(){
        for(int i=0; i<iteration; i++){
            cout<< "================================================================="<< endl << endl;
            cout<<"Iterasi ke = "<< i + 1 << endl;
            for(int j=0; j<num_of_siswa; j++){
                para_siswa[j].GetRoute();
                para_siswa[j].TryRoute(Jarak);
                para_siswa[j].Assess();
                para_siswa[j].bestpoint = para_siswa[j].initial;
                if (i > 0 && para_siswa[j].point > para_siswa[j].prefpoint){
                    para_siswa[j].point = para_siswa[j].prefpoint;
                }
                para_siswa[j].prefpoint = para_siswa[j].point;
            }
            PrepareNext();
            BestStudent();
            for (int j = 0; j < num_of_siswa; j++)
            {
                para_siswa[j].Evaluate(para_siswa);
            }
            
        }
    }

    void BestStudent(){
        int minIndex = 0;
        int minvalue = para_siswa[0].point;
        for(int i =0;i< num_of_siswa; i++){
            if(para_siswa[i].point < minvalue){
                minIndex = i;
                minvalue = para_siswa[i].point;
            }
        }
        para_siswa[minIndex].status = 0; //BEST
        cout << endl << "best student : " << minIndex + 1 << endl;
        cout << "score : " << minvalue << endl;
    }

    void PrepareNext(){
        for (int i = 0; i < num_of_siswa; i++) {
            uniform_int_distribution<int> dit(1,3);
            int rand = dit(gen);
            para_siswa[i].status = rand;
        }
    }

    void Start(){
        Input();
        set_siswa();
        Iterate();
    }
};

vector<double> randomDouble(int size){
    vector<double> Temp(size);
    for (int i = 0; i < size; i++) {
        double minr = 0.0;
        double maxr = 1.0;
        std::uniform_real_distribution<double> dis(minr, maxr);
        double random = dis(gen);
        Temp[i] = random;  // Store the random number in Temp
    }
    return Temp;
}

vector<int> randomData(int size, int min, int max){
    vector<int> temp;
    for(int i = 0; i < size; i++){
        uniform_int_distribution<int> dit(min,max);

        temp.push_back(dit(gen));
    }
    return temp;
}

vector<TargetData> readDataFromCSV(const string& filePath) {
    vector<TargetData> data;
    ifstream file(filePath);
    string line;
    getline(file, line); // Skip the header

    while (getline(file, line)) {
        stringstream iss(line);
        TargetData targetData;
        string value;

        // Read each value separated by comma
        getline(iss, value, ','); // Skipping the 'Target' column
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

// Function to print initial data
void printInitialData(const vector<TargetData>& targets) {
    cout << "DATA AWAL" << endl;
    int number = 0;
    for (const auto& target : targets) {
        cout << "No " << number
             << ", X: " << target.x
             << ", Y: " << target.y
             << ", Permintaan: " << target.permintaan
             << ", Waktu Buka: " << target.waktu_buka
             << ", Waktu Tutup: " << target.waktu_tutup << endl;
        number++;
    }
}

int main() {
    // Pastikan file ini ada di direktori yang sama dengan executable atau berikan path lengkap.
    int num;

    string csv_file = "sample.csv";
    string datakecil = "Data Kecil.csv";
    string datasedang = "Data Sedang.csv";
    string databesar = "Data Besar.csv";
    vector<TargetData> csv = readDataFromCSV(csv_file);
    vector<TargetData> kecil = readDataFromCSV(datakecil);
    vector<TargetData> sedang = readDataFromCSV(datasedang);
    vector<TargetData> besar = readDataFromCSV(databesar);

    vector<TargetData> targets;

    cout << "Pilih file yang akan digunakan : " << endl 
         << "1. Data Kecil" << endl 
         << "2. Data Sedang" << endl 
         << "3. Data Besar" << endl
         << endl;

    cin >> num;

    switch (num)
    {
    case 1:
        cout << "data yang akan digunakan : data kecil" << endl;
        targets = kecil;
        break;
    case 2:
        cout << "data yang akan digunakan : data sedang" << endl;
        targets = sedang;
        break;
    case 3:
        cout << "data yang akan digunakan : data besar" << endl;
        targets = besar;
        break;
    
    default:
        targets = csv;
    }
    //Call the function to print initial data
    //printInitialData(targets);

    DataJarak dataJarak(targets);
    Alghorithm sistem(dataJarak);

    sistem.Start();

    return 0;
}
