import cv2, numpy as np, glob

pattern = (9, 6)          # تعداد گوشه‌های داخلی (نه تعداد خانه‌ها)
square = 0.025            # طول ضلع هر خانه (متر)
objp = np.zeros((pattern[0] * pattern[1], 3), np.float32)
objp[:, :2] = np.mgrid[0:pattern[0], 0:pattern[1]].T.reshape(-1, 2) * square

obj_pts, img_pts = [], []
crit = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
for f in sorted(glob.glob("calib/*.png")):
    gray = cv2.cvtColor(cv2.imread(f), cv2.COLOR_BGR2GRAY)
    ok, corners = cv2.findChessboardCorners(gray, pattern)
    if ok:
        corners = cv2.cornerSubPix(gray, corners, (11, 11), (-1, -1), crit)
        obj_pts.append(objp); img_pts.append(corners)

rms, K, dist, _, _ = cv2.calibrateCamera(obj_pts, img_pts, gray.shape[::-1], None, None)
print("RMS reprojection error (px):", round(rms, 3))   # هدف: کمتر از ۰٫۵ تا ۱
print("fx, fy, cx, cy:", K[0, 0], K[1, 1], K[0, 2], K[1, 2])
np.savez("camera.npz", K=K, dist=dist)
# بعداً: und = cv2.undistort(frame, K, dist)
