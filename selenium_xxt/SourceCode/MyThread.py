#pyqt
from PyQt5.QtCore import QThread, pyqtSignal

#selenium
from selenium.webdriver import ActionChains
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.common.exceptions import TimeoutException
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.common.exceptions import NoSuchElementException 
from selenium.webdriver.common.keys import Keys
from time import sleep

class MyThread(QThread):
    
    tipSignal = pyqtSignal(str)#自定义信号，用于显示状态的字符串


    def __init__(self):
        super(MyThread,self).__init__()



    #传递设置
    def set(self,user,pswd,url,haveQuiz,headless):
        self.user=user
        self.pswd=pswd
        self.url=url
        self.haveQuiz=haveQuiz

        self.chrome_options = Options()
        self.chrome_options.add_argument('--ignore-certificate-errors')

        if(headless):
            self.chrome_options.add_argument('--headless')#无窗口
            self.chrome_options.add_argument('--disable-gpu')

        self.chrome_options.add_argument('--mute-audio') #静音

        self.driver = webdriver.Chrome(chrome_options=self.chrome_options)
        self.wait=WebDriverWait(self.driver,10)
        self.action=ActionChains(self.driver)
        self.answer=True


    #关闭弹窗
    def agreeClick(self):

        try:

            agree=self.driver.find_element_by_css_selector("body > div.maskDiv.coursenoticepop > div > div.course-content-dialog > div.bottom-div > div") 

            if agree :
                self.action.move_to_element(agree).click(agree).perform()  
                self.tipSignal.emit("正在关闭弹窗")
        
        except:
            pass

        finally:
            #返回框架
            frame_content =  self.driver.find_element_by_xpath('//*[@id="frame_content-zj"]')
            self.driver.switch_to.frame(frame_content)

            sleep(2)




    #登录
    def login(self,user,pswd,url):

        self.tipSignal.emit("正在登录")
        self.driver.get(url)#登录界面

        user_key=self.wait.until(EC.presence_of_element_located((By.ID,"phone")))#等待账号文本框元素加载完成
        pswd_key=self.wait.until(EC.presence_of_element_located((By.ID,"pwd")))#等待密码文本框元素加载完成
        login_button=self.wait.until(EC.element_to_be_clickable((By.ID,"loginBtn")))#等待登录按钮元素加载完成


        #操作均等待一秒，模拟人类登录
        sleep(1)
        user_key.send_keys(user)#输入账号
        sleep(1)
        pswd_key.send_keys(pswd)#输入密码
        sleep(1)
        login_button.click()#点击登录按钮

        sleep(2)


    # 完成一个页面的所有未完成的知识点
    def view_one_page_points(self):


        frist=True#第一次进入页面的标识符

        while (1):

            sleep(2)
            
            iframe = self.driver.find_element_by_id('iframe')  # 每次刷新后，都要进入内部iframe
            self.driver.switch_to.frame(iframe)


            # 筛选，去除已完成的知识点
            k_points = self.driver.find_elements_by_css_selector('div.ans-attach-ct:not(.ans-job-finished)')
            num = len(k_points)
            flag = False

            for i in range(0, num):
                if i == (num - 1):  # 是最后一个，表示该页刷完
                    flag = True
                
                k_point = k_points[i]


                # 查找任务图标个数，1为知识点，0为不是知识点
                icon_num = len(k_point.find_elements_by_xpath('./div[contains(@class,"ans-job-icon")]'))
                if icon_num == 0:  # 再次筛选，去除不是知识点的div
                    continue


                # 进行视频的播放
                video_iframe = k_point.find_element_by_xpath('./iframe')  # 视频iframe
                #print(video_iframe)
                sleep(2)

                self.driver.switch_to.frame(video_iframe)  # 进入视频iframe
                sleep(2)

                playBtn=self.driver.find_element_by_class_name('vjs-big-play-button')  # 播放按钮

                # 播放和暂停按钮
                pause_btn = self.driver.find_element_by_xpath('//button[contains(@class,"vjs-play-control")and '
                                                    'contains(@class,"vjs-control")and contains(@class,"vjs-button")]')

                #初次播放
                if(frist):
                    sleep(1)
                    self.driver.execute_script("arguments[0].click()",playBtn)
                    frist=False

                self.tipSignal.emit(self.process)#反馈进度


                while (1):  # 播放等待
                    sleep(1)  # 每1秒，检查视频是否播放完毕
                    if (pause_btn.get_attribute('title') == "重播"):  # 点击后播放，即播放完毕状态
                        break

                    sleep(2)
                    self.answerQuiz()#回答视频中问题


                # 视频播放完毕，退出播放iframe，然后退出循环，再次查找该页面的所有知识点（k_points)
                self.driver.switch_to.default_content()
                sleep(2)

                self.driver.refresh()  # 刷新页面，之后需重新进入iframe
                sleep(2)

                break


            if flag:  # 该页面知识点播放完毕
                break
            

    #回答视频中的问题
    def answerQuiz(self):
            
        try:
            #寻找提交按钮
            submit=self.driver.find_element_by_xpath("//a[@id='videoquiz-submit']")


            #若有则开始答题
            if(submit) :
                self.answer = not self.answer#选择另一个选项

                answerBtn=self.driver.find_element_by_xpath("//input[@value='"+(str)(self.answer).lower()+"']")
                self.driver.execute_script("arguments[0].click();",answerBtn)
                sleep(1)

                self.driver.execute_script("arguments[0].click();",submit)

            else:
                #print("SomethingError")
                pass

        #若找不到则说明还没到答题的时候
        except NoSuchElementException:
            #print("noFindsumbit")
            pass
            



    def finshAll(self,url,haveQuiz):

        sleep(3)
        #获取课程名
        self.titleName= self.driver.find_element_by_xpath('//dd[@class="textHidden colorDeep"]').text


        while (1):
            sleep(2)
            # 进入iframe
            frame_content =  self.driver.find_element_by_xpath('//*[@id="frame_content-zj"]')
            self.driver.switch_to.frame(frame_content)

            
            self.tipSignal.emit("正在主章节页面")
            sleep(2)
            self.driver.switch_to.window( self.driver.window_handles[-1])  # 跳转到该知识点界面

            self.agreeClick() #关闭弹窗


            if self.isFinshed(haveQuiz) :break #如果完成了就跳出

            allNoList=self.checkProcess(haveQuiz)#获得需要刷的视频列表


            # 跳转到第一个知识点页面
            allNoList[0].click()
            sleep(3)
            
            self.tipSignal.emit("正在进入播放页面")

            self.view_one_page_points()  # 播放该知识点页面的所有未完成的知识点视频

            self.driver.get(url)
            self.tipSignal.emit("正在回到主界面")
            sleep(1)

            
            # 刷新页面
            self.driver.refresh()
            sleep(2)



    #检查进度
    def checkProcess(self,haveQuiz):

        allNoList = self.driver.find_elements_by_xpath('//span[@class="catalog_points_yi"]')
        newList=[]

        #如果有视频后习题的话，筛去未完成问题但是完成播放的元素
        if (haveQuiz):
            for listElement in allNoList:
                
                if int(listElement.text) > 1 :
                    newList.append(listElement)
        
            return newList
        
        else:
            return allNoList




    #检查是否完成
    def isFinshed(self,haveQuiz):

        process= self.driver.find_element_by_xpath("//*[@id='fanyaChapter']/div/div[1]/h2")

        string=process.text

        string=string.replace('已完成任务点: ','')

        num=string.find('/')
        done=float(string[:num])
        all=float(string[num+1:])

        if(haveQuiz):

            all /= 2


        self.process=str(self.titleName)+"\n"+"已完成: "+str(int(done))+"/"+str(int(all))+"\n"+"正在播放: 第"+str(int(done+1))+"个视频"


        if done == all :
            return True
        else:
            return False


    #开启线程
    def run(self):
        self.tipSignal.emit("已启动")
        self.login(self.user,self.pswd,self.url)
        self.finshAll(self.url,self.haveQuiz)




# if __name__ == '__main__':

#     test1 = MyThread()
#     test1.set("","","",False)
#     test1.run()