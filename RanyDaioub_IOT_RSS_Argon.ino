
/*

***********************************
**            IOT_RSS            **
** project: Room Security System **
**        By: Rany Daioub        **
**                               **
***********************************

*/


#include <Wire.h>    //The Wire library to control communications over I2C


int reading = D4;    //The Door Magnatic sensor pin
int red = D5;        //The Red LED pin
int green = D6;      //The Green LED pin
int data;            //To store Door sensor readings 
int x;               //Voice command Value (1 = ON),(2 = OFF)
int sum;             //The sum of door and voice command readings(X + data)

/* 
Function that is going to handle
turning the system ON or OFF
*/ 


int sys_control(String command)
{
    if(command.equals("on"))        //If (command = ON) => 
    {
        digitalWrite(red, LOW);     //Turn Red LED OFF
        
        digitalWrite(green, HIGH);  //Turn Green LED ON
        
        x = 1;                      
        
        return 1;
    }
    
    else if(command.equals("off")) //If (command = OFF) =>
    {
        digitalWrite(green, LOW);  //Turn Green LED OFF
        
        digitalWrite(red, HIGH);
        delay(1000);
                                    //Turn Red LED ON then OFF
        digitalWrite(red, LOW);
        delay(1000);
        
        x = 3;
        
        return 3;
    }
    
    else
    {
        return -1;
    }
}


void setup() 
{
    pinMode(reading, INPUT);      //Setting the sensor pin to read data
    pinMode(red, OUTPUT);         //Setting the LED pin to output data
    pinMode(green, OUTPUT);       //Setting the LED pin to output data
    Serial.begin(9600);           
    Wire.begin(0x07);             //Start communicating on 0x07 for I2C
    Wire.onRequest(sendData);     // Send Data via I2C when requested
    Particle.function("System",sys_control); //Set the particle function "System"
}




void loop()
{
    data = digitalRead(reading); //read door sensor value and store it in data
    Serial.print("Door Status: ");  
    Serial.println(data);        //Print the reading from door sensor
    delay(1000);
    
    Serial.print("----> System Status: ");
    Serial.println(x); //Print the reading from google assistant
    delay(1000);
    
    sum = x + data;
    Serial.print("----> the Sum: ");
    Serial.println(sum);  //Print the sum readings from door and voice command
    
    
/*
-----------
Open = 1
Closed = 0
-----------
ON = 1
OFF = 3
-----------

The Sum:

Open + OFF => (1 + 3) => sum = 4
Open + ON => (1 + 1) => sum = 2 // We need Notifications only when this value is recieied in the RPi
Closed + OFF => (0 + 3) => sum = 3
Closed + ON => (0 + 1) => sum = 1

*/



    if(x == 1)     //If voice command is ON
    {
        if(data == 1)    //If door is opened 
        {
            digitalWrite(green, LOW); //Turn Green LED OFF
            
            digitalWrite(red, HIGH);  //Turn Red LED ON
            delay(5000);
            
            Particle.publish("door_open", PRIVATE); //publish to the Webhook
        }
        
        else         //If door is closed
        {
            digitalWrite(red, LOW);    //Turn Red LED OFF
            
            digitalWrite(green, HIGH); //Turn Green LED ON
            delay(5000);
        }
    }
    
    else if(x == 2)  //If voice command is OFF
    {
        data = 0;
        
        digitalWrite(red, LOW);
        
        digitalWrite(green, LOW);
    }
}

void sendData()
{
    Wire.write(sum); //To send door and voice readings via I2C to the RPi
}


