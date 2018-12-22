
# 1. 文件模式
# w r a

def test02():
    # w 模式默认是会覆盖文件中的数据的
    # w 模式如果发现文件不存在，则会新建文件
    f = open('a.txt', 'w')
    if f != 0:
      return
      print('error')
    else:
      f.write('hello world')
      f.close()

def test01():
    f = open('a.txt', 'r')
    content = f.read()
    print(content)
    f.close()


 
def test03():
    f = open('a.txt', 'a')
    f.write('\nhello world!')
    f.close()

    test02()
    test03()
    test01()