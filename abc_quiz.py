
letters = 'а а а в д е е е ж и к к к л л м м о о о п р р р с с т т т ч ш я я'

def check_words(words_to_check, chars):
    words_letters = []

    for word in words_to_check:
        for char in word:
            words_letters.append(char)

    #check len
    if len(chars) != len(words_letters):
        return False

    words_letters.sort()

    for i in range(0, len(chars)):
        if chars[i] != words_letters[i]:
            return False

    return True


def do_magic(words, chars):
    prog = 0
    for i0 in range(0, len(words)-4):
        for i1 in range(i0+1, len(words)-3):
            for i2 in range(i1+1, len(words)-2):
                for i3 in range(i2+1, len(words)-1):
                    for i4 in range(i3+1, len(words)):
                        words_to_check = [words[i0], words[i1], words[i2], words[i3], words[i4]]
                        if check_words(words_to_check, chars):
                            return words_to_check, True
    return [], False


def filter_words(all_words, chars):
    words = []
    for word in all_words:
        add = True
        for c in chars:
            if not c in word:
                add = False
                break
        if add:
            words.append(word)

    return words

if __name__ == '__main__':
    try:
        chars = letters.split(' ')
        chars.sort()

        all_words = open('word_rus.txt').readlines()

        words = filter_words(all_words, chars)

        solution, ok = do_magic(all_words, chars)
        if ok:
            print(solution)
        else:
            print("solution not found!")

    except Exception as e:
        print(e)
