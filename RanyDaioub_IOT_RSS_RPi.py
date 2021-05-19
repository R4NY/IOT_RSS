


#***********************************
#**            IOT_RSS            **
#** project: Room Security System **
#**        By: Rany Daioub        **
#**                               **
#***********************************



from smbus import SMBus                 #The SMBus library enable us to interact with I2C 
from picamera import PiCamera           #For RPi camera modual
import time                             #To use the delay time functions 
import telepot                          #The telegram library to allow us to interact with telegram bot ----> Telepot Docs: https://telepot.readthedocs.io/en/latest/
import RPi.GPIO as GPIO                 #To interact with the RPi pins


bot = telepot.Bot('INCLUDE_YOUR_TOKEN_HERE')     #The bot token please swap with your token (* Keep the '')
chat_id = YOUR_CHAT_ID_HERE                                                    #Your telegram chat id 
camera = PiCamera() 

addr = 0x07             #Setting the I2C address to 0x07
bus = SMBus(1)          #Communicating via bus 1
buzzerPin  = 26         #The buzzer pin

GPIO.setmode(GPIO.BOARD)                                #Setting the GPIO pins to read the numbers on the board not the ones beside GPIO(#)
GPIO.setup(buzzerPin, GPIO.OUT, initial=GPIO.LOW)       #Setting the buzzer pin to output data and initiate it to low

num = 1         #True value
i = 0           #The number that will be attached to images to make sure all are differant

while True:
    while num == 1:

            reading = bus.read_byte(addr)       #Read the value from the I2C bus
            print(reading)                      #Print the I2C value recived for verification
            time.sleep(2)
            if(reading == 2):                           #If reading is 2 => System is ON AND Door is Open
                    GPIO.output(buzzerPin, GPIO.HIGH)   #Make sound from the buzzer
                    time.sleep(1)
                    GPIO.output(buzzerPin, GPIO.LOW)
                    time.sleep(1)

                    GPIO.output(buzzerPin, GPIO.HIGH)
                    time.sleep(1)
                    GPIO.output(buzzerPin, GPIO.LOW)
                    time.sleep(1)

                    GPIO.output(buzzerPin, GPIO.HIGH)
                    time.sleep(1)
                    GPIO.output(buzzerPin, GPIO.LOW)
                    time.sleep(1)

                    camera.start_preview()              #Start the camera and take photo
                    time.sleep(2)
                    camera.capture('/home/pi/Desktop/image%s.jpg' %i)
                    camera.stop_preview()
                    time.sleep(3)
                    bot.sendPhoto(chat_id,photo=open('/home/pi/Desktop/image'+str(i)+'.jpg' ,'rb')) #Send the photo to the telegram bot
                    time.sleep(2)
                    i = i + 1
                    break
            else:                       #Else reading is other than 2 => Do nothing 
                    pass

