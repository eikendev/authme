## About

This repository demonstrates shared memory and process synchronization in C.
Multiple clients can connect to the same server and use it as a data store.
It was created as a project in the lecture "Operating Systems" at [TU Wien](https://www.tuwien.at/).

## Usage

`make build` will generate two binaries, a server (`auth-server`) and a client (`auth-client`), in the `out/` directory.
First, start a server, then you can start multiple clients.

A client can be used to register an account (flag `-r`), as well as storing and retrieving data (flag `-l`).
```
$ ./auth-client
Usage: ./auth-client { -r | -l } <username> <password>
```

After registration, connect to the server by providing the credentials as arguments.
If the login succeeds, the following options are offered.
```
Login successful.
Commands:
  1) write secret
  2) read secret
  3) logout
Please select a command (1-3):
```
