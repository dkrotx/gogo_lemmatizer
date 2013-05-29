#!/bin/sh

#substitute library mode in lemmatizer

for e in rsh rst dsh dst; do
    out=lemmatizer-$e.pc.in
    echo Creating $out
    sed -e "s/%CT%/$e/g" lemmatizer-pc.in.template >$out
done

cp lemmatizer-rsh.pc.in lemmatizer.pc.in
