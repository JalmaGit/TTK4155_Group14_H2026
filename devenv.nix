{
  pkgs,
  lib,
  config,
  ...
}: {
  # devenv.sh/languages/
  languages = {
    c = {
      enable = true;
      debugger = pkgs.gdb;
    };
  };

  packages = [
    pkgs.gcc-arm-embedded

    # AT90USB1287 and ATmega162: AVR programming, simulation, and runtime support.
    pkgs.avrdude
    pkgs.pkgsCross.avr.buildPackages.gcc
    pkgs.pkgsCross.avr.buildPackages.binutils

    # Hardware debugging support.
    pkgs.openocd

    # `bear -- make` to generate compile_commands.json
    pkgs.bear

    pkgs.picocom
  ];

  # Make clangd use avr-gcc
  scripts.clangd.exec = ''
    exec ${pkgs.clang-tools}/bin/clangd \
    --query-driver="/nix/store/*-avr-gcc-wrapper-*/bin/avr-gcc,/nix/store/*-avr-gcc-*/bin/avr-gcc" \
    "$@"
  '';

  # See full reference at https://devenv.sh/reference/options/
}
