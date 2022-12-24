import socket
import numpy

# Adapted from: https://realpython.com/python-sockets/#communication-breakdown

# PROTOCOL
# Server sends:
# 4 Byte header - 2 byte meta info (1 byte for num rotations, 1 for brightness) - 2 byte Message size
# x Byte body - decided by message size in header
#
# Client responds:
# 1 Byte rpm
# 1 Byte buffer status

HOST = "0.0.0.0"  # Listen on all ip's
PORT = 4242  # Port to listen on (non-privileged ports are > 1023)

conn = None
addr = None
sock = None


def init():
    global conn
    global addr
    global sock
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((HOST, PORT))
    sock.listen()
    print("Waiting for display to connect")
    conn, addr = sock.accept()
    print(f"Connected by {addr}")


def send_arr(data, debug=False):
    flattened = data.flatten()
    # This line is needed to convert numpy int32 to standard python int. Otherwise, bytearray produces four bytes per entry.
    # See: https://stackoverflow.com/questions/9452775/converting-numpy-dtypes-to-native-python-types
    flattened = getattr(flattened, "tolist", lambda: flattened)()

    flattened = bytearray(flattened)

    try:
        conn.sendall(flattened)
    except:
        print("Lost connection. Waiting for new connection.")
        init()

    print("Successfully sent image")
    ret_data = b''
    while True:
        try:
            ret_data = conn.recv(2)
        except:
            print("Lost connection. Waiting for new connection.")
            init()
            ret_data = conn.recv(2)
        if len(ret_data) == 2:
            break
    print("Display returned: ")
    print(ret_data)


def close():
    global conn
    global sock
    conn.close()
    sock.close()
