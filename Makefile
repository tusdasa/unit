PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
CC = gcc
TARGET = unit_app_test
INCLUDE_PATH = -I ~/unit_bin_1.24.0/include
LIB_PATH = -L ~/unit_bin_1.24.0/lib
LIB += -l unit -lm -lrt -ldl -lpthread -lpcre
$(TARGET): 
	$(CC) $(PROJECT_ROOT)nginx_unit_app.c $(INCLUDE_PATH) $(LIB_PATH) $(LIB) -o $(TARGET)
clean:
	rm -rf $(TARGET)
