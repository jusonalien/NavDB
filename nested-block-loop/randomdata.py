#!/usr/bin/python

import random, string

def random_name(randomlength = 8):
    a = list(string.ascii_letters)
    random.shuffle(a)
    return ''.join(a[:randomlength])

def random_ID():
    
    return random.randint(1,2000)

def random_slary():
    
    return random.randint(2000,10000)

def random_age():
    
    return random.randint(20,60)


id_list = {-1:-1}


for i in range(1,2001):
    print i,random_age()


for i in range(1,1001):
    id = random_ID()
    
    while (id_list.has_key(id)):
        id = random_ID()
    
    id_list[id] = 1
       
    print id,random_slary()




