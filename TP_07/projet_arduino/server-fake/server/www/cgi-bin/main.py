#!/usr/bin/env python

html="""
<head>
  <title>Peri Web Server</title>
</head>
<body>
LEDS:<br/>
<form method="POST" action="led.py">
  <input name="val" cols="20"></input>
  <input type="submit" value="Entrer">


Statistiques:<br/>
<form method="POST" action="stat.py">
  <input name="stat_val" cols="20"></input>
  <input type="submit" value="Entrer">

</form>
</body>
"""

print html
