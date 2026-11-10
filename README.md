# TTK4155 Group 14 - 2026

TTK4155 Labassignment

Remember to `chmod` and set the correct destination (default `sanntid-1`).

## Development

Activate development environment with `devenv shell`.
Requires `nix` and `devenv`.

Generate compile commands for the LSP with `make compiledb`.

Remember to `make fmt` before committing!

## Logging

### Configure SSH

In `~/.ssh/config`,
set `sanntid-1` to the lab computer/place,
and the correct ip.

```bat
Host sanntid-1
HostName 10.100.23.18
User student
IdentityFile ~/.ssh/id_ed25519
IdentitiesOnly yes
```

### Logging Node 1

```bash
ssh sanntid-1
sudo picocom --baud 9600 --databits 8 --stopbits 2 /dev/ttyS0
```

### Logging Node 2

```bash
ssh sanntid-1
sudo picocom --baud 9600 --databits 8 --parity n --stopbits 1 --flow n /dev/ttyACM0
```

## Flashing

### Node1

```bash
devenv shell

cd node1
make clean && make flash
```

### Node2

```bash
devenv shell

cd node2
make clean && make flash
```
