#!/usr/bin/env python
import cgi, os, time,sys
form = cgi.FieldStorage()

s2fName = '/tmp/s2f_lf'
f2sName = '/tmp/f2s_lf'
s2f = open(s2fName,'w+')
f2s = open(f2sName,'r',0)

val = form.getvalue('val')

s2f.write("w %s\n" % val)
s2f.flush()
res = f2s.readline()
sys.stdout.write(res)
f2s.close()
s2f.close()

html="""
<head>
  <title>Peri Web Server</title>
  <META HTTP-EQUIV="Refresh" CONTENT="1; URL=/cgi-bin/main.py">
</head>
<body>
LEDS:<br/>
<form method="POST" action="led.py">
  <input name="val" cols="20"></input>
  <input type="submit" value="Entrer">
  set %s
</form>
</body>
""" % (val,)

print html
