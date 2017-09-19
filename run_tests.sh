handle_diff(){
	diff_found=true
	echo "$2 generated by ${1##*/} is not as expected"
	echo 'Do you wish to perform a diff?'
	select ans in 'Yes' 'No' 'Quit'
	do
		case $ans in
			Yes )
				echo 'Expected - left, found - right'
				diff -y "$1" "${1/expected_/}"
				break;;
			No ) break;;
			Quit ) exit;;
		esac
	done
}

echo 'Running tests'
mkdir -p "tests/output"
mkdir -p "tests/saved_games"
for test in $(find tests/input -type f ! -name 'load*')
do
	echo
	echo "Test: ${test##*/}"
	./chessprog < "$test" > "${test/input/output}"
done
for test in $(find tests/input -type f -name 'load*')
do
	echo
	echo "Test: ${test##*/}"
	./chessprog < "$test" > "${test/input/output}"
done
echo 'Comparing results to expected output'
for test in tests/expected_output/*
do
	cmp --silent "$test" "${test/expected_/}" || handle_diff "$test" "Output"
done
if [ -z "${diff_found+x}" ]
then
	echo 'Results are as expected'
fi

echo 'Comparing saved games to expected files'
unset diff_found
for file in tests/expected_saved_games/*
do
	cmp --silent "$file" "${file/expected_/}" || handle_diff "$file" "Saved game"
done

if [ -z "${diff_found+x}" ]
then
	echo 'Results are as expected'
fi
