# openmediavault-gpuutils

This project is based on and inspired by [routmoute's openmediavault-nvidiastats](https://github.com/routmoute/openmediavault-nvidiastats).

This OpenMediaVault plugin adds dashboard widgets and management interfaces for AMD(in the near future) and Nvidia GPU devices.

## Build

You can build and package this plugin as a `.deb` package with the following commands(ran from project root):

```bash
debuild -us -uc
cd debian
dpkg-deb --build openmediavault-gpuutils

# OR

bash ./build
```

## Installation

Install the `.deb` package using `apt`:

```bash
apt install ./openmediavault-gpuutils.deb
```

### Uninstall

```bash
apt remove openmediavault-gpuutils
```

## Roadmap

This is a list of features and changes I am planning to implement:

- [ ] Add a widget to display the GPU fan speed
- [ ] Add support for AMD GPUs
- [ ] Add charts to diagnostics section
- [ ] Add multi-GPU support
- [ ] Add panel to adjust GPU configuration(powerlimit, fanspeed, etc...)
