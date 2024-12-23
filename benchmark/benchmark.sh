#!bin/bash
blue=$(tput setaf 4)
gray=$(tput setaf 7)
green=$(tput setaf 2)
teal=$(tput setaf 6)
normal=$(tput sgr0)

algorithms=("ABF") #"AZ" "AM" "ABF") #abf dodac pozniej bo sie wywala

par_file="par_file.txt"

benchmark() {
    tmpLogFile=$(mktemp)
    cat > $tmpLogFile <<EOF
Benchmarking Algorithm $algorithm with input $input_file $par_file
$normal============================[Input]============================$gray
$(echo $input_file $algorithm)
$normal============================[Program Output]===================$teal 

EOF

    printf "$(more $tmpLogFile)" #printf allows for pretty colors
    tmpFile=$(mktemp) #saving to tmp file so it can be logged in follow up log
    runcpp="../src/a.out"
    echo

    #printf "$algorithm"
    if [[ $algorithm == "AM" ]] 
    then
        result=$(/usr/bin/time -f "%S%M" $runcpp $algorithm $input_file $par_file 2>&1 >$tmpFile)
    else
        result=$(/usr/bin/time -f "%S%M" $runcpp $algorithm $input_file 2>&1 >$tmpFile)
    fi
    
    echo $(cat $tmpFile)
    time=$(cat $tmpFile | tr ' ' '\n' | grep "Time" )
    time=$(echo $time | cut -c 6-)
    result_file_name=$(echo "$algorithm")Time.csv
    echo "$instance_size,$time" >> $result_file_name
    cat > $tmpLogFile <<EOF

$(cat $tmpFile)
$normal============================[usr/bin/time]=====================$green
time = $(echo $time) milliseconds 
Saved to $result_file_name

$normal
EOF
        printf "$(more $tmpLogFile)" #printf allows for pretty colors

}

for algorithm in "${algorithms[@]}"; do
    if [[ $algorithm == "ABF" ]]
    then
        for input_file in "data/bruteForce"/*; do #tu rozbic na te trzy foldery
            instance_size=$(echo $input_file)
            instance_size=$(echo $instance_size | cut -c 22-)
            benchmark $instance_size $input_file $algorithm $input_file $par_file
        done
        echo >> $result_file_name
    else
        for input_file in "data/diffNumber"/*; do 
            instance_size=$(echo $input_file)
            instance_size=$(echo $instance_size | cut -c 22-)
        #printf "$instance_size"
            benchmark $instance_size $input_file $algorithm $input_file $par_file
        done
        echo >> $result_file_name

        for input_file in "data/diffCapacity"/*; do 
            instance_size=$(echo $input_file)
            instance_size=$(echo $instance_size | cut -c 27-)
        #printf "$instance_size"
            benchmark $instance_size $input_file $algorithm $input_file $par_file
        done
        echo >> $result_file_name
    fi
done


