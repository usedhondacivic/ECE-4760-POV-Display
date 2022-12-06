import time
import socket
import random
import winwifi
import bmp
import argparse
import numpy as np


# These constants should match the client
SERVER_PORT = 4242

# Import the library
import argparse
# Create the parser
parser = argparse.ArgumentParser()
# Add an argument
parser.add_argument('--r', type=int, default=90)
parser.add_argument('--debug', action='store_true')

# Parse the argument
args = parser.parse_args()

LED_NUM = 40
ROTATIONS = 90
RPB = 2
BUF_SIZE = LED_NUM * RPB * 3
PACKET_NUM = args.r // RPB

# Start connection
# wlan = network.WLAN(network.STA_IF)
# wlan.active(True)
# wlan.connect(WIFI_SSID, WIFI_PASSWORD)
wifi = winwifi.WinWiFi()
# wifi.connect()
try:
    interfaces = [i.ssid for i in winwifi.WinWiFi.get_connected_interfaces()]
except:
    interfaces = []
print(f'Connected to {interfaces[0]}')


# Wait for connect success or failure
max_wait = 20
while max_wait > 0:
    try:
        interfaces = [i.ssid for i in winwifi.WinWiFi.get_connected_interfaces()]
    except:
        interfaces = []
    if len(interfaces) > 0: break
    # if wlan.status() < 0 or wlan.status() >= 3:
    #     break
    max_wait -= 1
    print('waiting for connection...')
    time.sleep(1)

# Handle connection error
if len(interfaces) == 0:
    raise RuntimeError('wifi connection failed')




def send_data(name, debug=False):
    # Get image:
    if name == 'r':
        shp = np.zeros((args.r, LED_NUM, 3))
        rot_arr = np.full_like(shp, [200,1,1], dtype=int)
    elif name == 'bird':
        arrs = []
        for i in range(10):
            path = f"C:/Users/horwi/git/ECE-4760-final-project/PYTHON/pov_images/bird/frame-0{i}-delay-005s.bmp"
            rot_arr, _, _ = bmp.bmp_to_rad(path, LED_NUM, args.r)
            arrs.append(rot_arr)
        for _ in range(0,4,2):
            print('Sending animation')
            for i in range(7):
                try: 
                    send_arr(arrs[i])
                except Exception as e:
                    print(f'Send failed on frame {i} with below message:')
                    print(e)    
    elif name == 'flower':
        arrs = []
        for i in range(7):
            path = f"C:/Users/horwi/git/ECE-4760-final-project/PYTHON/pov_images/flower/{i}.bmp"
            rot_arr, _, _ = bmp.bmp_to_rad(path, LED_NUM, args.r)
            arrs.append(rot_arr)
        for _ in range(4):
            print('Sending animation')
            for i in range(7):
                try: 
                    send_arr(arrs[i])
                except Exception as e:
                    print(f'Send failed on frame {i} with below message:')
                    print(e)
    else:
        path = f"C:/Users/horwi/git/ECE-4760-final-project/PYTHON/pov_images/{name}.bmp"
        rot_arr, _, _ = bmp.bmp_to_rad(path, LED_NUM, args.r)
        send_arr(rot_arr, debug)
    
def send_arr(rot_arr, debug=False):
    # Open socket to the server
    sock = socket.socket()
    addr = ('0.0.0.0', SERVER_PORT)
    sock.bind(addr)
    sock.listen(1)
    print('server listening on', addr)

    # Wait for the client
    con = None
    con, addr = sock.accept()
    print('client connected from', addr)


    total_bytes = 0

    # repeat test for a number of iterations
    for i in range(PACKET_NUM):

        # Take RPB rotations from image
        curr_data = rot_arr[RPB*i : RPB*i+RPB][:][:]
        write_buf = bytearray(curr_data.flatten())
        if debug:
            print(len(curr_data.flatten()))
            print(len(write_buf))
            prstr = ""
            for i, b in enumerate(write_buf):
                if i % 3 == 0: prstr += 'r: ' + str(int(b)) + ' '
                if i % 3 == 1: prstr += 'g: ' + str(int(b)) + ' '
                if i % 3 == 2: prstr += 'b: ' + str(int(b)) + '\n'
            print(prstr)
        # write BUF_SIZE bytes to the client
        write_len = con.send(write_buf)
        print('Wrote %d bytes to client' % write_len)
        total_bytes += write_len

        # Check size of data written
        if write_len != BUF_SIZE:
            raise RuntimeError('wrong amount of data written')

    # All done
    con.close()
    sock.close()

    print(f"In total wrote {total_bytes} bytes")
    print("send completed")

while True:
    print('\nEnter an image name:')
    name = input()
    if name in ['e', 'exit']: break
    print('Sending image')
    try: 
        send_data(name, args.debug)
    except Exception as e:
        print('Send failed with below message:')
        print(e)

