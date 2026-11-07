# هدایت لاک‌پشت به نقطه‌ی هدف با Service و Action (ROS 2 Jazzy)

این راهنما **از صفر** شروع می‌کند: اول مفاهیم (نود/گره، تاپیک، پیام، سرویس، اکشن، پارامتر، پکیج، workspace، launch) را ساده توضیح می‌دهیم، بعد قدم‌به‌قدم پروژه را می‌سازیم و اجرا می‌کنیم.

---

## بخش ۰: مفاهیم پایه از ابتدا

### ROS 2 چیست؟
ROS 2 یک چارچوب برای نوشتن نرم‌افزار ربات است. به‌جای یک برنامه‌ی بزرگ، کار را به **چند برنامه‌ی کوچک مستقل** تقسیم می‌کنیم و این برنامه‌ها با هم **پیام رد و بدل می‌کنند**. مثلاً یکی سنسور را می‌خواند، یکی تصمیم می‌گیرد، یکی موتور را می‌چرخاند.

### نود (Node) = گره
**نود** یک برنامه‌ی کوچک با یک وظیفه‌ی مشخص است. به آن «گره» هم می‌گویند چون اگر کل سیستم را به‌صورت یک گراف بکشید، نودها **گره‌های گراف** هستند و ارتباط‌ها (تاپیک‌ها و...) **یال‌ها**.

در این پروژه سه نود داریم:

| نود | وظیفه |
|---|---|
| `turtlesim_node` | شبیه‌ساز لاک‌پشت (آماده، از ROS) |
| `go_to_server` | مغز پروژه: هدف را می‌گیرد و لاک‌پشت را می‌راند |
| `go_to_client` | یک برنامه‌ی کوچک که هدف را برای سرور می‌فرستد |

### تاپیک (Topic) و پیام (Message)
**تاپیک** یک کانال ارتباطی با اسم است، مثل یک کانال رادیویی. هر نود می‌تواند:
- **Publisher** باشد: روی کانال پخش کند،
- **Subscriber** باشد: به کانال گوش بدهد.

ارتباط **یک‌طرفه و پیوسته** است و فرستنده نمی‌داند چه کسی گوش می‌دهد. چیزی که روی تاپیک می‌رود **پیام** است (یک ساختار داده، مثل `Twist` یا `Pose`).

در این پروژه:
- `/turtle1/cmd_vel` ← فرمان سرعت (پیام `Twist`: سرعت خطی و زاویه‌ای)
- `/turtle1/pose` ← موقعیت لاک‌پشت (پیام `Pose`: x و y و زاویه‌ی θ)

### سرویس (Service)
**سرویس** یک **پرسش و پاسخ** کوتاه است: کلاینت درخواست می‌دهد، سرور جواب می‌دهد و تمام. مثل این‌که زنگ بزنید بپرسید «ساعت چند است؟».
در این پروژه: `set_speed` (تغییر حداکثر سرعت). درخواست = سرعت‌های جدید، پاسخ = موفق بود یا نه.

### اکشن (Action)
**اکشن** برای **کارهای طولانی** است که چند ثانیه یا بیشتر طول می‌کشند. مثل سفارش پیتزا:
1. **Goal (هدف):** سفارش را ثبت می‌کنید («برو به نقطه‌ی ۸،۳»)،
2. **Feedback (بازخورد):** در طول راه وضعیت می‌گیرید («۴ متر مانده»)،
3. **Result (نتیجه):** در پایان نتیجه را می‌گیرید («رسید»)،
4. **Cancel (لغو):** هر وقت خواستید می‌توانید سفارش را لغو کنید.

### مقایسه‌ی سه روش ارتباط

| | Topic | Service | Action |
|---|---|---|---|
| الگو | پخش پیوسته | درخواست ← پاسخ | هدف ← بازخورد... ← نتیجه |
| مدت کار | ــ | کوتاه | طولانی |
| قابل لغو | ندارد | ندارد | دارد |
| مثال این پروژه | `cmd_vel`، `pose` | `set_speed` | `go_to_point` |

### پارامتر (Parameter)
**تنظیمات یک نود** که بدون تغییر کد قابل تغییرند. اینجا: `tolerance` (چقدر نزدیک هدف را «رسیدن» حساب کنیم) و `timeout_sec`.

### پکیج (Package) و Workspace
- **پکیج:** واحد سازماندهی کد (یک پوشه با `package.xml` و کد). هر پکیج یک یا چند نود دارد.
- **Workspace:** پوشه‌ای که پکیج‌ها در `src/` آن هستند. با `colcon build` همه ساخته می‌شوند و نتیجه در `install/` می‌رود.
- **`source install/setup.bash`:** به ترمینال می‌گوید پکیج‌های ساخته‌شده کجا هستند. **در هر ترمینال جدید باید زده شود.**

