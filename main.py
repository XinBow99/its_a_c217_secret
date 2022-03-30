import cv2
import numpy as np
import requests

# 讓猜有哪幾種數字在裡面?
# "1","2",""....?
category_types = []
# 讀進檔案
img = cv2.imread("password.jpg")
h, w = img.shape[:2]
mask = np.zeros([h, w, 1], np.uint8)
label = requests.get('https://raw.githubusercontent.com/XinBow99/happy/main/password.json').json()


shapes = label["shapes"]
for shape in shapes:
    category = shape["label"]
    points = shape["points"]
    #填充
    points_array = np.array(points,dtype=np.int32)
    mask = cv2.fillPoly(mask, [points_array], category_types.index(category))
    for c in category_types:
        if category == c: 
            mask = cv2.fillPoly(mask, [points_array], 這缺了什麼?)
        else:
            mask = cv2.fillPoly(mask,[points_array],category_types.index(category))

cv2.imwrite("test.jpg",mask)