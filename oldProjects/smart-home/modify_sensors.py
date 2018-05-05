#!/usr/bin/python

import codecs
import datetime
from Tkinter import *
import tkMessageBox
import tkFont


def send():
	if tkMessageBox.askyesno("Saving to file", "Are you sure you want to save?"):
		global input_water, input_alarm
		input_water = WaterDetect.get()
		input_alarm = AlarmSystem.get()
		if input_alarm is not None and input_water is not None:
			tkMessageBox.showinfo("Saving File", "The data is saved!")
		main_window.destroy()
		return input_water, input_alarm


def date():
	right_now = datetime.datetime.now().strftime("%d-%m-%Y_%H:%M:%S")
	return right_now


# main
main_window = Tk()
main_window.title("Modify Sensors")
labelWater = Label(main_window, text="Water Detection data: ").grid(row=0)
labelAlarm = Label(main_window, text="Alarm System data: ").grid(row=1)
WaterDetect = Entry(main_window)
WaterDetect.grid(row=0, column=1)
AlarmSystem = Entry(main_window)
AlarmSystem.grid(row=1, column=1)

photo = PhotoImage(file="Hand_Pump_-_Animation.gif", format="gif")
label = Label(image=photo)
label.image = photo
label.grid(row=2, columnspan=2)
send = Button(main_window, text='Submit', command=send).grid(sticky=E)
tkFont.Font(root=main_window, family='Arial', size=24, weight='bold')
main_window.mainloop()

try:
	input_water = "WaterDetection " + input_water
	input_alarm = "\nAlarmSystem " + input_alarm
except NameError, TypeError:
	sys.exit("Please try again! Are you sure what are you doing?")
now = date()
my_date = "\nDate " + now
fileObj = codecs.open("sensors.txt", "w", "utf-8")

if input_alarm is not None and input_water is not None:
	try:
		fileObj.write(input_water)
		fileObj.write(input_alarm)
		fileObj.write(my_date)
	except:
		sys.exit("Can't save data!")

fileObj.close()
try:
	main_window.destroy()
except:
	pass
