import os
import serial
import pyotp

#Select Monitor Directory As The Script Directory
monDir = os.path.dirname(os.path.abspath(__file__))

#Change Monitor directory To Current Working Directory
os.chdir(monDir)

#List To Store User Input PassCode
#ipCode = [255,255,255,255,255,255]

#Initial Connection Status Is Set To False
connStat = False

#Initialize The HOTP Variable
hotp = pyotp.HOTP('JKFW4A5EJMHLHTQP')

otp = "031262"

while True :
        if not connStat :
                try :
                        ard = serial.Serial("/dev/ttyACM0")
                except :
                        try :
                                ard = serial.Serial("/dev/ttyACM1")
                        except :
                                try :
                                        ard = serial.Serial("/dev/ttyACM2")
                                except :
                                        print("Failed To Connect To Arduino")
                                        connStat = False
                                        continue

        connStat = True
        
        if "open" in os.listdir(monDir) :
                os.remove("open")
                print("Opened")
                ard.reset_output_buffer()
                ard.write(b"*o#")
                ard.reset_input_buffer()

        while ard.in_waiting :
                if ard.read() != b'*' :
                        continue

                cntFile = open("count", "r")
                cnt = int(cntFile.read())
                otp = hotp.at(cnt)
                cntFile.close()
                
                for i in range(0,6) :
                        ipDig = ard.read()
                        print(ord(ipDig))
                        if otp[i] != str(ord(ipDig)) :
                                print("Failed")
                                break
                        ard.read()
                else :
                        print("OK")
                        cntFile = open("count", "w")
                        cntFile.write(str(cnt + 1))
                        cntFile.close()
                        os.system("touch open")

                ard.reset_input_buffer()
                break

        
                

        connStat = ard.is_open
