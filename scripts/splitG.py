# open the input file for reading
with open("1000.100.10.graph", "r") as f:
    content = f.read()

# split the content by graph number
graphs = content.split("# graph number = ")

# iterate over the graphs and create a new file for each one
for i, graph in enumerate(graphs):
    if i == 0:
        continue  # skip the first element, which is empty

    # split the graph by newline and remove any empty lines
    lines = list(filter(lambda x: x.strip(), graph.split("\n")))

    # get the graph name and remove any whitespace
    name = lines.pop(0).strip()

    # create a new file for this graph and write the content
    with open(f"{i-1}.graph", "w") as f:
        f.write("\n".join(lines))
