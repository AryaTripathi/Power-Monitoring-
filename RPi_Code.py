#-------------------------------------------------------------------------------
# Name:        module1
# Purpose:
#
# Author:      Arya Tripathi
#
# Created:     25-03-2023
# Copyright:   (c) Arya Tripathi 2023
# Licence:     <your licence>
#-------------------------------------------------------------------------------

#Required libraries imported

import RPi.GPIO as GPIO
from tkinter import *
from tkinter import ttk
import time
from PIL import Image

# Email Setup
import smtplib

#Email Variables
SMTP_SERVER = 'smtp.gmail.com' #Email Server
SMTP_PORT = 587 #Server Port
GMAIL_USERNAME = '20BEC130@nirmauni.ac.in'
GMAIL_PASSWORD = 'nirma123'

class Emailer:
    def sendmail(self, recipient, subject, content):

    #Create Headers
        headers = ["From: " + GMAIL_USERNAME, "Subject: " + subject, "To: " + recipient,"MIME-Version: 1.0", "Content-Type: text/html"]
        headers = "\r\n".join(headers)

    #Connect to Gmail Server
        session = smtplib.SMTP(SMTP_SERVER, SMTP_PORT)
        session.ehlo()
        session.starttls()
        session.ehlo()

    #Login to Gmail
        session.login(GMAIL_USERNAME, GMAIL_PASSWORD)

    #Send Email & Exit
        session.sendmail(GMAIL_USERNAME, recipient, headers + "\r\n\r\n" + content)
        session.quit

sender = Emailer()

sendTo = '20BEC130@nirmauni.ac.in'
emailSubject = "Power Status"
emailContent = "UPS ON"

#RPi GPIO Setup
GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)
GPIO.setup(8,GPIO.OUT,initial=GPIO.HIGH)
GPIO.setup(3,GPIO.OUT,initial=GPIO.HIGH)
GPIO.setup(10,GPIO.IN)
GPIO.setup(12,GPIO.IN)

# Object created
win = Tk()

# Geometry of TKinter frame
win.geometry("700x450")

# Title
win.title("Embedded Systems Special Assignment")

# Create a Canvas
canvas = Canvas(win, width=700, height=3500)
canvas.pack(fill=BOTH, expand=True)

# Colour Change Functions
def change_color1():
   canvas1.itemconfig(circle, fill='green')

def change_color2():
   canvas1.itemconfig(circle, fill='red')

# D - Block
label = Label(win, text=" D block ", font=("Courier 25 bold")).place(x = 210, y = 15)

# Canvas for Main Supply
canvas1 = Canvas(win, width=250, height=250)
canvas1.place(x = 10, y = 60)
circle = canvas1.create_oval(60,60,210,210, fill='blue')
label_D1=Label(win, text="Main Supply",font=("Courier 22 bold")).place(x = 55, y = 80)

# Colour changes as per status of Main supply
if GPIO.input(10):
    change_color1()
else:
    change_color2()

# Canvas for UPS
canvas1 = Canvas(win, width=250, height=250)
canvas1.place(x = 300, y = 60)
circle = canvas1.create_oval(60,60,210,210, fill='blue')
label_D2=Label(win, text="UPS", font=("Courier 22 bold")).place(x = 410, y = 80)


# Colour changes as per status of UPS
if GPIO.input(12):
    change_color1()
    sender.sendmail(sendTo, emailSubject, emailContent)
else:
    change_color2()

sec = StringVar()
Entry(win, textvariable=sec, width = 3, font = 'Helvetica 14').place(x=440, y=280)
sec.set(' 00 ')
mins= StringVar()
Entry(win, textvariable = mins, width =3, font = 'Helvetica 14').place(x=400, y=280)
mins.set(' 60 ')

#Function for timer
def countdowntimer():
   times = int(mins.get())*60 + int(sec.get())
   while times > -1:
      minute,second = (times // 60 , times % 60)
      if minute > 60:
         minute = (minute // 60 , minute % 60)
      sec.set(second)
      mins.set(minute)

      #Update the time
      win.update()
      time.sleep(1)
      if(times == 0):
         sec.set('00')
         mins.set('00')
      times -= 1

while GPIO.input(12):
    countdowntimer()

# Total failure condition
if GPIO.input(12) |~ GPIO.input(10):
    label_Failure=Label(win, text="TOTAL FAILURE", font=("Courier 22 bold")).place(x = 170, y = 330)
    sec = StringVar()
    Entry(win, textvariable=sec, width = 3, font = 'Helvetica 14').place(x=440, y=280)
    sec.set(' 00 ')
    mins= StringVar()
    Entry(win, textvariable = mins, width =3, font = 'Helvetica 14').place(x=400, y=280)
    mins.set(' 00 ')

win.mainloop()
