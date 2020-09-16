## Dependencies

Go 1.14, Docker 19

## Usage

```ShellSession
make build

# Check config.yaml file first
make image
docker run -v {Path to data dir}:/usr/app/data xml-obfuscator:0.0.1
```