## get the rostopics and save them in a file
rostopic list > rostopics.txt

## get the rosmsg types and save them in a file

# remove the file if it already exists
if [ -f rosmsg_types.txt ]; then
    rm rosmsg_types.txt
fi


echo -e "Summary of rosmsg types:\n" >> rosmsg_types.txt

rostopic list -v >> rosmsg_types.txt
echo "=================================================" >> rosmsg_types.txt
echo -e "Detailed info of rosmsg types:\n" >> rosmsg_types.txt


# read the rostopics.txt file and get the rosmsg types
while read line
do  
    # UI
    echo "Appending $line info..."
    # info
    echo  "ROS topic - $line" >> rosmsg_types.txt
    rostopic info $line >> rosmsg_types.txt

    # run rostopic echo for a single message and exit
    # rostopic echo -n 1 $line > rosmsg_types.txt
    
    # delimiter
    echo -e "====================\n" >> rosmsg_types.txt

done < rostopics.txt