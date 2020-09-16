FROM golang:latest

ENV ROOT=/usr/app

WORKDIR ${ROOT}

COPY /bin ${ROOT}
ADD config.yaml ${ROOT}

RUN mkdir -p data

CMD ["./xml-obfuscator"]