# TÀI LIỆU THIẾT KẾ BÀI TẬP LỚN
**Môn học**: Kỹ thuật lập trình (2025-2026) 
**Đề tài**: BTL01 - Hệ thống phân tích dữ liệu thời tiết nhiều trạm đo

## 1. Phân tích bài toán (System Analysis)
Phương pháp thiết kế:

- Dữ liệu đầu vào: File `stations.csv` chứa thông tin trạm (ID, tên) và các bản ghi thời tiết (ngày, nhiệt độ, độ ẩm, lượng mưa). Hệ thống có xử lý giá trị thiếu `NA` bằng cách chuyển sang `NAN` để bỏ qua khi tính toán.
- Dữ liệu đầu ra:
  - `station_report.csv`: báo cáo tóm tắt theo trạm.
  - `anomaly.txt`: danh sách các đoạn ngày nổi bật theo Kadane và LIS.
- Quy trình xử lý (Pipeline): Đọc dữ liệu $\rightarrow$ Tiền xử lý & chuẩn hoá $\rightarrow$ Thống kê mô tả $\rightarrow$ Phân tích dãy con (Kadane, LIS) $\rightarrow$ Xuất báo cáo.

## 2. Thiết kế cấu trúc dữ liệu (Data Structure)
Hệ thống sử dụng cấu trúc Struct lồng nhau cấp 2 để quản lý dữ liệu hiệu quả:

- `struct Record`: Lưu trữ thông tin chi tiết của một lần đo bao gồm ngày, các chỉ số (nhiệt độ, độ ẩm, lượng mưa) và cờ `valid`.
- `struct Stats`: Lưu trữ các chỉ số thống kê gồm `meanTemp`, `stdTemp`, `meanRain`, `stdRain`, `minVal`, `maxVal`.
- `struct Station`: Cấp quản lý ngoài cùng, chứa thông tin trạm, một `vector<Record>` và `Stats`.

## 3. Sơ đồ phân cấp hàm (Functional Hierarchy)
Chương trình được tổ chức thành 3 module độc lập theo yêu cầu:

- Module IO (`io.h/.cpp`): Đảm nhận việc đọc/ghi file.
- Module Processing (`processing.h/.cpp`): Đảm nhận logic tính toán và thuật toán.
- Module Main (`main.cpp`): Điều phối luồng hoạt động chính.

## 4. Mô tả chi tiết hàm (Function Specification)
Dưới đây là mô tả chi tiết cho các hàm quan trọng trong hệ thống:

| Tên hàm | Tham số (Input) | Kết quả (Output) | Mô tả nhiệm vụ |
|--------|----------------|------------------|---------------|
| `readStationsFromCSV` | `const string &filename` | `vector<Station>` | Đọc file CSV, xử lý `NA`, nhóm `Record` vào đúng `Station` |
| `normalizeStationData` | `Station &s` | `void` | Chuẩn hoá dữ liệu chuỗi của trạm, hiện dùng để trim tên trạm |
| `calculateStationStats` | `Station &s` | `void` | Tính `meanTemp`, `stdTemp`, `meanRain`, `stdRain`, `minVal`, `maxVal` |
| `findMaxTempSegment` | `const Station &s` | `vector<Record>` | Áp dụng **Kadane** tìm đoạn có tổng nhiệt độ lớn nhất |
| `findLongestRainTrend` | `const Station &s` | `vector<Record>` | Áp dụng tư duy **LIS trên chuỗi tăng liên tiếp** để tìm xu hướng mưa tăng |
| `computePrefixSum` | `const vector<double> &data` | `vector<double>` | Xây dựng mảng cộng dồn để hỗ trợ truy vấn tổng đoạn trong `O(1)` |
| `classifyRainLevel` | `double rainAmount` | `string` | Phân loại lượng mưa thành `No Rain`, `Light`, `Moderate`, `Heavy` |
| `writeStationReport` | `ofstream &file, const Station &s` | `void` | Ghi báo cáo CSV theo trạm |
| `writeAnomalyReport` | `ofstream &file, const Station &s, const vector<Record>&, const vector<Record>&` | `void` | Ghi các đoạn nổi bật theo Kadane và LIS ra file TXT |

## 5. Phương pháp thuật toán (Algorithm Methodology)
Nhóm áp dụng các kỹ thuật xử lý mảng/dãy con tối ưu:

- **Thuật toán Kadane**: Tìm đoạn ngày có tổng nhiệt độ cao nhất với độ phức tạp $O(n)$.
- **Chuỗi tăng liên tiếp theo tư duy LIS**: Phát hiện chuỗi ngày có xu hướng lượng mưa tăng dần liên tiếp.
- **Prefix Sum 1D**: Được cài đặt sẵn để hỗ trợ truy vấn tổng trên đoạn dữ liệu trong $O(1)$ sau khi khởi tạo, nhưng hiện chưa được gọi trong luồng xử lý chính.
- **Thống kê mô tả**: Hệ thống tính `mean`, `std`, `min`, `max` cho nhiệt độ và lượng mưa để phục vụ phân tích.

## 6. Kế hoạch kiểm thử (Testing Plan)
Hệ thống sẽ thực hiện kiểm thử với 4 bộ dữ liệu:

| Mã Test | File Test | Trạm | Nội dung | Mục đích |
|---------|-----------|------|---------|----------|
| TC1 | `test_hanoi_normal.csv` | Tram_Ha_Noi | 30 ngày (01/01-29/01), dữ liệu đầy đủ không NA, nhiệt độ 15.4-39.6°C, mưa 1.2-19.6mm | Kiểm thử dữ liệu thường, toàn bộ hàm xử lý |
| TC2 | `test_sapa_edge_cases.csv` | Tram_Sapa | 3 ngày: ngày 1 toàn NA, ngày 2 có NA humidity, ngày 3 có NA humidity | Kiểm thử xử lý dữ liệu thiếu (NA handling), trạm biên (edge cases) |
| TC3 | `test_hue_lis.csv` | Tram_Hue | 10 ngày, nhiệt độ cố định 25°C, lượng mưa tăng dần (5.0→27.5mm), có 1 NA | Kiểm thử thuật toán **LIS** (Longest Increasing Subsequence) với xu hướng tăng rõ ràng |
| TC4 | `test_dalat_stress.csv` | Tram_Da_Lat | 30 ngày, lượng mưa cao (0.8-48.4mm), nhiệt độ 10.4-27.4°C | Kiểm thử **Kadane** và xử lý lớn, phát hiện bất thường nhiệt độ (stress test) |

**Kỳ vọng Output**:
- Ghi lại `station_report.csv` với các cột hiện tại: `StationID`, `StationName`, `MeanTemp`, `MaxTemp`.
- Ghi lại `anomaly.txt` chứa:
  - đoạn nhiệt độ cao nhất theo Kadane,
  - chuỗi ngày lượng mưa tăng liên tiếp theo LIS,
  cho từng trạm.
