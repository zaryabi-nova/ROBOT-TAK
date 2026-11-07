# ربات تک — کدها و فایل‌های کتاب

نویسنده: زهرا زریابی

این مخزن همه‌ی کدها و فایل‌های کتاب «ربات تک» را دارد. هر فصل یک پوشه دارد و نام هر فایل با شماره‌ی فصل شروع می‌شود (`chNN_...`). در جدول هر فصل، شماره‌ی نمونه و بخش کتاب کنار نام فایل آمده است.

## دریافت

```
git clone https://github.com/zaryabi-nova/robat-tak.git
```

یا از دکمه‌ی سبز **Code** گزینه‌ی **Download ZIP** را بزنید.

## پوشه‌ها

| پوشه | فصل |
|---|---|
| [`ch04_programming`](ch04_programming/) | فصل ۴: برنامه‌نویسی و نرم‌افزار بردها |
| [`ch05_projects`](ch05_projects/) | فصل ۵: ساخت ربات، پروژه‌های پایه |
| [`ch06_design`](ch06_design/) | فصل ۶: طراحی مکانیکی و نرم‌افزارهای CAD |
| [`ch07_wiring`](ch07_wiring/) | فصل ۷: نقشه‌ی سیم‌کشی و طراحی PCB |
| [`ch08_control`](ch08_control/) | فصل ۸: کنترل و فیلترها |
| [`ch09_wireless`](ch09_wireless/) | فصل ۹: ارتباط بی‌سیم و از راه دور |
| [`ch10_app`](ch10_app/) | فصل ۱۰: طراحی اپلیکیشن ربات |
| [`ch11_vision`](ch11_vision/) | فصل ۱۱: بینایی ماشین و هوش مصنوعی |
| [`ch12_ros2`](ch12_ros2/) | فصل ۱۲: ROS 2 |
| [`ch13_special_robots`](ch13_special_robots/) | فصل ۱۳: ربات‌های ویژه (بازو، پهپاد، قایق و زیردریایی) |
| [`ch14_projects`](ch14_projects/) | فصل ۱۴: پروژه‌های پیشرفته |

## فصل ۴: برنامه‌نویسی و نرم‌افزار بردها

| نمونه | بخش | فایل | توضیح |
|---|---|---|---|
| نمونه ۴-۱ | ۴-۳-۳ | [`ch04_blink.ino`](ch04_programming/ch04_blink.ino) | چشمک‌زن (Blink) |
| نمونه ۴-۲ | ۴-۴-۱ | [`ch04_two_tasks_millis.ino`](ch04_programming/ch04_two_tasks_millis.ino) | دو کار هم‌زمان |
| نمونه ۴-۳ | ۴-۴-۲ | [`ch04_tb6612_motor.ino`](ch04_programming/ch04_tb6612_motor.ino) | سرعت علامت‌دار، بین منفی ۲۵۵ و ۲۵۵ |
| نمونه ۴-۴ | ۴-۴-۳ | [`ch04_encoder_interrupt.ino`](ch04_programming/ch04_encoder_interrupt.ino) | انکودر دوفاز |
| نمونه ۴-۵ | ۴-۴-۴ | [`ch04_pid_speed.ino`](ch04_programming/ch04_pid_speed.ino) | PID ساده (اجرا هر ۱۰ میلی‌ثانیه) |
| نمونه ۴-۶ | ۴-۶-۴ | [`ch04_pico_blink_pwm.py`](ch04_programming/ch04_pico_blink_pwm.py) | چشمک‌زن و PWM (Raspberry Pi Pico) |
| نمونه ۴-۷ | ۴-۶-۴ | [`ch04_i2c_scan_adc.py`](ch04_programming/ch04_i2c_scan_adc.py) | اسکن I2C و خواندن ADC |
| نمونه ۴-۸ | ۴-۶-۴ | [`ch04_uart_wifi.py`](ch04_programming/ch04_uart_wifi.py) | UART و اتصال Wi-Fi (ESP32 / Pico W) |
| نمونه ۴-۹ | ۴-۶-۶ | [`ch04_circuitpython_blink_code.py`](ch04_programming/ch04_circuitpython_blink_code.py) | چشمک‌زن در CircuitPython (فایل code.py) |
| — | ۴-۷-۱ | [`ch04_platformio.ini`](ch04_programming/ch04_platformio.ini) | فایل platformio.ini برای دو برد |
| نمونه ۴-۱۰ | ۴-۸-۳ | [`ch04_gpiozero_motor_distance.py`](ch04_programming/ch04_gpiozero_motor_distance.py) | کنترل موتور و حسگر فاصله با gpiozero |
| — | ۴-۸-۵ | [`ch04_robot.service`](ch04_programming/ch04_robot.service) | سرویس systemd برای اجرای خودکار برنامه |
| نمونه ۴-۱۱ | ۴-۸-۶ | [`ch04_pi_serial_to_mcu.py`](ch04_programming/ch04_pi_serial_to_mcu.py) | ارسال فرمان به Arduino و دریافت پاسخ |
| — | ۴-۹-۲ | [`ch04_ssh_config.txt`](ch04_programming/ch04_ssh_config.txt) | فایل ~/.ssh/config برای میانبر |
| نمونه ۴-۱۲ | ۴-۱۰ | [`ch04_espidf_freertos_blink.c`](ch04_programming/ch04_espidf_freertos_blink.c) | چشمک‌زن با FreeRTOS در ESP-IDF |
| نمونه ۴-۱۳ | ۴-۱۱-۴ | [`ch04_stm32_hal_blink_encoder.c`](ch04_programming/ch04_stm32_hal_blink_encoder.c) | چشمک‌زن و انکودر با HAL (بخش USER CODE) |
| جدول ۴-۲۳ | ۴-۱۱-۴ | `ch04_stm32_01` تا `ch04_stm32_07` | هفت نمونه‌ی STM32 برای NUCLEO-F446RE |
| نمونه ۴-۱۴ | ۴-۱۳-۱ | [`ch04_serial_command_failsafe.ino`](ch04_programming/ch04_serial_command_failsafe.ino) | دریافت فرمان خط‌به‌خط در Arduino با توقف ایمن |
| — | ۴-۱۴-۱ | [`ch04_robot.gitignore`](ch04_programming/ch04_robot.gitignore) | فایل .gitignore برای پروژه‌ی ربات |

