# Telepong server

Available commands:
* **/show** -> Shows a list of clients in the general room.
* **/room [room id]** -> Join to the room with the id you just typed.

Related repository:
* [Client](https://github.com/huparelaa/pong-client)


# Juego Pong con Multijugador en Línea usando AWS

## 1. Introducción

Este proyecto se centra en el desarrollo de un juego clásico llamado 'Pong', pero con un toque más moderno. En lugar de jugar contra la máquina, permite que dos o más jugadores compitan en una partida de Pong en línea utilizando servidores de AWS.

La clave de este proyecto radica en su estilo arquitectónico cliente/servidor. Los jugadores descargan una aplicación cliente que se conecta a un servidor, lo que posibilita partidas en tiempo real entre jugadores desde sus propios dispositivos.

Para lograr esta funcionalidad, hemos utilizado el lenguaje de programación C y nos hemos apoyado en la API de Sockets Berkeley. Gracias a esta combinación, hemos creado una aplicación servidor capaz de recibir peticiones desde múltiples aplicaciones cliente, garantizando una experiencia de juego fluida y sin retrasos.

## 2. Desarrollo

Desarrollamos un juego Pong con una arquitectura cliente/servidor y presentamos un proceso de desarrollo paso a paso para llevar a cabo este proyecto utilizando el lenguaje C y la API de sockets de Berkeley:

### **Paso 1: Diseñar el Protocolo de Comunicación**

- Definimos el protocolo de comunicación entre el cliente y el servidor, llamado "Protocole". Esto incluye la estructura de los mensajes que se enviarán entre el cliente y el servidor para llevar a cabo las operaciones del juego, como el registro del jugador, la actualización de la posición de la paleta, el informe de colisiones, etc.

### **Paso 2: Configurar el Servidor**

- Creamos un programa de servidor en C que escucha en un puerto específico (especificado como argumento de línea de comandos) y registra toda la información relevante en un archivo de registro (especificado como argumento de línea de comandos). Utilizamos la API de sockets de Berkeley para gestionar las conexiones entrantes y salientes.

- Configuramos el servidor para aceptar múltiples conexiones de clientes de manera concurrente.

- Diseñamos la lógica del servidor para manejar las solicitudes entrantes de los clientes. Esto incluye el procesamiento de solicitudes de registro, actualizaciones de posición, puntuaciones, etc., según el protocolo definido en el Paso 1.

### **Paso 3: Crear el Cliente**

- Creamos un programa de cliente en C que se conecta al servidor utilizando la dirección IP o el nombre de host del servidor y el puerto específico. Utilizamos la API de sockets de Berkeley para establecer la conexión.

- Implementamos la funcionalidad de registro en el cliente. El cliente envía un mensaje de registro al servidor con un apodo (nickname) elegido por el jugador.

- Diseñamos la interfaz de usuario del cliente, mostrando una ventana interactiva para jugar al juego Pong.

### **Paso 4: Implementar la Lógica del Juego**

- En el servidor y el cliente, desarrollamos la lógica del juego Pong. Esto incluye la representación de la pelota, las paletas, la detección de colisiones, la actualización de la posición de la paleta, el seguimiento de las puntuaciones, etc.

- Aseguramos que el servidor y el cliente estén sincronizados para mantener el estado del juego consistente entre todos los jugadores.

### **Paso 5: Implementar la Comunicación Cliente-Servidor**

- Utilizamos el protocolo "Protocole" definido en el Paso 1 para permitir que el cliente y el servidor se comuniquen entre sí durante el juego. Esto incluye el envío de mensajes de actualización de posición, solicitudes de puntuación, notificaciones de colisiones, etc.

### **Paso 6: Implementar el Servidor en la Nube (AWS)**

- Por último, desplegamos el servidor de juego en un servidor en la nube, como AWS. Configuramos las reglas de seguridad y red para permitir que los clientes se conecten al servidor en la nube por medio de una conexión UDP.

### **Diagramas de apoyo UML**

- **Casos de uso**
  ![image](https://github.com/huparelaa/pong-client/assets/81880494/d542dede-4686-4058-99a6-96c08ba0af55)

- **Diagrama de secuencia**
  ![image](https://github.com/huparelaa/pong-client/assets/81880494/6ad31f59-1744-4b5b-b2c6-04daa06dc896)


## 3. Conclusiones

### Refinamiento de Habilidades de Programación en C

El desarrollo de un juego retro 'Pong' nos brindó la oportunidad de fortalecer y aplicar conceptos de programación en el lenguaje C, incluyendo el uso de nuevos conceptos de dicho lenguaje.

### Aplicación de Arquitectura Cliente/Servidor

La implementación de una arquitectura cliente/servidor no solo brindó la comprensión de la comunicación en red, sino que también fomentó el aprendizaje de buenas prácticas de programación y seguridad en aplicaciones conectadas a Internet, con una implementación en la nube de AWS.

### Énfasis en Buenas Prácticas de Programación

Durante el desarrollo del juego, aplicamos diversos principios de diseño de software, modularidad y organización de código. Esto nos permitió afianzar las buenas prácticas de programación, como la legibilidad del código, la documentación adecuada y la gestión de errores. Gracias a la creación de este proyecto, obtuvimos una visión más amplia de nuevos conceptos con la implementación de sockets.

## 4. Referencias

* https://github.com/gesslerpd/udp-chat
* https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
* https://beej.us/guide/bgnet/
* https://beej.us/guide/bgc/
