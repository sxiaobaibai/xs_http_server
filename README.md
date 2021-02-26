# xs_http_server
http server by C++

## Usage
build the project
```
mkdir build
cd build
cmake ..
make
```

make a public directory
```
cd ..
mkdir public
```

write index.html file in public directory. Then execute server by
```
cd build
./server
```
Accesss http://localhost:12345
The default port is 12345.

## Todo
- ~~TCP connection~~
- ~~listen~~
- generate new process for dealing request
- header parser
- send reponse
- valid path
- valid file