## فصل ۵: ساخت ربات، پروژه‌های پایه

| نمونه | بخش | فایل | توضیح |
|---|---|---|---|
| نمونه ۵-۱ | ۵-۲-۷ | [`ch05_color_detect_tcs3200.ino`](ch05_projects/ch05_color_detect_tcs3200.ino) | تشخیص رنگ با TCS3200، LEDهای رنگی، LED فرابنفش و LED رنگی RGB (Arduino Nano) |
| نمونه ۵-۲ | ۵-۳-۶ | [`ch05_smart_watering.ino`](ch05_projects/ch05_smart_watering.ino) | آبیاری هوشمند با آبیاری کوتاه، زمان جذب و توقف ایمن (Arduino Uno) |
| نمونه ۵-۳ | ۵-۳-۱۰ | [`ch05_smart_watering_web_esp32.ino`](ch05_projects/ch05_smart_watering_web_esp32.ino) | آبیاری هوشمند با صفحه‌ی وب (ESP32) |
| نمونه ۵-۴ | ۵-۴-۶ | [`ch05_fire_robot_fan.ino`](ch05_projects/ch05_fire_robot_fan.ino) | ربات آتش‌نشان بادی با هشدار نوری و صوتی (Arduino Uno) |
| نمونه ۵-۵ | ۵-۵-۶ | [`ch05_light_follower_ldr.ino`](ch05_projects/ch05_light_follower_ldr.ino) | ربات دنبال‌کننده‌ی نور با دو ماژول LDR (Arduino Uno) |
| نمونه ۵-۶ | ۵-۵-۹ | [`ch05_light_follower_analog.ino`](ch05_projects/ch05_light_follower_analog.ino) | نسخه‌ی آنالوگ با فرمان متناسب (تابع drive همان نمونه ۵ است) |
| نمونه ۵-۷ | ۵-۶-۶ | [`ch05_obstacle_avoid_fixed.ino`](ch05_projects/ch05_obstacle_avoid_fixed.ino) | ربات اجتناب از مانع با سنسور ثابت (Arduino Nano + L298N) |
| نمونه ۵-۸ | ۵-۶-۷ | [`ch05_obstacle_avoid_servo.ino`](ch05_projects/ch05_obstacle_avoid_servo.ino) | ربات اجتناب از مانع با سروو و انتخاب مسیر بازتر (Arduino Nano) |
| نمونه ۵-۹ | ۵-۷-۷ | [`ch05_line_follower_3ir.ino`](ch05_projects/ch05_line_follower_3ir.ino) | ربات خط‌دنبال‌کن با سه سنسور IR (Arduino Uno + L298N) |
| نمونه ۵-۱۰ | ۵-۷-۱۰ | [`ch05_line_sensor_calibration.ino`](ch05_projects/ch05_line_sensor_calibration.ino) | کالیبراسیون خودکار سنسورهای آنالوگ |
| نمونه ۵-۱۱ | ۵-۷-۱۱ | [`ch05_line_follower_micropython.py`](ch05_projects/ch05_line_follower_micropython.py) | ربات خط‌دنبال‌کن با سه سنسور (MicroPython، Raspberry Pi Pico) |
| نمونه ۵-۱۲ | ۵-۸-۶ | [`ch05_maze_wall_left_hand.ino`](ch05_projects/ch05_maze_wall_left_hand.ino) | ربات ماز با قانون دست چپ، اجتناب از برخورد و خط پایان (Arduino Uno + L298N) |
| نمونه ۵-۱۳ | ۵-۸-۹ | [`ch05_line_maze_lsrb.ino`](ch05_projects/ch05_line_maze_lsrb.ino) | ربات ماز خطی با قانون دست چپ و سه سنسور IR |
| نمونه ۵-۱۴ | ۵-۹-۶ | [`ch05_fire_robot_water_pump.ino`](ch05_projects/ch05_fire_robot_water_pump.ino) | ربات آتش‌نشان آبی با سه سنسور شعله، پمپ و سروو (Arduino Uno) |
| نمونه ۵-۱۵ | ۵-۱۰-۶ | [`ch05_wifi_robot_web.ino`](ch05_projects/ch05_wifi_robot_web.ino) | ربات Wi-Fi با صفحه‌ی وب، کنترل سرعت، توقف ایمن و حالت خودکار (ESP32) |

