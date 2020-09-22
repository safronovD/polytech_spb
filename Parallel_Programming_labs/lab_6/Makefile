NAME    := lab_6

BIN_DIR  := ./bin

.PHONY: build

build:
	mkdir -p ${BIN_DIR}
	g++ -fopenmp -std=c++20 main.cpp -o ${BIN_DIR}/${NAME}

run:
	${BIN_DIR}/${NAME}

clean:
	rm -rf ${BIN_DIR}