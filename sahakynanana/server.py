import socket
import os

def server():

    host = socket.gethostname() #the get of the hostname
    port = 5001 #defining port number

    server_socket = socket.socket() #get socket
    server_socket.bind((host, port)) #binding port and host
    server_socket.listen(3) #defining the number of client that can listen to server at the same time

    while True: #accepting new connection
        conn, address = server_socket.accept()
        print ("Connected to: " + str(address))
        pid=os.fork()
        if(pid==0):

            while True: #data stream receiving
                data=conn.recv(8192).decode()
                if not data:
                    break 
                print("user is: " + str(data))
                data = input(' -> ')
                conn.send(data.encode()) #sending data to client 
    conn.close() #closing


if __name__ == '__main__':
    server()
