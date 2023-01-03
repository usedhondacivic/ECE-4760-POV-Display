# DISPLAY_DRIVER.py
# Hub for taking a file and displaying it on the POV display
# Handles each variety of file separately, translates them to polar representation, then sends them over TCP to the display

import re

import file_translation
import tcp_server
import time
import cv2

from PIL import Image


def display_file(file_loc):
    # RegEx to get the file type
    file_type = re.search(r'.*\.(.*)', file_loc).group(1)
    match file_type.lower():
        case "png" | "jpg" | "jpeg" | "bmp":
            data = file_translation.translate_image_from_path(file_loc)
            # Uncomment to see plotted representation of the data being sent
            # file_translation.visualize_data(data)
            tcp_server.send_arr(data, True)
        case "gif":
            gif = Image.open(file_loc)
            print("sending looping gif, with: " +
                  str(gif.n_frames) + " frames.")
            while True:
                for i in range(gif.n_frames):
                    start_time = time.time()
                    gif.seek(i)
                    frame = gif.convert("RGB")
                    data = file_translation.translate_image_from_data(frame)
                    tcp_server.send_arr(data, True)
                    end_time = time.time()
                    d_t = end_time - start_time
                    time_remaining = (1/30) - d_t
                    if time_remaining < 0:
                        print("Missed 30 fps time")
                    time.sleep(time_remaining)

        case "mp4":
            cap = cv2.VideoCapture(file_loc)

            pass
        case _:
            pass
    pass
