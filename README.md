# Hypertext Transfer Protocol Daemon

This program is designed to be an extremely simple and minimalist web server that has very few dependencies.

## Configuration
This program requires that a configuration file is specified upon execution using the ```-c``` flag. You may look at ```conf/example.conf``` for an example of what the configuration file should look like.

## Build
This project uses the ```cmake``` build system.

```bash
mkdir build
cd build
cmake ..
make
```
