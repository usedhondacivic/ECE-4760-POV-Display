import time
import socket
import random
import winwifi
import bmp

# These constants should match the client
SERVER_PORT = 4242

LED_NUM = 4
ROTATIONS = 10
RPB = 5
BUF_SIZE = LED_NUM * RPB * 3
PACKET_NUM = ROTATIONS // RPB


# Get image:
path = "C:/Users/horwi/Downloads/dog.bmp"
rot_arr, _, _ = bmp.bmp_to_rad(path, LED_NUM, ROTATIONS)


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
print(interfaces)


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
# else:
#     print('connected')
#     status = wlan.ifconfig()
#     print( 'ip = ' + status[0] )

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

# repeat test for a number of iterations
for i in range(PACKET_NUM):

    # Generate a buffer of random data
    # random_list = []
    # for n in range(BUF_SIZE):
    #     random_list.append(random.randint(0, 255))
    # write_buf = bytearray(random_list)

    # Take RPB rotations from image
    curr_data = rot_arr[5*i : 5*i+5][:][:]
    print(curr_data)
    write_buf = bytearray(curr_data.flatten())

    # write BUF_SIZE bytes to the client
    write_len = con.send(bytearray(write_buf))
    print('Written %d bytes to client' % write_len)

    # Check size of data written
    if write_len != BUF_SIZE:
        raise RuntimeError('wrong amount of data written')

# All done
con.close()
sock.close()
print("test completed")
