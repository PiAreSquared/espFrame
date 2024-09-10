import io
from PIL import Image, ImageFont, ImageDraw
import numpy as np
import os
import cairosvg

# Function to convert an image to hex
def image_to_hex(image):
    pixels = np.array(image)
    hex_data = []
    for row in pixels:
        binary_string = ''.join('1' if pixel else '0' for pixel in row)
        row_hex = []
        for i in range(0, len(binary_string), 8):
            byte = binary_string[i:i + 8].ljust(8, '0')
            row_hex.append(f"0x{int(byte, 2):02X}")
        hex_data.append(row_hex)
    return hex_data

FONT_CHARSET = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"

def generate_font_data(font_path, font_size, force_font_pt=False):
    # Generate bitmap and convert to hex for each character
    max_height = 0
    font_pt = font_size // 2 if not force_font_pt else font_size
    while max_height < font_size or not force_font_pt:
        font = ImageFont.truetype(font_path, font_pt)
        widths = []
        heights = []
        for char in FONT_CHARSET:
            image = Image.new("1", (2 * font_size, 2 * font_size), 0)  # Create a black image
            draw = ImageDraw.Draw(image)
            width, height = draw.textsize(char, font=font)
            widths.append(width)
            heights.append(height)
        max_width = max(widths)
        max_height = max(heights)
        font_pt += 1

    char_data = {}
    for char in FONT_CHARSET:
        image = Image.new("1", (max_width, max_height), 0)  # Create a black image
        draw = ImageDraw.Draw(image)
        draw.text((0, 0), char, font=font, fill=1, spacing=0)  # Draw the character in white
        char_data[char] = image_to_hex(image)

    # Write to a C++ file
    font_name = font_path.split("/")[-1].split(".")[0].replace(" ", "_")
    cpp_file_name = f"{font_name.lower()}{font_size}.cpp"
    with open(cpp_file_name, "w") as cpp_file:
        cpp_file.write("#include \"fonts.h\"\n\n")
        cpp_file.write(f"const uint8_t {font_name}{font_size}_Table[] = {{\n")
        for char, hex_data in char_data.items():
            cpp_file.write(f"    // @0 '{char}' ({font_size} pixels wide)\n")
            for hex_value_row in hex_data:
                cpp_file.write(f"    {', '.join(hex_value_row)}, // {''.join('#' if pixel == '1' else ' ' for pixel in ''.join(f'{int(h, 16):08b}' for h in hex_value_row))}\n")
            cpp_file.write("\n")
        cpp_file.write("};\n")

        cpp_file.write(f"\nconst uint8_t width_{font_name}{font_size}[] = {{\n")
        for width in widths:
            cpp_file.write(f"    {width},\n")
        cpp_file.write("};\n")

        cpp_file.write(f"\nfont_t {font_name}{font_size} = {{\n")
        cpp_file.write(f"    {font_name}{font_size}_Table, /* Character descriptor array */\n")
        cpp_file.write(f"    width_{font_name}{font_size}, /* Width array */\n")
        cpp_file.write(f"    {max_width}, /* Max Character width */\n")
        cpp_file.write(f"    {max_height}, /* Character height */\n")
        cpp_file.write("};\n")

    return cpp_file_name

def generate_svg_data(svg_path, pixel_size):
    # generate bitmap from svg file
    with open(svg_path, 'r') as f:
        svg_data = f.read()
    
    png_data = cairosvg.svg2png(bytestring=svg_data)
    image = Image.open(io.BytesIO(png_data))
    image = image.resize((pixel_size, pixel_size))
    image = image.convert("1")
    pixels = np.array(image)
    hex_data = []
    for row in pixels:
        binary_string = ''.join('1' if pixel else '0' for pixel in row)
        row_hex = []
        for i in range(0, len(binary_string), 8):
            byte = binary_string[i:i + 8].ljust(8, '0')
            row_hex.append(f"0x{int(byte, 2):02X}")
        hex_data.append(row_hex)

    cpp_file_name = f"{svg_path.split('/')[-1].split('.')[0]}{pixel_size}.cpp"
    with open(cpp_file_name, "w") as cpp_file:
        cpp_file.write("#include \"graphics.h\"\n\n")
        cpp_file.write(f"const uint8_t Graphic_{svg_path.split('/')[-1].split('.')[0]}{pixel_size}_Table[] = {{\n")
        for hex_value_row in hex_data:
            cpp_file.write(f"    {', '.join(hex_value_row)}, // {''.join('#' if pixel == '1' else ' ' for pixel in ''.join(f'{int(h, 16):08b}' for h in hex_value_row))}\n")
        cpp_file.write("};\n")
        
        cpp_file.write(f"graphic_t {svg_path.split('/')[-1].split('.')[0]}{pixel_size} = {{\n")
        cpp_file.write(f"    .data = Graphic_{svg_path.split('/')[-1].split('.')[0]}{pixel_size}_Table,\n")
        cpp_file.write(f"    .size = {pixel_size},\n")
        cpp_file.write("};\n")

    # print(f"Graphic data written to {cpp_file_name}")
    # print(f"Add the following to your graphics.h file:\n")
    print(f"extern graphic_t {svg_path.split('/')[-1].split('.')[0]}{pixel_size};")



def font_to_cpp(font_path, font_size, force_font_pt=False):
    cpp_file_name = generate_font_data(font_path, font_size, force_font_pt)
    print(f"Font data written to {cpp_file_name}")
    print(f"Add the following to your fonts.h file:\n")
    font_name = font_path.split("/")[-1].split(".")[0].replace(" ", "_")
    print(f"extern font_t {font_name}{font_size};")

def find_all_ttf_files():
    filenames = []
    for root, _, files in os.walk("."):
        for file in files:
            if file.endswith(".ttf"):
                filenames.append(os.path.abspath(os.path.join(root, file)))

    return filenames

def main_ttf():
    ttf_files = find_all_ttf_files()
    for i, ttf_file in enumerate(ttf_files):
        print(f"{i + 1}. {ttf_file.split('/')[-1]}")

    font_index = int(input("\nEnter the index of the font file you want to convert: ")) - 1
    font_path = ttf_files[font_index]
    if (input("Would you like to force the font size to be in points? (y/N): ").lower() == "y"):
        font_size = int(input("Enter the font size in points: "))
        font_to_cpp(font_path, font_size, force_font_pt=True)
    else:
        font_size = int(input("Enter the font size in pixel height: "))
        font_to_cpp(font_path, font_size)

def main_svg():
    svg_files = [os.path.abspath(f) for f in os.listdir(".") if f.endswith(".svg")]
    for i, svg_file in enumerate(svg_files):
        print(f"{i + 1}. {svg_file.split('/')[-1]}")

    svg_index = int(input("\nEnter the index of the SVG file you want to convert: ")) - 1
    svg_path = svg_files[svg_index]
    pixel_size = int(input("Enter the pixel size: "))
    generate_svg_data(svg_path, pixel_size)

def svg_main():
    svg_files = ["/home/piaresquared/Projects/Project_Glass/espFrame/fonts/sunrise_icon.svg",
                 "/home/piaresquared/Projects/Project_Glass/espFrame/fonts/sunset_icon.svg"]
    print(svg_files)
    for i, svg_file in enumerate(svg_files):
        generate_svg_data(svg_file, 25)


if __name__ == "__main__":
    # svg_main()
    main_ttf()