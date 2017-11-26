import socket
import os

def server_program():
    # get the hostname
    host = socket.gethostname()
    port = 5002  # initiate port no above 1024

    server_socket = socket.socket()  # get instance
    # look closely. The bind() function takes tuple as argument
    server_socket.bind((host, port))  # bind host address and port together

    # configure how many client the server can listen simultaneously
    server_socket.listen(3)
    while True:
        conn, address = server_socket.accept()  # accept new connection
        print("Connection from: " + str(address))
        pid=os.fork()
        if(pid==0):
        # receive data stream. it won't accept data packet greater than 1024 bytes
            while True:
                data = conn.recv(8192).decode()
                if not data:
                # if data is not received break
                    break
                print("from connected user: " + str(data))
                data = input(' -> ')
                conn.send(data.encode())  # send data to the client
    conn.close()  # close the connection


if __name__ == '__main__':
    server_program()
