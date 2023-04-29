import random

with open('1000.100.10.truth', 'r') as f:
    file_content = f.readlines()

graph_number = 0
flag = 0
for line in file_content:
    if line.startswith('#'):
        graph_number += 1
        flag = 0
        graph_name = line.split(' ')[4].strip()
        new_file_name = f'{graph_name}.txt'
        with open(new_file_name, 'w') as new_file:
            continue
    elif flag == 0:
        with open(new_file_name, 'a') as new_file:
            flow = str( random.randint(1, 500) + (int)( line.split(' ')[0].strip()))
            line_list = line.split()
            line_list.pop(0)  # remove the first element
            new_line = " ".join(line_list)  # join the remaining elements
            new_file.write(new_line+ '\n' + flow)
            flag = 1
