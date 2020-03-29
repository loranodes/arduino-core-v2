import json
import os
import sys

with open('package_loranodes_v2_index.json', 'r') as file:
    data = json.load(file)

toInsert = {}

if(len(sys.argv)<3):
	print("Not enough arguments. Usage: index.py <version> <shasum>")
	exit()

# user input version number
toInsert['version'] = sys.argv[1]

toInsert['boards'] = []
toInsert['boards'].append({'name': 'microNodeV2'})

toInsert['archiveFileName'] = 'microNodeV2-' + toInsert['version'] + '.tar.bz2'

toInsert['checksum'] = 'SHA-256:' + sys.argv[2]

toInsert['url'] = 'https://github.com/loranodes/arduino-core-v2/raw/' \
    + toInsert['version'] + '/' + toInsert['archiveFileName']

cmd = toInsert['archiveFileName']
toInsert['size'] = os.path.getsize(cmd)


toInsert['architecture'] = 'samd'
toInsert['name'] = 'LoRaNodes M0 V2 Boards'
toInsert['category'] = 'LoRaNodes'

toInsert['help'] = {'online': 'https://github.com/loranodes'}

toInsert['toolsDependencies'] = []

tool1 = {"name": "arm-none-eabi-gcc", "packager": "arduino", "version": "4.8.3-2014q1"}
tool2 = {"name": "bossac", "packager": "arduino", "version": "1.7.0"}
tool3 = {"name": "openocd", "packager": "arduino", "version": "0.9.0-arduino6-static"}
tool4 = {"name": "CMSIS", "packager": "arduino", "version": "4.5.0"}
tool5 = {"name": "CMSIS-Atmel", "packager": "arduino", "version": "1.1.0"}
tool6 = {"name": "arduinoOTA", "packager": "arduino", "version": "1.2.0"}


toInsert['toolsDependencies'].append(tool1)
toInsert['toolsDependencies'].append(tool2)
toInsert['toolsDependencies'].append(tool3)
toInsert['toolsDependencies'].append(tool4)
toInsert['toolsDependencies'].append(tool5)
toInsert['toolsDependencies'].append(tool6)


data['packages'][0]['platforms'].append(toInsert)

with open('package_loranodes_v2_index.json', 'w') as file:
    json.dump(data, file, indent="\t")

print('\n\nNow commit this and tag the release with the tag "' + toInsert['version'] + '"')
