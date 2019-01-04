def scan(string):
    KEY_WORDS = ["Const", "Var", "if", "else", "then","while", "do", "begin", "end"]
    SYMBOLS = [',', '+', '-', '*', '/', ';', '(' ,')' ,'{' ,'}', '==', '<=', '>=', '<>', '<', '>', '=']
    word_map = {key: i+1 for i, key in enumerate(KEY_WORDS)}
    word_map.update({key: i+21 for i, key in enumerate(SYMBOLS)})
    def process(string):
        word = ''
        if ' ' in string:
            for s in string.split(' '):
                word += s
        else:
            word = string
        if '//' in word:
            return word.split('//')[0]
        return word

    def have_keywords(word):
        for keyword in KEY_WORDS:
            if keyword in word:
                return keyword
        return None

    def have_symbol(word):
        for sy in SYMBOLS:
            if sy in word:
                return sy
        return None

    def is_alpha(ch):
        return (ch >= 'a' and ch <= 'z') or (ch >= 'A' and ch <= 'Z')

    def is_digit(ch):
        return ch >= '0' and ch <= '9'

    # def is_in_symbols(c):
    #     return c in SYMBOLS

    # def is_keywords(c):
    #     return c in KEY_WORDS


    queue = [process(string)]
    if '/*' in queue[0] or '*/' in queue[0]:
        return 
    stop = False
    while not stop:
        changed = False
        tempQueue = []
        for item in queue:
            key = have_keywords(item)
            sy = have_symbol(item)
            if key and key != item:
                changed = True
                item1, item2 = item.split(key)
                if item1:
                    tempQueue.append(item1)
                tempQueue.append(key)
                if item2:
                    tempQueue.append(item2)
                continue
            elif sy and sy != item:
                changed = True
                sp = item.split(sy)
                for i, it in enumerate(sp):
                    if it:
                        tempQueue.append(it)
                    if i != len(sp) - 1:
                        tempQueue.append(sy)
                
            else:
                tempQueue.append(item)

            if item == queue[-1] and not changed:
                stop = True
        queue = tempQueue.copy()
    for item in queue:
        if item in word_map:
            print("<", item, ",", word_map[item],">")
        else:
            if item and is_alpha(item[0]):
                print("<", item, ",", 15,">")
            if item and is_digit(item[0]):
                print("<", item, ",", 16,">")

def main():
    with open('./test.cpp', 'r') as r_stream:
        for line in r_stream:
            if line:
                if '\n' in line:
                    scan(line[0:-1])
                else:
                    scan(line)

main()