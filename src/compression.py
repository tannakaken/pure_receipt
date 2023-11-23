"""
レシート小説をそのまま、ヘッダーファイルにすると、DRAMが溢れてしまう。
そこで、ヘッダーファイルを圧縮する。

uint8_t novel2[12500] PROGMEM = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    ...
};

という形のデータを

Data novel2[3941] PROGMEM = {
    {0b00000000, 255},
    ...
};

というデータに書き換える。
"""
import sys

indata = False
current_num = 0
current_data = None
data_num = 0
lines = []
with open(sys.argv[1]) as f:
    for line in f:
        if line.startswith("uint8_t"):
            lines.append('#include "data.h"\n')
            lines.append(line.replace("uint8_t", "Data"))
            indata = True
        elif line.startswith("};"):
            indata = False
            lines.append(line)
        elif indata:
            data = line.strip()
            if current_num == 0:
                current_num = 1
                current_data = data
            elif current_data == data and current_num < 255:
                current_num += 1
            else:
                lines.append("    {" + current_data + str(current_num) +
                             "},\n")
                data_num += 1
                current_num = 1
                current_data = data
        else:
            lines.append(line)

for line in lines:
    print(line.replace(sys.argv[2], str(data_num)), end="")
