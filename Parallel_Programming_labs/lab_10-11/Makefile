NAME        := lab_6
GEN_NAME    := data_gen
BIN_DIR     := ./bin

.PHONY: build

build:
	mkdir -p ${BIN_DIR}
	mpic++ -std=c++17 reduce_scatter.cpp -o ${BIN_DIR}/${NAME}

run: build
	sudo -u me mpirun -np ${proc_count} ${BIN_DIR}/${NAME}

build_example:
	mkdir -p ${BIN_DIR}
	mpic++ -std=c++17 bcast_example.cpp -o ${BIN_DIR}/${NAME}

run_example: build_example
	sudo -u me mpirun -np ${proc_count} ${BIN_DIR}/${NAME}

clean:
	rm -rf ${BIN_DIR}
