import socket
##import cv2
import struct
import time
import numpy as np

def get_socket():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    return sock

def send_kbytes(data):
    sock.sendto(data, ("47.102.216.81", 9090))
 
    return True

def encode_img(img):    

    img = cv2.resize(img, (640, 480))
    img_encode = cv2.imencode('.jpg', img)[1]
    data_encode = np.array(img_encode)
    str_encode = data_encode.tostring()
    file_len = len(str_encode)

    return str_encode, file_len

def send_img(data, file_len, sock):
    send_kbytes(b'begin')
    
    if sock.recvfrom(1024)[0] == b'begin recv':
        for i in range(0, file_len, 1024):
            send_kbytes(data[i: i+1024])
            time.sleep(0.005)
            
        send_kbytes(b'done')
        return True
        
    return False

if __name__ == '__main__':
##    cap = cv2.VideoCapture(0)
##    img = cv2.imread('./1.jpg')
##    data, file_len = encode_img(img)
    
    with open('./1.jpg', mode = 'rb') as img_R:
        data = img_R.read()

    sock = get_socket()
    send_img(data, len(data), sock)
       
    sock.close()
##    cap.release()
