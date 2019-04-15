import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("47.102.216.81", 9090))

while True:
    message, addr = sock.recvfrom(1024)
    print("Received from client: {}".format(message))

    sock.sendto(bytes(message), addr)
