# TÀI LIỆU THIẾT KẾ BÀI TẬP LỚN & HƯỚNG DẪN HỆ THỐNG

**Môn học**: Kỹ thuật lập trình (MTH10107)  
**Đề tài**: BTL01 - Hệ thống phân tích dữ liệu thời tiết nhiều trạm đo  
**Sinh viên thực hiện**: Lưu Thành Danh (24280055)  
**Trường**: Đại học Khoa học Tự nhiên - ĐHQG HCM  

---

## 📂 1. Cấu trúc Thư mục Hệ thống (Directory Structure)

Sau quá trình tái cấu trúc (refactoring), dự án được phân chia thành hai phân vùng độc lập cực kỳ sạch sẽ và khoa học:

```verbatim
KTLT BTL HCMUS/
├── cpp/                           # Thư mục mã nguồn C++
│   ├── main.cpp                   # Bộ điều phối luồng xử lý chính
│   ├── io.h                       # Khai báo các nguyên mẫu hàm nhập xuất
│   ├── io.cpp                     # Hiện thực chi tiết hàm đọc/ghi tệp
│   ├── processing.h               # Định nghĩa các Struct dữ liệu (Record, Stats, Station)
│   └── processing.cpp             # Hiện thực thuật toán (Kadane, LIS, Prefix Sum)
│
├── latex/                         # Thư mục tài liệu báo cáo học thuật LaTeX
│   ├── main.tex                   # Tệp cấu hình chính và điều phối liên kết báo cáo
│   ├── setup.tex                  # Nạp gói lệnh, định nghĩa style code C++, tiếng Việt
│   ├── title.tex                  # Trang bìa báo cáo học thuật
│   ├── introduction.tex           # Phần Mở đầu, mục tiêu và phạm vi hệ thống
│   ├── architecture.tex           # Phân tích quy trình Pipeline và mô hình Struct
│   ├── algorithms.tex             # Lý thuyết toán học, mã giả Kadane, LIS, Prefix Sum
│   ├── implementation.tex         # Bảng đặc tả hàm và mã nguồn C++ mẫu
│   ├── testing.tex                # Kịch bản kiểm thử TC1-TC4 và kết quả thực tế
│   ├── conclusion.tex             # Tổng kết ưu khuyết điểm và hướng phát triển tương lai
│   └── figure/                    # Thư mục tự quản lý chứa tài nguyên hình ảnh báo cáo
│       ├── hcmus-logo.png         # Logo trường ĐH Khoa học Tự nhiên
│       ├── workflow.png           # Sơ đồ Pipeline xử lý dữ liệu
│       ├── Weather_station_data_model.png # Sơ đồ mô hình Struct lồng nhau cấp 2
│       └── composite.png          # Biểu đồ phân tích tổng hợp
│
├── .gitignore                     # Cấu hình loại bỏ rác biên dịch C++ và LaTeX
├── stations.csv                   # Cơ sở dữ liệu khí tượng thời tiết thô đầu vào
├── [Bộ dữ liệu test].csv          # Các tệp test: test_hanoi_normal.csv, ...
├── generate_test_data.py          # Script sinh dữ liệu kiểm thử ngẫu nhiên
├── main.exe                       # Tệp thực thi C++ sau khi biên dịch
├── station_report.csv             # Báo cáo thống kê trạm kết xuất thực tế
└── anomaly.txt                    # Danh sách bất thường nhiệt độ & lượng mưa kết xuất
```

---

## 🛠️ 2. Hướng dẫn Biên dịch & Chạy Chương trình

### A. Đối với Mã nguồn C++
Chương trình được biên dịch từ thư mục gốc thông qua trình biên dịch `g++` hỗ trợ chuẩn C++11 trở lên:

```powershell
# 1. Lệnh biên dịch mã nguồn trong thư mục cpp/ thành file thực thi ở root
g++ cpp/main.cpp cpp/io.cpp cpp/processing.cpp -o main.exe

# 2. Lệnh thực thi chương trình
.\main.exe
```

