FROM openjdk:14-jdk-alpine

ENV ROOT=/usr/app
WORKDIR ${ROOT}

COPY build/libs/BCapp-1.0-SNAPSHOT-all.jar ${ROOT}/app.jar
RUN mkdir -p data

ENTRYPOINT ["java","-jar","app.jar"]
