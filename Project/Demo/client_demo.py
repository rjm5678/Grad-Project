import socket
import cv2
import struct
import time
import numpy as np
from scipy.misc import imresize

img_size = 64
channel_size = 1
timeout = 0

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
    data_len = len(str_encode)

    return str_encode, data_len

def send_img(data, file_len, sock):
    send_kbytes(b'begin')
    
    if sock.recvfrom(1024)[0] == b'begin recv':
        for i in range(0, file_len, 1024):
            send_kbytes(data[i: i+1024])
            time.sleep(0.001)
##            timeout = timeout + 1
##
##            if timeout >= 1000:
##                timeout = 0
##                return False
            
        send_kbytes(b'done')
        return True
        
    return False
    
def file_W(data, path):
    with open(path, mode = 'ab') as img_W:
        img_W.write(data)

    return True        

def img_preprocessing(img_raw):
    img_HSV = cv2.cvtColor(img_raw, cv2.COLOR_BGR2HSV)
    #skin color range for hsv color space 
    HSV_mask = cv2.inRange(img_HSV, (0, 15, 0), (17,170,255)) 
    HSV_mask = cv2.morphologyEx(HSV_mask, cv2.MORPH_OPEN, np.ones((3,3), np.uint8))

    #converting from gbr to YCbCr color space
    img_YCrCb = cv2.cvtColor(img_raw, cv2.COLOR_BGR2YCrCb)
    #skin color range for hsv color space 
    YCrCb_mask = cv2.inRange(img_YCrCb, (0, 135, 85), (255,180,135)) 
    YCrCb_mask = cv2.morphologyEx(YCrCb_mask, cv2.MORPH_OPEN, np.ones((3,3), np.uint8))

    #merge skin detection (YCbCr and hsv)
    global_mask=cv2.bitwise_and(YCrCb_mask,HSV_mask)
    global_mask=cv2.medianBlur(global_mask,3)
    global_mask = cv2.morphologyEx(global_mask, cv2.MORPH_OPEN, np.ones((4,4), np.uint8))


    HSV_result = cv2.bitwise_not(HSV_mask)
    YCrCb_result = cv2.bitwise_not(YCrCb_mask)
    global_result=cv2.bitwise_not(global_mask)

    img = global_result

    return img

if __name__ == '__main__':
    cap = cv2.VideoCapture(0)
    sock = get_socket()
    while True:
        ret, img = cap.read()
        img = cv2.flip(img ,1)
##        img = cv2.imread('./train.jpg')
##        img = img_preprocessing(img)
##        cv2.imshow('47.102.216.81', img)
        data, data_len = encode_img(img)

        with open('send.jpg', mode = 'wb') as img_W:
            img_W.write(data)
              
        
        send_img(data, data_len, sock)

        
    sock.close()
    cap.release()
