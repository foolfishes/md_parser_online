## a markdown parser run on tcp server

## use
server directory contains all the file needed; and client directory contains a simple client test program.
the file in sever_test and client_test are not collated.

run the client:
- cd client/build,
- cmake ../
- make
- ./client

run the server:
- cd server/build
- cmake ../
- make
- ./server

## introduce
the server use the epoll and create a thread pool to deal connection.
if a connection is created, it will add a task to the thread pool to deal it.


## to do 
add http support

by foolfishes.