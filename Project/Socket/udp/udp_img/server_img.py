#!/usr/bin/python3
import cv2
import socket
import numpy as np
import threading

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

def recv_img(sock):
    while True:
        data, addr = sock.recvfrom(1024)

        file_W(data)
        if data == b'done':
            print('done')
            break

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("", 9090))

#recvThread = threading.Thread(None, target=recv_img, args=sock)
recv_img(sock);

sock.close()

