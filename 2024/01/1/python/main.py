with open('input.txt', encoding="utf-8") as f:
    read_data = f.read()
    lines = str.split(read_data, sep="\n")
    values_a = []
    values_b = []
    for line in lines:
        if line == "":
            break
        vals = str.split(line)
        values_a.append(int(vals[0]))
        values_b.append(int(vals[1]))
    values_a.sort()
    values_b.sort()
    assert len(values_a) == len(values_b)
    distances = []
    for i in range(0, len(values_a)):
        distance = values_a[i] - values_b[i]
        if distance < 0:
            distance = (-1) * distance
        distances.append(distance)
    print(sum(distances))
