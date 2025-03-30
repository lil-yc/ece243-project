from PIL import Image

def convert_to_565(r, g, b):
    # Convert RGB to 5-6-5 format
    red = (r >> 3) & 0x1F
    green = (g >> 2) & 0x3F
    blue = (b >> 3) & 0x1F
    return (red << 11) | (green << 5) | blue

def png_to_array(image_path, output_path):
    img = Image.open(image_path).convert('RGB')
    width, height = img.size

    with open(output_path, 'w') as f:
        f.write(f"short int image[{height}][{width}] = {{\n")
        
        for y in range(height):
            f.write("    {")
            for x in range(width):
                r, g, b = img.getpixel((x, y))
                color_565 = convert_to_565(r, g, b)
                f.write(f"0x{color_565:04X}")
                if x < width - 1:
                    f.write(", ")
            f.write("},\n")
        
        f.write("};\n")

    print(f"Conversion completed. C array saved to {output_path}")

# Example usage
input_path  = "start_screen.png"
output_path = "start_screen.c"
png_to_array(input_path, output_path)
