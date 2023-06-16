#!/bin/bash

c++  -Wall -Wextra -Werror \
    ../../logs/logs.cpp  main.cpp ../../srcs/server/Socket.cpp \
    ../../srcs/server/Listner.cpp \
    ../../srcs/server/Kque.cpp \
    ../../srcs/client/Client.cpp \
    ../../srcs/recive_request/Receive.cpp \
    ../../srcs/recive_request/Parser.cpp \
    ../../srcs/recive_request/Parse_headers.cpp  \
    ../../srcs/recive_request/Parser_utils.cpp  \
    ../../srcs/GET/GET.cpp \
    ../../srcs/GET/GET_response.cpp  \
    ../../srcs/response/Respond.cpp\
    ../../srcs/parsing/parsing_lib.cpp -o test.exe \
 && ./test.exe && \
sleep 1 && \
echo && \
echo Checking leaks && \i
echo && \
echo && \
valgrind ./test.exe
