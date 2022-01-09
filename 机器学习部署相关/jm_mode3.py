import pandas as pd
import numpy as np
import sys
from sklearn.svm import SVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.decomposition import PCA
import joblib

# 导入模型并fit
rfc = joblib.load("/root/anaconda3/myprojects/jm/rfc_mode3.m")
# svc = joblib.load("/root/anaconda3/myprojects/jm/svc_model.m")
train_data = np.loadtxt('/root/anaconda3/myprojects/jm/jm_data_mode3.csv', dtype=int, delimiter=',', unpack=False)
X = train_data[:, :17]
y = train_data[:, 17]

# 压力数据
test_data = []
for i in range(0, len(sys.argv) - 1):
	test_data.append(int(sys.argv[i + 1]))
test_data = np.array(test_data).reshape(1, -1)  #转换成array元素

# 数据标准化
ss=StandardScaler()
ss.fit(X)
x_new = ss.fit_transform(X)
test_data_new = ss.transform(test_data)

# pca降维
#pca = PCA(n_components=5, random_state=10)
#pca.fit(x_new)
#x_new2 = pca.fit_transform(x_new)
#test_data_new2 = pca.transform(test_data_new)

#rfc.fit(X,y)

# 预测并输出结果
print(rfc.predict(test_data_new)[0])
#def pre_return():
#    return rfc.predict(np.array(test_data).reshape(1, -1))[0]
#pre_return()
# print(type(rfc.predict(np.array(test_data).reshape(1, -1))))
# print("预测结果为：", svc.predict(np.array(test_data).reshape(1, -1))[0])
# print(type(svc.predict(np.array(test_data).reshape(1, -1))))
