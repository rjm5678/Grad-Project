# Arda Mavi
import sys
import random
import os
import numpy as np
import matplotlib.pyplot as plt
from get_dataset import get_img
from scipy.misc import imresize
from database_process import get_data
from keras.models import model_from_json

image_size = 64
channel_size = 1

def predict(model, X): # Return: Y String , Y Possibility
    Y = model.predict(X)
    Y_index = np.argmax(Y, axis=1)
    Y_string = get_data('SELECT char FROM "id_char" WHERE id={0}'.format(Y_index[0]))
#    Y_string = Y_index[0]
    
    return Y_string[0][0], Y[0][Y_index][0]
#    return Y_string, Y[0][Y_index[0]]

def predict_model():
    n = random.randrange(0, 6, 5)
    all_img = os.listdir('./Data/Train_Data/{}'.format(n))
    n_img = random.randint(0, len(all_img)-1)

    img_dir = "./Data/Train_Data/{}/{}".format(n, all_img[n_img])
    img = 1-np.array(get_img(img_dir)).astype('float32')/255.
    img = img.reshape(1, image_size, image_size, channel_size)
    # Getting model:
    model_file = open('Data/Model/model.json', 'r')
    model = model_file.read()
    model_file.close()
    model = model_from_json(model)
    # Getting weights
    model.load_weights("Data/Model/weights.h5")
    Y_string, Y_possibility = predict(model, img)
    print('Label:', n, 'Class:', Y_string, '\nPossibility:', Y_possibility)
def predict_model():
    all_img = os.listdir('./testset')
    P, Y = [], []
    for i in range(len(all_img)):
        n_img = random.randint(0, len(all_img)-1)
        
        img_dir = './testset/{}'.format(all_img[n_img])
        n = all_img[n_img][0]
        img = 1-np.array(get_img(img_dir)).astype('float32')/255.
        img = img.reshape(1, image_size, image_size, channel_size)

        
        # Getting model:
        model_file = open('Data/Model/model.json', 'r')
        model = model_file.read()
        model_file.close()
        model = model_from_json(model)
        # Getting weights
        model.load_weights("Data/Model/weights.h5")
        Y_string, Y_possibility = predict(model, img)
        Y.append(n)
        P.append(Y_string)
    P = np.array(P)
    Y = np.array(Y)
    acc = np.sum(P==Y) / len(all_img)
        #print('Label:', n, 'Class:', Y_string, '\nPossibility:', Y_possibility)
    print(acc)
    
if __name__ == '__main__':
##    img_dir = sys.argv[1]
    img_dir = "./Data/Train_Data/2/IMG_1171.JPG"
    img_dir = './IMG_20190218_200042.jpg'
    img = 1-np.array(get_img(img_dir)).astype('float32')/255.
    img = img.reshape(1, image_size, image_size, channel_size)
    # Getting model:
    model_file = open('Data/Model/model.json', 'r')
    model = model_file.read()
    model_file.close()
    model = model_from_json(model)
    # Getting weights
    model.load_weights("Data/Model/weights.h5")
    Y_string, Y_possibility = predict(model, img)
    print('Class:', Y_string, '\nPossibility:', Y_possibility)
