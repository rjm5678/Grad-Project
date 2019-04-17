#!/usr/bin/python3
import cv2
import socket
import numpy as np

def file_W(data):
    with open('./0.jpg', mode = 'ab') as img_W:
        img_W.write(data)

def read_img(img_path):
    img = cv2.imread(img_path)
    img = cv2.resize(img, (640, 480))
    img_encode = cv2.imencode('.jpg', img)[1]
    data_encode = np.array(img_encode)
    str_encode = data_encode.tostring()
    file_len = len(str_encode)

    return str_encode, file_len

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("", 9090))

while True:
    data, addr = sock.recvfrom(1024)

    file_W(data)
    sock.sendto(b'ok', addr)
    if data == b'done':
        print('done')
        break
sock.close()

