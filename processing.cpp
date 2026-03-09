#include "processing.h"
#include <algorithm>
#include <numeric>

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