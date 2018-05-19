#!/usr/bin/env python
import os, time, sys

s2fName = '/tmp/s2f_lf'
f2sName = '/tmp/f2s_lf'
if not os.path.exists(s2fName):
   os.mkfifo(s2fName)
s2f = open(s2fName,'w+')

if not os.path.exists(f2sName):
   os.mkfifo(f2sName)
f2s = open(f2sName,'r')

data = sys.stdin.readline()
s2f.write(data)
s2f.flush()
str = f2s.readline()
print '%s' % str,

f2s.close()
s2f.close()
