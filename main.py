import cv2
import numpy as np
import requests

# 讓猜有哪幾種數字在裡面?
# 提示：小於五種
# "1","2",""....?
# 猜對了才能編過去...
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
    if category == ...?: 
        # 顯示？光明的顏色是什麼？閃光？
        mask = cv2.fillPoly(mask, [points_array], 這要填什麼？)
    else:
        mask = cv2.fillPoly(mask,[points_array],category_types.index(category))

cv2.imwrite("result.jpg",mask)