import cv2
import numpy as np
import serial
import time

# Serial communication setup with Arduino
try:
    arduino = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    time.sleep(2)  # Wait for serial connection to stabilize
    print("Connected to Arduino on /dev/ttyACM0")
except Exception as e:
    print(f"Serial connection error: {e}")
    arduino = None

# Initialize video capture
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

# Green detection HSV bounds
lower_green = np.array([35, 50, 50])
upper_green = np.array([85, 255, 255])

# Area filter threshold to eliminate tiny noise boxes
MIN_CONTOUR_AREA = 1000

# Variables to prevent redundant bounding box creation
last_x = -1
last_y = -1
POS_TOLERANCE = 15

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    # Convert BGR frame to HSV
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    # Create mask for green color
    mask = cv2.inRange(hsv, lower_green, upper_green)
    mask = cv2.erode(mask, None, iterations=2)
    mask = cv2.dilate(mask, None, iterations=2)

    # Find contours
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    largest_contour = None
    max_area = 0

    for cnt in contours:
        area = cv2.contourArea(cnt)
        if area > MIN_CONTOUR_AREA:
            if area > max_area:
                max_area = area
                largest_contour = cnt

    if largest_contour is not None:
        # Bounding box calculation
        x, y, w, h = cv2.boundingRect(largest_contour)
        cx = x + (w // 2)
        cy = y + (h // 2)

        # Draw bounding box and centroid
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        cv2.circle(frame, (cx, cy), 5, (0, 0, 255), -1)

        # Temporal check: Only transmit if target location shifted significantly
        if abs(cx - last_x) > POS_TOLERANCE or abs(cy - last_y) > POS_TOLERANCE:
            last_x = cx
            last_y = cy
            
            # Transmit X and Y coordinates separated by a comma or prefix
            data_string = f"X{cx}Y{cy}\n"
            if arduino and arduino.is_open:
                arduino.write(data_string.encode('utf-8'))
                print(f"Sent: {data_string.strip()}")

    # Display feeds
    cv2.imshow("Camera View", frame)
    cv2.imshow("Mask", mask)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
if arduino and arduino.is_open:
    arduino.close()