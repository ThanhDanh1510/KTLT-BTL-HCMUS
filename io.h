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
std::vector<Station> readStationsFromCSV(const std::string &filename);

/**
 * @brief Ghi báo cáo thống kê định dạng CSV cho từng trạm.
 */
void writeStationReport(std::ofstream &file, const Station &s);

/**
 * @brief Ghi các đoạn dữ liệu bất thường (từ Kadane và LIS) vào file TXT.
 */
void writeAnomalyReport(std::ofstream &file, const Station &s, 
                        const std::vector<Record> &hotDays, 
                        const std::vector<Record> &rainTrend);

#endif