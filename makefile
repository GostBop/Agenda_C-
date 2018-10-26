CC := g++
FLAGS := -std=c++11
INC_DIR := include
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
INCLUDE := -I./$(INC_DIR)

$(BIN_DIR)/main : $(BUILD_DIR)/User.o $(BUILD_DIR)/Meeting.o $(BUILD_DIR)/Date.o $(BUILD_DIR)/Storage.o $(BUILD_DIR)/Agenda.o $(BUILD_DIR)/AgendaUI.o $(BUILD_DIR)/AgendaService.o
	@mkdir -p $(BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -o $@ $(BUILD_DIR)/User.o $(BUILD_DIR)/Meeting.o $(BUILD_DIR)/Date.o $(BUILD_DIR)/Storage.o $(BUILD_DIR)/Agenda.o $(BUILD_DIR)/AgendaUI.o $(BUILD_DIR)/AgendaService.o

$(BUILD_DIR)/AgendaUI.o : $(SRC_DIR)/AgendaUI.cpp $(INC_DIR)/AgendaUI.hpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/AgendaUI.cpp

$(BUILD_DIR)/AgendaService.o : $(SRC_DIR)/AgendaService.cpp $(INC_DIR)/AgendaService.hpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/AgendaService.cpp

$(BUILD_DIR)/Storage.o : $(SRC_DIR)/Storage.cpp $(INC_DIR)/Storage.hpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/Storage.cpp

$(BUILD_DIR)/Meeting.o : $(SRC_DIR)/Meeting.cpp $(INC_DIR)/Meeting.hpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/Meeting.cpp

$(BUILD_DIR)/User.o : $(SRC_DIR)/User.cpp $(INC_DIR)/User.hpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/User.cpp

$(BUILD_DIR)/Date.o : $(SRC_DIR)/Date.cpp $(INC_DIR)/Date.hpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/Date.cpp

$(BUILD_DIR)/Agenda.o : $(SRC_DIR)/Agenda.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/Agenda.cpp

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(BIN_DIR)
