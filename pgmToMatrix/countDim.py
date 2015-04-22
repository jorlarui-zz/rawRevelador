import sys

assert(len(sys.argv) == 2)
f = open(sys.argv[1], 'r')

i = 0
while (f.readline() != ""):
  i = i + 1

height = i

f.seek(0,0)
sampleLine = f.readline()

sampleLine = sampleLine.split()
width = len(sampleLine)

print "width:", width, "height:", height
