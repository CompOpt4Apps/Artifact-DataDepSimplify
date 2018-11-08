#
# View Fractional Dot File
#
# Pass in the name of the incomplete dot file
# as the sole command line argument.
#
# Mark Heim
# April 19, 2012
#
echo "digraph structure {" > temp.dot
cat $1 >> temp.dot
echo "}" >> temp.dot
dotty temp.dot
rm -fr temp.dot
