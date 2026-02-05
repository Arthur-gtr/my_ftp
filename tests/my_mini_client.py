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
    try:
        port = int(input("Port : "))
    except ValueError:
        print("Port invalide.")
        return

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    try:
        sock.connect((HOST, port))
    except ConnectionRefusedError:
        print(f"Impossible de se connecter à {HOST}:{port}")
        return
    t = threading.Thread(target=reader, args=(sock,), daemon=True)
    t.start()

    while True:
        try:
            line = input("$> ")
            
            if line.lower() in ["exit", "quit"]:
                break
            print(line)
            to_send = line.replace("CRLF", "\r\n")
            sock.sendall(to_send.encode())
            
        except KeyboardInterrupt:
            print("\nArrêt du client.")
            break
        except BrokenPipeError:
            print("Erreur: Pipe cassé (serveur déconnecté ?)")
            break

    sock.close()

if __name__ == "__main__":
    main()