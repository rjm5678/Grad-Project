#!/usr/bin/python3
import os
import cv2
import platform
import time
from datetime import datetime
import numpy as np
from predict import predict
from scipy.misc import imresize
from multiprocessing import Process
from keras.models import model_from_json
from server_demo import *
import socket


img_size = 64
channel_size = 1


def main():
    # Getting model:
    model_file = open('Data/Model/model.json', 'r')
    model = model_file.read()
    model_file.close()
    model = model_from_json(model)
    # Getting weights
    model.load_weights("Data/Model/weights.h5")

    print('Press "ESC" button for exit.')

    # Get image from camera, get predict and say it with another process, repeat.
    old_char = ''

    # Get image from client
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(("", 9090))

    while True:
        
        addr = recv_img(sock);
        if addr != False:
            img = cv2.imread('recv.jpg')
            try:
                img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
                
                # Cropping image:
                img_height, img_width = img.shape[:2]
                side_width = int((img_width-img_height)/2)
                img = img[0:img_height, side_width:side_width+img_height]

                img = imresize(img, (img_size, img_size, channel_size))
                img = 1-np.array(img).astype('float32')/255.
                img = img.reshape(1, img_size, img_size, channel_size)
                
                Y_string, Y_possibility = predict(model, img)
            
            
            

                if Y_possibility < 0.4: # For secondary vocalization
                    old_char = ''

                if old_char != Y_string and Y_possibility > 0.95:
                    print(Y_string, Y_possibility)
            ##      arg = 'say {0}'.format(Y_string)
                    # Say predict with multiprocessing
            ##      Process(target=os.system, args=(arg,)).start()
                    old_char = Y_string
                sock.sendto((Y_string).encode('utf-8'), addr)

            except:
                print("error")

if __name__ == '__main__':
    main()
