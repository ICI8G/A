#pyqt
import  sys
from PyQt5.QtWidgets import QSystemTrayIcon, QApplication, QWidget, QMenu, QPushButton, QLineEdit, QLabel, QCheckBox, QAction
from PyQt5.QtGui import QIcon
from MyThread import *


class SeleniumXXT(QWidget):


    def __init__(self):
        super().__init__()


        self.setWindowTitle('设置配置')
        self.resize(400, 300)   # set dialog size to 400*300


        ###
        # 配置组件
        ###

        #账号
        self.userLabel = QLabel('账号', self)
        self.userLabel.move(50,50)

        self.userInput = QLineEdit(self)
        self.userInput.move(100,50)

        #密码
        self.pswdLabel = QLabel('密码', self)
        self.pswdLabel.move(50,100)

        self.pswdInput = QLineEdit(self)
        self.pswdInput.move(100,100)
        self.pswdInput.setEchoMode(QLineEdit.Password)#遮挡密码
        

        #url
        self.urlLabel = QLabel('课程url', self)
        self.urlLabel.move(30,150)

        self.urlInput = QLineEdit(self)
        self.urlInput.move(100,150)
        self.urlInput.setMaxLength(250)


        #习题
        self.haveQuizBtn = QCheckBox("有视频后习题",self)
        self.haveQuizBtn.setChecked(False)
        self.haveQuizBtn.move(50,200)


        #后台
        self.headlessBtn = QCheckBox("后台运行",self)
        self.headlessBtn.setChecked(True)
        self.headlessBtn.move(200,200)


        #开始按钮
        self.btn = QPushButton('启动', self)
        self.btn.move(280,250)
        self.btn.clicked.connect(self.start)

        
        #托盘
        self.menu = QMenu()
        self.endAction = QAction('退出')
        self.endAction.setIcon(QIcon(r"Pic/quitIcon.png"))
        self.endAction.triggered.connect(self.closeExe)
        self.menu.addAction(self.endAction)


        self.startAction = QAction('开始')
        self.startAction.setIcon(QIcon(r"Pic/startIcon.png"))
        self.startAction.triggered.connect(self.showUI)        
        self.menu.addAction(self.startAction)

        self.tray = QSystemTrayIcon(self)
        self.tray.setIcon(QIcon( r"Pic/trayIcon.png"))#托盘图标
        self.tray.setContextMenu(self.menu)


        self.tip="未启动"
        self.tray.setToolTip(self.tip)
        self.tray.show()


        #成员变量
        self.user=""
        self.pswd=""
        self.url=""
        self.haveQuiz=False
        self.headless=True
        self.process=""
        self.myThread=MyThread()

        self.myThread.tipSignal.connect(self.changeTip)




    #更新状态提醒
    def changeTip(self,tip):
        self.tip=tip
        self.tray.setToolTip(self.tip)
        

    #更新值
    def updateData(self):
        self.user=self.userInput.text()
        self.pswd=self.pswdInput.text()
        self.url=self.urlInput.text()
        
        if(self.haveQuizBtn.checkState() == 0):
            self.haveQuiz=False
        else:
            self.haveQuiz=True
        
        if(self.headlessBtn.checkState() == 0):
            self.headless=False
        else:
            self.headless=True

        self.myThread.set(self.user, self.pswd, self.url, self.haveQuiz,self.headless)

    #启动任务
    def start(self):
        self.hide()
        self.updateData()
        self.myThread.start()


    #显示主窗体
    def showUI(self):
        self.show()

    #关闭程序
    def closeExe(self):
        self.tray.hide()
        app=QApplication.instance()
        app.quit()
        sys.exit()




if __name__ == '__main__':

    app = QApplication(sys.argv)
    app.setQuitOnLastWindowClosed(False) #关闭窗体但不关闭进程
    test = SeleniumXXT()
    sys.exit(app.exec_())