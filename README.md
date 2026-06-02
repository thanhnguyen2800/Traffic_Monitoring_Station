Traffic_Monitoring_Station
Hệ thống giám sát tốc độ phương tiện giao thông sử dụng ESP32

📋 Mô tả
Dự án Traffic Monitoring Station là hệ thống đo tốc độ xe sử dụng 2 cảm biến hồng ngoại (IR) kết hợp màn hình LCD I2C và còi buzzer. Hệ thống sẽ:

- Phát hiện xe đi qua
- Tính toán tốc độ (km/h)
- Hiển thị kết quả trên LCD 16x2
- Cảnh báo và kêu buzzer khi xe vượt quá 50 km/h
  
🛠 Công nghệ sử dụng
- Board: ESP32
- Framework: ESP-IDF
- Cảm biến: 2x Cảm biến hồng ngoại (IR)
- Hiển thị: LCD 16x2 I2C (địa chỉ 0x27)
- Cảnh báo: Buzzer (GPIO 13)
- Ngôn ngữ: C

📁 Cấu trúc project

Traffic_Monitoring_Station/

├── main/

│   ├── main.c              # Code chính

│   ├── lcd_i2c.c/h         # Driver LCD I2C

│   └── CMakeLists.txt

├── CMakeLists.txt

├── sdkconfig               # Cấu hình ESP-IDF

├── .devcontainer/          # Dev Container (VS Code)

└── build/                  # Thư mục build


🔌 Kết nối phần cứng

- IR Sensor 1	GPIO 34	Cảm biến đầu
- IR Sensor 2	GPIO 35	Cảm biến sau
- Buzzer	GPIO 13	Còi cảnh báo
- LCD I2C	SDA/SCL	Màn hình hiển thị

🚀 Hướng dẫn sử dụng
Mở project trong VS Code với Dev Container (ESP-IDF)

Build project:
idf.py build
Flash lên board:
idf.py flash monitor
