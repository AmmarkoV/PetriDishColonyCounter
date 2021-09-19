# HTTP Multipart MJPEG downloader .. 
# Edited version to work with HTTP Multipart MJPEG
# Based on https://github.com/sglvladi/TensorFlowObjectDetectionTutorial

#pip install numpy matplotlib pil opencv-python --user

import numpy as np
import os
import six.moves.urllib as urllib
import sys
import tarfile
import zipfile
import cv2

from collections import defaultdict
from io import StringIO
from matplotlib import pyplot as plt
from PIL import Image

# Define the video stream
stream = urllib.request.urlopen('http://192.168.1.33:80', timeout = 5) # host address
bytebuffer = bytes()
should_stop = False
windowname = 'Object Detection'


# Helper code
def load_image_into_numpy_array(image):
    (im_width, im_height) = image.size
    return np.array(image.getdata()).reshape(
        (im_height, im_width, 3)).astype(np.uint8)
 
while not should_stop:

            if should_stop:
                break

            # Read frame from camera
            while True:
                bytebuffer += stream.read(1024)
                a = bytebuffer.find(b'\xff\xd8')
                b = bytebuffer.find(b'\xff\xd9')
                if a != -1 and b != -1:
                    jpg = bytebuffer[a:b+2]
                    bytebuffer = bytebuffer[b+2:]
                    image_np = cv2.imdecode(np.frombuffer(jpg, dtype=np.uint8), cv2.IMREAD_COLOR)
                    break
 
            # Display output
            cv2.imshow(windowname, cv2.resize(image_np, (800, 600)))

            if cv2.waitKey(10) & 0xff == ord('q'):
                cv2.destroyAllWindows()
                should_stop = True
                break
