#include "io.h"

using namespace std;

string trim(const string& s) {
    size_t first = s.find_first_not_of(" \t\r\n");
    if (string::npos == first) return "";
    size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, (last - first + 1));
}

vector<Station> readStationsFromCSV(const string &filename) {
    vector<Station> database;
    ifstream file(filename);
    string line, word;

    if (!file.is_open()){
        cerr << "Error: Cannot open file " << filename << endl;
        return database;
    } 

    getline(file, line); // Bỏ qua dòng header 

    while (getline(file, line)) {
        // Loại bỏ ký tự \r ở cuối dòng (nếu có) để tránh lỗi định dạng chuỗi
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        stringstream ss(line);
        string st_id, name, date, temp_str, hum_str, rain_str;

        // Cấu trúc: station_id, name, date, temp, humidity, rain 
        getline(ss, st_id, ',');
        getline(ss, name, ',');
        getline(ss, date, ',');
        getline(ss, temp_str, ',');
        getline(ss, hum_str, ',');
        getline(ss, rain_str, ',');

        st_id = trim(st_id);
        name = trim(name);
        temp_str = trim(temp_str);
        hum_str = trim(hum_str);
        rain_str = trim(rain_str);

        Record rec;
        rec.date = date;
        rec.valid = true;
        
        rec.temp = (temp_str == "NA" || temp_str.empty()) ? NAN : stod(temp_str);
        rec.humidity = (hum_str == "NA" || hum_str.empty()) ? NAN : stod(hum_str);
        rec.rain = (rain_str == "NA" || rain_str.empty()) ? NAN : stod(rain_str);

        bool found = false;

        for (size_t i = 0; i < database.size(); i++) {
            if (database[i].id == st_id) {
                database[i].records.push_back(rec);
                found = true;
                break;
            }
        }

        if (!found) {
            Station new_st;
            new_st.id = st_id;
            new_st.name = name;

            // Thêm record đầu tiên cho trạm
            new_st.records.push_back(rec);

            // Thêm trạm mới vào database
            database.push_back(new_st);
        }
    }
    file.close();
    return database;
}

void writeStationReport(ofstream &file, const Station &s) {
    file << s.id << "," 
         << s.name << "," 
         << s.stats.meanTemp << "," 
         << s.stats.maxVal << "\n";
}

void writeAnomalyReport(ofstream &file, const Station &s, 
                        const vector<Record> &hotDays, 
                        const vector<Record> &rainTrend) {
    // Ghi trực tiếp vào file đã được mở
    file << "--------------------------------------------------\n";
    file << "TRAM: " << s.name << " (ID: " << s.id << ")\n";

    file << "[Kadane] Doan nhiet do cao nhat:\n";
    for (const auto& r : hotDays) {
        file << "  " << r.date << ": " << r.temp << " C\n";
    }

    file << "[LIS] Chuoi ngay luong mua tang lien tiep:\n";
    for (const auto& r : rainTrend) {
        file << "  " << r.date << ": " << r.rain << " mm (" << classifyRainLevel(r.rain) << ")\n";
    }
    file << "--------------------------------------------------\n\n";
}