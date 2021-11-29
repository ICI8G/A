selenium配置

驱动下载
https://blog.csdn.net/yxw908186797/article/details/89217311?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_title~default-0.no_search_link&spm=1001.2101.3001.4242.1


环境变量设置
https://blog.csdn.net/qq_22200671/article/details/108638836


evaluate.py为不带gui的简易版本
evaluate_gui.py为gui版本，推荐使用






思路：
自动化登录操作
proxy代理得到响应体获得动态网页的量化评价页面（driver.page_source()得到的源代码之所以和f12得到的源代码不一样是因为网页用了动态）
分析出需要的url列表
自动化填表提交

