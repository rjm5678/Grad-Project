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

def read_img(path):

##    ret, img = cap.read()
    img = cv2.imread(path)
    img = cv2.resize(img, (640, 480))
    img_encode = cv2.imencode('.jpg', img)[1]
    data_encode = np.array(img_encode)
    str_encode = data_encode.tostring()
    file_len = len(str_encode)

    return str_encode, file_len


##cap = cv2.VideoCapture(0)
data, file_len = read_img('./0.jpg')

sock = get_socket()
for i in range(0, file_len, 1024):
    send_kbytes(data[i: i+1024], sock)
    time.sleep(0.01)

##message, addr = sock.recvfrom(1024)
##print('Received from server: {}'.format(message))
sock.sendto(b'done', ("47.102.216.81", 9090))
    
    
sock.close()
