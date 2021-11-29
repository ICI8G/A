from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.common.exceptions import TimeoutException
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from browsermobproxy import Server
from bs4 import BeautifulSoup
import time


#配置Proxy
server = Server("browsermob-proxy-2.1.4\\bin\\browsermob-proxy.bat")
server.start()
proxy = server.create_proxy()

#配置proxy启动WebDriver
chrome_options = Options()
chrome_options.add_argument('-–ignore-certificate-errors')
chrome_options.add_argument('--proxy-server={0}'.format(proxy.proxy))
driver = webdriver.Chrome(chrome_options=chrome_options)
wait=WebDriverWait(driver,10)



#登录
def login(user,pswd):
    driver.get("http://cas.shnu.edu.cn/cas/login?service=http%3A%2F%2Fcourse.shnu.edu.cn%2Feams%2Flogin.action")#登录界面
    user_key=wait.until(EC.presence_of_element_located((By.ID,"un")))#等待账号文本框元素加载完成
    pswd_key=wait.until(EC.presence_of_element_located((By.ID,"pd")))#等待密码文本框元素加载完成
    login_button=wait.until(EC.element_to_be_clickable((By.CLASS_NAME,"login_box_landing_btn")))#等待登录按钮元素加载完成

    
    user_key.send_keys(user)#输入账号
    pswd_key.send_keys(pswd)#输入密码

    login_button.click()#点击登录按钮



#去量化评价页面
def go_to_evaluate():
    time.sleep(1)#延迟一秒，不然可能未加载完成
    proxy.new_har(options={'captureContent':True})#一定要打开捕捉内容属性，不然捕捉不到响应体
    
    try:
        #进入评教
        driver.find_element_by_xpath('//a[@class="first_menu"]').click()#点击量化评价ul
        time.sleep(1)
        driver.find_element_by_xpath('//a[@href="/eams/quality/stdEvaluate.action"]').click()#点击量化评价li
        time.sleep(1)
    except:
        print('找不到量化评价，请检查是否正确登录')#因为大多数情况都是没登录进去导致
        exit(1)



#从har中找寻又有信息
def analysis():
    result=proxy.har
    return_list=[]#用于存放信息的列表

    #找寻需要的信息解析
    for entity in result['log']['entries']:
        html=entity['response']['content']
        if 'text' in html:#筛选有text键的字典
            bs=BeautifulSoup(html['text'],'lxml')#解析网页
        
            if bs.find('table',class_='gridtable'):#帅选出最终需要的网页
                
                table_list=bs.find('table',class_='gridtable').find('tbody')
                
                for tr in table_list:
                    td=tr.contents
                    course=td[3].text#课
                    teacher=td[5].text#老师
                    if td[7].find('a')!=None:#如果找不到,说明已经评教完成,不加入带评价列表
                        href='http://course.shnu.edu.cn'+td[7].find('a')['href']#评价地址
                        return_list.append([course,teacher,href])
                
    return return_list


#开始量化评价
def start_evaluate():
    evaluate_list=analysis()#获得评价列表

    #开始评价
    for need_post in evaluate_list:
        driver.get(need_post[2])
        sub_button=wait.until(EC.element_to_be_clickable((By.ID,"sub")))#等待提交按钮元素加载完成
        subbing()#填表
        sub_button.click()#提交
        driver.switch_to.alert.accept()#确定弹窗警告
        time.sleep(1)
        


#填表
def subbing():
    time.sleep(0.5)

    #选择四选一的七题
    for need_click in range(1,8):
        driver.find_element_by_xpath('//*[@id="option_'+(str)(need_click)+'_0"]').click()#点击

    #等待两个文本框元素加载完成
    text_1=wait.until(EC.presence_of_element_located((By.XPATH,"/html/body/div[2]/div[2]/div/div[11]/textarea")))
    text_2=wait.until(EC.presence_of_element_located((By.XPATH,"/html/body/div[2]/div[2]/div/div[12]/textarea")))


    #填写两个文本框
    text_1.send_keys('好')
    text_2.send_keys('好')


if __name__ == '__main__':

    #用户需配置的信息
    user=""#账号
    pswd=""#密码


    login(user,pswd)
    go_to_evaluate()
    start_evaluate()