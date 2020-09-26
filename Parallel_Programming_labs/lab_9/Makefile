NAME        := lab_6
GEN_NAME    := data_gen
BIN_DIR     := ./bin

.PHONY: build

build:
	mkdir -p ${BIN_DIR}
	mpic++ -std=c++17 main.cpp -o ${BIN_DIR}/${NAME}

run: build
	sudo -u me mpirun -np $(proc_count) ${BIN_DIR}/${NAME}

gen_file_build:
	g++ -std=c++20 data_gen.cpp -o ${BIN_DIR}/${GEN_NAME}

gen_file: gen_file_build
	${BIN_DIR}/${GEN_NAME}

clean:
	rm -rf ${BIN_DIR}