*Sau khi chạy, chương trình sẽ tự động đọc `stations.csv` ở thư mục gốc và sinh ra hai báo cáo là `station_report.csv` và `anomaly.txt`.*

### B. Đối với Tài liệu LaTeX
Báo cáo học thuật được thiết kế self-contained (tự quản lý hình ảnh). Bạn chỉ cần mở thư mục `latex/` và biên dịch tệp chính `main.tex` bằng công cụ yêu thích của mình (như Overleaf, TeXstudio, VS Code hoặc dòng lệnh):

```powershell
# Biên dịch tài liệu LaTeX bằng pdflatex (Chạy trong thư mục latex/)
cd latex
pdflatex -interaction=nonstopmode main.tex
pdflatex -interaction=nonstopmode main.tex
```

*Tệp PDF báo cáo hoàn chỉnh dài **26 trang** mang tên `main.pdf` sẽ được tạo ra tại thư mục `latex/`.*

---

## 📈 3. Phân tích Bài toán & Quy trình xử lý (Pipeline)

Hệ thống hoạt động theo mô hình luồng dữ liệu một chiều (Pipeline):
$$\text{Đọc dữ liệu thô (CSV)} \rightarrow \text{Chuẩn hóa \& Làm sạch} \rightarrow \text{Thống kê mô tả} \rightarrow \text{Phân tích thuật toán} \rightarrow \text{Ghi báo cáo}$$

* **Làm sạch dữ liệu thiếu (NA Handling):** Dữ liệu khuyết thiếu ký hiệu là `NA` được hệ thống chuyển đổi khoa học sang giá trị thực `NAN` (tiêu chuẩn IEEE 754) để tự động bỏ qua khi tính toán các phép đo thống kê $\mu, \sigma$ mà không làm lệch kết quả.
* **Đầu ra hệ thống:**
  * `station_report.csv`: Ghi lại ID trạm, Tên trạm, Nhiệt độ trung bình và Nhiệt độ cao nhất.
  * `anomaly.txt`: Đoạn nắng nóng gay gắt nhất (Kadane), chuỗi ngày lượng mưa tăng liên tục (LIS), tổng lượng mưa tích lũy của chuỗi tăng (Prefix Sum 1D).

---

## 💾 4. Thiết kế Cấu trúc Dữ liệu

Chương trình sử dụng mô hình Struct lồng nhau cấp 2 cực kỳ tối ưu bộ nhớ:
1. `struct Record`: Lưu chi tiết một lần đo gồm ngày, nhiệt độ, độ ẩm, lượng mưa.
2. `struct Stats`: Lưu các đại lượng thống kê sau tính toán gồm $\mu_{\text{temp}}, \sigma_{\text{temp}}, \mu_{\text{rain}}, \sigma_{\text{rain}}, \text{Min}, \text{Max}$.
3. `struct Station`: Cấp quản lý ngoài cùng chứa thông tin hành chính của trạm, danh sách các bản ghi `vector<Record>` và đối tượng thống kê `Stats stats`.

---

## 📋 5. Đặc tả chi tiết các Hàm trong Hệ thống

