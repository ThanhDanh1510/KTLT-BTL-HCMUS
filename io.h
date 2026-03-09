#ifndef IO_H
#define IO_H

#include "processing.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

/**
 * @brief Đọc dữ liệu từ file stations.csv và nhóm vào vector các Station.
 */
vector<Station> readStationsFromCSV(const string &filename);

/**
 * @brief Ghi báo cáo thống kê định dạng CSV cho từng trạm.
 */
void writeStationReport(const string &filename, const Station &s);

/**
 * @brief Ghi các đoạn dữ liệu bất thường (từ Kadane và LIS) vào file TXT.
 */
void writeAnomalyReport(const string &filename, const Station &s, 
                        const vector<Record> &hotDays, 
                        const vector<Record> &rainTrend);

#endif