# This script generates an OpenOCD configuration file based on the selected interface and target device.
# example usage: python3 tools/openocd_cfg_gen.py --interface STLINK --target STM32F4 --build-dir build
#TODO
from pathlib import Path
import argparse

# Define supported interfaces and targets
INTERFACES = {
    "STLINK": "interface/stlink.cfg",
    "JLINK": "interface/jlink.cfg",
    "CMSIS-DAP": "interface/cmsis-dap.cfg",
}

TARGETS = {
    "STM32F1": "target/stm32f1x.cfg",
    "STM32F4": "target/stm32f4x.cfg",
    "NRF52": "target/nrf52.cfg",
    "EFM32": "target/efm32.cfg",
    "RP2040": "target/rp2040.cfg",
}

CHIPNAMES = {
    "STM32F1": "STM32F103xx",
    "STM32F4": "STM32F407xx",
    "NRF52": "nRF52832_xxAA",
    "EFM32": "EFM32GG",
    "RP2040": "RP2040",
}

def main():
    parser = argparse.ArgumentParser(description="OpenOCD config file generator")
    parser.add_argument("--interface", required=True, choices=INTERFACES.keys(), help="Debug probe interface")
    parser.add_argument("--target", required=True, choices=TARGETS.keys(), help="Target device")
    parser.add_argument("--build-dir", required=True, help="Directory to output the config file")

    args = parser.parse_args()

    build_dir = Path(args.build_dir)
    cfg_path = build_dir / "openocd.cfg"

    build_dir.mkdir(parents=True, exist_ok=True)

    chipname = CHIPNAMES.get(args.target, "unknown")

    content = f"""\
    source [find {INTERFACES[args.interface]}]
    set CHIPNAME {chipname}
    transport select hla_swd
    source [find {TARGETS[args.target]}]
    reset_config srst_only srst_nogate
    """

    cfg_path.write_text(content)

    print(f"OpenOCD config file generated at: {cfg_path}")

if __name__ == "__main__":
    main()
