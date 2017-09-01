echo 'Running tests'
mkdir -p "tests/output"
for test in $(echo "tests/input/*")
do
	./chessprog < "$test" > "${test/input/output}"
done
echo 'Comparing results to expected output'
for test in $(echo "tests/expected_output/*")
do
	cmp --silent "$test" "${test/expected_/}" || \
		{
			diff_found=true
			echo "Output from ${test##*/} is not as expected";
			echo 'Do you wish to perform a diff?';
			select ans in 'Yes' 'No' 'Quit';
			do
				case $ans in
					Yes ) colordiff "$test" "${test/expected_/}"; break;;
					No ) break;;
					Quit ) exit;;
				esac
			done
		}
done
if [ -z "${diff_found+x}" ]
then
	echo 'Results are identical to expected output'
fi
