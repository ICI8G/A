import requests
import execjs
import time
from bs4 import BeautifulSoup


user_name="账号"#填入账号
password="密码"#填入密码

#第一次使用的请求头，用于获得jessionid和html网页
headers={
    'Host':'course.shnu.edu.cn',
    'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:75.0) Gecko/20100101 Firefox/75.0',
    'Referer':'http://cas.shnu.edu.cn/cas/login?service=http%3A%2F%2Fcourse.shnu.edu.cn%2Feams%2Flogin.action'
}


session = requests.Session()#创建一个会议
request=session.get('http://cas.shnu.edu.cn/cas/login?service=http%3A%2F%2Fcourse.shnu.edu.cn%2Feams%2Flogin.action',headers=headers)#第一次请求


#获得lt和execution参数
def get_lt_and_execution():
    bs=BeautifulSoup(request.text,'lxml')
    lt=bs.find('input',id='lt')['value']
    execution=bs.select('input[name="execution"]')[0]['value']

    return lt,execution

#获得jessionid
def get_jessionid():
    set_cookie=request.headers['Set-Cookie']
    jessionid=set_cookie[set_cookie.find('JSESSIONID='):].split(';')[0]
    return jessionid

#通过des.js文件获得rsa
def rsa():
    with open('des.js')as f:
        des = execjs.compile(f.read())
        elem=user_name+password+get_lt_and_execution()[0]
        return des.call('strEnc',elem,'1','2','3')

#用于提交的加密form
data={
    "rsa":rsa(),
    "ul":(str)(len(user_name)),
    "pl":(str)(len(password)),
    "lt":get_lt_and_execution()[0],
    "execution":get_lt_and_execution()[1],
    "_eventId":"submit"
}

#加上jessionid的请求头，用于后续操作
new_headers={
    'Host':'cas.shnu.edu.cn',
    'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:75.0) Gecko/20100101 Firefox/75.0',
    'Referer':'http://cas.shnu.edu.cn/cas/login?service=http://course.shnu.edu.cn/eams/login.action',
    'Origin':'http://cas.shnu.edu.cn',
    'Upgrade-Insecure-Requests': '1',
    'Cookie':get_jessionid()
}

#获得主界面
def get_html():

    response=session.post('http://cas.shnu.edu.cn/cas/login?service=http%3A%2F%2Fcourse.shnu.edu.cn%2Feams%2Flogin.action',headers=new_headers,data=data)

    step1=session.get('http://course.shnu.edu.cn/eams/login.action;'+get_jessionid(),headers=new_headers)
    time.sleep(0.5)
    step2=session.get('http://course.shnu.edu.cn/eams/home.action',headers=new_headers)
    time.sleep(0.5)
    print(step2.text)



get_html()