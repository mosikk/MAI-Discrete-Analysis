import random
import string

items_amount = random.randint(10, 1000000)
file = open("test.txt", "w")
for _ in range(items_amount):
    for __ in range(6):
        file.write(str(random.randint(0, 9)))
    file.write(' ')
    for __ in range(random.randint(1, 64)):
        file.write(random.choice(string.ascii_lowercase))
    file.write('\n')
file.close
