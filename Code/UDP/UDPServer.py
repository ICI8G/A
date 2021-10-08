from socket import *
import threading
import datetime

serverPort = 15201#开放端口
serverSocket = socket(AF_INET, SOCK_DGRAM)

serverSocket.bind(('',serverPort))

clientAddressList={}

print('ready')

def go():
    while True:
        message, clientAddress = serverSocket.recvfrom(2048)
        modifiedMessage = eval(message.decode(encoding='GB2312'))

        if modifiedMessage[1]=='login':
            clientAddressList.update({modifiedMessage[0]:clientAddress})
            print(modifiedMessage[0]+' login in')

        else:
            serverSocket.sendto(((str)(datetime.datetime.now())+'\n'+modifiedMessage[0]+' : '+modifiedMessage[2]).encode(encoding='GB2312'), clientAddressList[modifiedMessage[1]])

            print((str)(datetime.datetime.now())+'\n'+modifiedMessage[0]+' : '+modifiedMessage[2])

t1=threading.Thread(target=go)
t2=threading.Thread(target=go)
t3=threading.Thread(target=go)
t4=threading.Thread(target=go)

t1.start()
t2.start()
t3.start()
t4.start()