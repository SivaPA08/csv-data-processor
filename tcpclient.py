import json
import socket
import struct

HOST = "127.0.0.1"
PORT = 9000  # must match server


def recv_all(sock, n):
    data = b""
    while len(data) < n:
        chunk = sock.recv(n - len(data))
        if not chunk:
            raise ConnectionError("Socket closed")
        data += chunk
    return data


def main():
    cmd = "a(Col1,0,10)|u(Col2,*,*,x*2)"

    with socket.create_connection((HOST, PORT)) as sock:
        # ---- SEND COMMAND ----
        payload = cmd.encode("utf-8")
        sock.sendall(struct.pack("!I", len(payload)))
        sock.sendall(payload)

        # ---- RECEIVE RESPONSE ----
        raw_len = recv_all(sock, 4)
        msg_len = struct.unpack("!I", raw_len)[0]

        data = recv_all(sock, msg_len)
        response = json.loads(data.decode("utf-8"))

        print("=== SERVER RESPONSE ===")
        print(json.dumps(response, indent=4))


if __name__ == "__main__":
    main()
