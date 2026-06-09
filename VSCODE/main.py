import socket
import cv2
import struct
import mediapipe as mp
from mediapipe.tasks import python
from mediapipe.tasks.python import vision

# 1. Setup 2 Server Sockets: Một cho lệnh, một cho Video
cmd_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
cmd_server.bind(('127.0.0.1', 65432))
cmd_server.listen(1)

vid_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
vid_server.bind(('127.0.0.1', 65433))
vid_server.listen(1)

print("Python AI Engine: Đang đợi C++ kết nối...")
cmd_conn, addr1 = cmd_server.accept()
vid_conn, addr2 = vid_server.accept()
print(f"Đã kết nối! Bật Camera...")

# 2. Setup AI
base_options = python.BaseOptions(model_asset_path='hand_landmarker.task')
options = vision.HandLandmarkerOptions(base_options=base_options, num_hands=1)
detector = vision.HandLandmarker.create_from_options(options)

# Hàm đếm ngón tay
def count_fingers(landmarks):
    fingers = []
    if landmarks[4].x < landmarks[3].x: fingers.append(1)
    else: fingers.append(0)
    tips = [8, 12, 16, 20]
    for tip in tips:
        if landmarks[tip].y < landmarks[tip - 2].y: fingers.append(1)
        else: fingers.append(0)
    return sum(fingers)

# HÀM MỚI: Tự tay vẽ khung xương bằng OpenCV (Tuyệt đối không trượt đi đâu được)
def draw_bones(image, hand_landmarks):
    h, w, c = image.shape
    # Bản đồ nối 21 khớp xương tay
    connections = [
        (0, 1), (1, 2), (2, 3), (3, 4),         # Ngón cái
        (0, 5), (5, 6), (6, 7), (7, 8),         # Ngón trỏ
        (5, 9), (9, 10), (10, 11), (11, 12),    # Ngón giữa
        (9, 13), (13, 14), (14, 15), (15, 16),  # Ngón áp út
        (13, 17), (0, 17), (17, 18), (18, 19), (19, 20) # Ngón út
    ]
    
    # Quy đổi tọa độ AI ra pixel màn hình
    points = []
    for lm in hand_landmarks:
        cx, cy = int(lm.x * w), int(lm.y * h)
        points.append((cx, cy))
        
    # Kẻ các đường xương (Màu xanh lá)
    for conn in connections:
        cv2.line(image, points[conn[0]], points[conn[1]], (0, 255, 0), 2)
        
    # Chấm các khớp xương (Màu xanh dương)
    for point in points:
        cv2.circle(image, point, 4, (255, 0, 0), cv2.FILLED)

cap = cv2.VideoCapture(0)

while cap.isOpened():
    ret, frame = cap.read()
    if not ret: break

    # Lật ảnh giống gương để dễ điều khiển
    frame = cv2.flip(frame, 1)

    mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
    results = detector.detect(mp_image)

    cmd = 'S'
    if results.hand_landmarks:
        for hand_landmarks in results.hand_landmarks:
            
            # Gọi hàm vẽ xương tay tự chế
            draw_bones(frame, hand_landmarks)

            count = count_fingers(hand_landmarks)
            if count == 1: cmd = 'F'
            elif count == 2: cmd = 'B'
            elif count == 3: cmd = 'L'
            elif count == 4: cmd = 'R'
            
            cv2.putText(frame, f"CMD: {cmd} | Fingers: {count}", (10, 40), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

    # 3. Gửi Lệnh Sang C++
    try:
        cmd_conn.sendall((cmd + '\n').encode('utf-8'))
    except:
        break

    # 4. Nén và Gửi Video Livestream Sang C++
    _, buffer = cv2.imencode('.jpg', frame, [int(cv2.IMWRITE_JPEG_QUALITY), 80])
    size = len(buffer)
    try:
        vid_conn.sendall(struct.pack(">L", size))
        vid_conn.sendall(buffer.tobytes())
    except Exception as e:
        break

cap.release()
cmd_conn.close()
vid_conn.close()
