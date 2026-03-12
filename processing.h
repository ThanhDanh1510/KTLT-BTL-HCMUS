#ifndef PROCESSING_H
#define PROCESSING_H

#include <string>
#include <vector>
#include <cmath>

struct Record {
    std::string date;         // Ngày đo
    double temp;         // Nhiệt độ
    double humidity;     // Độ ẩm
    double rain;         // Lượng mưa
    bool valid;          // Trạng thái dữ liệu 
};

struct Stats {
    double meanTemp, stdTemp; 
    double meanRain, stdRain;
    double minVal, maxVal; 
};

struct Station {
    std::string id;
    std::string name;
    std::vector<Record> records; 
    Stats stats;
};

// =========================================================
// NHÓM 1: CÁC HÀM THUẬT TOÁN ĐỘC LẬP
// =========================================================

/**
 * @brief Thuật toán Kadane tìm tổng lớn nhất của dãy con liên tiếp 
 * @param data Mảng dữ liệu số (ví dụ: nhiệt độ)
 * @param start Biến lưu chỉ số bắt đầu của đoạn con
 * @param end Biến lưu chỉ số kết thúc của đoạn con
 * @return Tổng lớn nhất tìm được
 */
double runKadane(const std::vector<double>& data, int& start, int& end);

/**
 * @brief Tìm chuỗi tăng dài nhất (LIS tư duy) trên mảng liên tiếp 
 * @param data Mảng dữ liệu số (ví dụ: lượng mưa)
 * @return Độ dài chuỗi tăng liên tiếp dài nhất
 */
int runLIS(const std::vector<double>& data, int& start, int& end);

/**
 * @brief Xây dựng mảng Prefix Sum để truy vấn O(1) 
 * @param data Mảng dữ liệu gốc
 * @return Mảng cộng dồn (Prefix Sum array)
 */
std::vector<double> computePrefixSum(const std::vector<double>& data);

// =========================================================
// NHÓM 2: CÁC HÀM THAO TÁC TRÊN STRUCT
// =========================================================

/**
 * @brief Tính toán các chỉ số thống kê và chuẩn hoá Z-score cho trạm 
 * Công thức Z-score: $z = \frac{x - \mu}{\sigma}$
 */
void calculateStationStats(Station &s);

/**
 * @brief Tìm đoạn ngày có tổng nhiệt độ cao nhất (Sử dụng hàm runKadane nội bộ)
 */
std::vector<Record> findMaxTempSegment(const Station &s);

/**
 * @brief Tìm chuỗi ngày lượng mưa tăng dần liên tiếp (Sử dụng hàm runLIS nội bộ)
 */
std::vector<Record> findLongestRainTrend(const Station &s);

/**
 * @brief Chuẩn hoá dữ liệu chuỗi (trim khoảng trắng ở tên),  
 */
void normalizeStationData(Station &s);

/**
 * @brief Phân loại mức mưa theo chuỗi
 * rainAmount <= 0 -> "No Rain", rainAmount < 5 -> "Light",
 * rainAmount < 50 -> "Moderate", rainAmount >= 50 -> "Heavy"
 * @return "No Rain", "Light", "Moderate", "Heavy"
 */
std::string classifyRainLevel(double rainAmount);

#endif