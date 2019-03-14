# -*- coding: UTF-8 -*- 

from tkinter import *
import tkinter.messagebox as messagebox

class MainWindow(Frame):
    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.pack()
        self.creatWidgets()

#    def creatWidgets(self):
#        self.helloLabel = Label(self, text='Hello, world!')
#        self.quitButton = Button(self, text='Quit', command=self.quit)
#        self.helloLabel.pack()
#        self.quitButton.pack()

    def creatWidgets(self):
        self.nameInput = Entry(self)
        self.nameInput.pack()
        self.alertButton = Button(self, text='Hello', command=self.hello)
        self.alertButton.pack()

    def hello(self):
        name = self.nameInput.get() or 'world!'
        messagebox.showinfo('Message', 'Hello, %s' % name)

def main():
    win = MainWindow()
    win.master.title('Modnar')
    win.mainloop()

if __name__ == '__main__':
    main()
