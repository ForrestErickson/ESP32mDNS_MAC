# ESP32mDNS_MAC

ESP32mDNS_MAC
   Forrest Erickson  
   Date: 20210519  
   License / Dedication: Public domain, Free to good home.  
   Warranty: This program will kill you but is not guarenteed to do so.  

   Reads the ESP32 chip ID and puts it into the form of a MAC address.
   Composes a unique mDNS domain name composed of the PRODUCT_NAME and the MAC
   Reports unique mDNS domain name on serial port
   Serves a web page with the mDNS and suggests making a QT code of same.
   Winks LED

   NOTE: the mDNS tested by me to work on Windows 10, and iPhone and 
   acording to the intertubes works on MAC and Linux. 
   However it did not work on Androide Galaxy 7 which is expected 
   acourding to the intertubes.

   IMPORTANT: Set your own router credential
   
The first time you program and use your device, look for the **IP address** address and the **mDNS** on the serial port.
![Screen shot of serial port at boot.](SerialPort_IP_mDNS.gif)  

Type in the mDNS address on a browser. (Screen shot of served web page)  
![Screen shot of served web page](ServedWebPage.gif)

Make a QR code. (Screen shot of a QR generator web page)  
![Screen shot of a QR generator web page](QRgenerator.gif)

Print out this QR code and keep it with your device.
You can now browse fast your device with your iPhone.
