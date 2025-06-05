import sys

def convert_bin_to_header(bin_path, header_path):
    with open(bin_path, "rb") as f:
        data = f.read()

    with open(header_path, "w") as out:
        for i, byte in enumerate(data):
            if i % 12 == 0:
                out.write("    ")
            out.write(f"0x{byte:02X}, ")
            if i % 12 == 11:
                out.write("\n")
        if len(data) % 12 != 0:
            out.write("\n")


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python3 bin2header.py firmware.bin firmware.h")
        sys.exit(1)

    convert_bin_to_header(sys.argv[1], sys.argv[2])
