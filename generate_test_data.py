import csv
import random
from datetime import datetime, timedelta

def write_csv(filename, data):
    """Hàm phụ trợ để ghi dữ liệu vào file csv"""
    header = ["ID", "Name", "Date", "Temperature", "Humidity", "Rain"]
    with open(filename, mode='w', newline='', encoding='utf-8') as file:
        writer = csv.writer(file)
        writer.writerow(header)
        writer.writerows(data)
    print(f"File {filename} đã được tạo thành công!")

def generate_weather_data():
    start_date = datetime(2025, 1, 1)

    # ---------------------------------------------------------
    # Test Case 1: Dữ liệu thường (Trạm Hà Nội)
    # ---------------------------------------------------------
    hanoi_data = []
    for i in range(30):
        date_str = (start_date + timedelta(days=i)).strftime("%Y-%m-%d")
        temp = round(random.uniform(15.0, 35.0), 1)
        # Chèn đợt nắng nóng
        if i in range(10, 15): 
            temp = round(random.uniform(36.0, 40.0), 1)
        
        humidity = round(random.uniform(50.0, 90.0), 1)
        rain = round(random.uniform(0.0, 20.0), 1)
        hanoi_data.append([1, "Tram_Ha_Noi", date_str, temp, humidity, rain])
    
    write_csv("test_hanoi_normal.csv", hanoi_data)

    # ---------------------------------------------------------
    # Test Case 2: Dữ liệu biên và Lỗi (Trạm Sapa)
    # ---------------------------------------------------------
    sapa_data = [
        [2, "Tram_Sapa", "2025-01-01", "NA", "NA", "NA"],
        [2, "Tram_Sapa", "2025-01-02", -5.5, 80.0, 0.0],
        [2, "Tram_Sapa", "2025-01-03", 10.0, "NA", 15.0]
    ]
    write_csv("test_sapa_edge_cases.csv", sapa_data)

    # ---------------------------------------------------------
    # Test Case 3: Thuật toán LIS - Mưa tăng dần (Trạm Huế)
    # ---------------------------------------------------------
    hue_data = []
    for i in range(10):
        date_str = (start_date + timedelta(days=i)).strftime("%Y-%m-%d")
        rain_trend = 5.0 + (i * 2.5) 
        if i == 5:
            hue_data.append([3, "Tram_Hue", date_str, 25.0, 80.0, "NA"])
        else:
            hue_data.append([3, "Tram_Hue", date_str, 25.0, 80.0, rain_trend])
    
    write_csv("test_hue_lis.csv", hue_data)

    # ---------------------------------------------------------
    # Test Case 4: Dữ liệu lớn (Trạm Đà Lạt) - Stress Test
    # ---------------------------------------------------------
    dalat_data = []
    for i in range(5000):
        date_str = (start_date + timedelta(days=i)).strftime("%Y-%m-%d")
        temp = round(random.uniform(10.0, 28.0), 1)
        if random.random() < 0.05:
            temp = "NA"
        
        humidity = round(random.uniform(60.0, 100.0), 1)
        rain = round(random.uniform(0.0, 50.0), 1)
        dalat_data.append([4, "Tram_Da_Lat", date_str, temp, humidity, rain])
    
    write_csv("test_dalat_stress.csv", dalat_data)

if __name__ == "__main__":
    generate_weather_data()