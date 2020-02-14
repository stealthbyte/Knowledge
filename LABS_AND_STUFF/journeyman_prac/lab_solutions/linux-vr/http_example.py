import socket

def main():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('172.16.2.68', 31337))
    x = "GET HTTP/1.1 / \r\n"
    x += "Host: A\r\n"
    x += "Date: B\r\n\r\n"
    s.send(x.encode('utf-8'))
    b = s.recv(1024)
    print(b)


if __name__ == "__main__":
        main()
