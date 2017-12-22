import socket

def client():
   
    host = socket.gethostname() #getting the hostname
    port = 5001 #receiving the port 
    client_socket = socket.socket() #getting socket
    clien_socket.connect((host, port)) #binding/connecting host and port
    message = input(' -> ') #getting and using the input
    while message.lower().strip() != 'bye': #ghd condition of entering 'bye'
        client_socket.send(message.encode()) #sending message
        data = client_socket.recv(8192).decode() #receiving the answer
        print ("Received, the server is: " + data) #printing the terminal
       
        message = input(' -> ') #getting the input
   
    client_socket.close() #closing

if __name__ == '__main__':
    client()
