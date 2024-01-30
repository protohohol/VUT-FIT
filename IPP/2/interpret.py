import re
import argparse
import xml.etree.ElementTree as ET

class arg:
    def __init__(self, arg_type, value):
        self._type = arg.type
        self._value = value

class instruction:
    def __init__(self, name, number):
        self._name = name
        self._number = number
        self._args = []
    def add_argument(self, arg_type, value):
        self._args.append(arg(arg_type, value))

# argparse
ap = argparse.ArgumentParser()
ap.add_argument("--source", nargs=1, help="wrong source")
ap.add_argument("--input", nargs=1, help="wrong input")

args = vars(ap.parse_args())

#xml_load
tree = ET.parse(source)

root = tree.getroot()

#xml_check
if root.tag != "program":
    error 
for child in root:
    if child != "instruction":
        error 
    ca = list(child.attrib.keys)
    if not('order' in ca) or not ('opcode' in ca):
        error 
    for subelem in child:
        if not(re.match(r"arg[123]", subelem.tag)):
            error 

#xml_to_instr
for e in root:
    make instruction
    for sub in e:
        instruction.add_argument(arg_type, value)

for i in instructions:
    interpet(i)