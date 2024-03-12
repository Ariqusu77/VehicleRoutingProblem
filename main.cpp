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

// Inisialisasi Fungsi Random
std::random_device rd;
std::mt19937 gen(rd());

// Definisikan struktur untuk menyimpan data.
// Struct Target Data untuk menyimpan data yang dibaca dari csv
// ini termasuk lokasi spasial dari targed, waktubuka dan tutup serta data jarak dan waktu yang diperlukan untuk mencapainya dari koordinat lain
struct TargetData {
    int x;
    int y;
    int permintaan;
    int waktu_buka;
    int waktu_tutup;
    vector<double> data_jarak; // Vektor untuk menyimpan jarak ke target lain
    vector<double> data_waktu_tempuh; // Vektor untuk menyimpan data waktu tempuh
};

// class Kendaraan untuk menyimpan data yang berkaitan dengan kendaraan
// ini termasuk id dari kendaraan, kapasitas, lama waktu dia beroperasi dan jalur yang telah dia tempuh
class Kendaraan {
public:
    int id;
    int kapasitas = 0;
    int max;
    int waktu = 0;
    vector<int> routing;
    int caps = 0;

    // konstruktor dari kelas kendaraan
    Kendaraan(){
        routing.push_back(0);
    }
    Kendaraan(int Id, int kapasitas) : id(Id), max(kapasitas) {
        routing.push_back(0);
    }
};

// calss data jarak yang menyimpan kumpulan data Targetdata
// class ini menghitung waktu jarak dari sat tempat ke tempat yang lain juga waktu yang dibutuhkan dari satu tempat ke tempat yang lain
class DataJarak {
public:
    vector<TargetData> Data;

    // konstruktor dari kelas DataJarak
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

    // fungsi untuk menghitung dan menetukan waktu antar target
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

    // fungsi untuk menampilkan data waktu antar target
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

// class siswa menyimpan segala hal yang dibutuh kan oleh seorang siswa
// mulai dari inisialisasi performa awal, menyimpan data data rte yang akan dipakai dan data kendaraan, sampai bagaimana seorang siswa belajar
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
    vector<Kendaraan> bestroute;
 
    Siswa(){}

    Siswa(int i, int n,int kapasitas) : destination(n), id(i), kapasitas(kapasitas) {
        Setup();
    }

    //fungsi untuk mendapatkan performa awal dari sang siswa
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

    //fungsi untuk menampilkan performa awal dari sang siswa
    void ShowInitial(){
        cout << endl << "Murid " << this->id << endl; 
        for(auto num : initial){
            cout << " " << fixed << setprecision(3) << num << " |" ;
        }
        cout << endl;
    }

