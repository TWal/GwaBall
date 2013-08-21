#!/bin/bash
cd bin
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --track-origins=yes ./GwaBall