### رابط‌ها (Interfaces): `.msg` و `.srv` و `.action`
فایل‌های متنی کوتاه که **ساختار** پیام، سرویس یا اکشن را تعریف می‌کنند. ROS 2 از روی آن‌ها خودش کد پایتون/C++ می‌سازد. چون اینجا Action و Service **سفارشی** داریم، یک پکیج جدا به اسم `turtle_interfaces` برایشان ساخته‌ایم (باید اول ساخته شود).

### Launch file
یک فایل پایتون که **چند نود را با یک دستور** اجرا می‌کند و پارامترها را هم تنظیم می‌کند.

### Executor و Callback
نود با `spin` کار می‌کند: منتظر رویدادها می‌ماند (رسیدن پیام، درخواست سرویس، هدف جدید) و برای هر کدام تابع مربوطه (**callback**) را صدا می‌زند. چون تابع `execute` ما در حلقه می‌چرخد، از `MultiThreadedExecutor` استفاده کرده‌ایم تا هم‌زمان `pose` هم به‌روز شود و سرویس هم جواب بدهد.

---

## بخش ۱: ساختار کلی پروژه

```
go_to_client ──(Goal)──────────▶ go_to_server ──(cmd_vel)──▶ turtlesim_node
             ◀─(Feedback/Result)─      ▲                            │
                                       └──────(/turtle1/pose)───────┘

ros2 service call ──(set_speed)──▶ go_to_server
```

```
ros2_goto_ws/src/
├── turtle_interfaces/          ← تعریف Action و Service
│   ├── action/GoToPoint.action
│   ├── srv/SetSpeed.srv
│   ├── CMakeLists.txt
│   └── package.xml
└── turtle_goto/                ← نودها
    ├── turtle_goto/
    │   ├── control.py          ← فرمول کنترل (بدون ROS، قابل تست)
    │   ├── go_to_server.py     ← Action Server + Service Server
    │   └── go_to_client.py     ← Action Client
    ├── launch/go_to.launch.py
    ├── test/test_control.py
    ├── setup.py / setup.cfg / package.xml
```

---

## بخش ۲: گام‌به‌گام اجرا

### گام ۱: پیش‌نیاز
همان نصب بخش قبل (`ros-jazzy-desktop` و `ros-jazzy-turtlesim`). فایل zip را در پوشه‌ی home باز کنید تا `~/ros2_goto_ws` ساخته شود.

### گام ۲: بررسی رابط‌ها (چه چیزی تعریف کرده‌ایم؟)

**`GoToPoint.action`** (سه بخش با `---` جدا شده: Goal، Result، Feedback):
```
float32 x
float32 y
---
bool success
float32 final_distance
---
float32 distance_remaining
```

**`SetSpeed.srv`** (دو بخش: Request و Response):
```
float32 max_linear
float32 max_angular
---
bool success
string message
```

### گام ۳: ساخت (Build)
```bash
cd ~/ros2_goto_ws
source /opt/ros/jazzy/setup.bash
colcon build
source install/setup.bash
```
colcon خودش می‌فهمد `turtle_goto` به `turtle_interfaces` وابسته است و ترتیب ساخت را رعایت می‌کند. بررسی کنید رابط‌ها ساخته شده‌اند:
```bash
ros2 interface show turtle_interfaces/action/GoToPoint
ros2 interface show turtle_interfaces/srv/SetSpeed
```

### گام ۴: اجرای سرور و شبیه‌ساز
```bash
ros2 launch turtle_goto go_to.launch.py
```
یک پنجره‌ی لاک‌پشت باز می‌شود و در ترمینال این پیام را می‌بینید: `go_to_server ready`.

در **ترمینال دوم** (با `source /opt/ros/jazzy/setup.bash` و `source ~/ros2_goto_ws/install/setup.bash`):
```bash
ros2 node list                 # /sim و /go_to_server
ros2 action list               # /go_to_point
ros2 action info /go_to_point  # سرور و کلاینت‌ها
ros2 service list -t | grep set_speed
rqt_graph                      # نمودار نودها و تاپیک‌ها
```

### گام ۵: فرستادن هدف با دستور آماده‌ی ROS
```bash
ros2 action send_goal /go_to_point turtle_interfaces/action/GoToPoint "{x: 8.0, y: 3.0}" --feedback
```
لاک‌پشت اول به سمت هدف می‌چرخد، بعد حرکت می‌کند و در ترمینال `distance_remaining` را می‌بینید. در پایان `success: true`.

### گام ۶: فرستادن هدف با کلاینت پایتونی خودمان
```bash
ros2 run turtle_goto go_to_client 8 3
```
چند نقطه پشت‌سرهم (یک مربع):
```bash
ros2 run turtle_goto go_to_client 2 2  9 2  9 9  2 9
```
حالا هم **Goal** را فرستادید، هم **Feedback** گرفتید و هم **Result**.

### گام ۷: لغو (Cancel)
یک هدف دور بفرستید و وسط راه `Ctrl+C` بزنید:
```bash
ros2 run turtle_goto go_to_client 10 10
```
کلاینت درخواست لغو می‌فرستد و سرور لاک‌پشت را متوقف می‌کند (`Goal cancelled`).

