// main.cpp
#include "io.h"
#include "processing.h"
#include <fstream>
#include <iostream>

using namespace std;

int main() {
    // 1. Đọc dữ liệu
    vector<Station> database = readStationsFromCSV("stations.csv");

    // Mở file một lần duy nhất, chế độ ghi đè (tự động làm mới file mỗi lần chạy)
    ofstream reportFile("station_report.csv");
    if (!reportFile.is_open()) {
        cerr << "Loi: Khong the mo file station_report.csv" << "\n";
        return 1;
    }
    // Ghi header cho file CSV (Tùy chọn giúp file chuyên nghiệp hơn)
    reportFile << "StationID,StationName,MeanTemp,MaxTemp\n";

    ofstream anomalyFile("anomaly.txt");
    if (!anomalyFile.is_open()) {
        cerr << "Loi: Khong the mo file anomaly.txt" << "\n";
        return 1;
    }

    for (Station &s : database) {
        // 2. Tiền xử lý
        normalizeStationData(s);
        calculateStationStats(s);

        // 3. Phân tích thuật toán
        vector<Record> hotDays = findMaxTempSegment(s);
        vector<Record> rainTrend = findLongestRainTrend(s);
        
        // 4. Xuất kết quả qua luồng file
        writeStationReport(reportFile, s);
        writeAnomalyReport(anomalyFile, s, hotDays, rainTrend);
    }

    // Đóng file sau khi đã xử lý xong toàn bộ các trạm
    reportFile.close();
    anomalyFile.close();

    return 0;
}