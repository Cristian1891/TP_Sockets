import socket


def show_menu():
    print("Seleccione una opcion:")
    print("1. Generar nombre de usuario")
    print("2. Generar password")
    print("3. Salir")
    choice = int(input("Ingrese su eleccion (1-3): "))
    return choice


def get_length():
    return int(input("Ingrese la longitud deseada: "))

def main():
    # Definicion de la dirección IP y el puerto del servidor al que se va a conectar el cliente.
    server_ip = "127.0.0.1"
    server_port = 8080

    # Crea un socket de cliente para la comunicación con el servidor.
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # Intenta conectar el socket al servidor especificado.
    # Si ocurre un error al conectar, muestra un mensaje de error y termina el programa.
    try:
        client_socket.connect((server_ip, server_port))
    except socket.error as e:
        print(f"Error al conectar al servidor: {e}")
        return

    while True:
        choice = show_menu()
        if choice == 3:
            break
        if choice == 1:
            print("Generar nombre de usuario")
            length = get_length()
            request = f"USERNAME {length}"
        elif choice == 2:
            print("Generar contraseña")
            length = get_length()
            request = f"PASSWORD {length}"
        else:
            print("Opción inválida. Intente nuevamente.")
            continue

        # Envía la solicitud al servidor.
        client_socket.sendall(request.encode())
        
        # Intenta recibir la respuesta del servidor y la decodifica.
        # Si ocurre un error al recibir datos, muestra un mensaje de error y continúa con el bucle.
        try:
            response = client_socket.recv(1024).decode()
        except socket.error as e:
            print(f"Error al recibir datos del servidor: {e}")
            continue

        print(f"Respuesta del servidor: {response}")
    # Cierra el socket al terminar la comunicación.
    client_socket.close()

if __name__ == "__main__":
    main()