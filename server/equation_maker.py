item = 0
file_name = "메가MGC커피 제주한라대점"
franchise_name = "메가MGC커피"

f = open(f"{file_name}.csv", "r", encoding="euc-kr")
total = {}
for line in f.readlines():
    line = line.split(",")

    if line[3] == "게스트하우스":
        line[3] = "유스호스텔"
    elif line[3] == "여관":
        line[3] = "숙박"
    elif line[3] == "교회" or line[3] == "사찰" or line[3] == "성당":
        line[3] = "종교시설"
    elif line[3] == "카라반" or line[3] == "산장" or line[3] == "오토캠핑장" or line[3] == "글램핑장":
        line[3] = "야영"
    elif line[3] == "":
        line[3] = "문화시설"

    if total.keys() == "":
        if line[3] != "아파트":
            total[line[3]] = 1

        else:
            if line[7] != "None":
                total[line[3]] += int(line[7])

    else:
        if line[3] in total.keys():
            if line[3] != "아파트":
                total[line[3]] += 1

            else:
                if line[7] != "None":
                    total[line[3]] += int(line[7])

        else:
            total[line[3]] = 1

    if franchise_name in line[1]:
        item += 1

print(total)
print(item)
