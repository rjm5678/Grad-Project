#!/usr/bin/python3
import cv2
import socket
import numpy as np
import threading

def file_W(data):
    with open('./0.jpg', mode = 'ab') as img_W:
        img_W.write(data)

def recv_img(sock):
    data, addr = sock.recvfrom(1024)
    if data == b'begin':
        sock.sendto(b'begin recv', addr)
    while True:
        data, addr = sock.recvfrom(1024)

        file_W(data)
        if data == b'done':
            print('done\n')
            break

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("", 9090))

#recvThread = threading.Thread(None, target=recv_img, args=sock)
recv_img(sock);

sock.close()

