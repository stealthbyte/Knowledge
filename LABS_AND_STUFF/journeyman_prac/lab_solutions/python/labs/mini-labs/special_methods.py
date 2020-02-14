class SuperList(object):
    def __init__(self, *args):
        self.my_list = [x for x in args]

    def __add__(self, value):
        tmp_list = [elem + value for elem in self.my_list]
        
        return SuperList(*tmp_list)

    def __str__(self):
        return str(self.my_list)

    def __getitem__(self, index):
        return self.my_list[index]

    def __setitem__(self, index, value):
        self.my_list[index] = value * 4
        
        
if __name__ == '__main__':
    my_super_list = SuperList(1, 2, 3, 4, 5)
    print(my_super_list)
    
    print(my_super_list + 5)
    print(my_super_list)
    
    my_super_list[1] = 3
    print(my_super_list[1])
    print(my_super_list)
