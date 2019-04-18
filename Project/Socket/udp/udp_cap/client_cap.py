import socket
import cv2
import struct
import time
import numpy as np

def get_socket():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    return sock

def send_kbytes(data, sock):
    sock.sendto(data, ("47.102.216.81", 9090))
 
    return 0

def encode_img(img):

    

    img = cv2.resize(img, (640, 480))
    img_encode = cv2.imencode('.jpg', img)[1]
    data_encode = np.array(img_encode)
    str_encode = data_encode.tostring()
    data_len = len(str_encode)

    return str_encode, data_len

def send_img(data, file_len, sock):
    for i in range(0, file_len, 1024):
        send_kbytes(data[i: i+1024], sock)
      #  time.sleep(0.01)
      
    sock.sendto(b'done', ("47.102.216.81", 9090))
    
def file_W(data, path):
    with open(path, mode = 'ab') as img_W:
        img_W.write(data)
        
def clean_file(path):
    with open(path, mode = 'w') as img_W:
        img_W.write('')
        
def recv_img(sock, addr):
    while True:
        data, addr = sock.recvfrom(1024)
        file_W(data, './../{}.jpg'.format(0))
        if data == b'done':
            print('done')
            break
       
cap = cv2.VideoCapture(0)

sock = get_socket()
sock.sendto(b'begin', ("47.102.216.81", 9090))
while True:
    ret, img = cap.read()
    data, data_len = encode_img(img)
    send_img(data, data_len, sock)

    recv_img(sock, ("47.102.216.81", 9090))
    img = cv2.imread('./../{}.jpg'.format(0))
    cv2.imshow('47.102.216.81', img)
    clean_file('./../{}.jpg'.format(0))
    if cv2.waitKey(5) == 27: # Decimal 27 = Esc
        break
    

    
    
##message, addr = sock.recvfrom(1024)
##print('Received from server: {}'.format(message))

    
    
sock.close()
cap.release()
