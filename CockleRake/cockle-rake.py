import sqlite3
import random
from flask import g
from flask import Flask
from flask import render_template
from flask import request
from flask import Response
import datetime

app = Flask(__name__)
# Load our configuration from the file listed in the
# COCKLE_SETTINGS environment variable
app.config.from_envvar('COCKLE_SETTINGS')

# Potential names
# (taken from http://www.britishbeaches.info/beaches)
names = [
  'Blundellsands','Crosby',
  'Formby','Hightown',
  'Meols',
  'Moreton',
  'Ainsdale','Formby','Southport'
]

# Functions to connect to the database
#

def connect_db():
  return sqlite3.connect(app.config['DATABASE'])

@app.before_request
def before_request():
    g.db = connect_db()

@app.teardown_request
def teardown_request(exception):
    db = getattr(g, 'db', None)
    if db is not None:
        db.close()


# Functions to handle the different URLs that our
# server supports
#

@app.route("/")
def hello():
  # Get the recent readings
  readings = g.db.execute('select first, second, workshop, created_at, temperature from cockles left join readings where readings.cockle_id = cockles.id order by created_at desc limit 10');
  latest = [dict(name=row[0]+"-"+row[1]+"-"+row[2], recorded_at=row[3], value=row[4]) for row in readings.fetchall()]
  observations = g.db.execute('select location, created_at, observation from observations order by created_at desc limit 10');
  latest_observations = [dict(location=row[0], recorded_at=row[1], observation=row[2]) for row in observations.fetchall()]
  return render_template('index.html', readings = latest, observations = latest_observations)

@app.route("/register", methods=['GET', 'POST'])
def register():
  if request.method == 'POST':
    # Create a new Cockle name
    # This is fairly basic, but should suffice until *lots* are registered
    iterations = 0
    while iterations < 1000:
      # Choose a first and second name at random
      first = random.choice(names).lower()
      second = random.choice(names).lower()
      workshop = app.config['WORKSHOP_NAME'].lower()
      # See if this is already registered
      cur = g.db.execute('select first, second, workshop from cockles where first = ? and second = ? and workshop = ?', [first, second, workshop])
      if not cur.fetchone():
        # It doesn't already exist, so we can use this one
        g.db.execute('insert into cockles (first, second, workshop) values (?, ?, ?)', [first, second, workshop])
        g.db.commit()
        return render_template('registered.html', name=first+"-"+second+"-"+workshop)
      iterations += 1
    return first + "-" + second + "-" + app.config['WORKSHOP_NAME']
  else:
    return render_template('register.html')

import urllib2

@app.route("/space", methods=['GET'])
def space():
  info = urllib2.urlopen("http://api.open-notify.org/astros.json").read()
  resp = Response(info)
  resp.headers['Access-Control-Allow-Origin']="*"
  resp.headers['Content-Type']="application/json"
  return resp

@app.route("/<first>-<second>-<workshop>", methods=['GET', 'POST'])
def temperature(first, second, workshop):
  first = first.lower()
  second = second.lower()
  workshop = workshop.lower()
  if request.method == 'POST':
    # Record the temperature
    now = datetime.datetime.now()
    cur = g.db.execute('select id from cockles where first = ? and second = ? and workshop = ?', [first, second, workshop])
    cockle = cur.fetchone()
    if cockle:
      # We've got a cockle
      g.db.execute('insert into readings (created_at, cockle_id, temperature) values (?, ?, ?)', [now, cockle[0], request.form['temperature']])
      g.db.commit()
      return "success"
    else:
      return "not found"
  else:
    # Return the most recent recording
    cur = g.db.execute('select id from cockles where first = ? and second = ? and workshop = ?', [first, second, workshop])
    cockle = cur.fetchone()
    if cockle:
      # We've got a cockle
      # Find its most recent recording
      readings = g.db.execute('select created_at, temperature from readings where cockle_id = ? order by created_at desc', [cockle[0]]);
      latest = readings.fetchone()
      if latest:
        resp = Response("{\"recorded_at\":\""+latest[0]+"\", \"value\":"+str(latest[1])+"}")
        resp.headers['Access-Control-Allow-Origin']="*"
        resp.headers['Content-Type']="application/json"
        return resp;
      else:
        return "none"
    return abort(404)

@app.route("/<location>", methods=['GET'])
def get_observation(location):
  location = location.lower()
  # Return the most recent observation for this location
  observations = g.db.execute('select created_at, observation from observations where location = ? order by created_at desc', [location]);
  latest = observations.fetchone()
  if latest:
    resp = Response("{\"recorded_at\":\""+latest[0]+"\", \"value\":\""+str(latest[1])+"\"}")
    resp.headers['Access-Control-Allow-Origin']="*"
    resp.headers['Content-Type']="application/json"
    return resp;
  else:
    return "none"

@app.route("/observe", methods=['GET', 'POST'])
def make_observation():
  if request.method == 'POST':
    # Record the observation
    location = request.form['location'].lower()
    now = datetime.datetime.now()
    g.db.execute('insert into observations (created_at, location, observation) values (?, ?, ?)', [now, location, request.form['observation']])
    g.db.commit()
    return "success"
  else:
    return render_template('observe.html')

if __name__ == "__main__":
  app.debug = True
  # Include the "host='0.0.0.0'" so that external clients can connect
  app.run(host='0.0.0.0')
