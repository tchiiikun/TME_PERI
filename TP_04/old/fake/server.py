#!/usr/bin/env python
import os, time, sys

s2fName = '/tmp/s2f_lf'
if not os.path.exists(s2fName):
   os.mkfifo(s2fName)
s2f = open(s2fName,'w+')

f2sName = '/tmp/f2s_lf'
if not os.path.exists(f2sName):
   os.mkfifo(f2sName)
f2s = open(f2sName,'r')


s2f.write(raw_input("Ecrivez votre message pour fake\n"))
s2f.flush()

str = f2s.readline()
print '%s' % str,

f2s.close()
s2f.close()
