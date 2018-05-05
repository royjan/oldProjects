#!/usr/bin/python

from menu import *

if __name__ == "__main__":
	root = Tk()
	root.wm_title("Roy's Project")
	menu_bar = Menu(root)
	thread.start_new_thread(menu_build, (root, menu_bar))
	root.config(menu=menu_bar)
	photo = PhotoImage(file="image.gif")
	label = Label(image=photo)
	label.image = photo
	label.grid()
	root.mainloop()
