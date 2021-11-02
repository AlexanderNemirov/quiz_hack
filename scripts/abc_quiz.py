import datetime
import sys

#chars list to dictionary
def chars_to_dict(cs):
    c_dict = {}
    for c in cs:
        if c not in c_dict:
            c_dict[c] = 0
        c_dict[c] += 1
    return c_dict

#check 5 words
def check_words(words_to_check, chars_dict):
    letters_dict = {}
    for word in words_to_check:
        for char in word:
            if char not in letters_dict:
               letters_dict[char] = 0
            letters_dict[char] += 1

    return True if letters_dict == chars_dict else False

#check every 5 words from filtered
def do_magic(words, chars):
    chars_dict = chars_to_dict(chars)
 
    solution = []
    solved = False

    for i0 in range(0, len(words)-4):
        for i1 in range(i0+1, len(words)-3):
            for i2 in range(i1+1, len(words)-2):
                for i3 in range(i2+1, len(words)-1):
                    for i4 in range(i3+1, len(words)):
                        words_to_check = [words[i0], words[i1], words[i2], words[i3], words[i4]]
                        if check_words(words_to_check, chars_dict):
                            solution.append(words_to_check)
                            solved = True
    return solution, solved 

#filter words whether they contain required letters or not
def filter_words(all_words, chars):
    words = []
    chars_set = set(chars)

    for word in all_words:
        add = bool(word)
        for c in word:
            if c not in chars_set:
                add = False
                break
        if add:
            words.append(word)

    return words

if __name__ == '__main__':
    try:
        words_count = '1000'
        if len(sys.argv) > 1:
             words_count = sys.argv[1]

        print('enter required letters:')
        letters_str = sys.stdin.readline()
        chars = list(letters_str) 
        del chars[-1]
        start = datetime.datetime.now() 

        all_words = open('data/word_rus' + words_count + '.txt','r').read().split('\n')

        words = filter_words(all_words, chars)

        print('number of filtered words')
        print(len(words))

        solutions, ok = do_magic(words, chars)
        if ok:
            print('solution')
            for solution in solutions:
                print(solution)
        else:
            print("solution not found!")

        end = datetime.datetime.now() 
        time_exec = (end - start).microseconds / 1000
        print("program time execution: ",time_exec," ms") 
    except Exception as e:
        print(e)
