

from PIL import Image
import pytesseract
text = pytesseract.image_to_string(Image.open('D:/data/id/15f8806a239276ffe1e389571d12bc69.jpg'), lang='chi_sim')
print(text)
