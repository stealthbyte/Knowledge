import http.server
import socketserver

class RequestHandler(http.server.BaseHTTPRequestHandler):
    def doGET():
        pass

class ThreadedHTTPServer(socketserver.ThreadingMixIn, http.server.HTTPServer):
    pass

server = ThreadedHTTPServer(('localhost', 80), RequestHandler)
server.serve_forever()