    //fungsi untuk mendapatkan rute dari performa awal siswa yang diurtkan 
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
            Temp[num] = 10000000;  // Set Temp[num] to a value not less than 1.0 to avoid being selected again
            routes[num] = i + 1;  // Assign the route index
        }
    }

    // fungsi untuk menampilkan route yang telah didapatkan
    void ShowRoute(){
        for(auto num : routes){
            int width = (num < 10) ? 6 : 5;
            width = ( num < 100) ? width : 4;
            cout << setw(width) << num << " |" ;
        }
        cout << endl;
    }

    double NormalizeValue(double value, double inMin, double inMax, double outMin, double outMax) {
        return outMin + (value - inMin) * (outMax - outMin) / (inMax - inMin);
    }

    void Normalize(){
        double max = initial[0];
        double min = initial[0];
        for(auto value : initial){
            if (value > max){
                max = value;
            }
            if (value < min){
                min = value;
            }
        }
        for(auto i=0; i<initial.size(); i++){
            initial[i] = NormalizeValue(initial[i], min, max, 0.0, 1.0);
        }
    }

    // fungsi untuk mereset sang siswa setelah iterasi selesai sebelum memasuki iterasi selanjutnya
    void CleanUp(){
        routes.clear();
        vechicle.clear();
        this->Normalize();
    }

    //fungsi untuk mencoba data route yang telah didapatkan 
    //rute dicoba berdasarkan urutannya, bila kendaraan sudah tak mampu maka kendaraan selanjutnya akan melanjutkannya
    void TryRoute(DataJarak jarak){
        vector<int> temp = routes; 
        int id = 1;
        // cout << endl << "Murid " << this->id  << " Mulai Pelajaran" << endl;
        while(temp.size() != 0){
            Kendaraan TempVechicle(id , kapasitas);
            TempVechicle.kapasitas = 0;
            //cout << "Kendaraan " << TempVechicle.id << " berangkat ======"<< endl;
            //cout << TempVechicle.id << " " << TempVechicle.kapasitas << endl;
            for(auto it = temp.begin(); it != temp.end();){
                int current = *it;
                //cout << "Mencoba route : " << current << endl;
                // cout << jarak.Data[current].permintaan << endl;

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
                        TempVechicle.caps += muatan;
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
                    TempVechicle.caps += jarak.Data[current].permintaan;
                    //cout << "Route telah diamankan oleh kendaraan ini" << endl << endl;
                    it = temp.erase(it);
                    continue;
                }

                TempVechicle.waktu += jarak.Data[current].data_waktu_tempuh[TempVechicle.routing.back()] + jarak.Data[current].permintaan;
                TempVechicle.routing.push_back(current);
                TempVechicle.kapasitas += jarak.Data[current].permintaan;
                TempVechicle.caps += jarak.Data[current].permintaan;
                //cout << "Route telah diamankan oleh kendaraan ini" << endl << endl;
                it = temp.erase(it);
            }
            id++;
            vechicle.push_back(TempVechicle);
        }
    }

    //fungsi untuk menghitung nilai dari sang siswa
    void Assess(){
        int tdt = 0;
        for(auto vehicle : vechicle){
            tdt += vehicle.waktu;
        } 
        int rdt = vechicle.front().waktu - vechicle.back().waktu;
        point = vechicle.size() * 100000 + tdt * 100 + rdt * 0.00005;
    }

    //fungsi untuk menentukan mana evaluasi yang tepat untuk sang siswa berdasarkan statusnya
    void Evaluate(const vector<Siswa>& para_siswa){
        vector<double> temp(initial.size());
        switch (status)
        {
        case 0: //BEST   
            temp = caseBest(para_siswa);
            break;
        case 1: //GOOD
            temp = caseGood(para_siswa);
            break;
        case 2: //AVERAGE
            temp = caseAverage(para_siswa);
            break;
        case 3: //RANDOM
            temp = caseRandom(para_siswa);
            break;
        default:
            break;
        }
        initial = temp;
    }

    // fungsi untuk mengevaluasi kinerja dari siswa rata rata 
    // x = x(t-1) +- ( random * (x(t-1) - (nilai random parasiswa)))
    vector<double> caseBest(const vector<Siswa>& para_siswa){
        vector<double> temp(initial.size());
        vector<int> j = randomData(initial.size(),1,para_siswa.size());
        vector<int> k = randomData(initial.size(),1,2);
        vector<double> rand = randomDouble(initial.size());
        for(int i = 0; i < initial.size(); i++){
            temp[i] = initial[i] + pow(-1,k[i]) * rand[i] * (initial[i] - para_siswa[j[i]-1].initial[i]); 
        }
        return temp;
    }

    // fungsi untuk mengevaluasi kinerja dari siswa baik
    // x = x(t-1) + ( random * (best - x(t-1))) +( random * (mean - x(t-1)))
    vector<double> caseGood(const vector<Siswa>& para_siswa){
        vector<double> temp(initial.size());
        vector<double> rand = randomDouble(initial.size());
        vector<double> mean = meanData(para_siswa, initial.size());
        vector<double> best = bestData(para_siswa);
        for(int i = 0; i < initial.size(); i++){
            temp[i] = initial[i] + (rand[i] * (best[i] - initial[i])) + (rand[i] * (mean[i] - initial[i]));
        }
        return temp;
    }

    // fungsi untuk mengevaluasi kinerja dari siswa rata rata 
    // x = x(t-1) + ( random * (mean - x(t-1)))
    vector<double> caseAverage(const vector<Siswa>& para_siswa){
        vector<double> temp(initial.size());
        vector<double> rand = randomDouble(initial.size());
        vector<double> mean = meanData(para_siswa, initial.size());
        for(int i = 0; i < initial.size(); i++){
            temp[i] = initial[i] + (rand[i] * (mean[i] - initial[i]));
        }    
        return temp;
    }

    // fungsi untuk mengevaluasi kinerja dari siswa random 
    // x = x(t-1) + ( random * (max - min))
    vector<double> caseRandom(const vector<Siswa>& para_siswa){
        vector<double> temp(initial.size());
        vector<double> rand = randomDouble(initial.size());
        vector<double> min = minData(para_siswa, initial.size());
        vector<double> max = maxData(para_siswa, initial.size());
        for(int i = 0; i < initial.size(); i++){
            temp[i] = initial[i] + (rand[i] * (max[i] - min[i]));
        }
        return temp;
    }

    // fungsi yang akan mengenerate dara random natara 0 dan 1
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

    // fungsi yang akan menghasilkna data integer radnom berdasarkan min dan max data
    vector<int> randomData(int size, int min, int max){
        vector<int> temp;
        for(int i = 0; i < size; i++){
            uniform_int_distribution<int> dit(min,max);

            temp.push_back(dit(gen));
        }
        return temp;
    }

    // fungsi untuk mendapatkan nilai minimun dari setiap data awal para siswa
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

    //fungsi untuk mendapatkan nilai maximum dari setiap data awal para siswa
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

    // fungsi untuk mendapatkan nilai rata rata dari para siswa
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

    //fungsi untuk mendapatkan nilai nilai awal dari sang siswa terbaik
    vector<double> bestData(const vector<Siswa>& para_siswa){
        vector<double> temparray;
        for(auto student : para_siswa){
            if(student.status == 0){
                temparray = student.bestpoint;
            }
        }
        return temparray;
    }

    // menampilkan hasil pelajaran dari sang siswa
    void report(){
        cout << "Murid : " << this->id << endl
             << "Score : " << this->prefpoint << endl << endl;
        for(int i = 0; i < bestroute.size(); i++){
            vector<int> temp = bestroute[i].routing;
            cout << "=================================== >> Kendaraan " << i+1 << " << =========================================" << endl;
            cout << "Route : ";
            for(int j = 0; j < temp.size() - 1 ; j++){
                cout << temp[j] << " - ";
            }
            if(temp[temp.back()] == 0){
                cout << 0 << endl;
            }else{
                cout << temp[temp.size()-1] << " - 0" << endl;
            }
            cout << "permintaan : " << bestroute[i].caps << endl;
            cout << "waktu : " << bestroute[i].waktu << endl;
        }
    }
};