## فصل ۶: طراحی مکانیکی و نرم‌افزارهای CAD

| نمونه | بخش | فایل | توضیح |
|---|---|---|---|
| جدول ۶-۵ | ۶-۵ | [`ch06_design/`](ch06_design/) | صفحه‌ی نمونه‌ی شاسی: STL، DXF، SVG، OpenSCAD، ماکروی FreeCAD و نقشه |

## فصل ۷: نقشه‌ی سیم‌کشی و طراحی PCB

| نمونه | بخش | فایل | توضیح |
|---|---|---|---|
| — | ۷-۳-۲ | [`ch07_wiring/`](ch07_wiring/) | مدار LED با کلید در Fritzing: فقط بردبورد، و هر سه نما کامل |

## فصل ۸: کنترل و فیلترها

| نمونه | بخش | فایل | توضیح |
|---|---|---|---|
| نمونه ۸-۱ | ۸-۲ | [`ch08_onoff_pump.ino`](ch08_control/ch08_onoff_pump.ino) | آبیاری خودکار با کنترل روشن و خاموش و پسماند (سنسور A0، رله D7) |
| نمونه ۸-۲ | ۸-۳-۲ | [`ch08_pid_follow_distance.ino`](ch08_control/ch08_pid_follow_distance.ino) | ربات فاصله‌نگه‌دار با PID (HC-SR04 روی D9 و D8، درایور L298N) |
| نمونه ۸-۳ | ۸-۴-۲ | [`ch08_kalman_sim.py`](ch08_control/ch08_kalman_sim.py) | شبیه‌سازی و مقایسه‌ی فیلترها با پایتون |
| نمونه ۸-۴ | ۸-۴-۲ | [`ch08_kalman_mpu6050.ino`](ch08_control/ch08_kalman_mpu6050.ino) | فیلتر کالمن دو حالته برای زاویه‌ی MPU6050 (Arduino Uno یا Nano: SDA=A4، SCL=A5) |

## فصل ۹: ارتباط بی‌سیم و از راه دور

