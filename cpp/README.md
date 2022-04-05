## calc

The final calculator program running through Stroustrup's book. 

Very simple but supports arithmetic operations, some math constants and basic functions.

![screen](./cpp/screenshot/calc.png)

## reversi

By default (whitout options) the local version is launched. </p>
For a client-server game you need:
* Start the game as a server, specifying the port to connect to. 
* Start the game as a client, specifying the server address and the selected port. 
* Choose the color of the chips in the terminal on the server side. 

To exit, press Q. The server can start a new game using C-n, or start a new game by inverting the color of the chips using C-i.

```
Usage: ./reversi <--server|--client> -i <ip> -p <port>
--server -p <port> - run server version.
--client -i <ip> -p <port> - run client version.
-i <ip> - server IP addres
-p <port> - port
-h,--help - this help
```
![local](./cpp/screenshot/reversi.gif)


## solitaire
The fourth exercise of the seventeenth chapter :)

![local](./cpp/screenshot/solitaire.png)
