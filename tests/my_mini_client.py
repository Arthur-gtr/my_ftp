import socket
import threading
import sys

HOST = "127.0.0.1"
data_sock = None

def recv_all(sock):
    data = b""
    while True:
        chunk = sock.recv(4096)
        if not chunk:
            break
        data += chunk
    return data

def enter_pasv(control):
    global data_sock

    control.sendall(b"PASV\r\n")
    resp = control.recv(4096).decode(errors="ignore")
    print("\n<<", resp.strip())

    if not resp.startswith("227"):
        print("[PASV] Erreur: serveur a refusé")
        return
    try:
        start = resp.index("(") + 1
        end = resp.index(")")
        nums = resp[start:end].split(",")
        nums = list(map(int, nums))
    except:
        print("[PASV] Réponse invalide")
        return

    ip = ".".join(map(str, nums[:4]))
    port = nums[4] * 256 + nums[5]

    # On crée la socket data
    data_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    data_sock.connect((ip, port))
    print(f"[PASV] Connexion data ouverte sur {ip}:{port}")

def do_list(control):
    global data_sock

    if data_sock is None:
        print("[LIST] Erreur: PASV requis")
        return
    control.sendall(b"LIST\r\n")
    listing = recv_all(data_sock)
    data_sock.close()
    data_sock = None

    print("\n[LIST] Répertoire :\n")
    print(listing.decode(errors="ignore"))

def do_retr(control, filename):
    global data_sock
    if data_sock is None:
        print("[RETR] Erreur: PASV requis")
        return

    cmd = f"RETR {filename}\r\n".encode()
    control.sendall(cmd)

    # reçoit data
    content = recv_all(data_sock)
    data_sock.close()
    data_sock = None

    with open(filename, "wb") as f:
        f.write(content)

    print(f"[RETR] Fichier reçu → {filename}")

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
    global data_sock

    port = int(input("Port : "))

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((HOST, port))
    t = threading.Thread(target=reader, args=(sock,), daemon=True)
    t.start()

    while True:
        line = input("$>").strip()
        if line.upper().startswith("PASV"):
            enter_pasv(sock)
            continue
        if line.upper() == "LIST":
            do_list(sock)
            continue
        if line.upper().startswith("RETR "):
            filename = line[5:].strip()
            do_retr(sock, filename)
            continue
        to_send = line.replace("CRLF", "\r\n")
        sock.sendall(to_send.encode())

if __name__ == "__main__":
    main()

