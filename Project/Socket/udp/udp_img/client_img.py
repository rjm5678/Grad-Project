#!/usr/bin/python3
import cv2
import socket
import numpy as np

def file_W(data):
    with open('./0.jpg', mode = 'ab') as img_W:
        img_W.write(data)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("", 9090))

while True:
    data, addr = sock.recvfrom(1024)
    print("Received from addr: {}".format(addr))

    file_W(data)
    sock.sendto(b'ok', addr)
    if data == b'done':
        print('done')
        break
