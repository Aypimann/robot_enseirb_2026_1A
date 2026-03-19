import serial
import time

PORT = '/dev/ttyUSB0'
BAUDRATE = 115200
TIMEOUT = 2

if __name__ == "__main__":
    ser = serial.Serial(PORT, BAUDRATE, timeout=TIMEOUT)
    print(f"Connexion, port: {PORT}, baudrate: {BAUDRATE}")
    
    time.sleep(2)
    
    ser.reset_input_buffer()
    ser.reset_output_buffer()
    
    uin = ''
    while(not 'x' in uin):
        uin = input()
        ser.write(f"{uin}\n".encode('utf-8'))
    
    if ser.in_waiting > 0:
        startup_msg = ser.readline().decode('utf-8').strip()
        print(f"ESP32: {startup_msg}\n")
    
    success_count = 0
    for msg, i in zip(test_messages, range(len(test_messages))):
        print(f"Test {i}/{len(test_messages)}")

        print(f"  Send: '{msg}'")
        
        response = ser.readline().decode('utf-8').strip()
        print(f"  Reçu:  '{response}'")
        
        if response == msg:
            print("  Succes echo\n")
            success_count += 1
        else:
            print("  Unexpected result\n")
        
        time.sleep(0.5)
    
    print("=" * 50)
    print(f"Résults: {success_count}/{len(test_messages)}")
    
    ser.close()
