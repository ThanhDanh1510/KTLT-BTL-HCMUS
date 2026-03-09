#include "io.h"

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
        stringstream ss(line);
        string st_id, name, date, temp_str, hum_str, rain_str;

        // Cấu trúc: station_id, name, date, temp, humidity, rain 
        getline(ss, st_id, ',');
        getline(ss, name, ',');
        getline(ss, date, ',');
        getline(ss, temp_str, ',');
        getline(ss, hum_str, ',');
        getline(ss, rain_str, ',');

        Record rec;
        rec.date = date;
        // Xử lý dữ liệu thiếu (missing/NA) 
        if (temp_str == "NA" || hum_str == "NA" || rain_str == "NA") {
            rec.valid = false;
            rec.temp = rec.humidity = rec.rain = 0;
        } else {
            rec.valid = true;
            rec.temp = stod(temp_str);
            rec.humidity = stod(hum_str);
            rec.rain = stod(rain_str);
        }

        bool found = false;

        for (int i = 0; i < database.size(); i++) {
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

void writeStationReport(const string &filename, const Station &s) {
    ofstream file(filename, ios::app);

    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return;
    }

    file << s.id << "," 
         << s.name << "," 
         << s.stats.meanTemp << "," 
         << s.stats.maxVal << "\n";

    file.close();
}

void writeAnomalyReport(const string &filename, const Station &s, 
                        const vector<Record> &hotDays, 
                        const vector<Record> &rainTrend) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) return;

    file << "--------------------------------------------------" << endl;
    file << "TRAM: " << s.name << " (ID: " << s.id << ")" << endl;

    file << "[Kadane] Doan nhiet do cao nhat:" << endl;
    for (const auto& r : hotDays) {
        file << "  " << r.date << ": " << r.temp << " C" << endl;
    }

    file << "[LIS] Chuoi ngay luong mua tang lien tiep:" << endl;
    for (const auto& r : rainTrend) {
        file << "  " << r.date << ": " << r.rain << " mm (" << classifyRainLevel(r.rain) << ")" << endl;
    }
    file << "--------------------------------------------------" << endl << endl;

    file.close();
}