#!/usr/bin/python3
import cv2
import socket
import numpy as np
import threading

def file_W(data, path):
    with open(path, mode = 'ab') as img_W:
        img_W.write(data)

def send_kbytes(data, sock, addr):
    sock.sendto(data, addr)

def encode_img(img_path):
    img = cv2.imread(img_path)
    img = cv2.resize(img, (640, 480))
    img_encode = cv2.imencode('.jpg', img)[1]
    data_encode = np.array(img_encode)
    str_encode = data_encode.tostring()
    file_len = len(str_encode)

    return str_encode, file_len

def recv_img(sock, addr=("", 9090), path = './0.jpg'):
    while True:
        data, addr = sock.recvfrom(1024)

        file_W(data, path)
        if data == b'done':
            break

    return True

def clean_file(path):
    with open(path, mode = 'w') as img_W:
        img_W.write('')

def send_img(data, data_len, sock, addr):
    for i in range(0, data_len, 1024):
        send_kbytes(data[i: i+1024], sock, addr)
    
    sock.sendto(b'done', addr)

    return True


