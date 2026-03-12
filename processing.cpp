#include "processing.h"
#include <algorithm>
#include <numeric>
#include <cmath>

using namespace std;

// Task 1

double runKadane(const vector<double>& data, int& start, int& end) {
    if (data.empty()) return 0;

    double max_so_far = data[0];
    double max_ending_here = data[0];
    int temp_start = 0;
    
    start = 0;
    end = 0;

    // Sửa cảnh báo type mismatch: int -> size_t
    for (size_t i = 1; i < data.size(); i++) {
        if (data[i] > max_ending_here + data[i]) {
            max_ending_here = data[i];
            temp_start = i;
        } else {
            max_ending_here += data[i];
        }

        if (max_ending_here > max_so_far) {
            max_so_far = max_ending_here;
            start = temp_start;
            end = i;
        }
    }
    return max_so_far;
}

int runLIS(const vector<double>& data, int& start, int& end){
    size_t n = data.size(); // Sửa int -> size_t
    if (n == 0) return 0;
    int maxL = 1, currentL = 1;
    int tempStart = 0;
    start = 0; end = 0;

    for (size_t i = 1; i < n; i++) {
        if (data[i] > data[i - 1]) {
            currentL++;
            if (currentL > maxL) {
                maxL = currentL;
                start = tempStart;
                end = i;
            }
        } else {
            currentL = 1;
            tempStart = i;
        }
    }
    return maxL;
}

vector<double> computePrefixSum(const vector<double>& data){
    vector<double> prefix(data.size() + 1, 0);
    // Sửa cảnh báo type mismatch: int -> size_t
    for (size_t i = 0; i < data.size(); i++) {
        prefix[i + 1] = prefix[i] + data[i];
    }
    return prefix;
}

// Task 2

void calculateStationStats(Station &s){
    if (s.records.empty()) return;

    double sumTemp = 0, sumRain = 0;
    int countTemp = 0, countRain = 0; // Tách riêng biến đếm để tính toán độc lập
    bool firstTemp = true;

    for (const auto& r : s.records) {
        // Chỉ tính toán nhiệt độ nếu giá trị không bị NAN
        if (!isnan(r.temp)) {
            if (firstTemp) {
                s.stats.minVal = r.temp;
                s.stats.maxVal = r.temp;
                firstTemp = false;
            }
            sumTemp += r.temp;
            if (r.temp < s.stats.minVal) s.stats.minVal = r.temp;
            if (r.temp > s.stats.maxVal) s.stats.maxVal = r.temp;
            countTemp++;
        }
        
        // Chỉ tính toán lượng mưa nếu giá trị không bị NAN
        if (!isnan(r.rain)) {
            sumRain += r.rain;
            countRain++;
        }
    }

    // Tính trung bình an toàn (tránh chia cho 0)
    s.stats.meanTemp = (countTemp > 0) ? (sumTemp / countTemp) : 0;
    s.stats.meanRain = (countRain > 0) ? (sumRain / countRain) : 0;

    double varTemp = 0, varRain = 0;
    for (const auto& r : s.records) {
        if (!isnan(r.temp) && countTemp > 0) {
            varTemp += pow(r.temp - s.stats.meanTemp, 2);
        }
        if (!isnan(r.rain) && countRain > 0) {
            varRain += pow(r.rain - s.stats.meanRain, 2);
        }
    }
    
    // Tính độ lệch chuẩn an toàn
    s.stats.stdTemp = (countTemp > 0) ? sqrt(varTemp / countTemp) : 0;
    s.stats.stdRain = (countRain > 0) ? sqrt(varRain / countRain) : 0;
}

void normalizeStationData(Station &s){
    if (s.name.empty()) return;
    size_t first = s.name.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        s.name = "";
        return;
    }
    size_t last = s.name.find_last_not_of(" \t\r\n");
    s.name = s.name.substr(first, (last - first + 1));
}

string classifyRainLevel(double rainAmount){
    // Thêm check an toàn nếu dữ liệu đầu vào vô tình là NAN
    if (isnan(rainAmount) || rainAmount <= 0) return "No Rain";
    if (rainAmount < 5) return "Light";
    if (rainAmount < 50) return "Moderate";
    return "Heavy";
}

// Task 3
// Đã xóa bỏ phần khai báo nguyên mẫu hàm (prototype) thừa thãi ở đây

vector<Record> findMaxTempSegment(const Station &s) {
    vector<double> temps;
    vector<int> valid_indices; 
    
    // Trích xuất mảng nhiệt độ, dùng isnan để bảo vệ thuật toán
    for (size_t i = 0; i < s.records.size(); i++) {
        if (!isnan(s.records[i].temp)) { 
            temps.push_back(s.records[i].temp);
            valid_indices.push_back(i);
        }
    }

    int start = 0, end = -1;
    runKadane(temps, start, end);

    vector<Record> result;
    if (start <= end && start >= 0 && end < temps.size()) {
        for (int i = start; i <= end; i++) {
            result.push_back(s.records[valid_indices[i]]);
        }
    }
    return result;
}

vector<Record> findLongestRainTrend(const Station &s) {
    vector<double> rains;
    vector<int> valid_indices; 
    
    // Trích xuất mảng lượng mưa, dùng isnan để bảo vệ thuật toán
    for (size_t i = 0; i < s.records.size(); i++) {
        if (!isnan(s.records[i].rain)) { 
            rains.push_back(s.records[i].rain);
            valid_indices.push_back(i);
        }
    }

    int start = 0, end = -1;
    runLIS(rains, start, end);

    vector<Record> result;
    if (start <= end && start >= 0 && end < rains.size()) {
        for (int i = start; i <= end; i++) {
            result.push_back(s.records[valid_indices[i]]);
        }
    }
    return result;
}