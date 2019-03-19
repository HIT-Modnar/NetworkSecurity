#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

'''
// Author : Modnar
// Date   : 2019-03-19
// Copyright (C) 2019 Modnar. All rights reserved.
'''

import threading
import socket 
import tkinter as tk

'''
' Scan: scan a port to judge whether the port is on connection.
'
' @param domain The domain of the host need to connect with.
' @param port   The port number of which need to be judged.
'''
def scan(domain, port):
    tmp_str = ''
    target_ip = socket.gethostbyname(domain)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(0.1)
    result = sock.connect_ex((target_ip, port))
    lock = threading.Lock()
    if result == 0:
        lock.acquire()
        try:
            update_show_str(str('[+] Port : ') + str(port) + '\n')
            # This print is used to debug.
            # print("[+] Port :", port)
        finally:
            lock.release()

def update_show_str(input_str):
    show_text.insert('end', input_str)

'''
' Multi_thread_scan: scan the ports from begin_port to end_port using multi-threading.
'
' @param domain     The domain of the host need to connect with.
' @param begin_port The start port number, and default value is 0.
' @param end_port   The end port number, and default value is 1000. 
'''
def multi_thread_scan(domain, begin_port=0, end_port=1000):
    for i in range(begin_port, end_port):
        tmp_thread = threading.Thread(target=scan, args=(domain, i, ))
        tmp_thread.start()
        tmp_thread.join
'''
' Click_button: call this function when the user click 'Start!' button.
'''
def click_button():
    show_text.delete(1.0, tk.END)
    domain = domain_text.get()
    begin_port_num = begin_port_text.get()
    end_port_num = end_port_text.get()
    if domain == '' or begin_port_num == '' or end_port_num == '':
        var.set('Please input again!')
        return 
    host = 'Target Host:\n' + str(domain) + ': ' + str(socket.gethostbyname(domain))
    var.set(host + '\nStatus: Scanning...')
    lock = threading.Lock()
    lock.acquire()
    multi_thread_scan(domain, int(begin_port_num), int(end_port_num))
    lock.release()
    var.set(host + '\nStatus: Done.')

window = tk.Tk()
window.title('Network Scanner')
window.geometry('500x600')

var = tk.StringVar()

label = tk.Label(window, textvariable=var, font=('SF-Mono', 14), 
        width=100, height=5)
var.set('Welcome to use the Network Scanner!')
label.pack()

# Add the main text panel.
show_text = tk.Text(window, height = 10)
show_text.pack()

# Add the label to help user input correctly.
tmp_label = tk.Label(window, text='Input Domain:', width=100, height=2)
tmp_label.pack()
domain_text = tk.Entry(window)
domain_text.pack()
tmp_label = tk.Label(window, text='Input Start Port:', width=100, height=2)
tmp_label.pack()
begin_port_text = tk.Entry(window)
begin_port_text.pack()
tmp_label = tk.Label(window, text='Input End Port:', width=100, height=2)
tmp_label.pack()
end_port_text = tk.Entry(window)
end_port_text.pack()

# Click the button to start scanning.
button = tk.Button(window, text='Start!', width=15, height=2, 
         command=click_button)
button.pack()

window.mainloop()
