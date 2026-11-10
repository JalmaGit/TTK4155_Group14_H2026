# TTK4155 Group 14 - 2026

TTK4155 Labassignment

## Deployment

Deploy to lab computer with `./scripts/deploy.sh`.
Remember to `chmod` and set the correct destination (default `sanntid-1`).

```bat
Host sanntid-1
    HostName 10.100.23.18
    User student
    IdentityFile ~/.ssh/id_ed25519
    IdentitiesOnly yes
```

## Development

Activate development environment with `devenv shell`.
Requires `nix` and `devenv`.

Generate compile commands for the LSP with `make compiledb`.

Remember to `make fmt` before committing!

## Logging

```bash
sudo picocom --baud 9600 --databits 8 --stopbits 2 /dev/ttyS0
```

## Flashing

Node1: go to ´../node1/´ and run ´make flash´
Node2: go to ´../node2/´and run ´make flash´
