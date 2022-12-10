# Cooja Adapter (CoojaA)

An adapter facilitating more event driven applications to run on Cooja. Currently with libevent support. 

## Build Targets

Coojaa has two build targets:

1. `cooja`: This is the release target, which can be loaded by Cooja for simulation.

2. `native`: This is a target for debugging, which runs as a standalone binary on Linux.

## Codebases

The project is based on [contiki-ng (release-v4.8)](https://github.com/contiki-ng/contiki-ng/releases/tag/release%2Fv4.8) and [libevent (release-2.1.12-stable)](https://github.com/libevent/libevent/releases/tag/release-2.1.12-stable).