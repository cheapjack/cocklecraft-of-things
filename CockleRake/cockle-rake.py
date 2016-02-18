import sqlite3
import random
from flask import g
from flask import Flask
from flask import render_template
from flask import request

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
  return render_template('index.html')

@app.route("/register", methods=['GET', 'POST'])
def register():
  if request.method == 'POST':
    # Create a new Cockle name
    # This is fairly basic, but should suffice until *lots* are registered
    iterations = 0
    while iterations < 1000:
      # Choose a first and second name at random
      first = random.choice(names)
      second = random.choice(names)
      # See if this is already registered
      cur = g.db.execute('select first, second, workshop from cockles where first = ? and second = ? and workshop = ?', [first, second, app.config['WORKSHOP_NAME']])
      if not cur.fetchone():
        # It doesn't already exist, so we can use this one
        g.db.execute('insert into cockles (first, second, workshop) values (?, ?, ?)', [first, second, app.config['WORKSHOP_NAME']])
        g.db.commit()
        return render_template('registered.html', name=first+"-"+second+"-"+app.config['WORKSHOP_NAME'])
      iterations += 1
    return first + "-" + second + "-" + app.config['WORKSHOP_NAME']
  else:
    return render_template('register.html')

@app.route("/<first>-<second>-<workshop>", methods=['GET', 'POST'])
def temperature():
  if request.method == 'POST':
    # Record the temperature
    return "new user - "
  else:
    # Return the most recent recording
    return "new user form"

if __name__ == "__main__":
  app.debug = True
  app.run()
