# TÀI LIỆU THIẾT KẾ BÀI TẬP LỚN
**Môn học**: Kỹ thuật lập trình (2025-2026) 
**Đề tài**: BTL01 - Hệ thống phân tích dữ liệu thời tiết nhiều trạm đo

## 1. Phân tích bài toán (System Analysis)
Phương pháp thiết kế :

- Dữ liệu đầu vào: File stations.csv chứa thông tin trạm (ID, tên) và các bản ghi thời tiết (ngày, nhiệt độ, độ ẩm, lượng mưa). Có xử lý dữ liệu thiếu (NA).
- Dữ liệu đầu ra: File station_report.csv (thống kê tổng hợp) và anomaly.txt (danh sách các đoạn ngày bất thường).
- Quy trình xử lý (Pipeline): Đọc dữ liệu $\rightarrow$ Tiền xử lý & Validate $\rightarrow$ Thống kê mô tả $\rightarrow$ Phân tích dãy con (Kadane, LIS) $\rightarrow$ Xuất báo cáo.

## 2. Thiết kế cấu trúc dữ liệu (Data Structure)
Hệ thống sử dụng cấu trúc Struct lồng nhau cấp 2 để quản lý dữ liệu hiệu quả:

- `struct Record`: Lưu trữ thông tin chi tiết của một lần đo bao gồm ngày, các chỉ số (nhiệt độ, độ ẩm, lượng mưa) và trạng thái hợp lệ.
- `struct Stats`: Lưu trữ các chỉ số thống kê (mean, std, min, max) sau khi tính toán.
- `struct Station`: Cấp quản lý ngoài cùng, chứa thông tin trạm và một `vector<Record>`.

## 3. Sơ đồ phân cấp hàm (Functional Hierarchy)
Chương trình được tổ chức thành 3 module độc lập theo yêu cầu:

- Module IO (`io.h/.cpp`): Đảm nhận việc đọc/ghi file.
- Module Processing (`processing.h/.cpp`): Đảm nhận logic tính toán và thuật toán.
- Module Main (`main.cpp`): Điều phối luồng hoạt động chính.

## 4. Mô tả chi tiết hàm (Function Specification)
Dưới đây là mô tả chi tiết cho các hàm quan trọng trong hệ thống:

| Tên hàm | Tham số (Input) | Kết quả (Output) | Mô tả nhiệm vụ |
|--------|----------------|------------------|---------------|
| `readStationsFromCSV` | `string filename` | `vector<Station>` | Đọc file, xử lý `NA`, nhóm `Record` vào đúng `Station` |
| `calculateStationStats` | `Station &s` | `void` | Tính mean, std, min, max và chuẩn hoá score nhiệt độ |
| `findMaxTempSegment` | `const Station &s` | `vector<Record>` | Áp dụng **Kadane** tìm đoạn tổng nhiệt độ lớn nhất |
| `findLongestRainyTrend` | `const Station &s` | `vector<Record>` | Áp dụng **LIS** tìm chuỗi ngày lượng mưa tăng liên tiếp |
| `buildRainPrefixSum` | `const Station &s` | `vector<double>` | Xây dựng mảng Prefix sum cho lượng mưa. |
| `writeStationReport` | `string, Station` | `void` | Ghi dữ liệu thống kê ra file CSV. |

## 5. Phương pháp thuật toán (Algorithm Methodology)
Nhóm áp dụng các kỹ thuật xử lý mảng/dãy con tối ưu:

- **Thuật toán Kadane**: Tìm đoạn ngày có tổng nhiệt độ cao nhất với độ phức tạp $O(n)$.
- **LIS (Longest Increasing Subsequence)**: Phát hiện chuỗi ngày có xu hướng lượng mưa tăng dần liên tiếp.
- **Prefix Sum 1D**: Cho phép truy vấn tổng lượng mưa trong bất kỳ khoảng thời gian $[l, r]$ nào với độ phức tạp $O(1)$ sau khi khởi tạo.
- **Chuẩn hoá Z-score**: Sử dụng công thức $z = \frac{x - \mu}{\sigma}$ để đánh giá mức độ bất thường của nhiệt độ.

## 6. Kế hoạch kiểm thử (Testing Plan)
Hệ thống sẽ thực hiện kiểm thử với 4 bộ dữ liệu:

| Mã Test | File Test | Trạm | Nội dung | Mục đích |
|---------|-----------|------|---------|----------|
| TC1 | `test_hanoi_normal.csv` | Tram_Ha_Noi | 30 ngày (01/01-29/01), dữ liệu đầy đủ không NA, nhiệt độ 15.4-39.6°C, mưa 1.2-19.6mm | Kiểm thử dữ liệu thường, toàn bộ hàm xử lý |
| TC2 | `test_sapa_edge_cases.csv` | Tram_Sapa | 3 ngày: ngày 1 toàn NA, ngày 2 có NA humidity, ngày 3 có NA humidity | Kiểm thử xử lý dữ liệu thiếu (NA handling), trạm biên (edge cases) |
| TC3 | `test_hue_lis.csv` | Tram_Hue | 10 ngày, nhiệt độ cố định 25°C, lượng mưa tăng dần (5.0→27.5mm), có 1 NA | Kiểm thử thuật toán **LIS** (Longest Increasing Subsequence) với xu hướng tăng rõ ràng |
| TC4 | `test_dalat_stress.csv` | Tram_Da_Lat | 30 ngày, lượng mưa cao (0.8-48.4mm), nhiệt độ 10.4-27.4°C | Kiểm thử **Kadane** và xử lý lớn, phát hiện bất thường nhiệt độ (stress test) |

**Kỳ vọng Output**: 
- Ghi lại `station_report.csv` với các chỉ số thống kê (mean, std, min, max).
- Ghi lại `anomaly.txt` chứa danh sách các đoạn bất thường theo từng trạm.