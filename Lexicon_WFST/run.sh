#!/bin/bash

if [ $# -ne 3 ]; then
   echo "Usage: $0 input_phone_seq_file"
   echo "ex. $0 example"
   exit -1;
fi

input=$1
from=$2
to=$3

[ -f $input ] || exit -1;

# check neccessary files
files="phones.60-48-39.map timitdic.txt timit_norm_trans.pl make_lexicon_fst.pl add_disambig.pl fstaddselfloops"
for f in $files
do
  [ -f $f ] || exit -1;
done

if [ $to -eq 60 ]; then
  cut -f1 phones.60-48-39.map | sed -r 's/h#/sil/g'  | sort | uniq > phone_list
fi
if [ $to -eq 48 ]; then
  cut -f2 phones.60-48-39.map | grep -v "q" | sort | uniq > phone_list
fi
if [ $to -eq 39 ]; then
  cut -f3 phones.60-48-39.map | grep -v "q" | sort | uniq > phone_list
fi

echo "<eps> 0" > phones_num
awk '{ print $0 " " FNR }' phone_list >> phones_num

grep -v -E "^;" timitdic.txt | sed -e 's/\///g' -e 's/[0-9]//g' > lexicon.txt
echo "<s> sil" >> lexicon.txt
echo "</s> sil" >> lexicon.txt
echo "SIL sil" >> lexicon.txt

if [ ! $to -eq 60 ]; then
  ./timit_norm_trans.pl -i lexicon.txt -m phones.60-48-39.map -from 60 -to $to > tmp 
  cp tmp lexicon.txt
  rm tmp
fi

cut -f1 -d ' ' lexicon.txt | \
  cat - <(echo "#0") | \
  awk '{ print $0 " " FNR }' | \
  cat <(echo "<eps> 0") - > words.txt

# add disambig
ndisambig=`./add_lex_disambig.pl lexicon.txt lexicon_disambig.txt` 
ndisambig=$[$ndisambig+1];

./add_disambig.pl --include-zero phones_num $ndisambig > phones_disambig.txt 

phone_disambig_symbol=`grep \#0 phones_disambig.txt | awk '{print $2}'`
word_disambig_symbol=`grep \#0 words.txt | awk '{print $2}'`
   
#generate Lexicon.fst
if [ ! -f Lexicon.${to}.fst ]; then
  ./make_lexicon_fst.pl lexicon.txt 0.5 "sil" \
    | fstcompile --isymbols=phones_disambig.txt --osymbols=words.txt --keep_isymbols=false --keep_osymbols=false \
    | ./fstaddselfloops "echo $phone_disambig_symbol |" "echo $word_disambig_symbol |"  \
    | fstarcsort --sort_type=olabel > Lexicon.${to}.fst
fi

# generate input.fst
if [ $from -eq $to ]; then
  cp $input input
else
  ./timit_norm_trans.pl -i $input -m phones.60-48-39.map -from 60 -to $to > input
fi

if [ $to -eq 60 ]; then
  cat input | sed -r 's/h#/sil/g' > tmp 
  mv tmp input
fi

rm -f input.log
j=0
for phone in $(cat input)
do
  echo "$j $((j+1)) $phone $phone 0" >> input.log
  # deletion
  echo "$j $((j+1)) $phone <eps> 100" >> input.log
  # substitution
  for tmp in $(cat phone_list | grep -w -v $phone)
  do
    echo "$j $((j+1)) $phone $tmp 100" >> input.log
  done
  j=$((j+1))
done
echo "$j 0" >> input.log
rm input

fstcompile --isymbols=phones_disambig.txt --osymbols=phones_disambig.txt input.log | \
  fstarcsort --sort_type=olabel > input.fst

# compose Lexicon.fst and input.fst and output the shortest path
fstcompose input.fst Lexicon.${to}.fst | \
  fstshortestpath --nshortest=1 --unique=true | \
  fstprint --isymbols=phones_disambig.txt --osymbols=words.txt | \
  cut -f4 | grep -v "<eps>" | grep -v "0" | tac | tr '\n' ' '
echo
rm input.fst input.log

# use the following command to draw the fst.
# fstdraw --isymbols=phones_disambig.txt --osymbols=phones_disambig.txt -portrait input.fst | \
#   dot -Tsvg >ex.svg
exit 0

