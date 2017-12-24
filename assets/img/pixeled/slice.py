import xml.etree.ElementTree as et
from subprocess import call

SPRITESHEET = 'pixeled_regular_10.PNG'
SLICESHEET = 'pixeled_regular_10-line.xml'

e = et.parse('pixeled_regular_10-line.xml').getroot()

for char in e.findall('Char'):
    rect = char.get('rect').split(' ')
    call(['convert', '-extract', ''.join([rect[2], 'x', rect[3], '+', rect[0], '+', rect[1]]), SPRITESHEET, str(ord(char.get('code'))) + '.png'])
