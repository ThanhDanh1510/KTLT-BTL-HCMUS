#include "processing.h"
#include <algorithm>
#include <numeric>
#include <cmath>

// Task 1

double runKadane(const vector<double>& data, int& start, int& end);

int runLIS(const vector<double>& data, int& start, int& end);

vector<double> computePrefixSum(const vector<double>& data);

// Task 2

void calculateStationStats(Station &s);

void normalizeStationData(Station &s);

string classifyRainLevel(double rainAmount);

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