
print('hello world')
# 写文件：write writelines
def test01():
    '''1. write 函数用法'''

    f = open('a.txt', 'w')
    f.write('hello world\n')
    # writelines 一次可以写多行，参数是一个列表，列表每个元素都是一行数据
    lines = ['aaa\n', 'bbb\n', 'ccc\n']
    f.writelines(lines)
    f.close()

def test02():
    '''1. read 函数用法'''
    f = open('a.txt', 'r')
    content = f.read(4)
    f.close()
    print(content)

def test03():
    '''1. readline 函数用法'''
    f = open('a.txt', 'r')
    content = f.readline()
    f.close()
    print(content)

def test04():
    '''1. readline 函数用法'''
    f = open('a.txt', 'r')
    content = f.readlines()
    f.close()
    print(content)

def test05():
    '''1. readline 函数用法'''
    f = open('a.txt', 'r')
    lines = f.readlines()
    for line in lines:

        if line[-1] == '\n' :
          print(line[:-1])
        else:
          print(line)
    f.close()

#test01()
#test02()
#test03()
#test04()
test05()