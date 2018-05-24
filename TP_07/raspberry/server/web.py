#!/usr/bin/env python
from flask import Flask, render_template, request
import sqlite3
import json
 
app = Flask(__name__)
 
 
@app.route("/data.json")
def data():
    connection = sqlite3.connect("/home/pi/lee-fabre/lab7/toto.db")
    cursor = connection.cursor()
    cursor.execute("SELECT 1000*time, lumiere from Ard")
    results = cursor.fetchall()
    print results
    return json.dumps(results)
 
@app.route("/graph")
def graph():
    return render_template('graph.html')
 
 
if __name__ == '__main__':
    app.run(
    debug=True,
    threaded=True,
    host='0.0.0.0',
    port=8000
)

