# WEB_SERVER.py
# Creates a web server that allows users to submit files to show on the display.
# Files get saved to the ./media folder, and then passed to image_translation.py to get converted into a polar representation.


from http.server import BaseHTTPRequestHandler, HTTPServer
import logging
from requests_toolbelt import MultipartDecoder
import re

import display_driver

hostName = "localhost"
serverPort = 8080

server_html = open("index.html").read()


class MyServer(BaseHTTPRequestHandler):
    def _set_response(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

    def do_GET(self):
        if self.path == '/':
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.end_headers()
            self.wfile.write(bytes(server_html, 'utf-8'))
        elif self.path == '/bear.bmp':
            self.send_response(200)
            self.send_header("Content-type", "image/bmp")
            self.end_headers()
            with open('./bear.bmp', 'rb') as file_handle:
                self.wfile.write(file_handle.read())
        # todo: add a get path that sends status data to display on the dashboard

    def do_POST(self):
        # Handle the client POSTing a file to the server.
        # Data comes encoded using a special multipart/form-data format.
        # Here we decode the message to retrieve the filename and data, and save appropriately

        # Read the message and decode it
        file_length = int(self.headers['Content-Length'])
        content = self.rfile.read(file_length)
        decoder = MultipartDecoder(content, self.headers['Content-Type'])

        # RegEx to extract the file name from the header of the multipart/form-data message
        filename = re.search(
            r'(?<=\bfilename=)"(.*)"$', decoder.parts[0].headers[b'Content-Disposition'].decode()).group(1)

        # Write the data to the correct area
        with open("./media/"+filename, 'wb') as output_file:
            output_file.write(decoder.parts[0].content)

        self.send_response(201, 'Created')
        self.end_headers()

        # Pass the file to the display driver to update the screen
        display_driver.display_file(
            "./media/"+filename)


def run(server_class=HTTPServer, handler_class=MyServer, port=8080):
    logging.basicConfig(level=logging.INFO)
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    logging.info('Starting httpd...\n')
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    logging.info('Stopping httpd...\n')


if __name__ == '__main__':
    # Sent default start image
    display_driver.display_file("./bear.bmp")
    from sys import argv

    if len(argv) == 2:
        run(port=int(argv[1]))
    else:
        run()
