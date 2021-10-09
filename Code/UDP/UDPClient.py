from socket import *
from tkinter import *
import threading
import datetime

userName='发送人姓名'
sendTo='接收者姓名'
serverIpName = '服务器地址'
serverPort = 15200#端口号
clientSocket = socket(AF_INET, SOCK_DGRAM)#参数一：底层网络使用ipv4，参数二：一种UDP套接字类型

clientSocket.sendto(repr((userName, 'login')).encode(), (serverIpName,serverPort))#向套接字发送字节和分组,用于登陆

windows = Tk()#gui
windows.title('To: '+sendTo)

sendText = Text(windows)#发送本文框

receiveText = Text(windows)#接收文本框


def send():
    message = sendText.get("1.0","end")#让用户写一条报文
    sendText.delete("1.0","end")
    receiveText.insert(INSERT,(str)(datetime.datetime.now())+'\n'+userName+' : '+message)
    sentMessage = repr((userName, sendTo, message,(str)(datetime.datetime.now())))
    clientSocket.sendto((sentMessage).encode(encoding='GB2312'), (serverIpName,serverPort))#向套接字发送字节和分组

def receive():
    while True:
        modifiedMessage, serverAddress = clientSocket.recvfrom(2048)

        #print(modifiedMessage.decode(encoding='GB2312'))
        receiveText.insert(INSERT,modifiedMessage.decode(encoding='GB2312'))

sendButton = Button(windows,text='发送',width=10,height=5,command=send)#发送按钮

receiveText.pack(side = LEFT)
sendText.pack(side = LEFT)
sendButton.pack(side = RIGHT)


receiveThreading = threading.Thread(target=receive)
receiveThreading.start()

windows.mainloop()