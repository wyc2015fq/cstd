
from captcha.audio import AudioCaptcha
from captcha.image import ImageCaptcha

outdir = 'C:/local/captcha/'

audio = AudioCaptcha(voicedir=outdir + 'voices')
image = ImageCaptcha(fonts=[outdir+'Ubuntu-M.ttf', outdir+'platech.ttf'])

#data = audio.generate('1234')
#audio.write('1234', 'out.wav')

s = '1234'
data = image.generate(s)
image.write(s, outdir+s+'.png')
