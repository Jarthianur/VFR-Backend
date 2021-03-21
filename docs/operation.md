# Operation - How To Run

## Manually

```bash
{path to binary} -c {path to config file} -o {path to log file} &
```

### Example

```bash
./vfrb -c vfrb.conf -o vfrb.log &
```

The log will be in the specified file.

## As Service

By invoking `./run.sh install`, the systemd service for VFR-B was automatically configured to start after boot.

### Watch the log with

```bash
journalctl -u {servicename}.service
```

## Commandline arguments

VFRB supports a few commandline arguments to control its behavior.

| Argument           | Explanation                                        |
| ------------------ | -------------------------------------------------- |
| -h / --help        | Show an overview of all arguments and their usage. |
| -c / --config      | Set the configuration file to use.                 |
| -v / --verbose     | Enable debug logging                               |
| -g / --ground-mode | Forcibly enable ground mode.                       |
| -o / --output      | Set a file where to pu all the logs.               |

## Run as docker container

Assuming the docker image was built correctly as described [here](installation.md),
you can run the VFRB inside a minimal container without messing up your host with dependencies or whatever.
Also you can run the VFRB on any platform you like, wherever docker is available.
To run a docker container execute like `docker run --name VFRB -p <PORT>:4353 -dit user/vfrb:latest [OPTIONS]`.
Where _PORT_ can be any port you like and _OPTIONS_ can be any VFRB commandline argument, except for _-c_.
