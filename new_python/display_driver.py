# DISPLAY_DRIVER.py
# Hub for taking a file and displaying it on the POV display
# Handles each variety of file separately, translates them to polar representation, then sends them over TCP to the display

import re

import file_translation
import tcp_server


def display_file(file_loc):
    # RegEx to get the file type
    file_type = re.search(r'.*\.(.*)', file_loc).group(1)
    match file_type:
        case "png" | "jpg" | "jpeg" | "bmp":
            data = file_translation.translate_image(file_loc)
            tcp_server.send_arr(data)
        case "gif":
            pass
        case "mp4":
            pass
        case _:
            pass
    pass
