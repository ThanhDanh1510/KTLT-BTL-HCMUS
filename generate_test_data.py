import csv
import random
from datetime import datetime, timedelta

def generate_weather_data(filename="stations.csv"):
    start_date = datetime(2025, 1, 1)
    
    with open(filename, mode='w', newline='', encoding='utf-8') as file:
        writer = csv.writer(file)
        # Ghi Header
        writer.writerow(["ID", "Name", "Date", "Temperature", "Humidity", "Rain"])

        # ---------------------------------------------------------
        # Test Case 1: Dữ liệu thường (Trạm Hà Nội)
        # Sinh 30 ngày với các chỉ số dao động bình thường
        # ---------------------------------------------------------
        for i in range(30):
            date_str = (start_date + timedelta(days=i)).strftime("%Y-%m-%d")
            temp = round(random.uniform(15.0, 35.0), 1)
            humidity = round(random.uniform(50.0, 90.0), 1)
            rain = round(random.uniform(0.0, 20.0), 1)
            
            # Mục đích dòng này là chèn 1-2 đợt nắng nóng (nhiệt độ > 35) để test Heatwave
            if i in range(10, 15): 
                temp = round(random.uniform(36.0, 40.0), 1)
                
            writer.writerow([1, "Tram_Ha_Noi", date_str, temp, humidity, rain])

        # ---------------------------------------------------------
        # Test Case 2: Dữ liệu biên và Lỗi (Trạm Sapa)
        # Sinh các ca khó: toàn âm, khuyết dữ liệu (NA), 1 ngày duy nhất
        # ---------------------------------------------------------
        # Dòng 1: Toàn NA
        writer.writerow([2, "Tram_Sapa", "2025-01-01", "NA", "NA", "NA"])
        # Dòng 2: Nhiệt độ âm (Test thuật toán Kadane)
        writer.writerow([2, "Tram_Sapa", "2025-01-02", -5.5, 80.0, 0.0])
        # Dòng 3: Khuyết 1 phần
        writer.writerow([2, "Tram_Sapa", "2025-01-03", 10.0, "NA", 15.0])

        # ---------------------------------------------------------
        # Test Case Ép thuật toán LIS: Chuỗi mưa tăng dần (Trạm Huế)
        # ---------------------------------------------------------
        for i in range(10):
            date_str = (start_date + timedelta(days=i)).strftime("%Y-%m-%d")
            # Mưa tăng dần đều mỗi ngày (test LIS)
            rain_trend = 5.0 + (i * 2.5) 
            # Ngày thứ 5 cho NA để xem chuỗi LIS có bị ngắt đúng cách không
            if i == 5:
                writer.writerow([3, "Tram_Hue", date_str, 25.0, 80.0, "NA"])
            else:
                writer.writerow([3, "Tram_Hue", date_str, 25.0, 80.0, rain_trend])

        # ---------------------------------------------------------
        # Test Case 3: Dữ liệu lớn (Trạm Đà Lạt) - Stress Test
        # Sinh 5000 bản ghi để kiểm tra hiệu năng
        # ---------------------------------------------------------
        for i in range(5000):
            date_str = (start_date + timedelta(days=i)).strftime("%Y-%m-%d")
            temp = round(random.uniform(10.0, 28.0), 1)
            humidity = round(random.uniform(60.0, 100.0), 1)
            rain = round(random.uniform(0.0, 50.0), 1)
            
            # Random rải rác một vài giá trị NA
            if random.random() < 0.05: # 5% tỷ lệ khuyết dữ liệu
                temp = "NA"
                
            writer.writerow([4, "Tram_Da_Lat", date_str, temp, humidity, rain])

    print(f"File {filename} đã được tạo thành công với các Test Case!")

if __name__ == "__main__":
    generate_weather_data()