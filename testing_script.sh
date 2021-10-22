#!/bin/bash

#letters array
declare -a letters_arr=("тетачасчастьчетачечет"
            "уходолгнефтьдымрубль"
            "выпускднопочватемпвода"
            "судстолканалполетштат"
            "мигзолотоначалостихлист"
            "данныелесгодцифраозеро"
            "имяродительврагшеяформула"
            #fat tests
            "экономикатеатрновоегорломинистр"
            "оборотмэрмоментдухсобака"
            #fattest test
            "методсонрисинженеркампания")
            #program breaking test
            #"авгурвыжиглипомаотрывшрот")
words="1000"
lang="cpp"

while getopts 'smlp' flag; do
  case "${flag}" in
    s) words="1000" ;;
    m) words="34010" ;;
    l) words="51301" ;;
    p) lang="python" ;;
    *) exit 1
  esac
done

for letters in "${letters_arr[@]}"
do
    echo " "
    echo "letters:"
    echo "$letters"
    if [[ "$lang" == "python" ]]; then
        echo "$letters" | python scripts/abc_quiz.py "$words"
    else
        echo "$letters" | ./quiz_hack "$words"
    fi
    echo " "
done
