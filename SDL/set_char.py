# -*- coding: utf-8 -*-

import os
from random import *

path = "gfx\\potrait"

flist = os.listdir(path)
def name():
    a = input("이 사람 이름 - ")
    return a

def ranint(a,b):
    n = randint(a,b)
    print(n)
    return n
num = 1


print(flist)

for s in flist:
    print(s[:-4])
    f = open("characters\\"+s[:-4] + ".chr" ,"w",encoding="utf8")
    f.write("#Mans\n")
    f.write("Man = <\n")
    f.write("   name = %s\n"%name())
    f.write("   potrait = potrait\\%s\n"%s[0:-4])
    f.write("   id = %02d\n"%num)
    f.write("   live = true")
    f.write("   born_year = %d\n"%ranint(1920,1981))
    f.write("   money = %d\n"%ranint(20,10000))
    f.write("   prestige = %d\n"%randint(0,10000))
    f.write("   fascist = %d\n"%ranint(-128, 128))
    f.write("   liberty = %d\n"%ranint(-128,128))
    f.write("   honor = %d\n"%ranint(0,256))
    f.write("   ambition = %d\n"%ranint(0,256))
    f.write("   religion = %d\n"%ranint(0,256))
    f.write(">\n")
    num += 1
    f.close()
