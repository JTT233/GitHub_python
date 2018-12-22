import os

def file_rename():
    """file rename"""
    #os.rename('hello.txt', 'aa.txt')
    os.rename('hello.txt', '[www.test.com]' + 'hello.txt')
#[www.test.com]a1.txt


def file_remove():
    """file remove"""
    os.remove('/desktop/aa.txt')

# root problem,only file name, 旨在当前路径找，找不到就报错

def creatAndDelete_DIC():
    os.mkdir('desktop/tt.txt')
    os.rmdir('desktop/tt.txt')

def getFileList():
    os.chdir('/Users/yanweijun/Desktop/pythonAvs/hello')
    content = os.listdir()
    i = 0
    for name in content:
        print(name)
        os.rename(name, name)

        #os.rename(name, 'www.test.com' + name)


def getSetWorkSpacePath():
    cwd = os.getcwd()  # cwd current working directory
    print(cwd)
    os.chdir('/Users/yanweijun/Desktop/pythonAvs')
    os.mkdir('aaa')
#file_rename()
#file_remove()
#creatAndDelete_DIC()

#getFileList()
#getWorkSpacePath()
getFileList()