is_correct = True
file = open("result.txt", "r")
previous = 0
line_num = 0
for line in file:
    line_num += 1
    line = line.split()
    if previous > int(line[0]):
        print("NOT SORTED")
        print(line_num)
        break
    previous = int(line[0])
else:
    print("Correct!")
file.close()
