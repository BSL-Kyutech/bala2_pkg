import cv2
import requests
import numpy as np
from ultralytics import YOLO
import time

# ArduinoのIPアドレスとポートを指定
url = "http://172.17.39.97/"

# create yolo model
print('create yolo model')
model = YOLO("yolov8n.pt")
# クラス名の取得
names = model.names

# ストリームを開く
print('opening streaming')
cap = requests.get(url, stream=True)
print('done')

def detect_img(model, image):
    results = model(img)
    
    return results

count = 0



# データを解析して画像を抽出
if cap.status_code == 200:
    bytes = bytes()
    for chunk in cap.iter_content(chunk_size=1024):
        bytes += chunk
        a = bytes.find(b'\xff\xd8')
        b = bytes.find(b'\xff\xd9')
        if a != -1 and b != -1:
            jpg = bytes[a:b+2]
            bytes = bytes[b+2:]
            img = cv2.imdecode(np.frombuffer(jpg, dtype=np.uint8), cv2.IMREAD_COLOR)
            
            if img is not None:
                print('detecting')
                #cv2.imshow('Stream', img)
                print('count : ', count)
                count += 1
                #detect_img = model.predict(img)
                results = detect_img(model, img)

                for result in results:
                    boxes = result.boxes
                    for box in boxes:
                        x1, y1, x2, y2 = box.xyxy[0].tolist()
                        conf = box.conf[0]
                        cls = box.cls[0].item()
                        label = names[int(cls)]

                        # draw boxes and labes  
                        cv2.rectangle(img, (int(x1), int(y1)), (int(x2), int(y2)), (0, 255, 0), 2)
                        cv2.putText(img, f'{label} {conf:.2f}', (int(x1), int(y1) - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

                # 結果の表示
                cv2.imshow("YOLO Object Detection", img)
                #cv2.imshow("stream", result_img)
                #cv2.imshow('Stream', detect_img)
                if cv2.waitKey(1) == 27:
                    break
else:
    print("Failed to open stream")

cv2.destroyAllWindows()