import cv2
cap = cv2.VideoCapture(0)                       # وب‌کم USB
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)             # کاهش صف فریم‌ها
ok, frame = cap.read()

from picamera2 import Picamera2                 # دوربین CSI رزبری‌پای
cam = Picamera2()
cam.configure(cam.create_preview_configuration(main={"size": (640, 480), "format": "RGB888"}))
cam.start()
frame = cam.capture_array()   # با RGB888 خروجی در عمل به ترتیب BGR است و مستقیم به OpenCV می‌رود
