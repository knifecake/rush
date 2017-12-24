import xml.etree.ElementTree as et
from subprocess import call

SPRITESHEET = 'pixeled_regular_5.PNG'
SLICESHEET = 'pixeled_regular_5.xml'

e = et.parse(SLICESHEET).getroot()

for char in e.findall('Char'):
    rect = char.get('rect').split(' ')
    call(['convert', '-extract', ''.join([rect[2], 'x', rect[3], '+', rect[0], '+', rect[1]]), SPRITESHEET, str(ord(char.get('code'))) + '.png'])
