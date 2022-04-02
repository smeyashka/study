#!/bin/bash
#исп-е $IFS

output_args_one_per_lin()
{
for arg
do echo "[$arg]"
done
}

echo; echo "IFS=\" \""
echo "-------"

IFS=" "
var=" a  b c   "
output_args_one_per_lin $var

echo; echo "IFS=:"
echo "-------"

IFS=:
var=":a::b:c:::"
output_args_one_per_lin $var
echo
exit 0