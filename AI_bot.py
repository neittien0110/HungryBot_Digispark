from cProfile import label
import mysql.connector
import datetime as dt     # Phải có (dù ko dùng trực tiếp) để python convert dạng timeserial sang dattime phù hợp với numpy
import matplotlib.pyplot as plt
import numpy as np
from pandas import NA
from pyparsing import alphanums, alphas

#-------------------------------------------------------------------------------
DATA_FROM="2022-03-28 12:00:00"
DATA_TO="2022-03-28 23:59:00"
DATA_ATTRIBUTE1="Eating"  # "Lumination"  "Eating"
DATA_ATTRIBUTE2="Lumination"  # "Lumination"  "Eating"

MYSQL_ACCOUNT="siot_getdata"
MYSQL_PASSWORD=""
#-------------------------------------------------------------------------------

# Thông tin kết nối cơ sở dữ liệu
mydb = mysql.connector.connect(
  host="sinno.soict.ai",
  user=MYSQL_ACCOUNT,
  password=MYSQL_PASSWORD,
  database="siot"
)

# Kết nối cơ sở dữ liệu
mycursor = mydb.cursor()

# Truy vấn để lấy toàn bộ thông tin của 1 Thiết bị nào đó và Thuộc tính nào đó
mycursor.execute("SELECT created_at, value, name FROM AttributesOfDevice "+
                  "where (device_id = '15ff0f4d-2f4e-4eae-bb3e-0e9b92b893eb')" +
                  "and ((name='{}') or (name='{}')) ".format(DATA_ATTRIBUTE1,DATA_ATTRIBUTE2) +
                  "and (created_at >='{}' and created_at <='{}')".format(DATA_FROM,DATA_TO) + 
                  "")

# Lấy dữ liệu
myresult = mycursor.fetchall()

# Hiển thị dữ liệu để kiểm tra
for myRecord in myresult:    
    #print(myRecord)
    #print(myRecord[0], myRecord[1], myRecord[2])
    pass


# Đưa dữ liệu vào thư viện tính toán 
x1points = np.array([])
x2points = np.array([])
y1points = np.array([])
y2points = np.array([])
for myRecord in myresult:    
    if (myRecord[2] == DATA_ATTRIBUTE1):
      y1points = np.append(y1points, int(myRecord[1]))    # Dữ liệu bỏ qua khi vẽ đồ thị thì sử dụng np.NaN
      x1points = np.append(x1points, myRecord[0]) 
    else:
      y2points = np.append(y2points, int(myRecord[1]))   
      x2points = np.append(x2points, myRecord[0]) 
    pass

# Khai báo đồ thị kép, gồm 2 đồ thị
f, (ax1, ax2) = plt.subplots(2, sharex=True, sharey=False)
plt.title("Đồ thị theo thời gian của thiết bị từ hệ thống SIOT")
plt.xlabel("Datetime")
plt.xticks(rotation=75)         #Xoay nội dung text trục X 90 độ
ax1.plot(x1points, y1points)
ax1.legend(DATA_ATTRIBUTE1)

ax2.plot(x2points, y2points, 
         linestyle='solid',
         linewidth=1,
         color='green',                
         marker = 'o',        # Hình dạng của đốm đánh dấu số liệu. ['o', '.', ',', 'x', '+', 'v', '^', '<', '>', 's', 'd']
         markersize=4,        # Kích thước đốm tròn đánh dấu số liệu
         markevery=10,         # Chỉ hiện đốm đánh dấu mỗi 5 số liệu  
         markerfacecolor='white',
         markeredgecolor='gray',
         markeredgewidth=2         
         )
ax2.legend(DATA_ATTRIBUTE2)

plt.show()
exit()


plt.title("Đồ thị theo thời gian của thiết bị từ hệ thống SIOT")
plt.xlabel("Datetime")
plt.ylabel(DATA_ATTRIBUTE1)
plt.xticks(rotation=75)         #Xoay nội dung text trục X 90 độ
plt.plot(x1points, y1points, 
         linestyle='dashed',
         linewidth=2,
         color='green',                
         marker = 'o',        # Hình dạng của đốm đánh dấu số liệu. ['o', '.', ',', 'x', '+', 'v', '^', '<', '>', 's', 'd']
         markersize=4,        # Kích thước đốm tròn đánh dấu số liệu
         markevery=5,         # Chỉ hiện đốm đánh dấu mỗi 5 số liệu  
         markerfacecolor='white',
         markeredgecolor='gray',
         markeredgewidth=2         
         )
plt.show()