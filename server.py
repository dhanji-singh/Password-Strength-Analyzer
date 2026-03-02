#!/usr/bin/env python3
"""
Simple HTTP server to run the Password Strength Analyzer web interface
Usage: python3 server.py
Then open http://localhost:8000 in your browser
"""

import http.server
import socketserver
import os
import webbrowser
from threading import Timer

PORT = 8000
DIRECTORY = os.path.dirname(os.path.abspath(__file__))

class MyHTTPRequestHandler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=DIRECTORY, **kwargs)

    def log_message(self, format, *args):
        print(f"[{self.log_date_time_string()}] {format % args}")

def open_browser():
    """Open the browser after a short delay"""
    webbrowser.open(f'http://localhost:{PORT}')

if __name__ == "__main__":
    os.chdir(DIRECTORY)
    
    with socketserver.TCPServer(("", PORT), MyHTTPRequestHandler) as httpd:
        print(f"=" * 60)
        print(f"Password Strength Analyzer - Web Interface")
        print(f"=" * 60)
        print(f"Starting HTTP server on http://localhost:{PORT}")
        print(f"")
        print(f"Open your browser and visit: http://localhost:{PORT}")
        print(f"")
        print(f"Press Ctrl+C to stop the server")
        print(f"=" * 60)
        
        # Open browser automatically after 1 second
        timer = Timer(1.0, open_browser)
        timer.daemon = True
        timer.start()
        
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\n\nServer stopped.")
            httpd.server_close()
