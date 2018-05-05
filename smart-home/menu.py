#!/usr/bin/python

from Tkinter import *
from tkMessageBox import *
import tkSimpleDialog
import socket
import time
import cPickle as pickle
import datetime
import uuid
import thread
import re


def menu_build(root, menu_bar):
	file_menu = Menu(menu_bar, tearoff=0)
	file_menu.add_command(label="Connect", command=connect)

	file_menu.add_separator()

	file_menu.add_command(label="Exit", command=root.quit)
	menu_bar.add_cascade(label="File", menu=file_menu)

	help_menu = Menu(menu_bar, tearoff=0)
	help_menu.add_command(label="About...", command=about_func)
	menu_bar.add_cascade(label="Help", menu=help_menu)


def connect():
	global ip_split

	def now_date():
		right_now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
		return right_now

	def reading_sensors():
		sensors_data = {}
		with open("sensors.txt") as f:
			for line in f:
				(key, val) = line.split()
				sensors_data[str(key)] = val
			return sensors_data['WaterDetection'], sensors_data['AlarmSystem'], sensors_data['Date'].replace("_", " ")

	def mac_address():
		mac_id = "%012x" % uuid.getnode()
		mac = ':'.join(mac_id[i * 2:i * 2 + 2] for i in range(6))
		return mac

	def send():
		bsID = mac_address()
		try:
			while True:
				list_data = []
				now = now_date()
				WaterDetection, AlarmSystem, Date = reading_sensors()
				list_data.append(bsID)
				list_data.append(now)
				list_data.append(AlarmSystem)
				list_data.append(WaterDetection)
				s.send(pickle.dumps(list_data, -1))
				time.sleep(20)
		except EOFError:
			"Bad Connection"

	try:
		input_ip = tkSimpleDialog.askstring("IP entrance", "IP to connection: ")
		match = re.findall(r'[0-9]+(?:\.[0-9]+){3}:[0-9]+', input_ip)
		if not match:
			showerror('Error',"Wrong IP Format!")
			exit()
		ip_split = input_ip.split(":", 1)
	except NameError:
		showerror('Error', "IP Format is wrong!")

	except AttributeError:
		showerror('Error', "Connection Stopped!")

	try:
		write_ip = open("address.txt", "w")
		write_ip.write(ip_split[0])
		write_ip.write("\n")
		write_ip.write(ip_split[1])
		write_ip.close()
	except (NameError, IndexError, TypeError):
		showerror('Error', "Can't write IP text file")

	try:
		read_ip = open("address.txt", "r")
		ip_address = read_ip.readline().rstrip()
		port = int(read_ip.readline())
		read_ip.close()
	except (OSError, IOError, ValueError):
		showerror('Error', "File not found or wrong format!")

	try:
		s = socket.socket()
		s.connect((ip_address, port))
		showinfo('Connection', mac_address() + " Connected!")
		thread.start_new_thread(send, ())

	except socket.gaierror:
		showerror('Connection', "Can't connect to the IP above")


def about_func():
	showinfo('About', "Roy's Project!")


