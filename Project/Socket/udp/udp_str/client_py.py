import socket
import cv2

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
"""
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 100)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 100)
ret, img_raw = cap.read()
cv2.imshow('RAW', img_raw)
cv2.imwrite('0.jpg', img_raw)
"""
while True:
    message = input("q to quit: ")
    if message == 'q':
        break
    
    sock.sendto(message.encode("utf-8"), ("47.102.216.81", 9090))
    message, addr = sock.recvfrom(102400)
    print('Received from server: {}'.format(message))
    
##    if cv2.waitKey(5) == 27: # Decimal 27 = Esc
##        break
sock.close()
