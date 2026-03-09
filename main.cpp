// main.cpp
#include "io.h"
#include "processing.h"

int main() {
    // 1. Đọc dữ liệu (Giao Thành viên C/A viết trong io.cpp)
    vector<Station> database = readStationsFromCSV("stations.csv");

    for (Station &s : database) {
        // 2. Tiền xử lý
        normalizeStationData(s);
        calculateStationStats(s);

        // 3. Phân tích thuật toán
        vector<Record> hotDays = findMaxTempSegment(s);
        vector<Record> rainTrend = findLongestRainTrend(s);
        
        // 4. Xuất kết quả
        writeStationReport("station_report.csv", s);
        writeAnomalyReport("anomaly.txt", s, hotDays, rainTrend);
    }
    return 0;
}