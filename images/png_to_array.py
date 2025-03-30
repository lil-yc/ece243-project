import os
from PIL import Image

def image_to_c_array(image_path, var_name):
    img = Image.open(image_path).convert("L")  # Convert to grayscale
    img = img.resize((320, 240))  # Ensure size is 320x240
    pixels = list(img.getdata())
    
    # Reshape to 2D array
    pixel_2d = [pixels[i * 320:(i + 1) * 320] for i in range(240)]
    
    # Format as C array
    c_array = f"int input_image[HEIGHT][WIDTH] = {{\n"
    for row in pixel_2d:
        c_array += "    { " + ", ".join(map(str, row)) + " },\n"
    c_array += "};\n\n"
    
    return c_array

folder_path = "320x240_digits"
output_file = "array_320x240.c"

with open(output_file, "w") as f:
    f.write("/* ############################################################ 320 x 240 ############################################################ */\n")
    f.write("#define WIDTH 320\n")
    f.write("#define HEIGHT 240\n\n")
    for i in range(10):
        image_path = os.path.join(folder_path, f"{i}.png")
        if os.path.exists(image_path):
            c_array_code = image_to_c_array(image_path, f"digit_{i}")
            f.write(c_array_code)

print(f"C arrays saved to {output_file}")
