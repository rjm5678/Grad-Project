#!/usr/bin/python3
import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("", 9090))

while True:
    message, addr = sock.recvfrom(1024)
    print("Received from {}: {}".format(addr, message))
    sock.sendto(bytes(message), addr)
    if message == b'q':
         break
