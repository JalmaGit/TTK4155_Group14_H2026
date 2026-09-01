{
  pkgs,
  lib,
  config,
  ...
}:
{
  # devenv.sh/languages/
  languages = {
    c = {
      enable = true;
      debugger = pkgs.gdb;
    };
  };

  packages = [
    # nRF52840: ARM Cortex-M cross compiler and Nordic tooling.
    #pkgs.gcc-arm-embedded
    #pkgs.nrf-command-line-tools
    #pkgs.nrfutil
    #pkgs.nrf5-sdk

    # AT90USB1287 and ATmega162: AVR programming, simulation, and runtime support.
     pkgs.avrdude
     pkgs.pkgsCross.avr.buildPackages.gcc
     pkgs.pkgsCross.avr.buildPackages.binutils
    # pkgs.avrlibc
    # pkgs.avra
    # pkgs.simavr

    # Hardware debugging support.
    pkgs.openocd
  ];

  # See full reference at https://devenv.sh/reference/options/
}

