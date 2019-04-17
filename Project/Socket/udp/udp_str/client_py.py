import socket
import cv2

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

while True:
    message = input("q to quit: ")


    sock.sendto(message.encode("utf-8"), ("47.102.216.81", 9090))
    message, addr = sock.recvfrom(102400)
    print('Received from server: {}'.format(message))
    if message == b'q':
        break

sock.close()    