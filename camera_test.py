import cv2

# Try opening camera index 1
cap = cv2.VideoCapture(1)

if not cap.isOpened():
    print("ERROR: Unable to open camera at index 1")
    exit()

print("Camera at index 1 opened successfully. Press 'q' to quit.")

while True:
    ret, frame = cap.read()

    if not ret:
        print("Failed to grab frame")
        break

    cv2.imshow("USB Camera Index 1", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()