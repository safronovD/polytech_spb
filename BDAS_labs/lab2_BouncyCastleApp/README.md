## Dependencies

Java 14, Docker 19

## Usage

```ShellSession
# Build Jar
gradlew build
gradlew shadowJar

# Run Docker container
docker build . -t bc-app:0.1
docker run -v {Path to data dir}:/usr/app/data bc-app:0.1
```