| Tên hàm | Tham số (Input) | Kết quả (Output) | Mô tả nhiệm vụ |
| :--- | :--- | :--- | :--- |
| `readStationsFromCSV` | `const string &filename` | `vector<Station>` | Đọc tệp CSV, xử lý giá trị khuyết, nhóm `Record` vào đúng `Station`. |
| `normalizeStationData` | `Station &s` | `void` | Chuẩn hóa dữ liệu tên trạm (loại bỏ khoảng trắng thừa đầu/cuối). |
| `calculateStationStats` | `Station &s` | `void` | Tính toán giá trị $\mu, \sigma$, Min, Max cho nhiệt độ và lượng mưa của trạm đo. |
| `findMaxTempSegment` | `const Station &s` | `vector<Record>` | Áp dụng thuật toán **Kadane** tối ưu tìm đoạn ngày có tổng nhiệt độ tích lũy cao nhất. |
| `findLongestRainTrend` | `const Station &s, double &trendTotalRain` | `vector<Record>` | Áp dụng tư duy **LIS trên chuỗi tăng liên tiếp** tìm xu hướng mưa tăng và trả tổng mưa qua tham chiếu. |
| `computePrefixSum` | `const vector<double> &data` | `vector<double>` | Xây dựng mảng cộng dồn 1D cho mảng lượng mưa hợp lệ. |
| `queryRangeSum` | `const vector<double> &prefix, int l, int r` | `double` | Truy vấn tổng lượng mưa đoạn `[l, r]` trong thời gian hằng số $O(1)$. |
| `classifyRainLevel` | `double rainAmount` | `string` | Phân loại lượng mưa định tính (`No Rain`, `Light`, `Moderate`, `Heavy`). |
| `writeStationReport` | `ofstream &file, const Station &s` | `void` | Ghi dữ liệu thống kê trạm ra tệp CSV. |
| `writeAnomalyReport` | `ofstream &file, const Station &s...` | `void` | Ghi báo cáo bất thường cực đoan ra tệp văn bản TXT. |

---

## 🧮 6. Phương pháp Thuật toán & Đánh giá hiệu năng

Hệ thống áp dụng các kỹ thuật xử lý mảng/dãy con tối ưu:
* **Thuật toán Kadane ($O(n)$):** Tìm dãy con có tổng nhiệt độ lớn nhất một cách nhanh chóng, có cơ chế tự động ngắt đoạn khi gặp giá trị khuyết `NAN`.
* **Thuật toán LIS liên tiếp ($O(n)$):** Phát hiện xu hướng lượng mưa tăng liên tục kề nhau.
* **Mảng cộng dồn Prefix Sum 1D ($O(1)$ truy vấn):** Cho phép tính tổng lượng mưa tích lũy của chuỗi LIS một cách tức thời mà không cần duyệt vòng lặp cộng dồn.

### Bảng đánh giá độ phức tạp thuật toán:
| Thuật toán/Phép toán | Độ phức tạp thời gian | Mục đích sử dụng |
| :--- | :--- | :--- |
| **Tính toán thống kê** | $O(n)$ | Tính $\mu$, $\sigma$, Min, Max cho từng trạm đo. |
| **Thuật toán Kadane** | $O(n)$ | Phát hiện đợt nắng nóng cực đoan tích lũy cao nhất. |
| **LIS liên tiếp** | $O(n)$ | Phát hiện chuỗi ngày có xu hướng mưa tăng liên tục. |
| **Prefix Sum 1D** | $O(n)$ dựng, $O(1)$ truy vấn | Tối ưu hóa tính tổng lượng mưa tích lũy của một đoạn bất kỳ. |

---

## 📊 7. Kế hoạch Kiểm thử & Thực nghiệm

Hệ thống được xác thực kỹ lưỡng thông qua 4 kịch bản kiểm thử độc lập:

* **TC1 (`test_hanoi_normal.csv`):** 30 ngày dữ liệu đầy đủ không lỗi. Nhiệt độ $15.4^\circ\text{C}$ - $39.6^\circ\text{C}$, mưa $1.2\text{mm}$ - $19.6\text{mm}$.
* **TC2 (`test_sapa_edge_cases.csv`):** Kịch bản biên chứa nhiều dữ liệu khuyết `NA`. Dữ liệu lỗi được bỏ qua an toàn mà không làm sập chương trình.
* **TC3 (`test_hue_lis.csv`):** Nhiệt độ cố định $25^\circ\text{C}$, lượng mưa tăng dần từ $5.0\text{mm}$ đến $27.5\text{mm}$ (chứa 1 ngày lỗi `NA`). Xác thực tuyệt đối thuật toán LIS liên tiếp vượt lỗi thành công với tổng mưa dồn dập đạt **$145\text{mm}$**.
* **TC4 (`test_dalat_stress.csv`):** Thử nghiệm tải lớn với hàng trăm dòng dữ liệu thực tế, kiểm chứng tính bền vững của bộ nhớ và thuật toán Kadane.
