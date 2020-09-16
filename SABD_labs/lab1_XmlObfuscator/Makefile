NAME    := xml-obfuscator
VERSION := 0.0.1

BIN_DIR     := ./bin

.PHONY: build

build:
	GOOS=linux go build -o ${BIN_DIR}/${NAME} ./*.go

image:
	sudo docker build . -t ${NAME}:${VERSION}

lint:
	golangci-lint run ./...

test:
	go test -cover ./...  -coverprofile=coverage.out

coverage:
	go tool cover -html=coverage.out -o coverage.html

clean:
	go clean
	rm -f ${BIN_DIR}/${NAME}