| نمونه | بخش | فایل | توضیح |
|---|---|---|---|
| نمونه ۹-۱ | ۹-۲ | [`ch09_rs485_link.ino`](ch09_wireless/ch09_rs485_link.ino) | ارتباط دو آردوینو با MAX485 (RO به D10، DI به D11، DE و RE به D2) |
| نمونه ۹-۲ | ۹-۳ | [`ch09_rf433_rcswitch.ino`](ch09_wireless/ch09_rf433_rcswitch.ino) | فرستادن و گرفتن کد با ماژول‌های ۴۳۳ مگاهرتز و RCSwitch (فرستنده D10، گیرنده D2) |
| نمونه ۹-۳ | ۹-۴-۱ | [`ch09_hc05_car.ino`](ch09_wireless/ch09_hc05_car.ino) | ماشین بلوتوثی با Arduino Uno، HC-05 و L298N (SoftwareSerial روی پایه‌های ۱۰ و ۱۱) |
| نمونه ۹-۴ | ۹-۴-۲ | [`ch09_esp32_bt_car.ino`](ch09_wireless/ch09_esp32_bt_car.ino) | ماشین بلوتوثی با ESP32 و BluetoothSerial |
| نمونه ۹-۵ | ۹-۴-۳ | [`ch09_ps5_bluepad32_car.ino`](ch09_wireless/ch09_ps5_bluepad32_car.ino) | رانندگی ربات با دسته‌ی PS5 و Bluepad32 (استیک چپ جلو و عقب، استیک راست چرخش، دکمه‌ی X ایست) |
| نمونه ۹-۶ | ۹-۵ | [`ch09_ibus_read.ino`](ch09_wireless/ch09_ibus_read.ino) | خواندن شش کانال iBUS گیرنده‌ی FS-iA6B با ESP32، بدون کتابخانه |
| نمونه ۹-۷ | ۹-۶ | [`ch09_rfid_read.ino`](ch09_wireless/ch09_rfid_read.ino) | خواندن UID کارت با RC522 و ESP32 (SCK18، MISO19، MOSI23، SS5، RST4) |
| نمونه ۹-۸ | ۹-۷-۱ | [`ch09_fm_rda5807.ino`](ch09_wireless/ch09_fm_rda5807.ino) | رادیو FM با RDA5807M و آردوینو؛ دکمه‌ی جست‌وجو و صدا (کتابخانه‌ی PU2CLR RDA5807) |
| نمونه ۹-۹ | ۹-۷-۲ | [`ch09_fm_kt0803l.ino`](ch09_wireless/ch09_fm_kt0803l.ino) | تنظیم فرکانس فرستنده‌ی KT0803L با آردوینو (I2C نشانی 0x3E) |
| نمونه ۹-۱۰ | ۹-۸ | [`ch09_hc12_link.ino`](ch09_wireless/ch09_hc12_link.ino) | پل سریال دوطرفه با HC-12 و فرمان‌های AT |
| نمونه ۹-۱۱ | ۹-۹ | [`ch09_web_led.ino`](ch09_wireless/ch09_web_led.ino) | کوچک‌ترین کنترل وب: روشن و خاموش کردن LED با ESP32 |
| نمونه ۹-۱۲ | ۹-۹-۱ | [`ch09_espnow_remote.ino`](ch09_wireless/ch09_espnow_remote.ino) | ریموت ESP-NOW: خواندن جوی‌استیک و فرستادن ۲۰ بسته در ثانیه |
| نمونه ۹-۱۳ | ۹-۹-۱ | [`ch09_espnow_robot.ino`](ch09_wireless/ch09_espnow_robot.ino) | ربات گیرنده‌ی ESP-NOW با دو موتور و ایمنی قطع ارتباط |
| نمونه ۹-۱۴ | ۹-۱۱-۱ | [`ch09_gps_read.ino`](ch09_wireless/ch09_gps_read.ino) | خواندن موقعیت از ماژول GPS با ESP32 و TinyGPSPlus |
| نمونه ۹-۱۵ | ۹-۱۱-۲ | [`ch09_sms_test.ino`](ch09_wireless/ch09_sms_test.ino) | فرستادن یک پیامک آزمایشی با A7670E و ESP32 |

## فصل ۱۰: طراحی اپلیکیشن ربات

| نمونه | بخش | فایل | توضیح |
|---|---|---|---|
| نمونه ۱۰-۱ | ۱۰-۵ | [`ch10_arm_app_kivy.py`](ch10_app/ch10_arm_app_kivy.py) | اپ کنترل بازو با Kivy: چهار لغزنده و دکمه‌های Home، Save، Play و Stop |

## فصل ۱۱: بینایی ماشین و هوش مصنوعی

