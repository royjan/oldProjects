#!/usr/bin/python

import socket
import sys
import cPickle as pickle
import errno
import codecs
import datetime
import MySQLdb
import os
import subprocess
import webbrowser

timeout = 30

if os.environ["username"] is None:
    if sys.platform == "win32":
        print "Check your all settings in Windows.. Trying to RUN"
    elif sys.platform == "linux" or sys.platform == "linux2":
        print "Maybe I can help you.."
        subprocess.call(["sh", "mysql_creation.sh"])
    else:
        print "Not support OS!"


def log_file(kind, parameter, date):
    log = codecs.open("log.txt", "w", "utf-8")
    log.write("You have problem in your " + kind + " system, please fix it!\n")
    log.write("Instead of 0, The output system is: " + parameter + "\n")
    log.write("Wrote at " + date)
    log.close()


class Client:
    def __init__(self, socket=None, ip=None, port=None):
        self.socket = socket
        self.ip = ip
        self.port = port


# Gets from BASH
try:
    username = os.environ["username"]
    password = os.environ["pass"]
except:
    username = raw_input("Username of Database: ")
    password = raw_input("Password of Database: ")

try:
    ip = socket.gethostname()
    port = 54325
    s = socket.socket()
    s.bind((ip, port))
    s.listen(10)
    s.settimeout(timeout)
    client_list = []
    server_messages = ""
    something_changed = True
    print "Server: %s:%d \n" % (str(socket.gethostbyname(socket.gethostname())), port)
    url = 'http://127.0.0.1:8000/show_results.php?page=0'
    webbrowser.open_new(url)
    while True:
        if something_changed:
            if len(client_list) == 0:
                print "There are no client connected"
            else:
                for i, client in enumerate(client_list):
                    i += 1
            print
            print server_messages
            something_changed = False
        c = None

        try:
            c = s.accept()

        except socket.error:
            pass

        if c is not None:
            c = Client(socket=c[0], ip=c[1][0], port=c[1][1])
            c.socket.settimeout(timeout)
            client_list += [c]
            something_changed = True
            print "new client accepted\n\n"

        for client in list(client_list):
            cs = client.socket
            try:
                message = pickle.loads(cs.recv(1024))
                # DB connection
                db = MySQLdb.connect("localhost", username, password, "testdb")
                cursor = db.cursor()
                cursor.execute("INSERT INTO testdata (bsdID,time,alarmSystem,waterDetect)  VALUES (%s, %s, %s, %s) ",
                               (message[0], message[1], message[2], message[3]))
                db.commit()
                new_date = datetime.datetime.strptime(message[1], '%Y-%m-%d %H:%M:%S').strftime("%d-%m-%Y %H:%M:%S")
                if message[2] != "0" and message[3] == "0":
                    log_file("alarmSystem", message[2], new_date)
                elif message[3] != "0" and message[2] == "0":
                    log_file("waterDetect", message[3], new_date)
                elif message[2] != "0" and message[3] != "0":
                    join_message = message[3] + " | " + message[2]
                    log_file("waterDetect and alarm", join_message, new_date)
                # New date format
                print "Data stored - last updated on ", new_date
                if message == "":
                    client_list.remove(client)
            except EOFError:
                print "Can't receive data from client"
            except ValueError:
                message = "Wrong!"
            except socket.error as error:
                if error.errno == errno.WSAECONNRESET:
                    sys.exit("The connection failed, please try later!")

except KeyboardInterrupt:
    pass
