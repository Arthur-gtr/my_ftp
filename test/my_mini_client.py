import socket
import threading
import sys

HOST = "127.0.0.1"

def reader(sock: socket.socket):
    while True:
        try:
            data = sock.recv(4096)
            if not data:
                print("\n[Connexion fermée par le serveur]")
                sys.exit(0)
            print("\n<<", data.decode(errors="ignore"))
            print("$>", end="", flush=True)
        except:
            break

def main():
    port = int(input("Port : "))

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((HOST, port))
    t = threading.Thread(target=reader, args=(sock,), daemon=True)
    t.start()
    while True:
        line = input("$>")
        to_send = line.replace("CRLF", "\r\n")
        sock.sendall(to_send.encode())

if __name__ == "__main__":
    main()
