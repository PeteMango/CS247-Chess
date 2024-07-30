import subprocess
from Xlib import X, display
from Xlib.ext import record
from Xlib.protocol import rq

# Unicode characters for chess pieces
chess_chars = [0x2654, 0x2655, 0x2656, 0x2657, 0x2658, 0x2659, 0x265A, 0x265B, 0x265C, 0x265D, 0x265E, 0x265F]
chess_string = ''.join(chr(c) for c in chess_chars)

def get_fonts():
    result = subprocess.run(['xlsfonts'], stdout=subprocess.PIPE)
    return result.stdout.decode('utf-8').splitlines()

def check_font_support(font_name):
    disp = display.Display()
    root = disp.screen().root

    # Create a simple window
    window = root.create_window(0, 0, 1, 1, 0, disp.screen().root_depth)
    gc = window.create_gc()

    # Load the font
    try:
        font = disp.open_font(font_name)
    except:
        return False

    # Set the font and draw text
    gc.font = font
    window.draw_text(gc, 0, 10, chess_string)

    # Check if the font can render the chess characters
    try:
        chars = disp.list_fonts_with_info(chess_string)
        if any(c == 0 for c in chars):
            return False
    except:
        return False
    finally:
        font.close()
        window.destroy()
        disp.close()

    return True

if __name__ == "__main__":
    fonts = get_fonts()
    supported_fonts = []

    for font in fonts:
        if check_font_support(font):
            supported_fonts.append(font)

    if supported_fonts:
        print("Fonts that support chess Unicode characters:")
        for font in supported_fonts:
            print(font)
    else:
        print("No fonts found that support chess Unicode characters.")
