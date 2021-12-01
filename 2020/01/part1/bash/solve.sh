#!/bin/bash

get_nums() {
  cat ../../input \
    | sort -n
}

for num in $(get_nums)
do
  oth=$(( 2020 - num ))
  if [ -n "$(get_nums | grep "^${oth}$")" ]
  then
    echo "$((num * oth))"
    exit 0
  fi
done
