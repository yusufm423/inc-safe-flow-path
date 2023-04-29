with open('1000.100.10.truth', 'r') as f:
    file_content = f.readlines()

graph_number = 0
for line in file_content:
    if line.startswith('#'):
        graph_number += 1
        graph_name = line.split(' ')[4].strip()
        new_file_name = f'{graph_name}.truth'
        with open(new_file_name, 'w') as new_file:
            continue
    else:
        with open(new_file_name, 'a') as new_file:
            new_file.write(line)
