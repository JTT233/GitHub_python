# 1. 获取拷贝的文件名
old_file_name = input('please input file name :')
# 2. 读取要拷贝文件内容
new_file_name = old_file_name + '.bk'
# 3. 打开新的文件
f_old = open(old_file_name,'rb')
f_new = open(new_file_name, 'wb')
# 4. 将老文件拷贝到新文件里面
old_file_content = f_old.read()
f_new.write(old_file_content)
# 5. 关闭新文件 
f_old.close()
f_new.close()
