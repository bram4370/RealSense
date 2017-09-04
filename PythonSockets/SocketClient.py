import socket
import select
import threading


TCP_IP = '192.168.2.2'
TCP_PORT = 22045
BUFFER_SIZE = 1024
MESSAGE = 'hello from the other side!\n\0'
listen = 1
def listener(s):
    while(listen):        
        s.setblocking(0)        
        ready = select.select([s],[],[],10) #check for 10 seconds if there is data        <<<<<<<<<  NEED TO SEND DATA FOR IT TO NOT FUCK UP 
        if(ready[0]):                        
            data=s.recv(BUFFER_SIZE) #wait till information from other side    
            if(data==b''):
                print("error: disconnected?")
                listen=0
            print(data.decode("UTF-8"))        
            


if __name__ == '__main__':
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM) #socket with ipv4 and tcp
    s.connect((TCP_IP,TCP_PORT)) #establish connection with given parameters
    s.setblocking(0)    
    #s.send(MESSAGE.encode()) #send message to other side
    data = "\0"*BUFFER_SIZE
    listenthread = threading.Thread(None,listener,'listen_thread',args=(s,))
    listenthread.start()
    z="0"
    while(z!=''):
        z= input()
        s.send(z.encode())
    listen=0
    listenthread.join()    
    s.close()