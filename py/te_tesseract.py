

from PIL import Image
import pytesseract
text = pytesseract.image_to_string(Image.open('D:/code/data/id/6ac0b84bf27ef484af37ede7937454cc.jpg'), lang='chi_sim')
print(text)
