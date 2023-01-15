from socket import *

serverPort = 1903
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('', serverPort))
serverSocket.listen(1) # Configure how many client the server can listen simultaneously

connectionSocket, addr = serverSocket.accept()
print('Conectado ao cliente ', str(addr))
    
opcao = connectionSocket.recv(2048)
print('Operação: ', opcao.decode())

nome = connectionSocket.recv(2048).decode()
print("Nome do arquivo: ", nome)
        
if opcao == b'1': # Servidor recebe do cliente.
        
        arquivo = open(nome, 'wb')
        mensagem = connectionSocket.recv(6053)
	#arquivo.write(mensagem)
        #linha = arquivo.read(2048)
        mensagem_text = str(mensagem, 'UTF-8')
        #arquivo.write(mensagem_text)
        print("Mensagem do cliente: ", mensagem_text)
               
else:             # Servidor envia ao cliente.
        arquivo = open(nome, 'rb')
        bytesarray = arquivo.read(6053)
        connectionSocket.send(bytesarray)

arquivo.close()
connectionSocket.close()
