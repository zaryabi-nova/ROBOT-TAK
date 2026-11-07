import cv2

cap = cv2.VideoCapture(0)
cv2.namedWindow("tune")
for name, mx, v in [("H1", 179, 0), ("H2", 179, 10), ("S", 255, 120), ("V", 255, 70)]:
    cv2.createTrackbar(name, "tune", v, mx, lambda x: None)
while True:
    ok, frame = cap.read()
    if not ok:
        break
    g = lambda n: cv2.getTrackbarPos(n, "tune")
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, (g("H1"), g("S"), g("V")), (g("H2"), 255, 255))
    cv2.imshow("tune", mask)
    if cv2.waitKey(1) & 0xFF == ord("q"):          # با q مقدارها چاپ می‌شوند
        print("lower:", (g("H1"), g("S"), g("V")), "upper:", (g("H2"), 255, 255))
        break
