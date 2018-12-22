print("hello world !")
my_dict = {'aa':10, 'bb':20}
my_list = list(my_dict.items())
print(my_list)
i = 0
while i < len(my_list):
    print('key:',my_list[i][0], 'value:',my_list[i][1])
    i += 1
