#!/usr/bin/python3
import cv2
import socket
import numpy as np
import threading

def file_W(data, path = 'recv.jpg'):
    with open(path, mode = 'ab') as img_W:
        img_W.write(data)
    return True

def send_kbytes(data, sock, addr):
    sock.sendto(data, addr)
    return True

def encode_img(img_path):
    img = cv2.imread(img_path)
    img = cv2.resize(img, (640, 480))
    img_encode = cv2.imencode('.jpg', img)[1]
    data_encode = np.array(img_encode)
    str_encode = data_encode.tostring()
    file_len = len(str_encode)

    return str_encode, file_len

def recv_img(sock, addr=("", 9090), path = './recv.jpg'):
    data, addr = sock.recvfrom(1024)
    if data == b'begin':
        sock.sendto(b'begin recv', addr)
        while True:
            data, addr = sock.recvfrom(1024)

            file_W(data)
            if data == b'done':
    #            print('done')
                break
        return True
    return False

def clean_file(path = 'recv.jpg'):
    with open(path, mode = 'w') as img_W:
        img_W.write('')
    return True

if __name__ == '__main__':
    
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(("", 9090))
    clean_file()
    #recvThread = threading.Thread(None, target=recv_img, args=sock)
    recv_img(sock);

    sock.close()

