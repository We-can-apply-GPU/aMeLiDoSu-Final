#!/usr/bin/env sh

if [ $# -ne 4 ]; then
   echo "Usage: $0 input_phone_seq_file"
   echo "ex. $0 example"
   exit -1;
fi

input=$1
from=$2
output=$4
to=$3

tmp=temp/line_tmp
[ -f $input ] || exit -1;

cat $input | while read line
do
    echo $line > $tmp
    ./run.sh $tmp $2 $3 >> $4
done