// class alghoritma dimana semuanya akan dijalnkan disini
// class ini berisi kumpulan siswa dan permasalahannya, siswa akan mencoba mandiri dan mengevaluasi diri berdasarkan hasilnya terhadap siswa lain
class Alghorithm {
public:
    int num_of_siswa = 0 ;
    int iteration = 0;
    int mapel = 0;
    int kapasitas = 60;
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

    //fungsi untuk menerima input dari user untuk karakteristik dari alghoritma 
    void Input(){
        while (num_of_siswa < 1){
            cout << "Inputkan Jumlah Siswa : " ;
            if (!(cin >> num_of_siswa)) {
                cout << "Invalid input. Tolong masukkan angka." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        while (iteration < 1){
            cout << "Inputkan Jumlah Iteration : " ;
            if (!(cin >> iteration)) {
            cout << "Invalid input. Tolong masukkan angka." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        }
        while (mapel < 1){
            cout << "Inputkan Jumlah Mata Pelajaran : " ;
            if (!(cin >> mapel)) {
                cout << "Invalid input. Tolong masukkan angka." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        
    }

    // fungsi yang akan mengiterasi jalannya alghortima
    void Iterate(){
        cout << endl;
        for(int h=0; h<iteration; h++){
            cout << "===============================================================================================" << endl;
            cout << "                                 ITERASI KE - " << h+1 << endl;
            cout << "                                Memulai ITERASI" << endl;
            cout << "===============================================================================================" << endl;
            cout << "Performa Awal para Siswa :" << endl;
            for(int j=0; j<num_of_siswa; j++){
                para_siswa[j].ShowInitial();
                para_siswa[j].GetRoute();
                para_siswa[j].ShowRoute();
            }

            if(h == 0){
                for(int j=0; j<num_of_siswa; j++){
                    para_siswa[j].TryRoute(Jarak);
                    para_siswa[j].Assess();
                    para_siswa[j].bestpoint = para_siswa[j].initial;
                    para_siswa[j].prefpoint = para_siswa[j].point;
                    para_siswa[j].bestroute = para_siswa[j].vechicle;
                }
            }

            for(int i=0; i<mapel; i++){
                cout << "-----------------------------------------------------------------------------------------------" << endl;
                cout << "                              Mata Pelajaran ke - " << i+1 << endl;
                cout << "-----------------------------------------------------------------------------------------------" << endl << endl;
                cout << " ->> Memulai Pelajaran :" << endl;
                for(int j=0; j<num_of_siswa; j++){
                    para_siswa[j].ShowInitial();
                    para_siswa[j].GetRoute();
                    para_siswa[j].ShowRoute();
                    para_siswa[j].TryRoute(Jarak);
                    para_siswa[j].Assess();
                    cout << "score : " << para_siswa[j].point << endl;
                }
                cout << endl <<" ->> Hasil Pelajaran :" << endl << endl;
                for (int j = 0; j < num_of_siswa; j++){
                    if(para_siswa[j].point < para_siswa[j].prefpoint){
                        cout << "Murid " << para_siswa[j].id << endl;
                        cout << "score lama: " << para_siswa[j].prefpoint << endl;
                        para_siswa[j].bestpoint = para_siswa[j].initial;
                        para_siswa[j].prefpoint = para_siswa[j].point;
                        para_siswa[j].bestroute = para_siswa[j].vechicle;
                        cout << "score baru: " << para_siswa[j].prefpoint << endl;
                        cout << "Update Score diterima" << endl << endl;
                    }else{
                        cout << "Murid " << para_siswa[j].id << endl;
                        cout << "score lama: " << para_siswa[j].prefpoint << endl;
                        cout << "score baru: " << para_siswa[j].point << endl;
                        cout << "Update Score ditolak" << endl << endl;
                    }
                }
                TempBestStudent();
                PrepareNext();
                BestStudent();
                if(h == iteration - 1 && i == mapel-1){
                    continue;
                }
                for (int j = 0; j < num_of_siswa; j++)
                {
                    para_siswa[j].Evaluate(para_siswa);
                    para_siswa[j].CleanUp();
                }
            }
        }
    }

    //fungsi untuk menentukan murid terbaik dan yang akan mendapatkan label 0
    void BestStudent(){
        int minIndex = 0;
        int minvalue = para_siswa[0].prefpoint;
        for(int i =0;i< num_of_siswa; i++){
            if(para_siswa[i].prefpoint < minvalue){
                minIndex = i;
                minvalue = para_siswa[i].prefpoint;
            }
        }
        para_siswa[minIndex].status = 0; //BEST
    }

    // fungsi untuk menentukan murid terbaik sementara
    void TempBestStudent(){
        int minIndex = 0;
        int minvalue = para_siswa[0].prefpoint;
        for(int i =0;i< num_of_siswa; i++){
            if(para_siswa[i].prefpoint < minvalue){
                minIndex = i;
                minvalue = para_siswa[i].prefpoint;
            }
        }
        
        cout << "Siswa Terbaik sementara : " << minIndex + 1 << endl;
        cout << "score : " << minvalue << endl << endl;
    }

    //fungsi untuk menyiapkan iterasi selanjutnya dimana setiap murid diberikan label 1, 2 atau 3
    void PrepareNext(){
        for (int i = 0; i < num_of_siswa; i++) {
            uniform_int_distribution<int> dit(1,3);
            int rand = dit(gen);
            para_siswa[i].status = rand;
        }
    }

    //fungsi terakhir yang akan dipanggil yang akan menyimpulkan hasil dari alghoritma
    void Summarize(){
        Siswa BestStudent;
        for(int i = 0; i < num_of_siswa; i++){
            if(para_siswa[i].status == 0){
                BestStudent = para_siswa[i];
            }
        }
        cout << "===============================================================================================" << endl;
        cout << "                                  BEST CASE STUDENT " << endl;
        cout << "===============================================================================================" << endl;
        BestStudent.report();
        cout << endl << "============================== Perhitungan Selesai ============================================" << endl;
    }

    // fungsi untuk memulai alghoritma
    void Start(){
        Input();
        set_siswa();
        Iterate();
        Summarize();
    }
};

// fungsi untuk membaca filecsv yang akan diubah menjadi kumpulan data jarak
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

// Function to print initial data (Opsional) nampaknya bisa jadi tidak penting
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

// fungsi utama, dimana semuanya akan dimulai
int main() {
    // inisialisasi data yang akan digunakan menggunakan fungsi untuk membaca file csv
    int num = 0;
    string csv_file = "sample.csv";
    string datakecil = "Data Kecil.csv";
    string datasedang = "Data Sedang.csv";
    string databesar = "Data Besar.csv";
    vector<TargetData> csv = readDataFromCSV(csv_file);
    vector<TargetData> kecil = readDataFromCSV(datakecil);
    vector<TargetData> sedang = readDataFromCSV(datasedang);
    vector<TargetData> besar = readDataFromCSV(databesar);

    vector<TargetData> targets;

    //input dari user untk data yang akan digunakan
    cout << "Pilih file yang akan digunakan : " << endl 
         << "1. Data Kecil" << endl 
         << "2. Data Sedang" << endl 
         << "3. Data Besar" << endl
         << endl;
    while ( (num != 1 && num != 2) && num != 3 ){
        cout << "Pilih dataset yang mau dipakai : ";
        if (!(cin >> num)) {
            cout << "Invalid input. Tolong masukkan angka." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    //switch menentukan data mana yang akan digunakan
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

    //inisialisai class DataJarak dengan kumpulan data yang sudah dibaca
    //inisialisasi alghoritma dengan objek DataJarak yang telah ada
    DataJarak dataJarak(targets);
    Alghorithm sistem(dataJarak);

    //memulai alghoritma
    sistem.Start();

    return 0;
}
