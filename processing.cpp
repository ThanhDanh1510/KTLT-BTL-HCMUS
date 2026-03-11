#include "processing.h"
#include <algorithm>
#include <numeric>
#include <cmath>

// Task 1

double runKadane(const vector<double>& data, int& start, int& end){
    if (data.empty()) return 0;
    double max_so_far = data[0], max_ending_here = 0;
    int s = 0;
    for (int i = 0; i < data.size(); i++) {
        max_ending_here += data[i];
        if (max_so_far < max_ending_here) {
            max_so_far = max_ending_here;
            start = s;
            end = i;
        }
        if (max_ending_here < 0) {
            max_ending_here = 0;
            s = i + 1;
        }
    }
    return max_so_far;
}

int runLIS(const vector<double>& data, int& start, int& end){
    int n = data.size();
    if (n == 0) return 0;
    int maxL = 1, currentL = 1;
    int tempStart = 0;
    start = 0; end = 0;

    for (int i = 1; i < n; i++) {
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
    for (int i = 0; i < data.size(); i++) {
        prefix[i + 1] = prefix[i] + data[i];
    }
    return prefix;
}

// Task 2

void calculateStationStats(Station &s){
    if (s.records.empty()) return;

    double sumTemp = 0, sumRain = 0;
    int count = 0;
    bool first = true;

    for (const auto& r : s.records) {
        if (!r.valid) continue;
        if (first) {
            s.stats.minVal = r.temp;
            s.stats.maxVal = r.temp;
            first = false;
        }
        sumTemp += r.temp;
        sumRain += r.rain;
        if (r.temp < s.stats.minVal) s.stats.minVal = r.temp;
        if (r.temp > s.stats.maxVal) s.stats.maxVal = r.temp;
        count++;
    }

    if (count > 0) {
        s.stats.meanTemp = sumTemp / count;
        s.stats.meanRain = sumRain / count;
        double varTemp = 0, varRain = 0;
        for (const auto& r : s.records) {
            if (r.valid) {
                varTemp += pow(r.temp - s.stats.meanTemp, 2);
                varRain += pow(r.rain - s.stats.meanRain, 2);
            }
        }
        s.stats.stdTemp = sqrt(varTemp / count);
        s.stats.stdRain = sqrt(varRain / count);
    }
}

void normalizeStationData(Station &s){
    if (s.name.empty()) return;
    s.name.erase(0, s.name.find_first_not_of(" "));
    s.name.erase(s.name.find_last_not_of(" ") + 1);
}

string classifyRainLevel(double rainAmount){
    if (rainAmount <= 0) return "No Rain";
    if (rainAmount < 5) return "Light";
    if (rainAmount < 50) return "Moderate";
    return "Heavy";
}

// Task 3

vector<Record> findLongestRainTrend(const Station &s);

vector<Record> findMaxTempSegment(const Station &s);

vector<Record> findMaxTempSegment(const Station &s) {
    vector<double> temps;
    
    // Trích xuất mảng nhiệt độ
    for (const auto& r : s.records) {
        if (r.valid) {
            temps.push_back(r.temp);
        } else {
            temps.push_back(NAN);
        }
    }

    int start = 0, end = -1;
    runKadane(temps, start, end);

    vector<Record> result;
    if (start <= end && start >= 0 && end < s.records.size()) {
        for (int i = start; i <= end; i++) {
            result.push_back(s.records[i]);
        }
    }
    return result;
}

vector<Record> findLongestRainTrend(const Station &s) {
    vector<double> rains;
    
    // Trích xuất mảng lượng mưa
    for (const auto& r : s.records) {
        if (r.valid) {
            rains.push_back(r.rain);
        } else {
            rains.push_back(NAN);
        }
    }

    int start = 0, end = -1;
    runLIS(rains, start, end);

    vector<Record> result;
    if (start <= end && start >= 0 && end < s.records.size()) {
        for (int i = start; i <= end; i++) {
            result.push_back(s.records[i]);
        }
    }
    return result;
}