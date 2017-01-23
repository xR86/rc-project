#!/bin/sh
SHELL=/bin/bash

#recursion only for one level of depth
for a in $(ls); do
    if [ -d $a ]; then
        echo "processing folder $a";
        $(make -C $a clean);
    fi;
done;

# TODO: fix/refactor recursion for at least 2 levels of depth
# recursion for two levels of depth - broken 
# for a in $(ls); do
#     if [ -d $a ]; then
#         echo "processing folder $a";
#         $(make -C $a clean);
#         echo ">>$a "
#         echo "--nested folders in $a/"
#         for b in $(ls $a); do
#         	echo "--$a - $b >"
#         	#echo "$(ls $a)"
# 		    if [ -d $b ]; then
# 		        echo "processing folder $a/$b";
# 		        echo "$(ls $b)"
# 		        $(make -C $b clean);
# 		    fi;
# 		done;
#     fi;
# done;

echo "Done!"