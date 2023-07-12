# SETTINGS:
# ---------
filename=Makefile;
q="-q"; # delete -q for verbose mode
show_commands=false;
# ---------
decor=---------------------------------------------
echo echo "$decor" *CAT 1 -b
if [ "$show_commands" = true ]; then
echo  {cat -b "$filename"}  {./vas_cat -b "$filename"} -s $q
fi
diff <(cat -b "$filename") <(./vas_cat -b "$filename") -s $q
# -------------------------------------------------
echo echo "$decor" *CAT 2 -e
if [ "$show_commands" = true ]; then
echo  {cat -e "$filename"}  {./vas_cat -e "$filename"} -s $q
fi
diff <(cat -e "$filename") <(./vas_cat -e "$filename") -s $q
# -------------------------------------------------
echo echo "$decor" *CAT 3 -n
if [ "$show_commands" = true ]; then
echo  {cat -n "$filename"}  {./vas_cat -n "$filename"} -s $q
fi
diff <(cat -n "$filename") <(./vas_cat -n "$filename") -s $q
# -------------------------------------------------
echo echo "$decor" *CAT 4 flag after the filename
if [ "$show_commands" = true ]; then
echo  {cat "$filename" -n}  {./vas_cat "$filename" -n} -s $q
fi
diff <(cat "$filename" -n) <(./vas_cat "$filename" -n) -s $q
# -------------------------------------------------
echo echo "$decor" *CAT 5 -s
if [ "$show_commands" = true ]; then
echo  {cat -s "$filename"}  {./vas_cat -s "$filename"} -s $q
fi
diff <(cat -s "$filename") <(./vas_cat -s "$filename") -s $q
# -------------------------------------------------
echo echo "$decor" *CAT 6 -t
if [ "$show_commands" = true ]; then
echo  {cat -t "$filename"}  {./vas_cat -t "$filename"} -s $q
fi
diff <(cat -t "$filename") <(./vas_cat -t "$filename") -s $q
# -------------------------------------------------
echo echo "$decor" *CAT 7 Pure simple cat
if [ "$show_commands" = true ]; then
echo  {cat "$filename"}  {./vas_cat "$filename"} -s $q
fi
diff <(cat "$filename") <(./vas_cat "$filename") -s $q
# -------------------------------------------------
echo "$decor" *Tests ended*