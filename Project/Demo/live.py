# Arda Mavi
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
    cap = cv2.VideoCapture(0)
    old_char = ''
    time_start = time.time()
    framecount = 0
    fps = ""
    while 1:
        t1 = time.time()
        ret, img_raw = cap.read()
        
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
        
        # Cropping image:
        img_height, img_width = img.shape[:2]
        side_width = int((img_width-img_height)/2)
        img = img[0:img_height, side_width:side_width+img_height]

        framecount = framecount + 1
        time_end  = time.time()
        timediff = (time_end - time_start)
        if( timediff >= 1):
            #timediff = end - start
            fps = 'FPS:%s' %(framecount)
            time_start = time.time()
            framecount = 0
        img_raw = cv2.flip(img_raw, 1)
        img = cv2.flip(img, 1)
        cv2.putText(img_raw,fps,(10,20),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7,(0,255,0),2,1)
        
        # Show window:
        cv2.imshow('VSL', img) # cv2.flip(img,1) : Flip(mirror effect) for easy handling.
        cv2.imshow('RAW', img_raw)
##        img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        img = imresize(img, (img_size, img_size, channel_size))
        img = 1-np.array(img).astype('float32')/255.
        img = img.reshape(1, img_size, img_size, channel_size)
        
        Y_string, Y_possibility = predict(model, img)
        
        if Y_possibility < 0.4: # For secondary vocalization
            old_char = ''
			
        if(platform.system() == 'Windows') and old_char != Y_string and Y_possibility > 0.95:
##            time_end = datetime.now()
##            if (time_end - time_start).seconds >= 1 :
##                time_start = datetime.now()
            print(Y_string, Y_possibility)
##            arg = 'say {0}'.format(Y_string)
            # Say predict with multiprocessing
##            Process(target=os.system, args=(arg,)).start()
            old_char = Y_string

        if cv2.waitKey(5) == 27: # Decimal 27 = Esc
            break
        
    cap.release()
    cv2.destroyAllWindows()

		

if __name__ == '__main__':
    main()