| نمونه | بخش | فایل | توضیح |
|---|---|---|---|
| نمونه ۱۱-۱ | ۱۱-۴-۳ | [`ch11_camera_calibration.py`](ch11_vision/ch11_camera_calibration.py) | کالیبراسیون دوربین با OpenCV |
| نمونه ۱۱-۳ | ۱۱-۵-۱ | [`ch11_capture_usb_csi.py`](ch11_vision/ch11_capture_usb_csi.py) | گرفتن فریم از وب‌کم USB و دوربین CSI |
| نمونه ۱۱-۴ | ۱۱-۵-۱ | [`ch11_latest_frame_thread.py`](ch11_vision/ch11_latest_frame_thread.py) | خواندن دوربین در رشته‌ی جدا تا همیشه تازه‌ترین فریم را داشته باشیم |
| نمونه ۱۱-۵ | ۱۱-۵-۲ | [`ch11_hsv_tuner.py`](ch11_vision/ch11_hsv_tuner.py) | ابزار تنظیم بازه‌ی HSV با اسلایدر |
| نمونه ۱۱-۶ | ۱۱-۶-۴ | [`ch11_yolo26_detect.py`](ch11_vision/ch11_yolo26_detect.py) | اجرای مدل آماده‌ی YOLO26 روی یک تصویر |
| نمونه ۱۱-۷ | ۱۱-۶-۵ | [`ch11_data.yaml`](ch11_vision/ch11_data.yaml) | فایل data.yaml و فرمان آموزش YOLO روی داده‌ی خودمان |
| نمونه ۱۱-۹ | ۱۱-۹ | [`ch11_visual_servo_follow.py`](ch11_vision/ch11_visual_servo_follow.py) | ساختار کنترل دنبال‌کننده‌ی هدف |
| نمونه ۱۱-۱۰ | ۱۱-۱۰ | [`ch11_ros2_color_follower.py`](ch11_vision/ch11_ros2_color_follower.py) | گره‌ی ROS 2 که جسم قرمز را با چرخش در وسط تصویر نگه می‌دارد |

## فصل ۱۲: ROS 2

| نمونه | بخش | فایل | توضیح |
|---|---|---|---|
| — | ۱۲-۳ | [`ch12_ros2/ros2_goto_ws/`](ch12_ros2/ros2_goto_ws/) | پروژه‌ی کامل هدایت لاک‌پشت با Service و Action (راهنمای README_FA.md) |
| نمونه ۱۲-۲ | ۱۲-۳-۱ | [`ch12_GoToPoint.action`](ch12_ros2/ch12_GoToPoint.action) | فایل GoToPoint.action |
| نمونه ۱۲-۳ | ۱۲-۳-۱ | [`ch12_SetSpeed.srv`](ch12_ros2/ch12_SetSpeed.srv) | فایل SetSpeed.srv |
| نمونه ۱۲-۴ | ۱۲-۴-۱ | [`ch12_platformio.ini`](ch12_ros2/ch12_platformio.ini) | فایل platformio.ini |
| نمونه ۱۲-۵ | ۱۲-۴-۲ | [`ch12_microros_cmd_vel_l298n.cpp`](ch12_ros2/ch12_microros_cmd_vel_l298n.cpp) | ESP32 فرمان cmd_vel را می‌گیرد و دو موتور را با L298N می‌راند |
| نمونه ۱۲-۶ | ۱۲-۵-۲ | [`ch12_microros_encoders_odom.cpp`](ch12_ros2/ch12_microros_encoders_odom.cpp) | شمارش پالس انکودر با وقفه و فرستادن آن |
| نمونه ۱۲-۷ | ۱۲-۵-۳ | [`ch12_odom_node.py`](ch12_ros2/ch12_odom_node.py) | تابع اودومتری ربات دیفرانسیلی |
| نمونه ۱۲-۸ | ۱۲-۶-۴ | [`ch12_scan_guard.py`](ch12_ros2/ch12_scan_guard.py) | گره‌ی ایمنی با LiDAR |
| نمونه ۱۲-۹ | ۱۲-۷-۱ | [`ch12_robot_bringup.launch.py`](ch12_ros2/ch12_robot_bringup.launch.py) | فایل robot_bringup.launch.py: اودومتری، تبدیل‌های ثابت و LiDAR |
| نمونه ۱۲-۱۰ | ۱۲-۹-۶ | [`ch12_robot.service`](ch12_ros2/ch12_robot.service) | سرویس روشن شدن خودکار ربات |

## فصل ۱۳: ربات‌های ویژه (بازو، پهپاد، قایق و زیردریایی)

