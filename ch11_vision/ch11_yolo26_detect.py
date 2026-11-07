# نصب: pip install ultralytics
from ultralytics import YOLO
import cv2

model = YOLO("yolo26n.pt")                    # مدل nano؛ بار اول خودکار دانلود می‌شود
frame = cv2.imread("test.jpg")
res = model(frame, imgsz=320, conf=0.4, verbose=False)[0]
for b in res.boxes:
    x1, y1, x2, y2 = b.xyxy[0].tolist()
    name = model.names[int(b.cls)]
    print(name, round(float(b.conf), 2), "u =", round((x1 + x2) / 2))