### گام ۸: سرویس تغییر سرعت
```bash
ros2 service call /set_speed turtle_interfaces/srv/SetSpeed "{max_linear: 0.8, max_angular: 1.0}"
```
حالا دوباره هدف بفرستید؛ لاک‌پشت کندتر می‌رود. مقدار نامعتبر هم امتحان کنید:
```bash
ros2 service call /set_speed turtle_interfaces/srv/SetSpeed "{max_linear: -1.0, max_angular: 1.0}"
```
پاسخ: `success: false` با پیام خطا.

### گام ۹: پارامتر
```bash
ros2 param list
ros2 param set /go_to_server tolerance 0.5
```
از هدف بعدی به بعد، لاک‌پشت با فاصله‌ی ۰.۵ از هدف «رسیده» حساب می‌شود.

### گام ۱۰: حالت‌های رد شدن هدف
- هدف بیرون از محدوده (مثلاً `x=20`) ← **Rejected**
- ارسال هدف دوم وقتی اولی هنوز در حال اجراست ← **Rejected**
- اجرای سرور بدون turtlesim ← **Rejected** (هنوز pose نرسیده)

### گام ۱۱: تست فرمول کنترل (بدون ROS)
```bash
cd ~/ros2_goto_ws
colcon test --packages-select turtle_goto
colcon test-result --verbose
```

---

## بخش ۳: توضیح کد

### فرمول کنترل (`control.py`)
با فرض اختلاف مکان تا هدف `dx` و `dy`:

1. `distance = sqrt(dx² + dy²)`: فاصله تا هدف
2. `heading_error = atan2(dy, dx) − θ`: زاویه‌ای که باید بچرخد (به بازه‌ی −π تا π نرمال می‌شود)
3. `angular = 4 × heading_error`: هرچه بیشتر کج باشد، سریع‌تر می‌چرخد
4. `linear = 1.5 × distance`: هرچه دورتر باشد، سریع‌تر می‌رود
5. اگر `|heading_error| > 0.5` رادیان باشد، `linear = 0` (اول در جا بچرخ، بعد حرکت کن)
6. هر دو در محدوده‌ی `max_linear` و `max_angular` بریده می‌شوند.

به این روش **کنترل تناسبی (P-controller)** می‌گویند.

### سرور (`go_to_server.py`)
- `on_pose`: هر پیام Pose جدید را ذخیره می‌کند.
- `on_set_speed`: **Service callback**؛ سرعت‌ها را بعد از اعتبارسنجی عوض می‌کند.
- `on_goal`: تصمیم می‌گیرد هدف **پذیرفته یا رد** شود.
- `on_cancel`: درخواست لغو را می‌پذیرد.
- `execute`: حلقه‌ی ۲۰ هرتزی؛ هر بار به ترتیب بررسی می‌کند: لغو شده؟ رسیده؟ زمان تمام شده؟ وگرنه فرمان سرعت بفرست و Feedback بده.
- `succeed()` / `abort()` / `canceled()`: وضعیت نهایی هدف را به کلاینت اعلام می‌کنند.

### کلاینت (`go_to_client.py`)
- `send_goal_async`: هدف را می‌فرستد و `feedback_callback` را ثبت می‌کند.
- `get_result_async`: منتظر نتیجه می‌ماند.
- `cancel_goal_async`: هنگام `Ctrl+C` هدف را لغو می‌کند.

---

## بخش ۴: عیب‌یابی

| مشکل | علت و راه‌حل |
|---|---|
| `No module named 'turtle_interfaces'` | `source install/setup.bash` را در همین ترمینال نزده‌اید، یا رابط‌ها ساخته نشده‌اند |
| `Package 'turtle_goto' not found` | بعد از build، `source install/setup.bash` را بزنید |
| `Action server not found` | سرور اجرا نیست یا ترمینال source نشده است |
| `Goal was rejected` | بیرون از محدوده، هدف دیگر در حال اجراست، یا turtlesim اجرا نیست |
| لاک‌پشت تکان نمی‌خورد | `ros2 topic echo /turtle1/cmd_vel` را ببینید؛ نام نود شبیه‌ساز باید `turtlesim_node` باشد |
| بعد از ویرایش کد تغییری دیده نمی‌شود | دوباره `colcon build` بزنید و `source` کنید |

---

## بخش ۵: تمرین

1. یک پارامتر `k_linear` به سرور اضافه کنید.
2. سرویس `/stop` بسازید که هدف جاری را متوقف کند.
3. با `/spawn` در turtlesim لاک‌پشت دوم بسازید و سرور را برای `turtle2` عمومی کنید (نام لاک‌پشت پارامتر شود).
4. کلاینتی بنویسید که مسیر یک ستاره را طی کند.
5. همین پروژه را با C++ (`rclcpp` و `rclcpp_action`) بنویسید.