| نمونه | بخش | فایل | توضیح |
|---|---|---|---|
| نمونه ۱۳-۱ | ۱۳-۱-۲ | [`ch13_arm_joint_test.ino`](ch13_special_robots/ch13_arm_joint_test.ino) | آزمون بازوی شش‌سرووی با PCA9685: حالت خانه و حرکت یک مفصل از Serial Monitor |
| نمونه ۱۳-۲ | ۱۳-۱-۳ | [`ch13_arm_ik_test.ino`](ch13_special_robots/ch13_arm_ik_test.ino) | سینماتیک معکوس بازوی دومفصلی با PCA9685 (طول بازوها ۱۰۰ و ۸۰ میلی‌متر) |
| نمونه ۱۳-۳ | ۱۳-۲-۴ | [`ch13_esc_pot_test.ino`](ch13_special_robots/ch13_esc_pot_test.ino) | آزمون یک موتور براشلس و ESC با پتانسیومتر (بدون ملخ) |
| نمونه ۱۳-۴ | ۱۳-۴-۲ | [`ch13_rov_depth_leak.ino`](ch13_special_robots/ch13_rov_depth_leak.ino) | اندازه‌گیری عمق با MS5837 و حسگر ساده‌ی نشتی |

## فصل ۱۴: پروژه‌های پیشرفته

| نمونه | بخش | فایل | توضیح |
|---|---|---|---|
| نمونه ۱۴-۱ | ۱۴-۱-۶ | [`ch14_joyshield_nrf24_tx.ino`](ch14_projects/ch14_joyshield_nrf24_tx.ino) | فرستنده: خواندن جوی‌استیک و دکمه‌ها و ارسال بسته |
| نمونه ۱۴-۲ | ۱۴-۱-۶ | [`ch14_joyshield_nrf24_car_rx.ino`](ch14_projects/ch14_joyshield_nrf24_car_rx.ino) | گیرنده: ماشین ESP32 با nRF24L01 |
| نمونه ۱۴-۳ | ۱۴-۲-۶ | [`ch14_motor_speed_pid.ino`](ch14_projects/ch14_motor_speed_pid.ino) | کنترل سرعت یک موتور انکدردار با PID |
| نمونه ۱۴-۴ | ۱۴-۳-۵ | [`ch14_web_rfid_sms_robot.ino`](ch14_projects/ch14_web_rfid_sms_robot.ino) | ربات وای‌فای با جوی‌استیک وب، قفل RFID و هشدار پیامکی دما |
| نمونه ۱۴-۵ | ۱۴-۴-۶ | [`ch14_self_balancing_robot.ino`](ch14_projects/ch14_self_balancing_robot.ino) | ربات خودمتعادل با فیلتر کالمن و PID |
| نمونه ۱۴-۶ | ۱۴-۵-۷ | [`ch14_stm32_robot_pid_bt.c`](ch14_projects/ch14_stm32_robot_pid_bt.c) | ربات دوچرخ STM32 با PID و بلوتوث (بخش‌های USER CODE فایل main.c) |
| نمونه ۱۴-۷ | ۱۴-۶-۶ | [`ch14_arm_web_teach.ino`](ch14_projects/ch14_arm_web_teach.ino) | بازوی رباتیک با صفحه‌ی وب، آموزش و پخش حرکت |
| نمونه ۱۴-۸ | ۱۴-۷-۶ | [`ch14_color_sort_arm.py`](ch14_projects/ch14_color_sort_arm.py) | بازوی دسته‌بندی رنگ با دوربین و PCA9685 |
| نمونه ۱۴-۹ | ۱۴-۸-۷ | [`ch14_ai_person_follower.py`](ch14_projects/ch14_ai_person_follower.py) | ربات دنبال‌کننده‌ی آدم با Hailo و Picamera2 |

## نکته‌ها

- **Arduino:** هر فایل `.ino` را در پوشه‌ای هم‌نام خودش بگذارید تا Arduino IDE آن را باز کند.
- **STM32:** فایل‌های `.c` بخش‌هایی از `main.c` هستند که در قسمت‌های `USER CODE` هم‌نام قرار می‌گیرند؛ تنظیم CubeMX هر نمونه در سرآغاز فایل آمده است.
- **ROS 2:** پوشه‌ی `ch12_ros2/ros2_goto_ws` را در پوشه‌ی خانه کپی کنید و طبق بخش ۱۲-۳ کتاب با `colcon build` بسازید.
- پیش از اجرا، شماره‌ی پایه‌ها، نام شبکه‌ی Wi-Fi و رمز را با سخت‌افزار خودتان یکی کنید و رمزها را در مخزن عمومی نگذارید.

## مجوز

کدها برای آموزش آزادانه قابل استفاده‌اند.
