//

Test 146: ❌ README.md
mini exit code = 126
bash exit code = 127
mini error = ( Permission denied)
bash error = ( command not found)

//this file exists but is not an executable


Test 142: ✅⚠️  /test_files
mini error = ( command not found)
bash error = ( No such file or directory)


Test 140: ❌ test_files
mini exit code = 126
bash exit code = 127
mini error = ( Is a directory)
bash error = ( command not found)

//its an directory, but aint detecting as one (cuz of no /)


Test 137: ✅⚠️  ./missing.out
mini error = ( command not found)
bash error = ( No such file or directory)


Test 129: ❌ echo hi | echo >>./outfiles/outfile01 bye >./test_files/invalid_permission
mini exit code = 0
bash exit code = 1


Test 104: ❌ cat <missing >./outfiles/outfile01
mini exit code = 0
bash exit code = 1

//i do get 1 with no file or directory


Test  92: ❌ echo hi >./outfiles/outfile01 >./outfiles/outfile02 | echo bye
Files ./mini_outfiles/outfile02 and ./bash_outfiles/outfile02 differ
mini outfiles:
hbash outfiles:
hi


Test  79: ❌ cat <missing | cat <"./test_files/infile"
mini output = ()
bash output = (hi hello world 42)

//i get no sutch file or dir but return as 0...
//also 1 error is suposed to not afect next


Test  75: ❌ # cat <"./test_files/infile_big" | echo hi
mini output = (hi)
bash output = ()
mini error = ( command not found)
bash error = ()

146 140 99 79 75

99 KO sometimes
